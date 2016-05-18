#ifndef STARTPOINTPANDORAMODULE_CXX
#define STARTPOINTPANDORAMODULE_CXX

#include "StartPointPandora.h"

namespace showerreco {

StartPointPandora::StartPointPandora() {
  _name = "StartPointPandora";
  _range = 5.;
  return;
}

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

    // This is mainly aiming to get the right vertex from a merged pfparticle
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

    // Now focus on the situation where pandora doesn't find the vertex in the
    // shower starting region (or it finds a vertex in the end of the shower)
    TVector3 candStart = resultShower.fCentroid + 0.5*resultShower.fLength*resultShower.fDCosStart.Unit();
    // if ( _range > 0.5*resultShower.fLength ) _range = 0.5*resultShower.fLength;
    _range = resultShower.fLength / 4.;

    double spreadStart = 0., spreadCandStart = 0.;
    int nStart = 0, nCandStart = 0;
    for ( auto & sps : sortedSPS ) {
      TVector3 p( sps.X(), sps.Y(), sps.Z() );
      TVector3 pp = p - resultShower.fXYZStart;
      TVector3 projP = resultShower.fXYZStart + PointProjectedToLine( pp, resultShower.fDCosStart );
      TVector3 diffStart = projP - resultShower.fXYZStart;
      TVector3 diffCandStart = projP - candStart;
      if ( diffStart.Mag() < _range ) {
        spreadStart += pow( DCAPointToLine( p, resultShower.fXYZStart, resultShower.fDCosStart ), 2 );
        nStart += 1;
      } else if ( diffCandStart.Mag() < _range ) {
        spreadCandStart += pow( DCAPointToLine( p, candStart, resultShower.fDCosStart ), 2 );
        nCandStart += 1;
      }
    }
    std::cout << "  Starting point: spread: " << spreadStart/(double)nStart << " ( " << spreadStart << " / "
              << nStart << " )" << std::endl;
    std::cout << "  End point: spread: " << spreadCandStart/(double)nCandStart << " ( " << spreadCandStart
              << " / " << nCandStart << " )" << std::endl;
    double aveSpreadCandStart = spreadCandStart/(double)nCandStart;
    double aveSpreadStart = spreadStart/(double)nStart;
    if ( aveSpreadCandStart + sqrt(aveSpreadCandStart) < aveSpreadStart || ( nStart == 0 && nCandStart > 0 ) ) {
      resultShower.fXYZStart = candStart;
      resultShower.fDCosStart *= -1.;
      std::cout << "  Reversed!" << std::endl;
    }

  }
  else {
    throw ShowerRecoException("StartPointPandora requires vertexes but has none.");
  }
}

TVector3 StartPointPandora::PointProjectedToLine( const TVector3& point, const TVector3& line ) {
  return ( point * line / ( line * line ) ) * line;
}

double StartPointPandora::DCAPointToLine( const TVector3& p, const TVector3& c, const TVector3& m ) {
  TVector3 diff = c - p;
  double t = -1.*diff*m/ m.Mag2();
  TVector3 dca = c + t*m - p;
  return dca.Mag();
}

} //showerreco

#endif
