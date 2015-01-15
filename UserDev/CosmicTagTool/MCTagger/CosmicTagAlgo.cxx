#ifndef COSMICTAGALGO_CXX
#define COSMICTAGALGO_CXX

#include "CosmicTagAlgo.h"

//tracks is list of cosmic-tagged tracks; shr is simplified mcshower
//With some code from David Caratelli's ShowerCalculator.cxx
float CosmicTagAlgo::ShowerTrackScore(std::vector<::geoalgo::Trajectory> &tracks,::geoalgo::HalfLine_t &shr )
{
	double minDist = std::numeric_limits<double>::max();
	double minIP   = std::numeric_limits<double>::max();  
	double Dist 	= -999;
	double IP 		= -999;

	geoalgo::Vector_t c1(3);
	geoalgo::Vector_t c2(3);
	geoalgo::GeoAlgo geoObj ;

  if(shr.Start()[0] < 0 || shr.Start()[0] > 256.35 || shr.Start()[1] <-116.5 || shr.Start()[1] >116.5 || shr.Start()[2] <0 || shr.Start()[2] > 1037.)
		return -9. ;

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

	if(Dist >= 0 && Dist < 100 ){//&& IP >=0 && IP < 100){
		score *= 1 - (Dist/100) ;
	//	score *= 1 - (IP/100) ;
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
  double distBackAlongTraj, distToWall; 
  double distToTopWall ;
  double detHalfHeight = larutil::Geometry::GetME()->DetHalfHeight();

  if(shr.Start()[0] < 0 || shr.Start()[0] > 256.35 || shr.Start()[1] <-116.5 || shr.Start()[1] >116.5 || shr.Start()[2] <0 || shr.Start()[2] > 1037.)
		return -9. ;

  distBackAlongTraj = sqrt(shr.Start().SqDist(geoObj.Intersection(box,shr,true)[0])) ;
 // distToWall		= sqrt(geoObj.SqDist(shr.Start(),box)) ;
  distToTopWall	 	= (detHalfHeight - shr.Start()[1])* shr.Dir().Length()/(-1*shr.Dir()[1]) ;


 if(distToTopWall >= 0 && distToTopWall < 400 && distBackAlongTraj >=0 && distBackAlongTraj < 250 )
 	score = 1 - distToTopWall/400;
  //Very unlikely to be a cosmic shower if it is pointing upwards
  else if(distToTopWall < 0 ) 
  	score *= 0.000001 ;
  //else, if the shower is pointing downwards but is very far from the 
  //top wall, assign it a low score
  else 
  	score *= 0.001;
  
  return score;
}

#endif
