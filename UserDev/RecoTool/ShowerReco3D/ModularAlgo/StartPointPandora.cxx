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

    /* ************************************************
       Now focus on the situation where pandora doesn't find the vertex in the
       shower starting region (or it finds a vertex in the end of the shower)
    ************************************************ */
/*
    TVector3 start = resultShower.fCentroid - 0.5*resultShower.fLength*resultShower.fDCosStart.Unit();
    TVector3 candStart = resultShower.fCentroid + 0.5*resultShower.fLength*resultShower.fDCosStart.Unit();
    // if ( _range > 0.5*resultShower.fLength ) _range = 0.5*resultShower.fLength;
    constexpr int nSections = 4;
    _range = resultShower.fLength / (double)nSections;

    std::cout << "  Starting: ( " << start[0] << ", " << start[1] << ", " << start[2] << " )" << std::endl;
    std::cout << "  Ending:   ( " << candStart[0] <<  ", " << candStart[1] << ", " << candStart[2] << " )" << std::endl;
    std::cout << "  Center:   ( " << resultShower.fCentroid[0] << ", " << resultShower.fCentroid[1] << ", "
              << resultShower.fCentroid[2] << " )" << std::endl;

    double spreads[nSections], spreadsDOF[nSections];
    int nSpreads[nSections];
    TVector3 centroids[nSections];
    for ( int iRange = 0; iRange < nSections; ++iRange ) {
      spreads[iRange] = 0.;
      nSpreads[iRange] = 0;
      spreadsDOF[iRange] = 0.;
      centroids[iRange].SetXYZ( 0., 0., 0. );
    }

    // Calculate the centroid for each section
    for ( size_t isp = 0; isp < sortedSPS.size(); ++isp ) {
      auto & sps = sortedSPS[isp];
      TVector3 p( sps.X(), sps.Y(), sps.Z() );
      TVector3 pp = p - resultShower.fCentroid;
      TVector3 projP = resultShower.fCentroid + PointProjectedToLine( pp, resultShower.fDCosStart );
      TVector3 diffStart = projP - start;
      bool counted = false;
      for ( int iRange = 0; iRange < nSections; ++iRange ) {
        if ( diffStart.Mag() < _range*(double)(iRange + 1) ) {
          centroids[iRange][0] += p.X();
          centroids[iRange][1] += p.Y();
          centroids[iRange][2] += p.Z();
          nSpreads[iRange] += 1;
          counted = true;
          break;
        }
      }

      if ( !counted ) {
          centroids[nSections-1][0] += p.X();
          centroids[nSections-1][1] += p.Y();
          centroids[nSections-1][2] += p.Z();
          nSpreads[nSections-1] += 1;
          counted = true;
      }
    }
    for ( int iRange = 0; iRange < nSections; ++iRange ) {
      for ( int i = 0; i < 3; ++i ) centroids[iRange][i] /= (double)nSpreads[iRange];
      std::cout << "  Section " << iRange << ": ( " << centroids[iRange][0] << ", " << centroids[iRange][1]
                << ", " << centroids[iRange][2] << " )" << std::endl;
    }

    // Calculate DCA
    for ( size_t isp = 0; isp < sortedSPS.size(); ++isp ) {
      auto & sps = sortedSPS[isp];
      TVector3 p( sps.X(), sps.Y(), sps.Z() );
      TVector3 pp = p - resultShower.fCentroid;
      TVector3 projP = resultShower.fCentroid + PointProjectedToLine( pp, resultShower.fDCosStart );
      TVector3 diffStart = projP - start;
      TVector3 diffCandStart = projP - candStart;
      bool counted = false;
      for ( int iRange = 0; iRange < nSections; ++iRange ) {
        if ( diffStart.Mag() < _range*(double)(iRange + 1) ) {
          spreads[iRange] += pow( DCAPointToLine( p, centroids[iRange], resultShower.fDCosStart ), 2 );
          counted = true;
          break;
        }
      }
      if ( !counted ) {
          spreads[nSections-1] += pow( DCAPointToLine( p, centroids[nSections-1], resultShower.fDCosStart ), 2 );
          counted = true;
      }
    }

    for ( int iRange = 0; iRange < nSections; ++iRange ) {
      spreadsDOF[iRange] = spreads[iRange]/(double)nSpreads[iRange];
      std::cout << "  " << iRange << ": " << spreadsDOF[iRange] << " ( " << spreads[iRange] << " / " 
                << nSpreads[iRange] << " )" << std::endl;
    }

    for ( int iRange = 0; iRange < nSections; ++iRange ) {
      if ( nSpreads[iRange] == 0 ) continue;
      if ( spreadsDOF[iRange+1] > spreadsDOF[iRange+2] + 0.2 ) {
        resultShower.fXYZStart = candStart;
        resultShower.fDCosStart *= -1.;
        std::cout << "  Reversed!" << std::endl;
      }
      break;
    }
*/
/* v11_08
    int nSlope[2] = { 0, 0 };
    for ( int iRange = 0; iRange < nSections-1; ++iRange ) {
      if ( nSpreads[iRange] == 0 || nSpreads[iRange+1] == 0 ) continue;
      if ( spreadsDOF[iRange] < spreadsDOF[iRange+1] ) nSlope[0] += 1;
      else nSlope[1] += 1;
    }

    if ( nSlope[1] > nSlope[0] ) {
      resultShower.fXYZStart = candStart;
      resultShower.fDCosStart *= -1.;
      std::cout << "  Reversed!" << std::endl;
    }
*/
/* v11_07 before
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
*/
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
