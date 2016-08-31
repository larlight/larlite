#ifndef QLLMATCH_CXX
#define QLLMATCH_CXX

#include "QLLMatch.h"
#include "OpT0Finder/Base/OpT0FinderException.h"
#include "OpT0Finder/PhotonLibrary/PhotonVisibilityService.h"
#include <TMinuit.h>
#include <cmath>
#include <numeric>
#include <TMath.h>

namespace flashana {

  QLLMatch *QLLMatch::_me = nullptr;
  
  void MIN_vtx_qll(Int_t &, Double_t *, Double_t &, Double_t *, Int_t);
  
  QLLMatch::QLLMatch(const std::string name)
    : BaseFlashMatch(name), _mode(kChi2), _record(false), _normalize(false), _minuit_ptr(nullptr)
  { _current_llhd = _current_chi2 = -1.0; }

  QLLMatch::QLLMatch()
  { throw OpT0FinderException("Use QLLMatch::GetME() to obtain singleton pointer!"); }
  
  void QLLMatch::Configure(const ::fcllite::PSet &pset) {
    _record = pset.get<bool>("RecordHistory");
    _normalize = pset.get<bool>("NormalizeHypothesis");
    _mode   = (QLLMode_t)(pset.get<unsigned short>("QLLMode"));
  }
  
  FlashMatch_t QLLMatch::Match(const QCluster_t &pt_v, const Flash_t &flash) {
    
    this->CallMinuit(pt_v, flash);
    // Shit happens line above in CallMinuit
    
    // Estimate position
    FlashMatch_t res;
    if (isnan(_qll)) return res;
    
    res.tpc_point.x = res.tpc_point.y = res.tpc_point.z = 0;
    res.score = 1. / _qll;
    
    double weight = 0;
    
    for (size_t pmt_index = 0; pmt_index < NOpDets(); ++pmt_index) {

      res.tpc_point.y += OpDetY(pmt_index) * _hypothesis.pe_v[pmt_index];
      res.tpc_point.z += OpDetZ(pmt_index) * _hypothesis.pe_v[pmt_index];
      
      weight += _hypothesis.pe_v[pmt_index];
    }
    
    res.tpc_point.y /= weight;
    res.tpc_point.z /= weight;
    res.tpc_point.q = weight;

    res.tpc_point.x = _reco_x_offset;
    res.tpc_point_err.x = _reco_x_offset_err;
    res.hypothesis  = _hypothesis.pe_v;

    return res;
  }
  
    const Flash_t &QLLMatch::ChargeHypothesis(const double xoffset) {
        if (_hypothesis.pe_v.empty()) _hypothesis.pe_v.resize(NOpDets(), 0.);
        if (_hypothesis.pe_v.size() != NOpDets())
            Print(msg::kEXCEPTION, __FUNCTION__, "Hypothesis vector length != PMT count");

        for (auto &v : _hypothesis.pe_v) v = 0;

        // Apply xoffset
        _var_trk.resize(_raw_trk.size());
        for (size_t pt_index = 0; pt_index < _raw_trk.size(); ++pt_index) {
            //std::cout << "x point : " << _raw_trk[pt_index].x << "\t offset : " << xoffset << std::endl;
            _var_trk[pt_index].x = _raw_trk[pt_index].x + xoffset;
            _var_trk[pt_index].y = _raw_trk[pt_index].y;
            _var_trk[pt_index].z = _raw_trk[pt_index].z;
            _var_trk[pt_index].q = _raw_trk[pt_index].q;
        }

        FillEstimate(_var_trk, _hypothesis);

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
	
        for (size_t pmt_index = 0; pmt_index < hypothesis.pe_v.size(); ++pmt_index) {

            if (measurement.pe_v[pmt_index] < 0.01) continue;

            nvalid_pmt += 1;
	    
            auto O = measurement.pe_v[pmt_index]; // observation
            auto H = hypothesis.pe_v[pmt_index];  // hypothesis

	    switch(_mode) {

	    case kLLHD:
	      _current_llhd -= std::log10(TMath::Poisson(O,H));
	      if(isnan(_current_llhd) || isinf(_current_llhd)) _current_llhd = 1.e20; // 1.e50 exceeds double precision point accuracy
	    case kChi2:
	      _current_chi2 += std::pow((O - H), 2) / (O + H);
	      break;
	    default:
	      std::cout<<"Unexpected mode"<<std::endl;
	      throw std::exception();
	    }

            //result += std::fabs(  ) * measurement.pe_v[pmt_index];

        }

        //std::cout << "PE hyp : " << PEtot_Hyp << "\tPE Obs : " << PEtot_Obs << "\t Chi^2 : " << result << std::endl;

	_current_chi2 /= nvalid_pmt;
	_current_llhd /= (nvalid_pmt +1);

	return (_mode == kChi2 ? _current_chi2 : _current_llhd);
    }

    void MIN_vtx_qll(Int_t & /*Npar*/,
                     Double_t * /*Grad*/,
                     Double_t &Fval,
                     Double_t *Xval,
                     Int_t /*Flag*/) {

        //std::cout << "minuit offset : " << Fval << std::endl;
        ///std::cout << "minuit Xval?? : " << *Xval << std::endl;

        auto const &hypothesis = QLLMatch::GetME().ChargeHypothesis(*Xval);
        auto const &measurement = QLLMatch::GetME().Measurement();
        Fval = QLLMatch::GetME().QLL(hypothesis, measurement);

        QLLMatch::GetME().Record(Xval[0]);

        return;
    }

    double QLLMatch::CallMinuit(const QCluster_t &tpc, const Flash_t &pmt) {

        if (_measurement.pe_v.empty()) {
            _measurement.pe_v.resize(NOpDets(), 0.);
        }
        if (_measurement.pe_v.size() != pmt.pe_v.size())
            throw OpT0FinderException("PMT dimension has changed!");
	
        //
        // Prepare TPC
        //
        _raw_trk.resize(tpc.size());
	
        double min_x = 1e9;
        for (size_t i = 0; i < tpc.size(); ++i) {
            auto const &pt = tpc[i];
            _raw_trk[i] = pt;
            if (pt.x < min_x) min_x = pt.x;
        }

        for (auto &pt : _raw_trk) pt.x -= min_x;
	
        //
        // Prepare PMT
        //
        double max_pe = 1.;

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
	
        double reco_x = 128.;
        double reco_x_err = 0;
        double MinFval;
        int ierrflag, npari, nparx, istat;
        double arglist[4], error[4], Fmin, Fedm, Errdef;
        ierrflag = npari = nparx = istat = 0;

        _minuit_ptr->SetPrintLevel(-1);
        arglist[0] = 2.0;
        _minuit_ptr->mnexcm("SET STR", arglist, 1, ierrflag);

        _minuit_ptr->SetFCN(MIN_vtx_qll);

        _minuit_ptr->DefineParameter(0, "X", reco_x, 10, 0.0, 256.);

        _minuit_ptr->Command("SET NOW");

        // use Migrad minimizer

        arglist[0] = 500;
        _minuit_ptr->mnexcm("MIGRAD", arglist, 1, ierrflag);

        //arglist[0]   = 5.0e+2;
        //arglist[1]   = 1.0e-6;
        //_minuit_ptr->mnexcm ("simplex",arglist,2,ierrflag);

        _minuit_ptr->GetParameter(0, reco_x, reco_x_err);

        _minuit_ptr->mnstat(Fmin, Fedm, Errdef, npari, nparx, istat);

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
        _qll = MinFval;

        // Clear:
        _minuit_ptr->mnexcm("clear", arglist, 0, ierrflag);

        if (_minuit_ptr) delete _minuit_ptr;
        _minuit_ptr = 0;

        return _qll;
    }

}
#endif
