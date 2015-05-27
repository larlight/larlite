/**
 * \file partid.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for particle id data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/

#ifndef LARLITE_PARTID_H
#define LARLITE_PARTID_H
#include "Base/GeoTypes.h"
#include "data_base.h"
#include <vector>
namespace larlite{
  /**
     \class partid
     LArSoft anab::ParticleID class equivalent data container
  */
  class partid : public data_base {
    
  public:
    
    /// Default constructor
    partid();

    /// Alternative ctor
    partid(int Pdg,
	   int Ndf,
	   double MinChi2,
	   double DeltaChi2,
	   double Chi2Proton,
	   double Chi2Kaon,
	   double Chi2Pion,
	   double Chi2Muon,
	   double MissingE,
	   double MissingEavg,
	   double PIDA,
	   geo::PlaneID planeID);

    /// self-description to the stream output 
    friend std::ostream& operator << (std::ostream &o, partid const& a);
    
    /// Default destructor
    virtual ~partid(){}

    void clear_data();

    const int&    Pdg()           const;
    const int&    Ndf()           const;
    const double& MinChi2()       const;
    const double& DeltaChi2()     const;
    const double& Chi2Proton()    const;
    const double& Chi2Kaon()      const;
    const double& Chi2Pion()      const;
    const double& Chi2Muon()      const;
    const double& MissingE()      const;
    const double& MissingEavg()   const;
    const double& PIDA()          const;
    const geo::PlaneID& PlaneID() const;

  protected:

    int    fPdg;             ///< determined particle ID                                                                                          
    int    fNdf;             ///< ndf for chi2 test                                                                                               
    double fMinChi2;         ///< Minimum reduced chi2                                                                                            
    double fDeltaChi2;       ///< difference between two lowest reduced chi2's                                                                    
    double fChi2Proton;      ///< reduced chi2 using proton template                                                                              
    double fChi2Kaon;        ///< reduced chi2 using kaon template                                                                                
    double fChi2Pion;        ///< reduced chi2 using pion template                                                                                
    double fChi2Muon;        ///< reduced chi2 using muon template                                                                                
    double fMissingE;        ///< missing energy from dead wires for contained particle                                                           
    double fMissingEavg;     ///< missing energy from dead wires using average dEdx                                                               
    double fPIDA;            ///< PID developed by Bruce Baller      
    geo::PlaneID fPlaneID;   ///< Plane ID

  private:
    
  };
  
  /**
     \class event_partid
     A collection storage class of multiple partid.
  */
  class event_partid : public std::vector<larlite::partid>, 
		       public event_base {
    
  public:
    
    /// Default constructor
    event_partid(std::string name="noname") : event_base(data::kParticleID,name) { clear_data(); }
    
    /// Default copy constructor
    event_partid(const event_partid& original) : std::vector<larlite::partid>(original), event_base(original)
    {}
    
    /// Default destructor
    ~event_partid(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}

  private:
    
  };
}
#endif

/** @} */ // end of doxygen group 
