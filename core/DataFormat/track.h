/**
 * \file track.h
 *
 * \ingroup DataFormat
 * 
 * \brief Class def header for track data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/

#ifndef TRACK_H
#define TRACK_H

#include "data_base.h"
#include "TVector3.h"
#include "TMatrixD.h"

namespace larlite {
  /**
     \class track
     LArSoft Track class equivalent data container
  */
  class track : public data_base {
    
  public:
    
    /// Default constructor
    track() : data_base(data::kTrack) { clear_data(); }
    
    /// Default destructor
    virtual ~track(){}
    
    inline Int_t     track_id      ()               const { return fID;                  }
    inline size_t    n_point       ()               const { return fDir.size();          }
    inline size_t    n_covariance  ()               const { return fCov.size();          }
    inline size_t    n_momentum    ()               const { return fFitMomentum.size();  }
    inline size_t    n_dQdx        ()               const { return fdQdx.size();         }
    
    inline const TVector3& direction_at  (UInt_t p) const { return fDir.at(p);           }
    inline const TVector3& vertex_at     (UInt_t p) const { return fXYZ.at(p);           }
    inline       Double_t    momentum_at (UInt_t p) const { return fFitMomentum.at(p);   }
    inline const TMatrixD& covariance_at (UInt_t p) const { return fCov.at(p);           }
    
    inline void set_track_id   (const Int_t id)     { fID = id;                  }
    inline void add_vertex     (const TVector3 v) { fXYZ.push_back(v);         }
    inline void add_direction  (const TVector3 v) { fDir.push_back(v);         }
    inline void add_momentum   (const Double_t   v) { fFitMomentum.push_back(v); }
    inline void add_covariance (const TMatrixD m) { fCov.push_back(m);         }
    
    virtual void clear_data();
    
  protected:
    
    /// track's ID
    Int_t fID;            
    
    /// position of points along the track
    std::vector<TVector3> fXYZ;
    
    /// direction at each point along the track
    std::vector<TVector3> fDir;
    
    /** 
	covariance matrix of positions
	possibly only end points are stored
    */
    std::vector<TMatrixD> fCov;
    
    /**
       charge deposition per unit length at points
       along track outer vector index is over view
    */
    std::vector< std::vector <Double_t> > fdQdx;
    
    /**
       momentum at start and end of track
       determined from a fit. Intermediate points
       can be added if desired
    */
    std::vector<Double_t> fFitMomentum;
    
  private:
    
    
    ////////////////////////
    ClassDef(track,2)
    ////////////////////////
      
  };

  /**
     \class event_track 
     A collection storage class of multiple tracks.
  */
  class event_track : public std::vector<larlite::track>, public event_base {
    
  public:
    
    /// Default constructor
    event_track(std::string name="noname") : event_base(data::kTrack,name) { clear_data(); }
    
    /// Default copy constructor
    event_track(const event_track& original) : std::vector<larlite::track>(original),
					       event_base(original)
    {}
    
    /// Default destructor
    ~event_track(){}
    
    /// Method to clear currently held data contents in the buffer
    virtual void clear_data(){event_base::clear_data(); clear();}
    
  private:
    
    ////////////////////////
    ClassDef(event_track,3)
    ////////////////////////
  };
}
#endif

/** @} */ // end of doxygen group 
