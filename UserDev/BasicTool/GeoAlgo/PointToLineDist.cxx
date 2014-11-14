#ifndef POINTTOLINEDIST_CXX
#define POINTTOLINEDIST_CXX

#include "PointToLineDist.h"

namespace geoalgo {

  double PointToLineDist::Distance(std::vector<double> *point,
				   std::vector<double> *segmentStart,
				   std::vector<double> *segmentEnd){

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
    
    std::vector<double> line; // End-Start
    std::vector<double> pToStart; // point - Start
    std::vector<double> pToEnd;  // point - End
    
    for (size_t i=0; i < point->size(); i++){
      line.push_back(segmentEnd->at(i)-segmentStart->at(i));
      pToStart.push_back(point->at(i)-segmentStart->at(i));
      pToEnd.push_back(point->at(i)-segmentEnd->at(i));
    }
    
    double e = DotProduct(&pToStart,&line);
    double segLenSquared = DotProduct(&line,&line);
    if (e <= 0.) return DotProduct(&pToStart,&pToStart);
    else if (e > segLenSquared) return DotProduct(&pToEnd,&pToEnd);
    else return DotProduct(&pToStart,&pToStart)-(e*e)/segLenSquared;
    
  }
  
  
  double PointToLineDist::DistanceToTrack(std::vector<double> *p, std::vector<std::vector<double> > *track){
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
    for (size_t i=0; i < track->size()-1; i++){
      double distTmp = Distance(p, &(track->at(i)), &(track->at(i+1)));
      if ( distTmp < dist )
	dist = distTmp;
    }
    
    return dist;
  }
  
  
  double PointToLineDist::DistanceToListOfTracks(std::vector<double> *p, std::vector<std::vector<std::vector<double> > > *trackList){
    
    //keep track of shortest disntace
    double dist = 9999999.;
    for (size_t j=0; j < trackList->size(); j++){
      for (size_t i=0; i < trackList->at(j).size()-1; i++){
	double distTmp = Distance(p, &(trackList->at(j).at(i)), &(trackList->at(j).at(i+1)));
	if ( distTmp < dist )
	  dist = distTmp;
      }//for all segments in track
    }//for all tracks in trackList
    
    return dist;
  }
  
  
  double PointToLineDist::DistanceToTrack(std::vector<double> *p, std::vector<std::vector<double> > *track, double bufferDist){
    
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
    for (size_t i=0; i < track->size()-1; i++){
      //if point i is closer then buffer-region
      if ( ( (p->at(0)-track->at(i).at(0))*(p->at(0)-track->at(i).at(0)) + 
	     (p->at(1)-track->at(i).at(1))*(p->at(1)-track->at(i).at(1)) + 
	     (p->at(2)-track->at(i).at(2))*(p->at(2)-track->at(i).at(2)) ) < (bufferDist*bufferDist) ){
	double distTmp = Distance(p, &(track->at(i)), &(track->at(i+1)));
	if ( distTmp < dist )
	  dist = distTmp;
      }//if within buffer-distance
    }//for all track points
    
    return dist;
  }
  
  
  double PointToLineDist::DotProduct(std::vector<double> *A, std::vector<double> *B){
    
    /*
    if (A->size() != B->size()){
      std::cout << "Error: Did not provide equal-dimension points" << std::endl;
      return 0;
    }
    */

    double dotProd = 0.;
    for (size_t i=0; i < A->size(); i++)
      dotProd += A->at(i)*B->at(i);
    
    return dotProd;
  }
  
  void PointToLineDist::TestDist(){
    
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
      
      double D = Distance(&point,&A,&B);
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
