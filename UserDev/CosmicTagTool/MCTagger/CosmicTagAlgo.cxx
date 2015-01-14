#ifndef COSMICTAGALGO_CXX
#define COSMICTAGALGO_CXX

#include "CosmicTagAlgo.h"

//tracks is list of cosmic-tagged tracks; shr is simplified mcshower
//With some code from David Caratelli's ShowerCalculator.cxx
float CosmicTagAlgo::ShowerTrackScore(std::vector<::geoalgo::Trajectory> &tracks,::geoalgo::HalfLine_t& shr )
{
	double minDist = std::numeric_limits<double>::max();
	double minIP   = std::numeric_limits<double>::max();  
	double Dist 	= -999;
	double IP 		= -999;

	geoalgo::Vector_t c1(3);
	geoalgo::Vector_t c2(3);
	geoalgo::GeoAlgo geoObj ;

    // shr segment starts 3 meters before start point,
    // ends 10 cm after start point aligned with shr momentum. Use for Impact Parameter calc
    geoalgo::LineSegment shrSeg(shr.Start()-shr.Dir()*300,shr.Start()+shr.Dir()*10);

	for(size_t i=0; i < tracks.size(); i++){
		double tmpDist = geoObj.SqDist(shr.Start(),tracks.at(i))  ;
   		double tmpIP   = geoObj.SqDist(tracks.at(i),shrSeg,c1,c2) ;
		
		if(tmpDist < minDist) minDist = tmpDist ;
		if(tmpIP < minIP) minIP = tmpIP ;
      	   
  	   } //for all tracks

	if (minDist != std::numeric_limits<double>::max()){
    	Dist = sqrt(minDist);
    	IP = sqrt(minIP);
		}

	float score=1;

	if(Dist < 100 && IP < 100){
		score *= 1 - (Dist/100) ;
		score *= 1 - (IP/100) ;
		}
	else
		score = 0 ;

	return score;  
}



float CosmicTagAlgo::ShowerBoxScore(const ::geoalgo::HalfLine& shr,
									const ::geoalgo::AABox& box) 
{

  geoalgo::GeoAlgo geoObj ;
  float score = 0 ;
//  double distBackAlongTraj, distToWall; 
  double distToTopWall ;
  double detHalfHeight = larutil::Geometry::GetME()->DetHalfHeight();

 // distBackAlongTraj = sqrt(shr.Start().SqDist(geoObj.Intersection(box,shr,true)[0])) ;
 // distToWall		= sqrt(geoObj.SqDist(shr.Start(),box)) ;
  distToTopWall	 	= (detHalfHeight - shr.Start()[1])* shr.Dir().Length()/(-1*shr.Dir()[1]) ;


  if(distToTopWall >= 0)
	score = exp(-1*distToTopWall);
  //Very unlikely to be a cosmic shower if it is pointing upwards
  else 
  	score *= 0.01 ;
  
  return score;
}

#endif
