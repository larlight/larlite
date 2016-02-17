
////////////////////////////////////////////////////////////////////////
//
//    trivia : Class to hold the Muon Counter System (MuCS) reco fields 
//    author : Matt Bass
//    e-mail : Matthew.Bass@physics.ox.ac.uk
//
////////////////////////////////////////////////////////////////////////

#ifndef LARLITE_MuCSReco_H
#define LARLITE_MuCSReco_H

#include "data_base.h"

namespace larlite {
 
  class mucsreco : public data_base {
    
  public:
    mucsreco();
    virtual ~mucsreco();  
    mucsreco( float theta_xy, float theta_xy_rms, float x,
	      float x_rms, float theta_yz, float theta_yz_rms,
	      float z, float z_rms, float y, int xmatches, int zmatches ); 
    
    float theta_xy() const;
    float theta_xy_rms() const;
    float x() const;
    float x_rms() const;
    float theta_yz() const;
    float theta_yz_rms() const;
    float z() const;
    float z_rms() const;
    float y() const;
    int xmatches() const;
    int zmatches() const;
    
    float theta() const;
    float phi() const;
    
  private:
    float ftheta_xy;
    float ftheta_xy_rms;
    float fx;
    float fx_rms;
    float ftheta_yz;
    float ftheta_yz_rms;
    float fz;
    float fz_rms;  
    float fy;
    int fxmatches;
    int fzmatches;
  };

  /**
     \class event_mucsreco
     A collection storage class of multiple mucsreco.
  */
  class event_mucsreco : public std::vector<larlite::mucsreco>,
			 public event_base {
    
  public:
    
    /// Default constructor
    event_mucsreco(std::string name="noname") : event_base(data::kMuCSReco,name) { clear_data(); }
    
    /// Default copy constructor
    event_mucsreco(const event_mucsreco& original) : std::vector<larlite::mucsreco>(original), event_base(original)
    {}
    
    /// Default destructor
    ~event_mucsreco(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}
    
  private:
    
  };
  
}
#endif 

