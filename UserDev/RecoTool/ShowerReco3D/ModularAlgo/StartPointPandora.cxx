#ifndef STARTPOINTPANDORAMODULE_CXX
#define STARTPOINTPANDORAMODULE_CXX

#include "StartPointPandora.h"

namespace showerreco {

void StartPointPandora::do_reconstruction(
  const ::protoshower::ProtoShower & proto_shower,
  Shower_t& resultShower) {


  // This is really simple.  Read in the 3D params and use that to
  // set the 3D start point of the shower from a vertex.
  // Right now, no checks against the
  // 2D projection but should be added somewhere.

  // Also, just taking the first vertex but could make a selection for
  // the best one.

  // If there is no 3D params, throw an exception.
  //
  if (proto_shower.hasVertex()) {

    double dir = 1.;
    auto const& sps = proto_shower.params3D().point_vector;

    auto vertexes = proto_shower.vertexes();
    auto pca = proto_shower.params3D().principal_dir;

    std::sort( vertexes.begin(), vertexes.end(),
         [ &pca ]( TVector3 const & a, TVector3 const & b) { return (a * pca) < (b * pca); } );

    auto sortedSPS = sps;
    std::sort( sortedSPS.begin(), sortedSPS.end(),
               [ &pca ]( ::cluster3D::Point3D const & a, ::cluster3D::Point3D const & b )
         { return ( a.X() * pca.X() + a.Y() * pca.Y() + a.Z() * pca.Z() ) < 
                  ( b.X() * pca.X() + b.Y() * pca.Y() + b.Z() * pca.Z() ); } );

    TVector3 firstSPS( sortedSPS.front().X(), sortedSPS.front().Y(), sortedSPS.front().Z() );
    TVector3 lastSPS( sortedSPS.back().X(), sortedSPS.back().Y(), sortedSPS.back().Z() );

    if ( firstSPS * pca >= vertexes.front() * pca && lastSPS * pca > vertexes.back() * pca ) {
      resultShower.fXYZStart = vertexes.front();
    } else if ( firstSPS * pca < vertexes.front() * pca && lastSPS * pca <= vertexes.back() * pca ) {
      resultShower.fXYZStart = vertexes.back();
      dir = -1.;
    } else {
      // std::cout << "SPS in both sides are within or without the vertices!" << std::endl;
      TVector3 frontStretch = firstSPS - vertexes.front();
      TVector3 backStretch = lastSPS - vertexes.back();
      if ( frontStretch.Mag() < backStretch.Mag() ) resultShower.fXYZStart = vertexes.front();
      else {
        resultShower.fXYZStart = vertexes.back();
        dir = -1.;
      }
    }

    if ( dir < 0. ) resultShower.fDCosStart *= dir;

  }
  else {
    throw ShowerRecoException("StartPointPandora requires vertexes but has none.");
  }
}

TVector3 StartPointPandora::PointProjectedToLine( const TVector3& point, const TVector3& line ) {
  return ( point * line / ( line * line ) ) * line;
}

} //showerreco

#endif
