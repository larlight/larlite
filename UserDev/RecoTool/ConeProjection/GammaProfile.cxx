#ifndef LARLITE_GAMMAPROFILE_CXX
#define LARLITE_GAMMAPROFILE_CXX

#include "GammaProfile.h"


namespace larlite {

  bool GammaProfile::initialize() {
        fFracL0 = new TH2D("fLengthA0","dCharge/dX :DeDx ShowerProfile ;Length of Shower in CM; Energy Deposit/Length ",200,0,200,200,0,3000);
        fFracL1 = new TH2D("fLengthA1","dCharge/dx :DeDx ShowerProfile;Length of Shower in CM; Energy Deposit/Length",200,0,200,200,0,3000);
        fFracL2 = new TH2D("fLengthA2","dCharge/dx :DeDx ShowerProfile;Length of Shower in CM; Energy Deposit/Length",200,0,200,200,0,3000);
        f3FracL0 = new TH3D("f3LengthA0","dCharge/dx :DeDx ShowerProfile;Length of Shower in CM; Energy Deposit/Length",200,0,200,50,0,10000,1000,0,1000);
        f3FracL1 = new TH3D("f3LengthA1","dCharge/dx :DeDx ShowerProfile;Length of Shower in CM;Energy Deposit/Length",200,0,200,50,0,10000,1000,0,1000);
        f3FracL2 = new TH3D("f3LengthA2","dCharge/dx :DeDx ShowerProfile;Length of Shower in CM; Energy Deposit/Length",200,0,200,50,0,10000,1000,0,1000);


        fmcEn = new TH1D("fmcEn","dCharge/dX ",200,0,1000);
        fmcEnGood = new TH1D("fmcEnGood","Good Showers ",200,0,1000);
   return true;
  }
  
  bool GammaProfile::analyze(storage_manager* storage) {
//=================================================
//Define some things 
//=================================================
        auto geom = larutil::GeometryHelper::GetME();
        auto tservice = larutil::TimeService::GetME();
	auto const& tpc_clock = tservice->TPCClock();
	double tick_offset = tservice->TriggerOffsetTPC() * tpc_clock.Frequency();// Kazu's fix
        unsigned int nplanes = larutil::Geometry::GetME()->Nplanes();
        // these things will be filled and used 
	//== LarLight vector of hits
		std::vector<larlite::hit> hitsvect;
	//== 2D: This had the vertex point and end point in two 2d 
		std::vector<std::pair<larutil::PxPoint,larutil::PxPoint>> AxisSEpt(nplanes);
	//== 2D: This had the vertex point and end point in two 2d 
		std::vector<std::vector<larutil::PxPoint>> ConePolygonProjection(nplanes);
	//== Slope and Cept of the cone axis
		std::vector<std::pair<double,double>> ConeAxisSlopeCept;
        //== Make the pxhit  vector by plane for now...
		std::vector<std::vector<larutil::PxHit>> PxHitsVect(nplanes);
	//== RecoFit Based on weights 
		std::vector<std::pair<double,double>> recofitvec(nplanes);
	//== Truth Photon Start Position  
		TLorentzVector StartConePos;
		TLorentzVector StartShowerPos;
	//== Truth Photon Start Dir  
		TLorentzVector StartConeDir;
		TLorentzVector StartShowerDir;
        //== Make the pxhit  vector by plane for now... using hits
		std::vector<std::vector<larutil::PxHit>> ContainedPxHitsVect(nplanes);
        //== Make the pxhit  vector by plane for now...
		std::vector<std::vector<unsigned int>> ContainedHitsVect(nplanes);
        //== ratio of charge contained...
		double mctcontain = -999;
        //== true energy...
		double mcEn = -999;
        //== Detector energy...
		double DetEn = -999;
//=================================================
//$$$$$$$$$$$$$$$$$---END---$$$$$$$$$$$$$$$$$$$$$$$
//-----------Define some variables-----------------
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//=================================================

//=========================================
//========== Bring in info  ===============
//=========================================
        auto hits = storage->get_data<event_hit>("gaushit");
        if(!hits){print(msg::kERROR,__FUNCTION__,"No DBCluster or associated hits found!");
                throw std::exception();
                return false;}
        for(auto const& h : *hits) hitsvect.push_back(h);
                        for(auto const& hit : *hits){ 
                              ::larutil::PxHit h;
                              h.t = (hit.PeakTime() + tick_offset )* geom->TimeToCm() ;
                              h.w = hit.WireID().Wire     * geom->WireToCm();
                              h.charge = hit.Integral();
                              h.plane  = hit.View();
                                if( (int)hit.View() ==0) PxHitsVect[0].push_back(h);
                                if( (int)hit.View() ==1) PxHitsVect[1].push_back(h);
                                if( (int)hit.View() ==2) PxHitsVect[2].push_back(h);
                                  }
//  This uses the truth info to get the cone
        auto mcshower = storage->get_data<event_mcshower>("mcreco");
			for(auto const& mcs : *mcshower){
				auto SP = mcs.Start();
                                auto ShowerDetProf =  mcs.DetProfile();
				//StartConePos = SP.Position();
				//StartShowerPos = ShowerDetProf.Position();
					StartShowerPos = SP.Position();
					StartConePos = ShowerDetProf.Position();
				auto pos = ShowerDetProf.Position();
				//StartConeDir = SP.Momentum();
				//StartShowerDir = ShowerDetProf.Momentum();
					StartShowerDir = SP.Momentum();
					StartConeDir = ShowerDetProf.Momentum();
				auto dir = ShowerDetProf.Momentum();
				DetEn = ShowerDetProf.E();
				mcEn = SP.E();
				mctcontain = DetEn/mcEn;
						}//mcshower 
//=================================================
//$$$$$$$$$$$$$$$$$---END---$$$$$$$$$$$$$$$$$$$$$$$
//-------------Bring in info-----------------------
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//=================================================


//=--=-=-=-=-=-=-=-=-=-=-=-=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-=
		std::vector<double>  ChargeinCone(nplanes);
		//coneintpc = fgeoconic.ConeInTPC(StartConePos,StartConeDir,ConeLength,angle, smoothness);
		bool showerintpc =  false;
		try{
		showerintpc = fgeoconic.ConeInTPC(StartShowerPos,StartShowerDir,ConeLength,angle, smoothness);
		showerintpc = fgeoconic.ConeInTPC(StartShowerPos,StartShowerDir,2,angle, smoothness);
		}catch(const ::larutil::LArUtilException& e){
		return false;
		showerintpc = false;
                }
		std::vector<double> ratio(nplanes);

	//if(coneintpc && energy>100 && energy<205)
	//if(showerintpc && DetEn/mcEn > 0.95 &&mcEn>200 &&mcEn<250)
	//if(showerintpc && DetEn/mcEn > 0.9 &&mcEn>200 &&mcEn<250 && StartConeDir.Pz()/StartConeDir.E()>0.8 &&StartConeDir.Px()/StartConeDir.E()<0.01)
	if(showerintpc && DetEn/mcEn > 0.9 &&mcEn>200 &&mcEn<255 && StartConeDir.Pz()/StartConeDir.E()>0.8&&StartConeDir.Px()/StartConeDir.E()>0.1 )
	//if(showerintpc &&mcEn<400)
	//asdf
	{
	//std::cout<<"pz" <<StartConeDir.Pz()/StartConeDir.E()<<std::endl;
		std::vector<double> DChargeDL(nplanes,0.0);
		for(unsigned int plane=0; plane<nplanes; plane++)
		{
			double DL = 2;
			for(double a=2.0; a<ConeLength ; a+=DL)
			{
			//auto polyproj = fgeoconic.ConicalFeatures(StartConePos, StartConeDir,a, angle, plane ,smoothness);
			std::vector<larutil::PxHit> contp;
			try{
			auto polyproj = fgeoconic.ConicalFeatures(StartShowerPos, StartShowerDir,a, angle, plane ,smoothness);
			contp = fgeoconic.PolyContain(PxHitsVect[plane], polyproj);
			}catch(const ::larutil::LArUtilException& e){
			continue;}
			// change the dir
				double newchargecont=0.0;
				for(auto const h: contp) newchargecont+=h.charge;
				double dCdX = newchargecont/a;
				if(dCdX<100 && a<14 ) continue;	
				if(plane==0) fFracL0->Fill(a,dCdX);
				if(plane==1) fFracL1->Fill(a,dCdX);
				if(plane==2) fFracL2->Fill(a,dCdX);
				if(plane==0) f3FracL0->Fill(a,dCdX,energy);
				if(plane==1) f3FracL1->Fill(a,dCdX,energy);
				if(plane==2) f3FracL2->Fill(a,dCdX,energy);
			}
		}
	fmcEnGood->Fill(mcEn);
	}//
	
	//
	fmcEn->Fill(mcEn);

    return true;
  }

  bool GammaProfile::finalize() {

	if(_fout)
	_fout->cd();
	fFracL0->Write();
	fFracL1->Write();
	fFracL2->Write();
	f3FracL0->Write();
	f3FracL1->Write();
	f3FracL2->Write();
	fmcEn->Write();
	fmcEnGood->Write();
    return true;
  }

}
#endif
