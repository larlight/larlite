#ifndef RECOTOOL_FORCEREGIONS_CXX
#define RECOTOOL_FORCEREGIONS_CXX


#include "ForceRegions.h"
//#include "ClusterParamsAlg.hh"
//#include "LArUtilBase.hh"
#include <math.h>       /* cos sin */
#define PI 3.14159265

namespace cluster {


  std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>>  ForceRegions::ForceTwoRegions(const std::vector<larlite::hit>& hits, std::vector<std::pair<double,double>> APP) {
    double T2CM = larutil::GeometryUtilities::GetME()->TimeToCm();
    double W2CM = larutil::GeometryUtilities::GetME()->WireToCm();
    std::vector<unsigned int> startervector;
    std::pair<std::vector<unsigned int>,std::vector<unsigned int>> planepair(startervector,startervector);
    std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>> vectplanepair(3,planepair);
    // loop over all the hits.  
    for(unsigned int a=0;  a < hits.size();a++) {
      //which plane?
      int cplane= hits[a].View();
      //if(APP[cplane].first==-999||APP[cplane].second==-999){ std::cout<<" \033[98m GOT A -999"; continue;}
      if(APP[cplane].first==-999||APP[cplane].second==-999) continue;
      double regionval= (-APP[cplane].first * hits[a].WireID().Wire*W2CM+ hits[a].PeakTime()*T2CM - APP[cplane].second) / sqrt(APP[cplane].first*APP[cplane].first +1);
      // sort into proper places using viewPP
	      if(regionval>0.0000) vectplanepair[cplane].first.push_back(a);
	      if(regionval<0.0000) vectplanepair[cplane].second.push_back(a);
    }// for loop over hit index
    return vectplanepair;
	}

//--------------------------------------------------------------------------------------------------------------------------------------

  std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>>  ForceRegions::ForceTwoFlipRegions(const std::vector<larlite::hit>& hits, std::vector<std::pair<double,double>> APP) {
    double T2CM = larutil::GeometryUtilities::GetME()->TimeToCm();
    double W2CM = larutil::GeometryUtilities::GetME()->WireToCm();
    std::vector<unsigned int> startervector;
    std::pair<std::vector<unsigned int>,std::vector<unsigned int>> planepair(startervector,startervector);
    std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>> vectplanepair(3,planepair);
    // loop over all the hits.  
    for(unsigned int a=0;  a < hits.size();a++) {
      //which plane?
      int cplane= hits[a].View();
      //if(APP[cplane].first==-999||APP[cplane].second==-999){ std::cout<<" \033[98m GOT A -999"; continue;}
      if(APP[cplane].first==-999||APP[cplane].second==-999) continue;
//      double regionval= (-APP[cplane].first * hits[a].WireID().Wire*W2CM+ hits[a].PeakTime()*T2CM - APP[cplane].second) / sqrt(APP[cplane].first*APP[cplane].first +1);
      double regionval= (-APP[cplane].first * hits[a].PeakTime()*T2CM+ hits[a].WireID().Wire*W2CM - APP[cplane].second) / sqrt(APP[cplane].first*APP[cplane].first +1);
      // sort into proper places using viewPP
	      if(regionval>0.0000) vectplanepair[cplane].first.push_back(a);
	      if(regionval<0.0000) vectplanepair[cplane].second.push_back(a);
    }// for loop over hit index
    return vectplanepair;
	}
//--------------------------------------------------------------------------------------------------------------------------------------

  std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>>  ForceRegions::ForceTwoRotateRegions(const std::vector<larlite::hit>& hits, std::vector<std::pair<double,double>> APP, double angle) {
    double T2CM = larutil::GeometryUtilities::GetME()->TimeToCm();
    double W2CM = larutil::GeometryUtilities::GetME()->WireToCm();
    int nplanes = larutil::Geometry::GetME()->Nplanes();
    std::vector<unsigned int> startervector;
    std::pair<std::vector<unsigned int>,std::vector<unsigned int>> planepair(startervector,startervector);
    std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>> vectplanepair(3,planepair);
    // loop over all the hits.  

	// first get the conversions for the rotations

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
        double HowMuch = angle;// this is in degrees
        // This needs to figured out... Dang Magic Numbers!!!!!
        double C = cos(HowMuch * PI / 180);
        double S = sin(HowMuch * PI / 180);





    for(unsigned int a=0;  a < hits.size();a++) {
      //which plane?
      int currentplane= hits[a].View();
      //if(APP[currentplane].first==-999||APP[currentplane].second==-999){ std::cout<<" \033[98m GOT A -999"; continue;}
      if(APP[currentplane].first==-999||APP[currentplane].second==-999) continue;
      double regionval= (-APP[currentplane].first *( (hits[a].WireID().Wire* W2CM - CenterChargeWire[currentplane])*C -(hits[a].PeakTime()* T2CM - CenterChargeTime[currentplane])*S + CenterChargeWire[currentplane]    )     +    ((hits[a].WireID().Wire* W2CM - CenterChargeWire[currentplane])*S + (hits[a].PeakTime()* T2CM - CenterChargeTime[currentplane])*C + CenterChargeTime[currentplane]) - APP[currentplane].second) / sqrt(APP[currentplane].first*APP[currentplane].first +1);
//      double regionval= (-APP[cplane].first * hits[a].PeakTime()*T2CM+ hits[a].WireID().Wire*W2CM - APP[cplane].second) / sqrt(APP[cplane].first*APP[cplane].first +1);
      // sort into proper places using viewPP
	      if(regionval>0.0000) vectplanepair[currentplane].first.push_back(a);
	      if(regionval<0.0000) vectplanepair[currentplane].second.push_back(a);
    }// for loop over hit index
    return vectplanepair;
	}






}

#endif


