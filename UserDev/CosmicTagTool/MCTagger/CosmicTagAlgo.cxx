#ifndef COSMICTAGALGO_CXX
#define COSMICTAGALGO_CXX

#include "CosmicTagAlgo.h"

//tracks is list of cosmic-tagged tracks; shr is simplified mcshower
//With some code from David Caratelli's ShowerCalculator.cxx
float CosmicTagAlgo::ShowerTrackScore(std::vector<::geoalgo::Trajectory> &tracks,::geoalgo::HalfLine_t &shr)
{
	double minDist = std::numeric_limits<double>::max();
	double minIP   = std::numeric_limits<double>::max();  
	double Dist 	= -999;
	double IP 		= -999;

	geoalgo::Vector_t c1(3);
	geoalgo::Vector_t c2(3);
	geoalgo::GeoAlgo geoObj ;

  if(shr.Start()[0] < 0 || shr.Start()[0] > 256.35 || shr.Start()[1] <-116.5 || shr.Start()[1] >116.5 || shr.Start()[2] <0 || shr.Start()[2] > 1037. )
		return -99. ;

    // shr segment starts 3 meters before start point,
    // ends 10 cm after start point aligned with shr momentum. Use for Impact Parameter calc
    geoalgo::LineSegment shrSeg(shr.Start()-shr.Dir()*300,shr.Start()+shr.Dir()*10);

	for(size_t i=0; i < tracks.size(); i++){
		double tmpDist = geoObj.SqDist(shr.Start(),tracks.at(i))  ;
   		double tmpIP   = geoObj.SqDist(tracks.at(i),shrSeg,c1,c2) ;
		
		if(tmpDist < minDist) minDist = tmpDist ;
	//		std::cout<<"track at x,y,z : "<<tracks.at(i).X()<<", "<<tracks.at(i).Y()<<", "<<tracks.at(i).Z()<<std::endl;
		if(tmpIP < minIP) minIP = tmpIP ;
      	   
  	   } //for all tracks

	if (minDist != std::numeric_limits<double>::max()){
    	Dist = sqrt(minDist);
    	IP = sqrt(minIP);
		}
	
	float score = 1;
//	float amp = 0.01754;
	float mean = 93.284;
	float sigma = 56.277;
	float gaus = 0.5*exp(-1*(pow((Dist - mean)/sigma,2))); 

//	if(Dist==-999)
//		score = -9;

	if(tracks.size()==0 ){
		score=0;
//	std::cout<<"Dist is: "<<Dist<<std::endl;

		}
	else if(Dist >=0 && Dist < mean ){//&& IP >=0 && IP < 100){
		score = 1 - gaus  ;
		}
	else
		score = gaus ;

	return score;  
}



float CosmicTagAlgo::ShowerBoxScore(const ::geoalgo::HalfLine& shr,
									const ::geoalgo::AABox& box) 
{

  geoalgo::GeoAlgo geoObj ;
  float score = 1 ;
  double distBackAlongTraj, distToWall; 
  double detHalfHeight = larutil::Geometry::GetME()->DetHalfHeight();
  double distToTopWall;

  if(shr.Start()[0] < 0 || shr.Start()[0] > 256.35 || shr.Start()[1] <-116.5 || shr.Start()[1] >116.5 || shr.Start()[2] <0 || shr.Start()[2] > 1037.)
		return -99. ;

  distBackAlongTraj = sqrt(shr.Start().SqDist(geoObj.Intersection(box,shr,true)[0])) ;
 // distToWall		= sqrt(geoObj.SqDist(shr.Start(),box)) ;
  distToTopWall	 	= (shr.Start()[1] - detHalfHeight)*shr.Dir().Length()/(shr.Dir()[1]) ;
 // std::cout<<"Det half Height: "<<(shr.Start()[1]-detHalfHeight)*shr.Dir().Length()/shr.Dir()[1]<<std::endl;



 if(distToTopWall > 0 && distToTopWall < 5000 && distBackAlongTraj >=0 )//&& distBackAlongTraj < 250 )
 	score = 1 - distToTopWall/5000;
  //Very unlikely to be a cosmic shower if it is pointing upwards
  else //if(distToTopWall < 0 ) 
  	score = 0 ;
  //else, if the shower is pointing downwards but is very far from the 
  //top wall, assign it a low score
 // else if(distToTopWall >= 1000 && distToTopWall <10000 && distToTopWall > 0)
  //	score = 1- distToTopWall/100000;
  
  return score;
}

#endif
