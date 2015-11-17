#ifndef QLLMATCH_CXX
#define QLLMATCH_CXX

#include "QLLMatch.h"
#include "OpT0Finder/Base/OpT0FinderException.h"
#include "OpT0Finder/PhotonLibrary/PhotonVisibilityService.h"
#include <TMinuit.h>
#include <cmath>

namespace flashana {

  QLLMatch* QLLMatch::_me = nullptr;

  void  MIN_vtx_qll (Int_t &, Double_t *, Double_t &, Double_t *, Int_t);

  QLLMatch::QLLMatch()
    : _pmt_x_v()
    , _pmt_y_v()
    , _pmt_z_v()
    , _minuit_ptr(nullptr)
    , _use_library(false)
  {}

  void QLLMatch::UsePhotonLibrary(bool use)
  { _use_library = use;}

  void QLLMatch::SetOpDetPositions( const std::vector<double>& pos_x,
				    const std::vector<double>& pos_y,
				    const std::vector<double>& pos_z )
  {
    if(pos_x.size() != pos_y.size() || pos_x.size() != pos_z.size() )
      throw OpT0FinderException("Unmatching optical detector position array length!");
    _pmt_x_v = pos_x;
    _pmt_y_v = pos_y;
    _pmt_z_v = pos_z;

  }

  FlashMatch_t QLLMatch::Match(const QCluster_t& pt_v, const Flash_t& flash)
  {

    this->CallMinuit(pt_v,flash);

    // Estimate position
    FlashMatch_t res;
    if(isnan(_qll)) return res;
    res.tpc_point.x = res.tpc_point.y = res.tpc_point.z = 0;
    res.score = 1./_qll;
    double weight_sum = 0;
    for(auto const& pt : _var_trk) {

      double weight = 0;

      for(size_t pmt_index = 0; pmt_index < _pmt_x_v.size(); ++pmt_index) {
	
	double r2 = ( pow( _pmt_x_v[pmt_index] - pt.x, 2 ) +
		      pow( _pmt_y_v[pmt_index] - pt.y, 2 ) +
		      pow( _pmt_z_v[pmt_index] - pt.z, 2 ) );
	
	weight += pt.q / r2;
      }

      res.tpc_point.x += pt.x * weight;
      res.tpc_point.y += pt.y * weight;
      res.tpc_point.z += pt.z * weight;

      weight_sum += weight;
    }
    res.tpc_point.x /= weight_sum;
    res.tpc_point.y /= weight_sum;
    res.tpc_point.z /= weight_sum;

    return res;
  }

  const std::vector<double>& QLLMatch::ChargeHypothesis(const QCluster_t& trk) {

    double qmax = 0;
    if(_pmt_x_v.empty()) Print(msg::kEXCEPTION,__FUNCTION__,"PMT Geometry not set!");
    if(_qll_hypothesis_v.empty()) _qll_hypothesis_v.resize(_pmt_x_v.size(),0);
    if(_qll_hypothesis_v.size() != _pmt_x_v.size())
      Print(msg::kEXCEPTION,__FUNCTION__,"Hypothesis vector length != PMT count");

    for(auto& v : _qll_hypothesis_v) v=0;
    
    for(size_t pmt_index=0; pmt_index < _pmt_x_v.size(); ++pmt_index) {

      for(size_t pt_index=0; pt_index < trk.size(); ++pt_index) {

	auto const& pt = trk[pt_index];

	if(!_use_library) {

	  double dx = _pmt_x_v[pmt_index] - pt.x;
	  double dy = _pmt_y_v[pmt_index] - pt.y;
	  double dz = _pmt_z_v[pmt_index] - pt.z;
	  
	  double r2 = ( pow(dx,2) + pow(dy,2) + pow(dz,2) );
	  
	  double angle = dx / sqrt(r2);

	  if(angle<0) angle *= -1;
	
	  _qll_hypothesis_v[pmt_index] += pt.q * angle / r2;
	}else{

	  double q = pt.q;
	  q *= ::phot::PhotonVisibilityService::GetME().GetVisibility(pt.x,pt.y,pt.z,pmt_index);
	  _qll_hypothesis_v[pmt_index] += q;
	    
	}
      }
      if(_qll_hypothesis_v[pmt_index] > qmax) qmax = _qll_hypothesis_v[pmt_index];
    }
      
    //for(auto& v : _qll_hypothesis_v) v /= qmax;
    double qsum =0;
    for(auto const& v : _qll_hypothesis_v) qsum += v;
    for(auto& v : _qll_hypothesis_v) v/= qsum;
    
    return _qll_hypothesis_v;
  }

  const QCluster_t& QLLMatch::VarTrack(double x)
  {
    for(size_t i=0; i<_raw_trk.size(); ++i) {

      _var_trk[i] = _raw_trk[i];

      _var_trk[i].x += x;

    }
    return _var_trk;
  }

  double QLLMatch::QLL(const std::vector<double>& qll_hypothesis)
  {
    double result = 0;
    double nvalid_pmt = 0;
    for(size_t pmt_index=0; pmt_index < qll_hypothesis.size(); ++pmt_index) {

      if( _flash_pe_v[pmt_index] < 0.01 ) continue;

      nvalid_pmt += 1;
      
      result += std::fabs( qll_hypothesis[pmt_index] - _flash_pe_v[pmt_index] ) * _flash_pe_v[pmt_index];

    }

    return result / nvalid_pmt;

  }
  
  void MIN_vtx_qll(Int_t & /*Npar*/,
		   Double_t * /*Grad*/,
		   Double_t & Fval,
		   Double_t * Xval,
		   Int_t /*Flag*/){

    auto const& var_trk = QLLMatch::GetME().VarTrack(Xval[0]);
    auto const& qll_hypothesis = QLLMatch::GetME().ChargeHypothesis(var_trk);
    Fval = QLLMatch::GetME().QLL( qll_hypothesis );

    QLLMatch::GetME().Record(Fval,Xval[0]);

    return;
  }  

  double QLLMatch::CallMinuit(const QCluster_t& tpc, const Flash_t& pmt) {

    if(_flash_pe_v.empty()) {
      if(_pmt_x_v.empty()) throw OpT0FinderException("No optical detector found!");
      _flash_pe_v.resize(_pmt_x_v.size(),0);
    }else if(_flash_pe_v.size() != _pmt_x_v.size())
      throw OpT0FinderException("PMT dimension has changed!");

    //
    // Prepare TPC
    //
    _raw_trk.resize(tpc.size());
    _var_trk.resize(tpc.size());

    double min_x = 1e9;
    for(size_t i=0; i<tpc.size(); ++i) {
      auto const& pt = tpc[i];
      _raw_trk[i] = pt;
      if(pt.x < min_x) min_x = pt.x;
    }

    for(auto& pt : _raw_trk) pt.x -= min_x;

    //
    // Prepare PMT
    //
    double max_pe = 1.;

    if(_normalize) {
      max_pe = 0;
      for(auto const& v : pmt.pe_v) if(v > max_pe) max_pe = v;
    }

    for(size_t i=0; i<pmt.pe_v.size(); ++i)

      _flash_pe_v[i] = pmt.pe_v[i] / max_pe;

    _minimizer_record_fval_v.clear();
    _minimizer_record_x_v.clear();
    
    if(!_minuit_ptr) _minuit_ptr = new TMinuit(4);

    double  reco_x = 128.;
    double  reco_x_err = 0;
    double  MinFval;
    int     ierrflag,npari,nparx,istat;
    double  arglist[2],error[4],Fmin,Fedm,Errdef;
    ierrflag = npari = nparx = istat = 0;

    _minuit_ptr->SetPrintLevel(-1);
    arglist[0] = 1.0;
    _minuit_ptr->mnexcm("SET STR",arglist,1,ierrflag);

    _minuit_ptr->SetFCN(MIN_vtx_qll);

    _minuit_ptr->DefineParameter (0,"X", reco_x, 10, 0.0, 256.);

    _minuit_ptr->Command("SET NOW");

    arglist[0]   = 5.0e+2;
    arglist[1]   = 1.0e-6;
    _minuit_ptr->mnexcm ("simplex",arglist,2,ierrflag);

    _minuit_ptr->GetParameter (0,reco_x,reco_x_err);

    _minuit_ptr->mnstat (Fmin,Fedm,Errdef,npari,nparx,istat);
    MinFval = Fmin;
    double *grad=0;
    int nPar=1;
    double fValue[1];
    fValue[0]=reco_x;
    // Transfer the minimization variables:
    MIN_vtx_qll(nPar, grad, Fmin,
		fValue, ierrflag);
    static bool show = true;
    /*
    if(show){
      if(Fmin!=MinFval)std::cout<<"Fmin "<<Fmin<<" not equall to "<<MinFval<<std::endl;
      show=false;
    }
    */

    // Transfer the minimization variables:
    _reco_x = reco_x;
    _reco_x_err = reco_x_err;
    _qll    = MinFval;

    // Clear:
    _minuit_ptr->mnexcm ("clear",arglist,0,ierrflag);

    if (_minuit_ptr) delete _minuit_ptr;
    _minuit_ptr = 0;

    return _qll;
  }

}
#endif
