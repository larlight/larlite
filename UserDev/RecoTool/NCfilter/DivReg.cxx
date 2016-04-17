#ifndef RECOTOOL_DIVREG_CXX
#define RECOTOOL_DIVREG_CXX

#include "DivReg.h"
#include "LArUtil/GeometryHelper.h"
#include "LArUtil/Geometry.h"
//#include "ClusterParamsAlg.hh"
//#include "LArUtilBase.hh"
#include <math.h>       /* cos sin */
#define PI 3.14159265


namespace cluster {

  std::vector<std::pair<double,double>> DivReg::SplitLine(const std::vector<std::vector<larutil::PxHit>>& hits) const
  {
// first 
    int nplanes = larutil::Geometry::GetME()->Nplanes();
    std::vector<std::pair<double,double>> AvgPairSI(nplanes);
    // Need to get the average of the incomming clusters
    // Everything is in CM space already
    double S0 = 0;
    double S1 = 0;
    double S2 = 0;
    double I0 = 0;
    double I1 = 0;
    double I2 = 0;
    int count0 = 0;
    int count1 = 0;
    int count2 = 0;

    for(auto const& hitvect : hits) {
	// This is like the loop over clusters
      // only run when we have a cluster of hits greater than
      //auto const& hit_index_array = c.association(hit_type);
      if(hitvect.size()>15){
	// calculate slope and cept in cm
	// double T2CM = larutil::GeometryHelper::GetME()->TimeToCm();
        //double W2CM = larutil::GeometryHelper::GetME()->WireToCm();
        double totcharge  = 0;
        double avgcharge = 0;
        double awiretime=  0;
        double awire =  0;
        double atime =  0;
        double awirewire = 0;
        int na=0;
	//int currentplane = -999;
	int currentplane= hitvect[0].plane;

	for(auto const& thehit : hitvect) {
	     totcharge+=thehit.charge;}
        avgcharge=totcharge/hitvect.size();

	for(auto const& thehit : hitvect) {
                if(thehit.charge>avgcharge){
                na+=1;
                awiretime += thehit.w * thehit.t;
                awire += thehit.w;
                atime += thehit.t;
                awirewire += thehit.w * thehit.w;
                                        }//if charge
                                   }//end of loop over the hits
        double aroughslope = (na * awiretime - awire * atime)/(na*awirewire-awire*awire);
        double aroughcept= atime/na - aroughslope *(awire/na);
        std::pair<double,double> rs(aroughslope,aroughcept);
 

//std::cout<< "\033[92m This is from the function "<< currentplane<<"\033[90m"<<std::endl;
        if(currentplane==0){
          count0+=1;
          S0+=rs.first;
          I0+=rs.second;
          //std::cout<<" We have a 0 plane "<<std::endl;
        }// if view ==0
        if(currentplane==1){
          count1+=1;
          S1+=rs.first;
          I1+=rs.second;
          //std::cout<<" We have a 1 plane "<<std::endl;
        }// if view ==1 
        if(currentplane==2){
          count2+=1;
          S2+=rs.first;
          I2+=rs.second;
          //std::cout<<" We have a 2 plane "<<std::endl;
        }// if view ==2
      }//  if hits are over 15
    }// for loop over the incoming clusters

    if(count0>0){
      std::pair<double,double>  t0(S0/count0,I0/count0);
      AvgPairSI[0] = t0;}
    else{
      std::pair<double,double> n0(-999,-999);
      AvgPairSI[0] = n0;}

    if(count1>0){
      std::pair<double,double>  t1(S1/count1,I1/count1);
      AvgPairSI[1] = t1;}
    else{
      std::pair<double,double> n1(-999,-999);
      AvgPairSI[1] = n1;}

    if(count2>0){
      std::pair<double,double>  t2(S2/count2,I2/count2);
      AvgPairSI[2] = t2;}
    else{
      std::pair<double,double> n2(-999,-999);
      AvgPairSI[2] = n2;}
//    std::cout<<" Size of avg pairs from First Pass"<<AvgPairSI.size()<<std::endl;



    return AvgPairSI;
  }


//--------------------------------------------------------------------------------------------------------------------------------------
  std::vector<std::pair<double,double>> DivReg::SplitLineC(const std::vector<larlite::cluster>& clusters, 
							   const std::vector<std::vector<unsigned int> >& cluster_hit_ass,
							   const std::vector<larlite::hit>& hits) const
  {
    // first 
    int nplanes = larutil::Geometry::GetME()->Nplanes();
    std::vector<std::pair<double,double>> AvgPairSI(nplanes);
    // calculate slope and cept in cm
    double T2CM = larutil::GeometryHelper::GetME()->TimeToCm();
    double W2CM = larutil::GeometryHelper::GetME()->WireToCm();
    double S0 = 0;
    double S1 = 0;
    double S2 = 0;
    double I0 = 0;
    double I1 = 0;
    double I2 = 0;
    int count0 = 0;
    int count1 = 0;
    int count2 = 0;
    // finding the hit type
    //for(auto const& c : *clusters) {
    for(unsigned int a= 0 ; a < clusters.size();a++) {
      auto const& hit_index_array = cluster_hit_ass[a];
      // only run when we have a cluster of hits greater than
      if(hit_index_array.size()>15){
	// This can be an alg
	double totcharge  = 0;
	double avgcharge = 0;
	double awiretime=  0;
	double awire =  0;
	double atime =  0;
	double awirewire = 0;
	int currentplane= -999;
	int na=0;	
	// getting the average charge of the cluster and setting the current plane of this cluster
	for( auto const& thehit : hit_index_array){
	  currentplane = hits[thehit].View();
	  totcharge+=hits[thehit].Integral();}
	avgcharge=totcharge/hit_index_array.size();
	// now calculating the average
	for(auto const& thehit : hit_index_array) {
	  if(hits[thehit].Integral()>avgcharge){
	    na+=1;
	    awiretime += hits[thehit].WireID().Wire* W2CM * hits[thehit].PeakTime()*T2CM;
	    awire += hits[thehit].WireID().Wire*W2CM;
	    atime += hits[thehit].PeakTime()*T2CM;
	    awirewire += hits[thehit].WireID().Wire*W2CM * hits[thehit].WireID().Wire*W2CM;
	  }//if charge
	}//end of loop over the hits
        double aroughslope = (na * awiretime - awire * atime)/(na*awirewire-awire*awire);
        double aroughcept= atime/na - aroughslope *(awire/na);
        std::pair<double,double> rs(aroughslope,aroughcept);
	//put the cluster info in the proper plane
        if(currentplane==0){
          count0+=1;
          S0+=rs.first;
          I0+=rs.second; }// if view ==0
        if(currentplane==1){
          count1+=1;
          S1+=rs.first;
          I1+=rs.second;}// if view ==1
        if(currentplane==2){
          count2+=1;
          S2+=rs.first;
          I2+=rs.second;}// if view ==2
      			}//  if hits are over 15
   		 }// for loop over the incoming clusters

	    if(count0>0){
	      std::pair<double,double>  t0(S0/count0,I0/count0);
	      AvgPairSI[0] = t0;}
	    else{
	      std::pair<double,double> n0(-999,-999);
	      AvgPairSI[0] = n0;}
	    if(count1>0){
	      std::pair<double,double>  t1(S1/count1,I1/count1);
	      AvgPairSI[1] = t1;}
	    else{
	      std::pair<double,double> n1(-999,-999);
	      AvgPairSI[1] = n1;}
	    if(count2>0){
	      std::pair<double,double>  t2(S2/count2,I2/count2);
	      AvgPairSI[2] = t2;}
	    else{
	      std::pair<double,double> n2(-999,-999);
	      AvgPairSI[2] = n2;}
	
//    std::cout<<" Size of avg pairs from First Pass"<<AvgPairSI.size()<<std::endl;
	// return the info
    return AvgPairSI;
  }

//--------------------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------------------
  std::vector<std::pair<double,double>> DivReg::FlipLineC(const std::vector<larlite::cluster>& clusters, 
							  const std::vector<std::vector<unsigned int> >& cluster_hit_ass,
							  const std::vector<larlite::hit>& hits) const
  {
    // first 
    // This is pretty much the same as the divline.... but now we are fliping the space....  Wire->Time & Time->Wire   all in CM
    int nplanes = larutil::Geometry::GetME()->Nplanes();
    std::vector<std::pair<double,double>> AvgPairSI(nplanes);
    // calculate slope and cept in cm
    double T2CM = larutil::GeometryHelper::GetME()->TimeToCm();
    double W2CM = larutil::GeometryHelper::GetME()->WireToCm();
    double S0 = 0;
    double S1 = 0;
    double S2 = 0;
    double I0 = 0;
    double I1 = 0;
    double I2 = 0;
    int count0 = 0;
    int count1 = 0;
    int count2 = 0;
    //for(auto const& c : *clusters) {
    for(unsigned int a= 0 ; a < clusters.size();a++) {
      auto const& hit_index_array = cluster_hit_ass[a];
      // only run when we have a cluster of hits greater than
      if(hit_index_array.size()>15){
	// This can be an alg
	double totcharge  = 0;
	double avgcharge = 0;
	double awiretime=  0;
	double awire =  0;
	double atime =  0;
	double atimetime = 0;
	int currentplane= -999;
	int na=0;	
	// getting the average charge of the cluster and setting the current plane of this cluster
	for( auto const& thehit : hit_index_array){
	  currentplane = hits[thehit].View();
	  totcharge+=hits[thehit].Integral();}
	avgcharge=totcharge/hit_index_array.size();
	// now calculating the average
	for(auto const& thehit : hit_index_array) {
	  if(hits[thehit].Integral()>avgcharge){
	    na+=1;
	    awiretime += hits[thehit].WireID().Wire* W2CM * hits[thehit].PeakTime()*T2CM;
	    awire += hits[thehit].WireID().Wire*W2CM;
	    atime += hits[thehit].PeakTime()*T2CM;
	    atimetime += hits[thehit].PeakTime()*T2CM * hits[thehit].PeakTime()*T2CM;
	  }//if charge
	}//end of loop over the hits
        double aroughslope = (na * awiretime - atime * awire)/(na*atimetime-atime*atime);
        double aroughcept= awire/na - aroughslope *(atime/na);
        std::pair<double,double> rs(aroughslope,aroughcept);
	//put the cluster info in the proper plane
        if(currentplane==0){
          count0+=1;
          S0+=rs.first;
          I0+=rs.second; }// if view ==0
        if(currentplane==1){
          count1+=1;
          S1+=rs.first;
          I1+=rs.second;}// if view ==1
        if(currentplane==2){
          count2+=1;
          S2+=rs.first;
          I2+=rs.second;}// if view ==2
      			}//  if hits are over 15
   		 }// for loop over the incoming clusters

	    if(count0>0){
	      std::pair<double,double>  t0(S0/count0,I0/count0);
	      AvgPairSI[0] = t0;}
	    else{
	      std::pair<double,double> n0(-999,-999);
	      AvgPairSI[0] = n0;}
	    if(count1>0){
	      std::pair<double,double>  t1(S1/count1,I1/count1);
	      AvgPairSI[1] = t1;}
	    else{
	      std::pair<double,double> n1(-999,-999);
	      AvgPairSI[1] = n1;}
	    if(count2>0){
	      std::pair<double,double>  t2(S2/count2,I2/count2);
	      AvgPairSI[2] = t2;}
	    else{
	      std::pair<double,double> n2(-999,-999);
	      AvgPairSI[2] = n2;}
	
//    std::cout<<" Size of avg pairs from flip"<<AvgPairSI.size()<<std::endl;
	// return the info
    return AvgPairSI;
  }

//--------------------------------------------------------------------------------------------------------------------------------------
  std::vector<std::pair<double,double>> DivReg::RotateLineC(const std::vector<larlite::cluster>& clusters, 
							    const std::vector<std::vector<unsigned int> >& cluster_hit_ass,
							    const std::vector<larlite::hit>& hits) const
  {
	// Here we will rotate the hits by some angle before we calulate the dividing line
    int nplanes = larutil::Geometry::GetME()->Nplanes();
    std::vector<std::pair<double,double>> AvgPairSI(nplanes);
    // calculate slope and cept in cm
    double T2CM = larutil::GeometryHelper::GetME()->TimeToCm();
    double W2CM = larutil::GeometryHelper::GetME()->WireToCm();
    double S0 = 0;
    double S1 = 0;
    double S2 = 0;
    double I0 = 0;
    double I1 = 0;
    double I2 = 0;
    int count0 = 0;
    int count1 = 0;
    int count2 = 0;
    // find the center of charge for each plane. This will be the point of which we rotate the hits around
    std::vector<double> TotalCharge(nplanes);
    for(unsigned int a= 0 ; a < hits.size();a++) {
      int CurrentView = hits[a].View();
      TotalCharge[CurrentView]+=hits[a].Integral();
    }// for loop over hits getting the total charge for each plane	
    
    // now find the center of charge for each plane
    std::vector<double> CenterChargeWire(nplanes); 
    std::vector<double> CenterChargeTime(nplanes); 
    
    
    for(unsigned int a= 0 ; a < hits.size();a++) {
      int CurrentView = hits[a].View();
      double tempcw = hits[a].Integral() * hits[a].WireID().Wire*W2CM;
      double tempct = hits[a].Integral() * hits[a].PeakTime()*T2CM;
      CenterChargeWire[CurrentView]+=tempcw;
      CenterChargeTime[CurrentView]+=tempct;
    }// Looping over the hits summing up all the charge
    
    // making the center of charge points
    for(int i = 0 ; i <nplanes; i++) {
      CenterChargeWire[i]/=TotalCharge[i];
      CenterChargeTime[i]/=TotalCharge[i];
    }// end of for loop for making the final center of charge points in wire time weights. 
    
    //#################################
    //# Define how much to Rotate######
    //#################################
    
    //double HowMuch = -60;// this is in degrees
    double HowMuch = 60;// this is in degrees
    // This needs to figured out... Dang Magic Numbers!!!!!
    double C = cos(HowMuch * PI / 180);
    double S = sin(HowMuch * PI / 180);
    
    //=-=-=-=-=--=-=-=-=-=-=-=-=-=
    for(unsigned int a= 0 ; a < clusters.size();a++) {
      auto const& hit_index_array = cluster_hit_ass[a];
      // only run when we have a cluster of hits greater than
      if(hit_index_array.size()>15){
	// This can be an alg
	double totcharge  = 0;
	double avgcharge = 0;
	double awiretime=  0;
	double awire =  0;
	double atime =  0;
	double atimetime = 0;
	int currentplane= -999;
	int na=0;	
	// getting the average charge of the cluster and setting the current plane of this cluster
	for( auto const& thehit : hit_index_array){
	  currentplane = hits[thehit].View();
	  totcharge+=hits[thehit].Integral();}
	avgcharge=totcharge/hit_index_array.size();
	// now calculating the average
	for(auto const& thehit : hit_index_array) {
	  if(hits[thehit].Integral()>avgcharge){
	    na+=1;
	    awiretime += (  (hits[thehit].WireID().Wire* W2CM - CenterChargeWire[currentplane])*C -(hits[thehit].PeakTime()* T2CM - CenterChargeTime[currentplane])*S + CenterChargeWire[currentplane])  *   ((hits[thehit].WireID().Wire* W2CM - CenterChargeWire[currentplane])*S + (hits[thehit].PeakTime()* T2CM - CenterChargeTime[currentplane])*C +CenterChargeTime[currentplane]  )            ;
	    awire += (  (hits[thehit].WireID().Wire* W2CM - CenterChargeWire[currentplane])*C -(hits[thehit].PeakTime()* T2CM - CenterChargeTime[currentplane])*S +CenterChargeWire[currentplane]);
	    atime += ((hits[thehit].WireID().Wire* W2CM - CenterChargeWire[currentplane])*S + (hits[thehit].PeakTime()* T2CM - CenterChargeTime[currentplane])*C  + CenterChargeTime[currentplane]);
	    atimetime +=pow( ((hits[thehit].WireID().Wire* W2CM - CenterChargeWire[currentplane])*S + (hits[thehit].PeakTime()* T2CM - CenterChargeTime[currentplane])*C +CenterChargeTime[currentplane] ),2);
	  }//if charge
	}//end of loop over the hits
        double aroughslope = (na * awiretime - atime * awire)/(na*atimetime-atime*atime);
        double aroughcept= awire/na - aroughslope *(atime/na);
        std::pair<double,double> rs(aroughslope,aroughcept);
	//put the cluster info in the proper plane
        if(currentplane==0){
          count0+=1;
          S0+=rs.first;
          I0+=rs.second; }// if view ==0
        if(currentplane==1){
          count1+=1;
          S1+=rs.first;
          I1+=rs.second;}// if view ==1
        if(currentplane==2){
          count2+=1;
          S2+=rs.first;
          I2+=rs.second;}// if view ==2
      			}//  if hits are over 15
   		 }// for loop over the incoming clusters

	    if(count0>0){
	      std::pair<double,double>  t0(S0/count0,I0/count0);
	      AvgPairSI[0] = t0;}
	    else{
	      std::pair<double,double> n0(-999,-999);
	      AvgPairSI[0] = n0;}
	    if(count1>0){
	      std::pair<double,double>  t1(S1/count1,I1/count1);
	      AvgPairSI[1] = t1;}
	    else{
	      std::pair<double,double> n1(-999,-999);
	      AvgPairSI[1] = n1;}
	    if(count2>0){
	      std::pair<double,double>  t2(S2/count2,I2/count2);
	      AvgPairSI[2] = t2;}
	    else{
	      std::pair<double,double> n2(-999,-999);
	      AvgPairSI[2] = n2;}
	
//    std::cout<<" Size of avg pairs From Rot"<<AvgPairSI.size()<<std::endl;
	// return the info
    return AvgPairSI;
  }

















}

#endif


