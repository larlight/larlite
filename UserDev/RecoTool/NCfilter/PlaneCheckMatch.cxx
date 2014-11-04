#ifndef RECOTOOL_PLANECHECKMATCH_CXX
#define RECOTOOL_PLANECHECKMATCH_CXX

#include "PlaneCheckMatch.h"

namespace cluster {

//-----------------------------------------------------------------------------------------------------------------------
  std::vector<bool> PlaneCheckMatch::PlaneCheck(const std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>> Regions, const larlite::event_hit* hits  ) {
	// Bring in magic numbers for test values
        double QlevelLo = 0.1;
        double QlevelHi = 0.99;
        double QlevelVert = 0.8;
	int nplanes = 3;
	// The return will be a vector of bools...
	// The pattern to fill the vector will be of the form Q01, Q02, Q12 
        bool Q01 = false;
        bool Q02 = false;
        bool Q12 = false;
	std::vector<bool>  returnvect(nplanes,false);

	// first make the vectors for the pxhits
            std::vector<larutil::PxHit> pxfa_v0;
            std::vector<larutil::PxHit> pxfb_v0;
            std::vector<larutil::PxHit> pxfa_v1;
            std::vector<larutil::PxHit> pxfb_v1;
            std::vector<larutil::PxHit> pxfa_v2;
            std::vector<larutil::PxHit> pxfb_v2;

        //hit index, all the hits, and the vector of pxf
        if(Regions[0].first.size() > 15 && Regions[0].second.size()>15){
            fCRUHelper.GeneratePxHit(Regions[0].first, hits, pxfa_v0);
            fCRUHelper.GeneratePxHit(Regions[0].second, hits, pxfb_v0);
                if(Regions[1].first.size() > 15 && Regions[1].second.size()>15){
                    fCRUHelper.GeneratePxHit(Regions[1].first, hits, pxfa_v1);
                    fCRUHelper.GeneratePxHit(Regions[1].second, hits, pxfb_v1);
                double aa = fQuality.TimeProf(pxfa_v0,pxfa_v1);
                double ab = fQuality.TimeProf(pxfa_v0,pxfb_v1);
                double ba = fQuality.TimeProf(pxfb_v0,pxfa_v1);
                double bb = fQuality.TimeProf(pxfb_v0,pxfb_v1);
                        if(aa>QlevelHi && bb>QlevelHi && ab<QlevelLo && ba<QlevelLo) Q01 = true;
                        if(aa<QlevelLo && bb<QlevelLo && ab>QlevelHi && ba>QlevelHi) Q01 = true;
                        if(aa>QlevelVert && bb>QlevelVert && ab>QlevelVert && ba>QlevelVert) Q01 = false;// need a switch... vertical showers   
                                        }//plane 1

                if(Regions[2].first.size() > 15 && Regions[2].second.size()>15){
                    fCRUHelper.GeneratePxHit(Regions[2].first, hits, pxfa_v2);
                    fCRUHelper.GeneratePxHit(Regions[2].second, hits, pxfb_v2);
                double aa = fQuality.TimeProf(pxfa_v0,pxfa_v2);
                double ab = fQuality.TimeProf(pxfa_v0,pxfb_v2);
                double ba = fQuality.TimeProf(pxfb_v0,pxfa_v2);
                double bb = fQuality.TimeProf(pxfb_v0,pxfb_v2);
                        if(aa>QlevelHi && bb>QlevelHi && ab<QlevelLo && ba<QlevelLo) Q02 = true;
                        if(aa<QlevelLo && bb<QlevelLo && ab>QlevelHi && ba>QlevelHi) Q02 = true;
                        if(aa>QlevelVert && bb>QlevelVert && ab>QlevelVert && ba>QlevelVert) Q02 = false;// need a switch... vertical showers
                                        }//plane 2
                                }//if plane 0 
        if(Regions[1].first.size() > 15 && Regions[1].second.size()>15){
                    fCRUHelper.GeneratePxHit(Regions[1].first, hits, pxfa_v1);
                    fCRUHelper.GeneratePxHit(Regions[1].second, hits, pxfb_v1);
                if(Regions[2].first.size() > 15 && Regions[2].second.size()>15){
                    fCRUHelper.GeneratePxHit(Regions[2].first, hits, pxfa_v2);
                    fCRUHelper.GeneratePxHit(Regions[2].second, hits, pxfb_v2);
                double aa = fQuality.TimeProf(pxfa_v1,pxfa_v2);
                double ab = fQuality.TimeProf(pxfa_v1,pxfb_v2);
                double ba = fQuality.TimeProf(pxfb_v1,pxfa_v2);
                double bb = fQuality.TimeProf(pxfb_v1,pxfb_v2);
                        if(aa>QlevelHi && bb>QlevelHi && ab<QlevelLo && ba<QlevelLo) Q12 = true;
                        if(aa<QlevelLo && bb<QlevelLo && ab>QlevelHi && ba>QlevelHi) Q12 = true;
                        if(aa>QlevelVert && bb>QlevelVert && ab>QlevelVert && ba>QlevelVert) Q12 = false;// need a switch... vertical showers
                                        }// if plane 2
                                }//plane 1



	returnvect[0]= Q01;
	returnvect[1]= Q02;
	returnvect[2]= Q12;

	return returnvect;
	}//plane check function



//-----------------------------------------------------------------------------------------------------------------------






























}

#endif


