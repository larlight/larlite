#ifndef RECOTOOL_CFALGOTIMEPROF_CXX
#define RECOTOOL_CFALGOTIMEPROF_CXX

#include "LArUtil/GeometryHelper.h"
#include "CFAlgoTimeProf.h"
// ROOT includes
#include "TF1.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TPrincipal.h"
#include "TVectorD.h"
#include "TGraph.h"
#include "TMath.h"
#include "TH1D.h"
#include "TVirtualFitter.h"


namespace cmtool {

  //-------------------------------------------------------
  CFAlgoTimeProf::CFAlgoTimeProf() : CFloatAlgoBase()
  //-------------------------------------------------------
  {

  }

  //-------------------------------
  CFAlgoTimeProf::~CFAlgoTimeProf()
  //-------------------------------
  {
  }

  //-----------------------------
  void CFAlgoTimeProf::Reset()
  //-----------------------------
  {

  }

  //----------------------------------------------------------------------------------------------
  float CFAlgoTimeProf::Float(const std::vector<const cluster::cluster_params*> &clusters)
  //----------------------------------------------------------------------------------------------
  {
  // We now have a vector a clusters.

  //### need a pointer to the cluster just return -1  
    for(auto const& ptr : clusters) if(!ptr) return -1;

        std::vector<larutil::PxHit> hits0;
        std::vector<larutil::PxHit> hits1;
        std::vector<larutil::PxHit> hits2;

	// loop over the clusters
        for(auto const& c : clusters)
	{
//	std::cout<<"Size of the xluster vector"<<clusters.size()<<std::endl;
//	auto Plane = c->Plane();
//	auto StartPoint = c->GetParams().start_point.t;
//	auto EndPoint = c->GetParams().end_point.t;
//	std::cout<<"\t RG Cluster info:\n \t plane: "<<Plane<<std::endl;
//      std::cout<<"\tStart point: "<<StartPoint<<std::endl;
//        std::cout<<"\tEnd Point: "<<EndPoint <<std::endl;

	// Get assosiations for this cluster
        if(c->plane_id.Plane ==0) hits0 = c->hit_vector;
        if(c->plane_id.Plane ==1) hits1 = c->hit_vector;
        if(c->plane_id.Plane ==2) hits2 = c->hit_vector;

	}// for over the clusters

//	std::cout<<"Looking for the hits vector size"<<hits0.size()<<","<<hits1.size()<<","<<hits2.size()<<std::endl;
//        std::cout<<"############# End of loop############# "<<std::endl;
	// make an integrale over the cluster
	//bool pl0 = false;
	//bool pl1 = false;
	//bool pl2 = false;
	float tprof01 = -1;
	float tprof02 = -1;
	float tprof12 = -1;
	if(hits0.size()>0)
	{
	  //pl0 = true;
	  if(hits1.size()>0)
	    {
	      //pl1 = true;
	      //we need to do a profile
	      tprof01 =TProfCompare(hits0,hits1);
	    }// hits1size>0
	  if(hits2.size()>0)
	    {
	      //pl2 = true;
	      //we need to do a profile
	      tprof02 =TProfCompare(hits0,hits2);
		}// hits2size>0
	}// hits0size >0
	
	if(hits1.size()>0)
	  {
	    //pl1 = true;
	    if(hits2.size()>0)
	      {
		//pl2 = true;
		//we need to do a profile
		tprof12 =TProfCompare(hits1,hits2);
	      }//hits2.size>0
	  }// hits1size>0
	
	// This is going to be slow is we are having to re-calutlate this ever time. For now it will have to do
	
	//	std::cout<< " \t summary of timeprof Planes that are accepted :" <<pl0<<" | " <<pl1<<" | " <<pl2<<" |"<<std::endl;
	std::vector<float> tprofmatches;	
//	std::cout<< " \t                  Value of timeprof(01,02,12) :" <<tprof01<<" | " <<tprof02<<" | " <<tprof12<<" |"<<std::endl;
	tprofmatches.push_back(tprof01);
	tprofmatches.push_back(tprof02);
	tprofmatches.push_back(tprof12);
	
	float matchscore=0;
	float avgcounter=0;
//	std::cout<<"SIZE of trpfmoatch"<< tprofmatches.size()<<std::endl;
	for( unsigned int a=0;a<tprofmatches.size();a++)
	{
	if(tprofmatches[a]==-1) continue;	
	else {
		matchscore +=tprofmatches[a];
		avgcounter +=1;
		}// end of else
	}//for over the tprofmatchs
		if(avgcounter!=0){
//		std::cout << " Match Score pree "<< matchscore<<std::endl;		
//		std::cout<< " Counter "<< avgcounter;
		matchscore /= avgcounter;
//		std::cout << " Match Score "<< matchscore<<std::endl;		
		}
		else{
//		std::cout << " Match Score "<< -1<<std::endl;		
		 return -1;
		}
		return matchscore;
		
	
		
  
    //if(clusters.size()) return 1.;
    //else return -1.;
  }

// Making a function to do the profile test
 float CFAlgoTimeProf::TProfCompare(std::vector<larutil::PxHit> hita ,std::vector<larutil::PxHit> hitb)
 {
//   int nts = larutil::DetectorProperties::GetME()->NumberTimeSamples()*larutil::GeometryHelper::GetME()->TimeToCm();
   // Where is this?
   //int nplanes = geom->Nplanes();
   int nplanes = 3;
   double ks = 0.0;
   std::vector< std::vector<TH1D*> > signals(nplanes);
   std::vector< std::vector<TH1D*> > pulses(nplanes);
   
   double time_diff = ( larutil::DetectorProperties::GetME()->GetXTicksOffset(hita.at(0).plane) - 
			larutil::DetectorProperties::GetME()->GetXTicksOffset(hitb.at(0).plane) ) * larutil::GeometryHelper::GetME()->TimeToCm();
   
   // First go look for the min & max of hits 
   double min_time = larutil::DetectorProperties::GetME()->NumberTimeSamples()*larutil::GeometryHelper::GetME()->TimeToCm();
   double max_time = 0;
   for(auto const& h : hita) {
     if(h.t > max_time) max_time = h.t;
     if(h.t < min_time) min_time = h.t;
   }
   for(auto const& h : hitb) {
     if( (h.t + time_diff) > max_time ) max_time = h.t + time_diff;
     if( (h.t + time_diff) < min_time ) min_time = h.t + time_diff;
   }

   TH1D histo_a("ha", "", 200, min_time-1, max_time+1);
   TH1D histo_b("hb", "", 200, min_time-1, max_time+1);
   TH1D histo_inta("hinta", "", 200, min_time-1, max_time+1);
   TH1D histo_intb("hintb", "", 200, min_time-1, max_time+1);

   // First loop over hits in A and make the hist
   // in this case let's just use plane 0,1
   for( auto const& ha : hita){
     double time = ha.t;
     //time -= larutil::DetectorProperties::GetME()->GetXTicksOffset(ha.plane)*larutil::GeometryHelper::GetME()->TimeToCm();
     double charge = ha.charge;

     int bin = histo_a.FindBin(time);
     histo_a.SetBinContent(bin,histo_a.GetBinContent(bin)+charge);
     for (int j = bin; j<=histo_a.GetNbinsX(); ++j){
       histo_inta.SetBinContent(j,histo_inta.GetBinContent(j)+charge);
     }
   }
   if (histo_inta.Integral()) histo_inta.Scale(1./histo_inta.GetBinContent(histo_inta.GetNbinsX()));
   for( auto const& hb : hitb){
     double time = hb.t + time_diff;
     //time -= larutil::DetectorProperties::GetME()->GetXTicksOffset(hb.plane)*larutil::GeometryHelper::GetME()->TimeToCm();
     double charge = hb.charge;
     int bin = histo_b.FindBin(time);
     histo_b.SetBinContent(bin,histo_b.GetBinContent(bin)+charge);
     for (int j = bin; j<=histo_b.GetNbinsX(); ++j){
       histo_intb.SetBinContent(j,histo_intb.GetBinContent(j)+charge);
     }
   }

/*
   std::cout 
     << "siga: "<< histo_a.GetEntries() << std::endl
     << "sigb: "<< histo_b.GetEntries() << std::endl
     << "siginta: "<<histo_inta.GetEntries() << std::endl
     << "sigintb: "<<histo_intb.GetEntries() << std::endl
     << std::endl;
*/

   if (histo_intb.Integral()) histo_intb.Scale(1./histo_intb.GetBinContent(histo_intb.GetNbinsX()));
   ks = histo_inta.KolmogorovTest(&histo_intb);

   std::cout<<ks<<std::endl;
   return ks;
 }

  //------------------------------
  void CFAlgoTimeProf::Report()
  //------------------------------
  {

  }
    
}
#endif
