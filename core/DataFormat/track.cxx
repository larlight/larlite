#ifndef LARLITE_TRACK_CXX
#define LARLITE_TRACK_CXX

#include <exception>
#include "track.h"
#include "LArUtil/Geometry.h"
namespace larlite {

  //########################################
  void track::clear_data()
  //########################################
  {
    data_base::clear_data();  
    fID = -1; 
    fXYZ.clear(); 
    fDir.clear(); 
    fCov.clear(); 
    fdQdx.clear();
    fFitMomentum.clear(); 
  }

  void track::set_track_id   (const int id)     { fID = id;                  }
  void track::add_vertex     (const TVector3 v) { fXYZ.push_back(v);         }
  void track::add_direction  (const TVector3 v) { fDir.push_back(v);         }
  void track::add_momentum   (const double   v) { fFitMomentum.push_back(v); }
  void track::add_covariance (const TMatrixD m) { fCov.push_back(m);         }
  void track::add_dqdx       (const std::vector<double>& v) { fdQdx.push_back(v);  }
  void track::reserve(size_t n)
  {
    fXYZ.reserve(n);
    fDir.reserve(n);
    fFitMomentum.reserve(n);
    fCov.reserve(n);
    fdQdx.reserve(n);
  }

  int             track::ID()                               const { return fID;                  }
  size_t          track::NumberTrajectoryPoints()           const { return fDir.size();          }
  size_t          track::NumberCovariance()                 const { return fCov.size();          }
  size_t          track::NumberFitMomentum()                const { return fFitMomentum.size();  }
  const TVector3& track::DirectionAtPoint (unsigned int p)  const { return fDir.at(p);           }
  const TVector3& track::LocationAtPoint  (unsigned int p)  const { return fXYZ.at(p);           }
  const double&   track::MomentumAtPoint  (unsigned int p)  const { return fFitMomentum.at(p);   }
  const TMatrixD& track::CovarianceAtPoint(unsigned int p)  const { return fCov.at(p);           }

  const TVector3& track::Vertex()                           const { return fXYZ.front();         }
  const TVector3& track::End()                              const { return fXYZ.back();          }
  const TVector3& track::VertexDirection()                  const { return fDir.front();         }
  const TVector3& track::EndDirection()                     const { return fDir.back();          }
  const TMatrixD& track::VertexCovariance()                 const { return fCov.front();         }
  const TMatrixD& track::EndCovariance()                    const { return fCov.back();          }
  const double&   track::VertexMomentum()                   const { return fFitMomentum.front(); }
  const double&   track::EndMomentum()                      const { return fFitMomentum.back();  }
  double          track::Theta()                            const { return fXYZ.front().Theta(); }
  double          track::Phi()                              const { return fXYZ.front().Phi();   }

  //----------------------------------------------------------------------
  size_t track::NumberdQdx(geo::View_t view) const
  {
    if (fdQdx.size() == 0) return 0;
    if(view == geo::kUnknown){
      std::cout<<"<<Track::NumberdQdx>> asking for unknown view to get number of dQdX entries"
	       << " return the size for the 0th view vector";
      return fdQdx.at(0).size();
    }

    return fdQdx.at(view).size();
  }

  //----------------------------------------------------------------------
  const double& track::DQdxAtPoint(unsigned int p,
				   geo::View_t view) const
  {
    if(view == geo::kUnknown){
      std::cout<<"<<Track::DQdxAtPoint>> asking for unknown view to get number of dQdX entries"
	       << " return the size for the 0th view vector";
      return fdQdx.at(p).at(0);
    }

    return fdQdx.at(p).at(view);
  }

  //----------------------------------------------------------------------
  void track::Extent(std::vector<double> &xyzStart,
		     std::vector<double> &xyzEnd) const
  {
    xyzStart.resize(3);
    xyzEnd.resize(3);

    xyzStart[0] = fXYZ.front().X();
    xyzStart[1] = fXYZ.front().Y();
    xyzStart[2] = fXYZ.front().Z();

    xyzEnd[0]   = fXYZ.back().X();
    xyzEnd[1]   = fXYZ.back().Y();
    xyzEnd[2]   = fXYZ.back().Z();

    return;
  }

  //----------------------------------------------------------------------
  void track::Direction(double *dcosStart,
			double *dcosEnd) const
  {
    dcosStart[0] = fDir.front().X();
    dcosStart[1] = fDir.front().Y();
    dcosStart[2] = fDir.front().Z();

    dcosEnd[0]   = fDir.back().X();
    dcosEnd[1]   = fDir.back().Y();
    dcosEnd[2]   = fDir.back().Z();

    return;
  }

  //----------------------------------------------------------------------
  double track::ProjectedLength(geo::View_t view) const
  {
    ///\todo CAREFUL: using view to determine projected length does not work for LBNE
    ///\todo need to think more about this
    if(view == geo::kUnknown) {
      std::cerr<< "<<Track::ProjectedLength>> "
	       << "cannot provide projected length for "
	       << "unknown view\n";
      throw std::exception();
    }

    double length = 0.;

    //art::ServiceHandle<geo::Geometry> geo;
    auto geo = ::larutil::Geometry::GetME();
    double angleToVert = 0.;
    for(unsigned int i = 0; i < geo->Nplanes(); ++i){
      if(geo->PlaneToView(i) == view){
	angleToVert = geo->WireAngleToVertical(view) - 0.5*TMath::Pi();
	break;
      }
    }

    // now loop over all points in the trajectory and add the contribution to the
    // to the desired view
    for(size_t p = 1; p < fXYZ.size(); ++p){
      double dist = std::sqrt( pow(fXYZ[p].x() - fXYZ[p-1].x(), 2) +
			  pow(fXYZ[p].y() - fXYZ[p-1].y(), 2) +
			  pow(fXYZ[p].z() - fXYZ[p-1].z(), 2) );
      
      // (sin(angleToVert),cos(angleToVert)) is the direction perpendicular to wire
      // fDir[i-1] is the direction between the two relevant points
      double cosgamma = TMath::Abs(TMath::Sin(angleToVert)*fDir[p-1].Y() +
				   TMath::Cos(angleToVert)*fDir[p-1].Z() );
      
      /// \todo is this right, or should it be dist*cosgamma???
      length += dist/cosgamma;
    } // end loop over distances between trajectory points

    return length;
  }

  //----------------------------------------------------------------------
  // provide projected wire pitch for the view
  // by default, gives pitch at the beginning of the trajectory
  double track::PitchInView(geo::View_t view,
			    size_t trajectory_point) const
  { 
    if(view == geo::kUnknown) {
      std::cerr << "<<Track::PitchInView>> Warning cannot obtain pitch for unknown view\n" << std::endl;
      throw std::exception();
    }
    
    if(trajectory_point > fDir.size()) {
      std::cerr << "<<Track::PitchInView>> ERROR: Asking for trajectory point " 
		<< trajectory_point
		<< " when direction vector size is " 
		<< fDir.size() << ".\n";
      throw std::exception();
    }
    if(trajectory_point > fXYZ.size()) {
      std::cerr << "<<Track::PitchInView>> ERROR: Asking for trajectory point " 
		<< trajectory_point
		<< " when XYZ vector size is " 
		<< fXYZ.size() << ".\n";
      throw std::exception();
    }
    //art::ServiceHandle<geo::Geometry> geo;
    auto geo = ::larutil::Geometry::GetME();
    //int TPC  = 0;
    //int Cryo = 0;
    //double Position[3];
    //Position[0] = fXYZ[trajectory_point].X();
    //Position[1] = fXYZ[trajectory_point].Y();
    //Position[2] = fXYZ[trajectory_point].Z();
    /*
    geo::TPCID tpcid = geo->FindTPCAtPosition ( Position );
    if (tpcid.isValid) {
      TPC  = tpcid.TPC;
      Cryo = tpcid.Cryostat;
    }
    double wirePitch   = geo->WirePitch(view, TPC, Cryo);
    double angleToVert = geo->WireAngleToVertical(view, TPC, Cryo) - 0.5*TMath::Pi();
    */
    double wirePitch   = geo->WirePitch(view);
    double angleToVert = geo->WireAngleToVertical(view) - 0.5*TMath::Pi();

    //(sin(angleToVert),cos(angleToVert)) is the direction perpendicular to wire
    double cosgamma = std::abs(std::sin(angleToVert)*fDir[trajectory_point].Y() +
			       std::cos(angleToVert)*fDir[trajectory_point].Z());

    if(cosgamma < 1.e-5) {
      std::cerr << "<<Track::PitchInView>> cosgamma is basically 0, that can't be right\n";
      throw std::exception();
    }

    return wirePitch/cosgamma;
  }

  //----------------------------------------------------------------------
  // This is a simple summation of the distance between consecutive
  // points on the track starting with the p-th point
  // It assumes that there are sufficient points to make a
  // reasonable measurement of the length
  // This method can be combined with a particle id hypothesis to
  // get an estimate of momentum from range.
  double track::Length(size_t p) const
  {
    double length = 0.;

    for(size_t i = p+1; i < fXYZ.size(); ++i)
      length += std::sqrt( pow(fXYZ[i].x() - fXYZ[i-1].x(), 2) +
			   pow(fXYZ[i].y() - fXYZ[i-1].y(), 2) +
			   pow(fXYZ[i].z() - fXYZ[i-1].z(), 2) );
    return length;
  }

  //----------------------------------------------------------------------
  // distance from point p on the trajectory to the end of the track



  //----------------------------------------------------------------------
  void track::TrajectoryAtPoint(unsigned int p,
				TVector3    &pos,
				TVector3    &dir) const
  {
    pos = fXYZ.at(p);
    dir = fDir.at(p);

    return;
  }

  //----------------------------------------------------------------------
  // Fill the global-to-local rotation matrix based on direction at point fDir[p].
  // The third axis of the local coordinate system points along the track direction.
  void track::GlobalToLocalRotationAtPoint(unsigned int p, TMatrixD& rot) const
  {
    // Make sure matrix has the correct size.

    if(rot.GetNrows() != 3 || rot.GetNcols() != 3)
      rot.ResizeTo(3,3);

    // Calculate the global-to-local rotation matrix.

    const TVector3& dir = fDir.at(p);
    double dirmag = dir.Mag();
    double diryz = std::sqrt(dir.Y()*dir.Y() + dir.Z()*dir.Z());

    double sinth = dir.X() / dirmag;
    double costh = diryz / dirmag;
    double sinphi = 0.;
    double cosphi = 1.;
    if(diryz != 0) {
      sinphi = -dir.Y() / diryz;
      cosphi = dir.Z() / diryz;
    }
    rot(0,0) = costh;
    rot(1,0) = 0.;
    rot(2,0) = sinth;
    rot(0,1) = sinth * sinphi;
    rot(1,1) = cosphi;
    rot(2,1) = -costh * sinphi;
    rot(0,2) = -sinth * cosphi;
    rot(1,2) = sinphi;
    rot(2,2) = costh * cosphi;
  }

  //----------------------------------------------------------------------
  // Fill the local-to-global rotation matrix based on direction at point fDir[p].
  // The third axis of the local coordinate system points along the track direction.
  void track::LocalToGlobalRotationAtPoint(unsigned int p, TMatrixD& rot) const
  {
    // Make sure matrix has the correct size.

    if(rot.GetNrows() != 3 || rot.GetNcols() != 3)
      rot.ResizeTo(3,3);

    // Calculate the global-to-local rotation matrix.

    const TVector3& dir = fDir.at(p);
    double dirmag = dir.Mag();
    double diryz = std::sqrt(dir.Y()*dir.Y() + dir.Z()*dir.Z());

    double sinth = dir.X() / dirmag;
    double costh = diryz / dirmag;
    double sinphi = 0.;
    double cosphi = 1.;
    if(diryz != 0) {
      sinphi = -dir.Y() / diryz;
      cosphi = dir.Z() / diryz;
    }
    rot(0,0) = costh;
    rot(0,1) = 0.;
    rot(0,2) = sinth;
    rot(1,0) = sinth * sinphi;
    rot(1,1) = cosphi;
    rot(1,2) = -costh * sinphi;
    rot(2,0) = -sinth * cosphi;
    rot(2,1) = sinphi;
    rot(2,2) = costh * cosphi;
  }

  //----------------------------------------------------------------------------
  bool operator < (const track & a, const track & b)
  {
    if(a.ID() != b. ID())
      return a.ID() < b.ID();

    return false; //They are equal
  }    
  
}
#endif
