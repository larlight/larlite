
////////////////////////////////////////////////////////////////////////
//
//    trivia : Class to hold the Muon Counter System (MuCS) reco fields 
//    author : Matt Bass
//    e-mail : Matthew.Bass@physics.ox.ac.uk
//
////////////////////////////////////////////////////////////////////////

#include "mucsreco.h"
#include <cmath>

namespace larlite {
  
  mucsreco::mucsreco() : data_base(data::kMuCSReco)
  {}
  
  mucsreco::~mucsreco()
  {}
  
  mucsreco::mucsreco( float theta_xy, float theta_xy_rms, float x,
		      float x_rms, float theta_yz, float theta_yz_rms,
		      float z, float z_rms, float y, int xmatches, int zmatches )
    : data_base(data::kMuCSReco)
  {
    ftheta_xy=theta_xy;
    ftheta_xy_rms=theta_xy_rms;
    fx=x;
    fx_rms=x_rms;
    ftheta_yz=theta_yz;
    ftheta_yz_rms=theta_yz_rms;
    fz=z;
    fz_rms=z_rms;
    fy=y;
    fxmatches=xmatches;
    fzmatches=zmatches;
  }
  
  //compute from underlying variables
  float mucsreco::theta() const{
    return -M_PI/2+acos((pow(1+pow(tan(ftheta_xy),-2)+pow(tan(ftheta_yz),-2),-0.5)));
  }
  float mucsreco::phi() const{
    float ltheta = theta();
    return atan2(sin(ltheta)/tan(ftheta_yz),sin(ltheta)/tan(ftheta_xy));
  }
  
  float mucsreco::theta_xy() const{
    return ftheta_xy;
  }
  float mucsreco::theta_xy_rms() const{
    return ftheta_xy_rms;
  }
  float mucsreco::x() const{
    return fx;
  }
  float mucsreco::x_rms() const{
    return fx_rms;
  }
  
  float mucsreco::theta_yz() const{
    return ftheta_yz;
  }
  float mucsreco::theta_yz_rms() const{
    return ftheta_yz_rms;
  }
  float mucsreco::z() const{
    return fz;
  }
  float mucsreco::z_rms() const{
    return fz_rms;
  }
  
  float mucsreco::y() const{
    return fy;
  }
  int mucsreco::xmatches() const{
    return fxmatches;
  }
  int mucsreco::zmatches() const{
    return fzmatches;
  }
}

////////////////////////////////////////////////////////////////////////
//
//    The end !
//
////////////////////////////////////////////////////////////////////////
