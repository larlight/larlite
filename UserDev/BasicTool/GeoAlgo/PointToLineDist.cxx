#ifndef POINTTOLINEDIST_CXX
#define POINTTOLINEDIST_CXX

#include "PointToLineDist.h"
#include <sstream>
namespace geoalgo {

  double PointToLineDist::Distance(const std::vector<double> &point,
				   const std::vector<double> &segmentStart,
				   const std::vector<double> &segmentEnd) const
  {

    /*
    if ( (_point.size() == 0) or (_segmentStart.size() == 0) or (_segmentEnd.size() == 0) ){
      std::cout << "Error: point and segment not initialized." << std::endl;
      return 0;
    }
    
    if ( (_point.size() != 3) or (_segmentStart.size() != 3) or (_segmentEnd.size() != 3) ){
      std::cout << "Error: Did not provide 3D points." << std::endl;
      return 0;
    }
    */
    //Distance Algorithm: see "Real-Time Collision Analysis" book, section 5.1.2.1

    if(point.size() != segmentStart.size() || point.size() != segmentEnd.size()) {

      std::ostringstream msg;
      msg << "<<" << __FUNCTION__ << ">>"
	  << " Unmatched length!"
	  << " point=" << point.size() 
	  << " segmentStart=" <<segmentStart.size()
	  << " segmentEnd=" << segmentEnd.size()
	  << std::endl;
      throw GeoAlgoException(msg.str());
      
    }
    
    std::vector<double> line; // End-Start
    std::vector<double> pToStart; // point - Start
    std::vector<double> pToEnd;  // point - End

    line.reserve(point.size());
    pToStart.reserve(point.size());
    pToEnd.reserve(point.size());

    for (size_t i=0; i < point.size(); i++){
      line.push_back     ( segmentEnd[i] - segmentStart[i] );
      pToStart.push_back ( point[i]      - segmentStart[i] );
      pToEnd.push_back   ( point[i]      - segmentEnd[i]   );
    }
    
    double e = DotProduct(pToStart,line);
    double segLenSquared = DotProduct(line,line);
    if (e <= 0.) return DotProduct(pToStart,pToStart);
    else if (e > segLenSquared) return DotProduct(pToEnd,pToEnd);
    else return DotProduct(pToStart,pToStart)-(e*e)/segLenSquared;
    
  }
  
  
  double PointToLineDist::DistanceToTrack(const std::vector<double> &p, 
					  const std::vector<std::vector<double> > &track) const
  {
    /*
      if (p.size() != 3){
      std::cout << "Gave non-3D point...exiting and returning -1" << std::endl;
      return -1;
      }
      for (size_t i=0; i < track.size(); i++){
      if ( track.at(i).size() != 3 ){
      std::cout << "Gave non-3D point...exiting and returning -1" << std::endl;
      return -1;
      }
      }
    */
    //now that we checked the dimension of the points...we can perform our calculation!
    
    //keep track of shortest disntace
    double dist = 9999999.;
    if(!track.size()) return dist;
    for (size_t i=0; i < track.size()-1; i++){
      double distTmp = Distance(p, track[i], track[i+1]);
      if ( distTmp < dist )
	dist = distTmp;
    }
    
    return dist;
  }
  
  double PointToLineDist::DistanceToListOfTracks(const std::vector<double> &p, 
						 const std::vector<std::vector<std::vector<double> > > &trackList) const
  {
    
    //keep track of shortest disntace
    double dist = 9999999.;
    for (size_t j=0; j < trackList.size(); j++){
      for (size_t i=0; i < trackList[j].size()-1; i++){
	double distTmp = Distance(p, trackList[j][i], trackList[j][i+1]);
	if ( distTmp < dist )
	  dist = distTmp;
      }//for all segments in track
    }//for all tracks in trackList
    
    return dist;
  }
  
  double PointToLineDist::DistanceToTrack(const std::vector<double> &p, 
					  const std::vector<std::vector<double> > &track, 
					  const double &bufferDist) const
  {
    
    // This algorithm first checks that any of the trakectory points are closer to p than bufferDist amount...
    // if so then calculates actual distance...should hopefully make things faster
    
    /*
    if (p.size() != 3){
      std::cout << "Gave non-3D point...exiting and returning -1" << std::endl;
      return -1;
    }
    for (size_t i=0; i < track.size(); i++){
      if ( track.at(i).size() != 3 ){
	std::cout << "Gave non-3D point...exiting and returning -1" << std::endl;
	return -1;
      }
    }
    */
    //now that we checked the dimension of the points...we can perform our calculation!
    
    //keep track of shortest disntace
    double dist = 9999999.;
    if(!track.size()) return dist;

    if(p.size()<3) {
      std::ostringstream msg;
      msg << "<<" << __FUNCTION__ << ">>"
	  << "point length is < 3!" << std::endl;
      throw GeoAlgoException(msg.str());
    }

    for (size_t i=0; i < track.size()-1; i++) {
      //if point i is closer then buffer-region
      if ( ( (p[0]-track[i][0])*(p[0]-track[i][0]) + 
	     (p[1]-track[i][1])*(p[1]-track[i][1]) + 
	     (p[2]-track[i][2])*(p[2]-track[i][2]) ) < (bufferDist*bufferDist) ){
	double distTmp = Distance(p, track[i], track[i+1]);
	if ( distTmp < dist )
	  dist = distTmp;
      }//if within buffer-distance
    }//for all track points
    
    return dist;
  }
  
  double PointToLineDist::DotProduct(const std::vector<double> &A, 
				     const std::vector<double> &B) const
  {
    
    /*
    if (A->size() != B->size()){
      std::cout << "Error: Did not provide equal-dimension points" << std::endl;
      return 0;
    }
    */
    if(A.size() != B.size()) {
      std::ostringstream msg;
      msg << "<<" << __FUNCTION__ << ">>"
	  << " incompatible length "
	  << "A:" << A.size()
	  << " != "
	  << "B:" << B.size() <<std::endl;

      throw GeoAlgoException(msg.str());
    }

    double dotProd = 0.;
    for (size_t i=0; i < A.size(); i++)
      dotProd += A[i]*B[i];
    
    return dotProd;
  }
  
  void PointToLineDist::TestDist() const
  {
    
    bool cont = true;
    int next;
    
    std::vector<double> point(3,1);
    std::vector<double> A;
    std::vector<double> B;
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
      
      double D = Distance(point,A,B);
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
