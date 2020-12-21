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

#ifndef LARLITE_TRACK_H
#define LARLITE_TRACK_H

#include "data_base.h"
#include "Base/GeoTypes.h"
#include "Base/GeoConstants.h"
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

    track(const track& original) :   data_base(original),
				     fXYZ(original.fXYZ),
				     fDir(original.fDir),
				     fCov(original.fCov),
				     fdQdx(original.fdQdx),
				     fFitMomentum(original.fFitMomentum),
				     fID(original.fID)
    {}
    
    /// Default destructor
    virtual ~track(){}

    virtual void clear_data();
    void set_track_id   (const int id);
    void add_vertex     (const TVector3 v);
    void add_direction  (const TVector3 v);
    void add_momentum   (const double   v);
    void add_covariance (const TMatrixD m);
    void add_dqdx       (const std::vector<double>& v);
    void reserve(size_t n);
    void            Extent(std::vector<double> &xyzStart,
			   std::vector<double> &xyzEnd)        const;
    void            Direction(double *dcosStart,
			      double *dcosEnd)                 const;
    double          ProjectedLength(geo::View_t view)          const;
    double          PitchInView(geo::View_t view,
				size_t trajectory_point=0)     const;
    int             ID()                                       const;
    // A trajectory point is the combination of a position vector
    // and its corresponding direction vector
    size_t          NumberTrajectoryPoints()                    const;
    size_t          NumberCovariance()                          const;
    size_t          NumberFitMomentum()                         const;
    //#ifndef __CINT__
    //#ifndef __CLING__
    size_t          NumberdQdx(geo::View_t view=geo::kUnknown)  const;
    const double&   DQdxAtPoint(unsigned int p,
				geo::View_t view=geo::kUnknown) const;
    //#endif
    //#endif
    double          Length(size_t p=0)                          const;
    void            TrajectoryAtPoint(unsigned int  p,
				      TVector3     &pos,
				      TVector3     &dir)        const;
    const TVector3& DirectionAtPoint (unsigned int p)           const;
    const TVector3& LocationAtPoint  (unsigned int p)           const;
    const double&   MomentumAtPoint  (unsigned int p)           const;
    const TMatrixD& CovarianceAtPoint(unsigned int p)           const;

    const TVector3& Vertex()                                    const;
    const TVector3& End()                                     	const;
    const TVector3& VertexDirection()                         	const;
    const TVector3& EndDirection()                            	const;
    const TMatrixD& VertexCovariance()                        	const;
    const TMatrixD& EndCovariance()                           	const;
    const double&   VertexMomentum()                          	const;
    const double&   EndMomentum()                             	const;

    double          Theta()                                     const;
    double          Phi()                                       const;

    // Calculate rotation matrices between global (x,y,z) and local (u,v,w)
    // coordinate systems based on track direction (fDir).
    // The local w-axis points along the track direction.
    void GlobalToLocalRotationAtPoint(unsigned int p, TMatrixD& rot) const;
    void LocalToGlobalRotationAtPoint(unsigned int p, TMatrixD& rot) const;

    friend bool     operator <   (const track & a, const track & b);
  protected:

    std::vector<TVector3>               fXYZ;           ///< position of points along the track
    std::vector<TVector3>               fDir;           ///< direction at each point along the track
    std::vector<TMatrixD >              fCov;           ///< covariance matrix of positions
                                                        ///< possibly only end points are stored
    std::vector< std::vector <double> > fdQdx;          ///< charge deposition per unit length at points
                                                        ///< along track outer vector index is over view
    std::vector<double>                 fFitMomentum;   ///< momentum at start and end of track
                                                        ///< determined from a fit. Intermediate points
                                                        ///< can be added if desired
    int                                 fID;            ///< track's ID

  private:
    
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
    
  };
}
#endif

/** @} */ // end of doxygen group 
