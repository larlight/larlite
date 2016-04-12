#ifndef PCA3DAXISMODULE_CXX
#define PCA3DAXISMODULE_CXX

#include "PCA3DAxis.h"
#include "TMath.h"
// #include <algorithm>

namespace showerreco {
  
  // If there is no 3D params, throw an exception.
  // 
  if (proto_shower.hasCluster3D()){
    double dir = 1.;
    auto const& sps = proto_shower.params3D().point_vector;

    if ( proto_shower.hasVertex() ) {
      auto vertexes = proto_shower.vertexes();
      auto pca = proto_shower.params3D().principal_dir;

      std::sort( vertexes.begin(), vertexes.end(), 
                 [ &pca ]( TVector3 const& a, TVector3 const& b) { return (a*pca) < (b*pca); } );

      auto sortedSPS = sps;
      std::sort( sortedSPS.begin(), sortedSPS.end(),
                 [ &pca ]( ::cluster3D::Point3D const& a, ::cluster3D::Point3D const& b ) 
                 { return ( a.X()*pca.X() + a.Y()*pca.Y() + a.Z()*pca.Z() ) < ( b.X()*pca.X() + b.Y()*pca.Y() + b.Z()*pca.Z() ); } );

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

      // std::cout << "PCA: ( " << pca.X() << ", " << pca.Y() << ", " << pca.Z() << " )" << std::endl;
      // std::cout << "Front vertex: ( " << vertexes.front().X() << ", " << vertexes.front().Y()
      //           << ", " << vertexes.front().Z() << " )" << std::endl;
      // std::cout << "Back vertex: ( " << vertexes.back().X() << ", " << vertexes.back().Y()
      //           << ", " << vertexes.back().Z() << " )" << std::endl;
      // std::cout << "Chosen vertex: ( " << resultShower.fXYZStart.X() << ", " << resultShower.fXYZStart.Y()
      //           << ", " << resultShower.fXYZStart.Z() << " )" << std::endl;
    } else {
      throw ShowerRecoException("PCA3DAxis requires vertex but has none.");
    }
    resultShower.fDCosStart = dir * proto_shower.params3D().principal_dir;
  }
  else{
    throw ShowerRecoException("PCA3DAxis requires 3D cluster but has none.");
  }
}

TVector3 PCA3DAxis::PointProjectedToLine( const TVector3& point, const TVector3& line ) {
  return ( point * line / ( line * line ) ) * line;
}

} //showerreco

#endif
