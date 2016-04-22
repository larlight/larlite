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

    const double& Energy(int pl=-1)    const;
    const double& EnergyErr(int pl=-1) const;
    const std::vector< double >& Energy_v()    const;
    const std::vector< double >& EnergyErr_v() const;
    const std::vector< double >& MIPEnergy_v()    const;
    const std::vector< double >& MIPEnergyErr_v() const;

    int    best_plane() const;
    double Length() const;
    const double* Width() const;
    double OpeningAngle() const;

    const double& dEdx(int pl=-1)    const;
    const double& dEdxErr(int pl=-1) const;
    const std::vector< double >& dEdx_v()    const;
    const std::vector< double >& dEdxErr_v() const;
    const double& dQdx(int pl=-1)    const;
    const double& dQdxErr(int pl=-1) const;
    const std::vector< double >& dQdx_v()    const;
    const std::vector< double >& dQdxErr_v() const;

    // Set methods

    // id setter function
    void set_id (const int id) { fID = id; }

    // Energy setter functions
    void set_total_energy     (const double& q) { fTotalEnergy = q;      }
    void set_total_energy_err (const double& q) { fSigmaTotalEnergy = q; }
    void set_total_energy_v        (const std::vector< double >& q) { fTotalEnergy_v = q;         }
    void set_total_energy_err_v    (const std::vector< double >& q) { fSigmaTotalEnergy_v = q;    }
    void set_total_MIPenergy_v     (const std::vector< double >& q) { fTotalMIPEnergy_v = q;      }
    void set_total_MIPenergy_err_v (const std::vector< double >& q) { fSigmaTotalMIPEnergy_v = q; }

    // Best Plane setter function
    void set_total_best_plane (const int q) { fBestPlane = q;}

    // Start Direction setter functions
    void set_direction       (const TVector3& dir)   { fDCosStart = dir;        }
    void set_direction_err   (const TVector3& dir_e) { fSigmaDCosStart = dir_e; }

    // Start Point setter functions
    void set_start_point     (const TVector3& xyz)   { fXYZstart = xyz;        }
    void set_start_point_err (const TVector3& xyz_e) { fSigmaXYZstart = xyz_e; }

    // dEdx setter functions
    void set_dedx       (const double& q) { fdEdx = q;      }
    void set_dedx_err   (const double& q) { fSigmadEdx = q; }
    void set_dedx_v     (const std::vector< double >& q) { fdEdx_v = q;      }
    void set_dedx_err_v (const std::vector< double >& q) { fSigmadEdx_v = q; }

    // dQdx setter functions
    void set_dqdx       (const double& q) { fdQdx = q;      }
    void set_dqdx_err   (const double& q) { fSigmadQdx = q; }
    void set_dqdx_v     (const std::vector< double >& q) { fdQdx_v = q;      }
    void set_dqdx_err_v (const std::vector< double >& q) { fSigmadQdx_v = q; }

    // Length setter function
    void set_length(const double& l) { fLength = l; }

    // Width setter function
    void set_width( const double* w ) { fWidth[0] = w[0]; fWidth[1] = w[1]; }

    // Opening Angle setter function
    void set_opening_angle(const double& oa) { fOpeningAngle = oa; }

  protected:

    int fID;
    TVector3 fDCosStart;      ///< direction cosines at start of shower
    TVector3 fSigmaDCosStart; ///< uncertainting on initial direction cosines
    TVector3 fXYZstart;       ///< direction cosines at start of shower
    TVector3 fSigmaXYZstart;  ///< uncertainting on initial direction cosines
    // Energy information
    double fTotalEnergy;                          ///< Calculated Energy
    double fSigmaTotalEnergy;                     ///< Calculated Energy error
    std::vector< double > fTotalEnergy_v;         ///< Calculated Energy per each plane
    std::vector< double > fSigmaTotalEnergy_v;    ///< Calculated Energy per each plane
    std::vector< double > fTotalMIPEnergy_v;      ///< Calculated Energy per each plane
    std::vector< double > fSigmaTotalMIPEnergy_v; ///< Calculated Energy error per each plane
    // dQdx
    double fdQdx;                       ///< Calculated dQdx
    double fSigmadQdx;                  ///< Calculated dQdx error
    std::vector< double > fdQdx_v;      ///< Calculated dQdx per each plane
    std::vector< double > fSigmadQdx_v; ///< Calculated dQdx error per each plane
    // dEdx
    double fdEdx;                       ///< Calculated dQdx
    double fSigmadEdx;                  ///< Calculated dQdx error
    std::vector< double > fdEdx_v;      ///< Calculated dEdx per each plane
    std::vector< double > fSigmadEdx_v; ///< Calculated dEdx error per each plane

    int fBestPlane;
    double fLength;
    double fWidth[2];
    double fOpeningAngle;
    
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
