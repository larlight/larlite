#ifndef QLLMATCH_CXX
#define QLLMATCH_CXX

#include "QLLMatch.h"
#include "OpT0Finder/Base/OpT0FinderException.h"
#include <TMinuit.h>
#include <cmath>
#include <numeric>
#include <TMath.h>

namespace flashana {

  static QLLMatchFactory __global_QLLMatchFactory__;

  QLLMatch *QLLMatch::_me = nullptr;
  
  void MIN_vtx_qll(Int_t &, Double_t *, Double_t &, Double_t *, Int_t);
  
  QLLMatch::QLLMatch(const std::string name)
    : BaseFlashMatch(name), _mode(kChi2), _record(false), _normalize(false), _cosmic_disc_correction(false), _minuit_ptr(nullptr)
  { _current_llhd = _current_chi2 = -1.0; }

  QLLMatch::QLLMatch()
  { throw OpT0FinderException("Use QLLMatch::GetME() to obtain singleton pointer!"); }
  
  void QLLMatch::_Configure_(const Config_t &pset) {
    _record = pset.get<bool>("RecordHistory");
    _normalize = pset.get<bool>("NormalizeHypothesis");
    _mode   = (QLLMode_t)(pset.get<unsigned short>("QLLMode"));
    _cosmic_disc_correction = pset.get<bool>("ApplyCosmicDiscCorrection");
    _apply_cosmic_disc_threshold = pset.get<bool>("ApplyCosmicDiscThreshold");
    _cosmic_disc_threshold  = pset.get<double>("CosmicDiscThreshold");
    _skip_nodata_bins = pset.get<bool>("SkipZeroFlashBins");    
    _penalty_threshold_v = pset.get<std::vector<double> >("PEPenaltyThreshold");
    _penalty_value_v = pset.get<std::vector<double> >("PEPenaltyValue");

    _recox_penalty_threshold = pset.get<double>("XPenaltyThreshold");
    _recoz_penalty_threshold = pset.get<double>("ZPenaltyThreshold");

    _onepmt_score_threshold = pset.get<double>("OnePMTScoreThreshold");
    _onepmt_xdiff_threshold = pset.get<double>("OnePMTXDiffThreshold");
    _onepmt_pesum_threshold = pset.get<double>("OnePMTPESumThreshold");
    _onepmt_pefrac_threshold = pset.get<double>("OnePMTPEFracThreshold");
    
    _run_migrad = pset.get<bool>("RunMIGRAD",true);
    _minuit_penalize_x0_deviations = pset.get<bool>("ConstainX0",false);
    _xoffset_penalty_sigma = pset.get<double>("X0ConstraintSigma",10.0);
    
    _current_flash_isfrom_cosmicdisc = false;

  }
  
  FlashMatch_t QLLMatch::Match(const QCluster_t &pt_v, const Flash_t &flash) {
    
    //
    // Prepare TPC
    //
    _raw_trk.resize(pt_v.size());
    double min_x =  1e9;
    double max_x = -1e9;
    for (size_t i = 0; i < pt_v.size(); ++i) {
      auto const &pt = pt_v[i];
      _raw_trk[i] = pt;
      if ( pt.y<ActiveYMin() || pt.y>ActiveYMax() || pt.z<ActiveZMin() || pt.z>ActiveZMax() )
	continue;
      if (pt.x < min_x) { min_x = pt.x; _raw_xmin_pt = pt; }
      if (pt.x > max_x) { max_x = pt.x; _raw_xmax_pt = pt; }
    }
    for (auto &pt : _raw_trk) pt.x -= min_x;

    auto res = PESpectrumMatch(pt_v,flash,true);
    //auto res2 = PESpectrumMatch(pt_v,flash,false);
    
    //std::cout << "Using   mid-x-init: " << res.tpc_point.x << " [cm] @ " << res.score << std::endl;
    //std::cout << "Without mid-x-init: " << res2.tpc_point.x << " [cm] @ " << res2.score << std::endl;
    

    // FlashMatch_t res;
    // if (_run_migrad)
    //   //res = (res1.score > res2.score ? res1 : res2);
    //   res = res1;
    // else
    //   res = res1;

    if(res.score < _onepmt_score_threshold) {

      auto res_onepmt = OnePMTMatch(flash);

      if(res_onepmt.score >= 0.)
	return res_onepmt;
    }

    return res;
  }
  
  FlashMatch_t QLLMatch::OnePMTMatch(const Flash_t& flash) {

    FlashMatch_t res;
    res.score=-1;
    // Check if pesum threshold condition is met to use this method
    double pesum = flash.TotalPE();
    if(pesum < _onepmt_pesum_threshold) {
      //std::cout <<"PESumThreshold not met (pesum=" << pesum << ")" << std::endl;
      return res;
    }

    // Check if pe max fraction condition is met to use this method
    size_t maxpmt = 0;
    double maxpe  = 0.;
    for(size_t pmt=0; pmt<flash.pe_v.size(); ++pmt) {
      if(flash.pe_v[pmt] < maxpe) continue;
      maxpe  = flash.pe_v[pmt];
      maxpmt = pmt;
    }
    if(maxpe / pesum < _onepmt_pefrac_threshold) {
      //std::cout << "PERatioThreshold not met (peratio=" << maxpe/pesum << ")" << std::endl;
      return res;
    }

    // Now see if Flash T0 can be consistent with an assumption MinX @ X=0.
    double xdiff = fabs(_raw_xmin_pt.x - flash.time * DriftVelocity());
    if( xdiff > _onepmt_xdiff_threshold ) {
      //std::cout << "XDiffThreshold not met (xdiff=" << xdiff << ")" << std::endl;
      return res;
    }

    // Reaching this point means it is an acceptable match
    _reco_x_offset = 0.;
    _reco_x_offset_err = std::fabs(OpDetXArray().at(maxpmt));

    // Compute hypothesis with MinX @ X=0 assumption.
    _hypothesis = flash;
    FillEstimate(_raw_trk,_hypothesis);
    res.hypothesis = _hypothesis.pe_v;

    // Compute TPC point
    res.tpc_point.x = res.tpc_point.y = res.tpc_point.z = 0;
    double weight = 0;
    for (size_t pmt_index = 0; pmt_index < NOpDets(); ++pmt_index) {

      res.tpc_point.y += OpDetY(pmt_index) * _hypothesis.pe_v[pmt_index];
      res.tpc_point.z += OpDetZ(pmt_index) * _hypothesis.pe_v[pmt_index];
      
      weight += _hypothesis.pe_v[pmt_index];
    }
    
    res.tpc_point.y /= weight;
    res.tpc_point.z /= weight;

    res.tpc_point.x = _reco_x_offset;
    res.tpc_point_err.x = _reco_x_offset_err;
    
    // Use MinX point YZ distance to the max PMT and X0 diff as weight
    res.score = 1.;
    res.score *= 1. / xdiff;
    res.score *= 1. / (sqrt(pow(_raw_xmin_pt.y - OpDetY(maxpmt),2) + pow(_raw_xmin_pt.z - OpDetZ(maxpmt),2)));

    return res;
    
  }

  FlashMatch_t QLLMatch::PESpectrumMatch(const QCluster_t &pt_v, const Flash_t &flash, const bool init_x0) {
    
    this->CallMinuit(pt_v, flash, init_x0);
    // Shit happens line above in CallMinuit
    
    // Estimate position
    FlashMatch_t res;
    if (std::isnan(_qll)) return res;
    
    res.tpc_point.x = res.tpc_point.y = res.tpc_point.z = 0;
    
    double weight = 0;
    
    for (size_t pmt_index = 0; pmt_index < NOpDets(); ++pmt_index) {

      res.tpc_point.y += OpDetY(pmt_index) * _hypothesis.pe_v[pmt_index];
      res.tpc_point.z += OpDetZ(pmt_index) * _hypothesis.pe_v[pmt_index];
      
      weight += _hypothesis.pe_v[pmt_index];
    }
    
    res.tpc_point.y /= weight;
    res.tpc_point.z /= weight;

    res.tpc_point.x = _reco_x_offset;
    res.tpc_point_err.x = _reco_x_offset_err;
    
    res.hypothesis  = _hypothesis.pe_v;

    //
    // Compute score
    //
    res.score = 1. / _qll;

    // if ( _mode==kLLR )
    //   return res;
    
    // Compute X-weighting
    double x0 = _raw_xmin_pt.x - flash.time * DriftVelocity();
    if( fabs(_reco_x_offset - x0) > _recox_penalty_threshold )
      res.score *= 1. / (1. + fabs(_reco_x_offset - x0) - _recox_penalty_threshold);
    // Compute Z-weighting
    double z0 = 0;
    weight = 0;
    for (size_t pmt_index = 0; pmt_index < NOpDets(); ++pmt_index) {
      z0 += OpDetZ(pmt_index) * flash.pe_v[pmt_index];
      weight += flash.pe_v[pmt_index];
    }
    z0 /= weight;
    if( fabs(res.tpc_point.z - z0) > _recoz_penalty_threshold )
      res.score *= 1. / (1. + fabs(res.tpc_point.z - z0) - _recoz_penalty_threshold);
    
    return res;
  }
  
  const Flash_t &QLLMatch::ChargeHypothesis(const double xoffset) {
    if (_hypothesis.pe_v.empty()) _hypothesis.pe_v.resize(NOpDets(), 0.);
    if (_hypothesis.pe_v.size() != NOpDets()) {
      throw OpT0FinderException("Hypothesis vector length != PMT count");
    }
    
    for (auto &v : _hypothesis.pe_v) v = 0;
    _current_xoffset = xoffset;
    
    // Apply xoffset
    _var_trk.resize(_raw_trk.size());
    for (size_t pt_index = 0; pt_index < _raw_trk.size(); ++pt_index) {
      //std::cout << "x point : " << _raw_trk[pt_index].x << "\t offset : " << xoffset << std::endl;
      _var_trk[pt_index].x = _raw_trk[pt_index].x + xoffset;
      _var_trk[pt_index].y = _raw_trk[pt_index].y;
      _var_trk[pt_index].z = _raw_trk[pt_index].z;
      _var_trk[pt_index].q = _raw_trk[pt_index].q;
    }

    // go out to photonlibary and calculate flash hypothesis
    FillEstimate(_var_trk, _hypothesis);

    if (_cosmic_disc_correction && _current_flash_isfrom_cosmicdisc ) {
      for (size_t ich=0; ich<_hypothesis.pe_v.size(); ich++ ) {
	float pe = _hypothesis.pe_v.at(ich);
	if ( pe < 60.0 ) 
	  pe *= 0.424;
	else if ( pe >= 60.0 )
	  pe *= 0.354;
	_hypothesis.pe_v[ich] = pe;
      }
    }
    if (_apply_cosmic_disc_threshold && _current_flash_isfrom_cosmicdisc ) {
      for (size_t ich=0; ich<_hypothesis.pe_v.size(); ich++ ) {
	float pe = _hypothesis.pe_v.at(ich);
	if ( pe<_cosmic_disc_threshold )
	  _hypothesis.pe_v.at(ich) = 1.0e-6;
      }
    }
    
    if (_normalize) {
      double qsum = std::accumulate(std::begin(_hypothesis.pe_v),
				    std::end(_hypothesis.pe_v),
				    0.0);
      for (auto &v : _hypothesis.pe_v) v /= qsum;
    }
    
    return _hypothesis;
  }

  const Flash_t &QLLMatch::Measurement() const { return _measurement; }
  
  double QLLMatch::QLL(const Flash_t &hypothesis,
		       const Flash_t &measurement) {
    
    double nvalid_pmt = 0;
    
    double PEtot_Hyp = 0;
    for (auto const &pe : hypothesis.pe_v)
      PEtot_Hyp += pe;
    double PEtot_Obs = 0;
    for (auto const &pe : measurement.pe_v)
      PEtot_Obs += pe;
    
    _current_chi2 = _current_llhd = 0.;
    
    if (measurement.pe_v.size() != hypothesis.pe_v.size())
      throw OpT0FinderException("Cannot compute QLL for unmatched length!");
    
    double O, H, Error;
    for (size_t pmt_index = 0; pmt_index < hypothesis.pe_v.size(); ++pmt_index) {
      
      O = measurement.pe_v[pmt_index]; // observation
      H = hypothesis.pe_v[pmt_index];  // hypothesis
      
      if( H < 0 ) throw OpT0FinderException("Cannot have hypothesis value < 0!");
      
      if(O < 0) {
	if(H < _penalty_threshold_v[pmt_index]) continue;
	O = _penalty_value_v[pmt_index];
      }
      
      nvalid_pmt += 1;

      if(_mode == kLLHD) {
	/* Replaced block to be used in uboonecode
	_current_llhd -= std::log10(TMath::Poisson(O,H));
	if(isnan(_current_llhd) || isinf(_current_llhd)) _current_llhd = 1.e6;
	*/
	// Updated block
	double arg = TMath::Poisson(O,H);
	if ( !_skip_nodata_bins ) {
	  // include all bins
	  if(arg > 0. && !std::isnan(arg) )
	    _current_llhd -= std::log10(arg);
	  else
	    _current_llhd = 1.e6;
	}
	else {
	  // only include non-zero bins
	  if ( O>0 ) {
	    if(arg > 0. && !std::isnan(arg) )
	      _current_llhd -= std::log10(arg);
	    else
	      _current_llhd = 1.e6;	    
	  }
	}
	if(std::isinf(_current_llhd)) _current_llhd = 1.e6;
	// Updated block ends
      } else if (_mode == kChi2) {
	Error = O;
	if( Error < 1.0 ) Error = 1.0;
	_current_chi2 += std::pow((O - H), 2) / (Error);
      } else if (_mode == kLLR ) {
	// Poisson log-likelihood ratio from PDG
	double arg1 = H-O;
	double arg2 = 0;
	if ( O>0 ) {
	  if ( H>1.0e-6 )
	    arg2 = O*(std::log(O)-std::log(H));
	  else
	    arg2 = O*(std::log(O)-std::log(1.0e-6));
	}
	_current_llhd += 2.0*(arg1+arg2);
	_current_chi2 = _current_llhd;
      } else {
	FLASH_ERROR() << "Unexpected mode" << std::endl;
	throw OpT0FinderException();
      }
      
      //result += std::fabs(  ) * measurement.pe_v[pmt_index];
      
    }

    //std::cout << "PE hyp : " << PEtot_Hyp << "\tPE Obs : " << PEtot_Obs << "\t Chi^2 : " << result << std::endl;
    
    _current_chi2 /= nvalid_pmt;
    _current_llhd /= (nvalid_pmt +1);

    if ( _minuit_penalize_x0_deviations ) {
      double x0 = _raw_xmin_pt.x - measurement.time*DriftVelocity();
      double penalty = (_current_xoffset-x0)/_xoffset_penalty_sigma;
      _current_chi2 += 0.5*penalty*penalty;
      _current_llhd += 0.5*penalty*penalty;
    }
    
    
    return (_mode == kChi2 ? _current_chi2 : _current_llhd);
  }
  
  void MIN_vtx_qll(Int_t & /*Npar*/,
		   Double_t * /*Grad*/,
		   Double_t &Fval,
		   Double_t *Xval,
		   Int_t /*Flag*/) {
    
    //std::cout << "minuit offset : " << Fval << std::endl;
    ///std::cout << "minuit Xval?? : " << *Xval << std::endl;
    
    auto const &hypothesis = QLLMatch::GetME()->ChargeHypothesis(*Xval);
    auto const &measurement = QLLMatch::GetME()->Measurement();
    Fval = QLLMatch::GetME()->QLL(hypothesis, measurement);
    
    QLLMatch::GetME()->Record(Xval[0]);
    
    return;
  }
  
  double QLLMatch::CallMinuit(const QCluster_t &tpc, const Flash_t &pmt, const bool init_x0) {
    
    if (_measurement.pe_v.empty()) {
      _measurement.pe_v.resize(NOpDets(), 0.);
    }
    if (_measurement.pe_v.size() != pmt.pe_v.size())
      throw OpT0FinderException("PMT dimension has changed!");
    
    if (_penalty_threshold_v.size() != pmt.pe_v.size()) {
      throw OpT0FinderException("Penalty threshold array has a different size than PMT array size!");
    }
    
    if (_penalty_value_v.size() != pmt.pe_v.size()) {
      throw OpT0FinderException("Penalty value array has a different size than PMT array size!");
    }
    
    //
    // Prepare PMT
    //
    double max_pe = 1.;
    const double cm_per_tick = 0.5*DriftVelocity();

    // Debug: Print out expected PE spectrum
    //for(size_t i=0; i<pmt.pe_v.size(); ++i) {
    //std::cout << "PE meas: " << i << " " << pmt.pe_v[i] << std::endl;
    //}
    
    if (_normalize) {
      max_pe = 0;
      for (auto const &v : pmt.pe_v) if (v > max_pe) max_pe = v;
    }
    
    for (size_t i = 0; i < pmt.pe_v.size(); ++i)  _measurement.pe_v[i] = pmt.pe_v[i] / max_pe;
    
    _minimizer_record_chi2_v.clear();
    _minimizer_record_llhd_v.clear();
    _minimizer_record_x_v.clear();
    
    if (!_minuit_ptr) _minuit_ptr = new TMinuit(4);
    
    double reco_x = 0.;
    //double x0 = _raw_xmin_pt.x - flash.time * DriftVelocity();    
    if (init_x0) {
      reco_x = _raw_xmin_pt.x - pmt.time*DriftVelocity();
      //reco_x = (ActiveXMax() - (_raw_xmax_pt.x - _raw_xmin_pt.x)) / 2.;
    }

    double reco_x_err = (ActiveXMax() - (_raw_xmax_pt.x - _raw_xmin_pt.x)) / 2.;
    double MinFval;
    int ierrflag, npari, nparx, istat;
    double arglist[4], Fmin, Fedm, Errdef;
    ierrflag = npari = nparx = istat = 0;
    
    _minuit_ptr->SetPrintLevel(-1);
    arglist[0] = 2.0;
    _minuit_ptr->mnexcm("SET STR", arglist, 1, ierrflag);
    
    _minuit_ptr->SetFCN(MIN_vtx_qll);
    
    _minuit_ptr->DefineParameter(0, "X", reco_x, reco_x_err, -20.0, ActiveXMax() - (_raw_xmax_pt.x - _raw_xmin_pt.x) + 20.0 );
    
    _minuit_ptr->Command("SET NOW");
    
    // use Migrad minimizer
    
    arglist[0] = 5000;
    if ( _run_migrad ) {
      _minuit_ptr->mnexcm("MIGRAD", arglist, 1, ierrflag);
    
      //arglist[0]   = 5.0e+2;
      //arglist[1]   = 1.0e-6;
      //_minuit_ptr->mnexcm ("simplex",arglist,2,ierrflag);
      _minuit_ptr->GetParameter(0, reco_x, reco_x_err);
      _minuit_ptr->mnstat(Fmin, Fedm, Errdef, npari, nparx, istat);
    }
    
    // use this for debugging, maybe uncomment the actual minimzing function (MIGRAD / simplex calls)
    // scanning the parameter set
    //arglist[0] = 0;       // Parameter No (in our case x is the only and therefore the 0th parameter)
    //arglist[1] = 500;    // Number of points
    //arglist[2] = 0;       // Start point of scan
    //arglist[3] = 256;     // End point of scan
    //_minuit_ptr->mnexcm("scan", arglist,4, ierrflag);
    
    MinFval = Fmin;
    double *grad = 0;
    int nPar = 1;
    double fValue[1];
    fValue[0] = reco_x;
    // Transfer the minimization variables:
    MIN_vtx_qll(nPar, grad, Fmin,
		fValue, ierrflag);
    //static bool show = true;
    /*
      if(show){
      if(Fmin!=MinFval)std::cout<<"Fmin "<<Fmin<<" not equall to "<<MinFval<<std::endl;
      show=false;
      }
    */
    
    // Transfer the minimization variables:
    _reco_x_offset = reco_x;
    _reco_x_offset_err = reco_x_err;
    _qll = Fmin;
    
    // Clear:
    _minuit_ptr->mnexcm("clear", arglist, 0, ierrflag);
    
    if (_minuit_ptr) delete _minuit_ptr;
    _minuit_ptr = 0;
    
    return _qll;
  }
  
}
#endif
