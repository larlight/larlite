#ifndef RECOTOOL_LOOKBACK_CXX
#define RECOTOOL_LOOKBACK_CXX

#include "LookBack.h"
#include "LArUtil/GeometryHelper.h"

namespace cluster {


  std::pair<double,double> LookBack::CWAngle(std::vector<unsigned int> hitindex, std::vector<larlite::hit> hitsvect){
    double T2CM = larutil::GeometryHelper::GetME()->TimeToCm();
    double W2CM = larutil::GeometryHelper::GetME()->WireToCm();

	if(hitindex.size()>=4){

        double charge=0;
        double average_charge=0;
        for(unsigned int k =0 ; k<hitindex.size(); k++){
           charge+=hitsvect[hitindex[k]].Integral();}
        average_charge=charge/hitindex.size();

        double na = 0;
        double awiretime = 0;
        double awire = 0;
        double atime = 0;
        double awirewire = 0;
        for(unsigned int z =0 ; z<hitindex.size(); z++){
                if(hitsvect[hitindex[z]].Integral()>average_charge){
                na+=1;
                awiretime +=hitsvect[hitindex[z]].WireID().Wire*W2CM  * hitsvect[hitindex[z]].PeakTime()*T2CM;
                awire += hitsvect[hitindex[z]].WireID().Wire*W2CM;
                atime += hitsvect[hitindex[z]].PeakTime()*T2CM;
                awirewire += pow(hitsvect[hitindex[z]].WireID().Wire*W2CM,2);
                                        }//if charge
                                   }//end of loop over the hits
        double slope = (na * awiretime - awire * atime)/(na*awirewire-awire*awire);
        double cept= atime/na - slope *(awire/na);
	
	std::pair<double,double> retpair(slope,cept);
    return retpair;
	}
	
	else{ std::pair<double,double> bad(-999,-999); return bad;}
	
        }//plane check function





  std::pair<std::vector<unsigned int>,std::vector<unsigned int>> LookBack::ClusterRefine(std::pair<std::vector<unsigned int>,std::vector<unsigned int>> hitpair, std::vector<larlite::hit> hitsvect){

    double T2CM = larutil::GeometryHelper::GetME()->TimeToCm();
    double W2CM = larutil::GeometryHelper::GetME()->WireToCm();
		std::vector<std::pair<unsigned int,double>> hits_distance;
                std::vector<unsigned int> inspecting_cluster;
                std::vector<unsigned int> looping_cluster;
                std::vector<unsigned int> transfer_hits_cluster;

                bool switchstartbool = false;
	// find which cluster will be for looping and which will be for inspecting 
                // making the looping cluster be the larger cluster
                // the inspecting clutser is going to be the smaller one to start
                if(switchstartbool ==false){
                if(hitpair.first.size()<hitpair.second.size()){
                        inspecting_cluster = hitpair.first;
                        looping_cluster = hitpair.second;}
                if(hitpair.first.size()>hitpair.second.size()){
                        inspecting_cluster = hitpair.second;
                        looping_cluster = hitpair.first;}
                }

   // start the switch loop here
                trans_restart:
                if(switchstartbool ==true){
        std::cout<<" SWITCH RESTART"<<std::endl;
                hits_distance.clear();
                inspecting_cluster.clear();
                looping_cluster.clear();
                transfer_hits_cluster.clear();
                if(hitpair.first.size()<hitpair.second.size()){
                        inspecting_cluster = hitpair.second;
                        looping_cluster = hitpair.first;}
                if(hitpair.first.size()>hitpair.second.size()){
                        inspecting_cluster = hitpair.first;
                        looping_cluster = hitpair.second;}
                }

       // make a the vector for the inspecting cluster
        // This will need to be sorted and starting with hits closest to the PV
/*     std::vector<std::pair<double,double>> PVertex = fQuality.ProtoVertexCW(BestClusters,hits);
      for(unsigned int i = 0 ; i<inspecting_cluster.size(); i++){
        double wire = hitsvect[inspecting_cluster[i]].WireID().Wire*W2CM -PVertex[a].first*W2CM;
        double time = hitsvect[inspecting_cluster[i]].PeakTime()*T2CM-PVertex[a].second*T2CM;
        double tempdist = sqrt(wire*wire + time*time);
        std::pair<unsigned int,double> temppair(inspecting_cluster[i],tempdist);
         hits_distance.push_back(temppair);
        }// closing the for loop over inspecting_cluster
        // now sort the list based on distance
                sort(hits_distance.begin(),hits_distance.end(),
                []( const std::pair<unsigned int, double> &p1, const std::pair<unsigned int, double> &p2 )
                {
                   return ( p1.second < p2.second ||
                          ( !( p2.second < p1.second ) && p1.first < p2.first ) );
                });
*/

	startloop:
        for(unsigned int b =0 ; b<inspecting_cluster.size(); b++){
               // 
                for(unsigned int j =0 ; j<looping_cluster.size(); j++){
                // calculate the distance between the two hits
                double deltawire = hitsvect[inspecting_cluster[b]].WireID().Wire*W2CM - hitsvect[looping_cluster[j]].WireID().Wire*W2CM;
                double deltatime = hitsvect[inspecting_cluster[b]].PeakTime()*T2CM - hitsvect[looping_cluster[j]].PeakTime()*T2CM;
                double prox = sqrt( deltawire*deltawire + deltatime*deltatime );
                // needs to be globally defined _
                        if(prox<radius_of_interest)
                        {
                        // this means that the hit in the looping cluster is close to the hit that we are inspecting    
                        // now we need to add this to the inspecting cluster and remove it from the looping cluster
                        // the question will be where to insert it into the the hits_distance list. 
                        // this should also preserve the hit to being close to the protovertex
//                        double loop_hit_dist = sqrt( pow( hitsvect[looping_cluster[j]].WireID().Wire*W2CM-PVertex[a].first*W2CM,2) +pow( hitsvect[looping_cluster[j]].PeakTime()*T2CM-PVertex[a].second*T2CM,2));
                                        // this can be a push_back because order is not important at point because we are moving it for sure
                                        // RG is this true ? 
                                        inspecting_cluster.push_back(looping_cluster[j]);
                                        transfer_hits_cluster.push_back(looping_cluster[j]);
                                        // need to add it to inspecting cluster too
                                        // remove it from looping cluster
                                        looping_cluster.erase(looping_cluster.begin()+j);
                                        goto startloop;
                        }// if prox < radius

                }// for loop over loopingcluster
        }

	
        // now lets see what we can do before we fill this... 

        // first address the case of if the cluster size() is pretty big ~ >25  
        // see if slope/angle is closer to inspecting or looping 
        // we want it to be closer to inspecting. 
        // if it is closer to looping then we might want to do things the other way 
        // This needs to be algo-abstract
        if(switchstartbool==false)
        {
        double transfer_check_size = 50;
        if(transfer_hits_cluster.size()>=transfer_check_size){
        // find the angle of the cluster
        std::pair<double,double> transfer_slope_cept = CWAngle(transfer_hits_cluster,hitsvect);
        double transfer_slope = transfer_slope_cept.first;
        std::pair<double,double> inspecting_slope_cept = CWAngle(inspecting_cluster,hitsvect);
        double inspecting_slope = inspecting_slope_cept.first;
        std::pair<double,double> looping_slope_cept = CWAngle(looping_cluster,hitsvect);
        double looping_slope = looping_slope_cept.first;

        std::cout<<" This is the angle of the transfer cluster : " << atan(transfer_slope) * 180/ PI<< std::endl;
        std::cout<<" This is the angle of the inspecting cluster : " << atan(inspecting_slope) * 180/ PI<< std::endl;
        std::cout<<" This is the angle of the looping cluster : " << atan(looping_slope) * 180/ PI<< std::endl;

        double delta_trans_loop = fabs(atan(transfer_slope) - atan(looping_slope));
        double delta_trans_inspect = fabs(atan(transfer_slope) - atan(inspecting_slope));


        if( delta_trans_loop < delta_trans_inspect)
        {
        // this means that the transfered hits slope are more similar to the loop cluster
        // so we would not want to tranfer these to the inspecting cluster 
        // so go back and try again starting with the opposite conditions 
        switchstartbool= true;
        goto trans_restart;
        }// if tranfer hits.size > check size

        }// if hits are big enough
        }// if the switch bool is false



std::pair<std::vector<unsigned int>,std::vector<unsigned int>> retpair(looping_cluster,inspecting_cluster);
return retpair;
}//





/*
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
	std::vector<bool>  returnvect(false,nplanes);

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
                std::cout<< "\t \033[97m  Plane 01:"<<std::endl;
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
                std::cout<< "\t \033[97m Plane 02:"<<std::endl;
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
                std::cout<< "\t \033[97m  Plane 12:"<<std::endl;
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
*/


//-----------------------------------------------------------------------------------------------------------------------







}

#endif


