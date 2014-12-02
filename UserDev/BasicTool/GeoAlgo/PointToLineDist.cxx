#ifndef POINTTOLINEDIST_CXX
#define POINTTOLINEDIST_CXX

#include "PointToLineDist.h"
#include <sstream>
namespace geoalgo {

  double PointToLineDist::Distance(const Point_t &point,
				   const Point_t &segmentStart,
				   const Point_t &segmentEnd) const
  {
    auto line     = segmentEnd - segmentStart;
    auto pToStart = point - segmentStart;
    auto pToEnd   = point - segmentEnd;

    double e = pToStart * line;
    double segLenSquared = line.Length();
    if (e <= 0.) return pToStart.Length();
    else if (e > segLenSquared) return pToEnd.Length();
    else return pToStart.Length() - (e*e)/segLenSquared;
    
  }
  
  
  double PointToLineDist::DistanceToTrack(const Point_t &p,
					  const Trajectory_t &trk) const
  {
    trk.compat(p);
    //keep track of shortest disntace
    double dist = std::numeric_limits<double>::max();
    auto npoints = trk.size();

    if(!npoints) return dist;
    if(npoints==1) return trk[0]._Dist_(p);

    for (size_t i=0; i < trk.size()-1; i++){
      double distTmp = Distance(p, trk[i], trk[i+1]);
      if ( distTmp < dist )
	dist = distTmp;
    }
    
    return dist;
  }
  
  double PointToLineDist::DistanceToListOfTracks(const Point_t &p,
						 const std::vector<Trajectory_t> &trkList) const
  {
    
    //keep track of shortest disntace
    double dist = std::numeric_limits<double>::max();
    for(auto const& trk : trkList) {
      double tmpdist = DistanceToTrack(p,trk);
      if(tmpdist < dist) dist = tmpdist;
      
    }
    
    return dist;
  }
  
  double PointToLineDist::DistanceToTrack(const Point_t &p,
					  const Trajectory_t &trk,
					  const double &bufferDist) const
  {
    
    // This algorithm first checks that any of the trakectory points are closer to p than bufferDist amount...
    // if so then calculates actual distance...should hopefully make things faster

    trk.compat(p);
    
    //keep track of shortest disntace
    double dist = std::numeric_limits<double>::max();
    if(!trk.size()) return dist;
    if(trk.size()==1) return trk[0]._Dist_(p);

    double bufferDistSquared = pow(bufferDist,2);
    for (size_t i=0; i < trk.size()-1; i++) {
      double tmpdist = trk[i]._SqDist_(p);
      if(tmpdist < bufferDistSquared) {
	
	tmpdist = Distance(p,trk[i],trk[i+1]);
	if(tmpdist < dist) dist = tmpdist;
	
      }//if within buffer-distance
    }//for all track points
    
    return dist;
  }
  
  void PointToLineDist::TestDist() const
  {
    
    bool cont = true;
    int next;
    
    Point_t pt(3,1);
    Point_t A, B;
    double Ax,Ay,Az,Bx,By,Bz;
    
    while (cont){
      
      std::cout << "A - x coord:" << std::endl;
      std::cin >> Ax;
      std::cout << "A - y coord:" << std::endl;
      std::cin >> Ay;
      std::cout << "A - z coord:" << std::endl;
      std::cin >> Az;
      std::cout << "B - x coord:" << std::endl;
      std::cin >> Bx;
      std::cout << "B - y coord:" << std::endl;
      std::cin >> By;
      std::cout << "B - z coord:" << std::endl;
      std::cin >> Bz;
      A.clear();
      B.clear();
      A.push_back(Ax);
      A.push_back(Ay);
      A.push_back(Az);
      B.push_back(Bx);
      B.push_back(By);
      B.push_back(Bz);
      
      double D = Distance(pt,A,B);
      std::cout << "Calculated distance is: " << D << std::endl;
      
      std::cout << std::endl << "Next Event? 1: Yes, 0: No." << std::endl;
      std::cin >> next;
      if (next) { cont = true; }
      else { cont = false; }
    }
    
  return;
  }
  
}
#endif
