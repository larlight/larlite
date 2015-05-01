/**
 * \file shower.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for shower data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/

#ifndef LARLITE_SHOWER_H
#define LARLITE_SHOWER_H

#include "data_base.h"
#include <TVector3.h>
namespace larlite{
  /**
     \class shower
     LArSoft Shower class equivalent data container
  */
  class shower : public data_base {
    
  public:
    
    /// Default constructor
    shower() : data_base(data::kShower) {clear_data();}
    
    /// Default destructor
    virtual ~shower(){}
    
    virtual void clear_data();

    // Get Methods 
    int    ID()                    const;
    const TVector3& Direction()    const;
    const TVector3& DirectionErr() const;
    
    const TVector3& ShowerStart()    const;
    const TVector3& ShowerStartErr() const;

    const std::vector< double >& Energy()    const;
    const std::vector< double >& EnergyErr() const;
    
    const std::vector< double >& MIPEnergy()    const;
    const std::vector< double >& MIPEnergyErr() const;

    int    best_plane() const;
    double Length() const;
    const std::vector< double >& dEdx()    const;
    const std::vector< double >& dEdxErr() const;

#ifndef __CINT__
    // Set methods
    void set_id                (const int id)                      { fID = id;                 }
    void set_total_energy      (const std::vector< double >& q)    { fTotalEnergy = q;         }
    void set_total_energy_err  (const std::vector< double >& q)    { fSigmaTotalEnergy = q;    }
    void set_total_MIPenergy   (const std::vector< double >& q)    { fTotalMIPEnergy = q;      }
    void set_total_MIPenergy_err  (const std::vector< double >& q) { fSigmaTotalMIPEnergy = q; }
    void set_total_best_plane  (const int q)                         { fBestPlane = q;        }
    
    void set_direction       (const TVector3& dir)        { fDCosStart = dir;        }
    void set_direction_err   (const TVector3& dir_e)      { fSigmaDCosStart = dir_e; }
    void set_start_point     (const TVector3& xyz)        { fXYZstart = xyz;        }
    void set_start_point_err (const TVector3& xyz_e)      { fSigmaXYZstart = xyz_e; }
    void set_dedx      (const std::vector< double >& q) { fdEdx = q;        }
    void set_dedx_err  (const std::vector< double >& q) { fSigmadEdx = q;        }
    void set_length(const double& l) { fLength = l; }
    
  protected:

    int fID;
    TVector3 fDCosStart;             ///< direction cosines at start of shower
    TVector3 fSigmaDCosStart;        ///< uncertainting on initial direction cosines
    TVector3 fXYZstart;             ///< direction cosines at start of shower
    TVector3 fSigmaXYZstart;        ///< uncertainting on initial direction cosines
    std::vector< double > fTotalEnergy;           ///< Calculated Energy per each plane
    std::vector< double > fSigmaTotalEnergy;           ///< Calculated Energy per each plane
    std::vector< double > fdEdx;           ///< Calculated dEdx per each plane
    std::vector< double > fSigmadEdx;           ///< Calculated dEdx per each plane
    
    std::vector< double > fTotalMIPEnergy;           ///< Calculated Energy per each plane
    std::vector< double > fSigmaTotalMIPEnergy;           ///< Calculated Energy per each plane
    int fBestPlane;
    double fLength;
#endif    
    
  };
  
  /**
     \class event_shower
     A collection storage class of multiple showers.
  */
  class event_shower : public std::vector<larlite::shower>, public event_base {
    
  public:
    
    /// Default constructor
    event_shower(std::string name="noname") : event_base(data::kShower,name) {clear_data();}
    
    /// Default copy constructor
    event_shower(const event_shower& original) : std::vector<larlite::shower>(original), event_base(original)
    {}
    
    /// Default destructor
    ~event_shower(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}

  protected:
    
  private:
    
  };
}
#endif

/** @} */ // end of doxygen group 
