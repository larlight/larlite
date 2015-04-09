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
    
//     void set_max_width     (double x, double y) 
//     { fMaxWidthX=x; fMaxWidthY=y; }
//     void set_distance_max_width (double d)       { fDistanceMaxWidth = d;   }

    // Get Methods 
    //inline double TotalCharge()      const { return fTotalCharge;      }
    inline int    ID()               const { return fID;               }
   // inline double MaxWidthX()        const { return fMaxWidthX;           }
  //  inline double MaxWidthY()        const { return fMaxWidthY;           }
  //  inline double DistanceMaxWidth() const { return fDistanceMaxWidth; }
    inline const TVector3& Direction()    const { return fDCosStart;          }
    inline const TVector3& DirectionErr() const { return fSigmaDCosStart;     }
    
    inline const TVector3& ShowerStart()    const { return fXYZstart;          }
    inline const TVector3& ShowerStartErr() const { return fSigmaXYZstart;     }

    inline const std::vector< double >& Energy()    const { return fTotalEnergy;          }
    inline const std::vector< double >& EnergyErr() const { return fSigmaTotalEnergy;     }
    
    inline const std::vector< double >& MIPEnergy()    const { return fTotalMIPEnergy;          }
    inline const std::vector< double >& MIPEnergyErr() const { return fSigmaTotalMIPEnergy;     }
    inline int    best_plane()               const { return fBestPlane;               }
    inline double Length() const { return fLength; }
    inline const std::vector< double >& dEdx()    const { return fdEdx;          }
    inline const std::vector< double >& dEdxErr() const { return fSigmadEdx;     }
    
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
    //  double fMaxWidthX;             ///< maximum width of the prong in the x(0)
    //  double fMaxWidthY;             ///< maximum width of the prong in the y(0)
    //  double fDistanceMaxWidth;      ///< distance from the start of the prong to its maximum width
    //  double fTotalCharge;           ///< total charge of hits in the shower
    
  private:
    
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
