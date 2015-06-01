#ifndef LARLITE_GAMMACONATINMENT_CXX
#define LARLITE_GAMMACONATINMENT_CXX

#include "GammaContainment.h"


namespace larlite {

  bool GammaContainment::initialize() {
	
	InitializeAnaTree();
	        fA30_L3_P0 = new TH1D("fA30L3P0","Good Showers ",100,0,1);
	        fA30_L3_P1 = new TH1D("fA30L3P1","Good Showers ",100,0,1);
	        fA30_L3_P2 = new TH1D("fA30L3P2","Good Showers ",100,0,1);
	        fA30_L4_P0 = new TH1D("fA30L4P0","Good Showers ",100,0,1);
	        fA30_L4_P1 = new TH1D("fA30L4P1","Good Showers ",100,0,1);
	        fA30_L4_P2 = new TH1D("fA30L4P2","Good Showers ",100,0,1);
	        fA30_L5_P0 = new TH1D("fA30L5P0","Good Showers ",100,0,1);
	        fA30_L5_P1 = new TH1D("fA30L5P1","Good Showers ",100,0,1);
	        fA30_L5_P2 = new TH1D("fA30L5P2","Good Showers ",100,0,1);

	        fA45_L3_P0 = new TH1D("fA45L3P0","Good Showers ",100,0,1);
	        fA45_L3_P1 = new TH1D("fA45L3P1","Good Showers ",100,0,1);
	        fA45_L3_P2 = new TH1D("fA45L3P2","Good Showers ",100,0,1);
	        fA45_L4_P0 = new TH1D("fA45L4P0","Good Showers ",100,0,1);
	        fA45_L4_P1 = new TH1D("fA45L4P1","Good Showers ",100,0,1);
	        fA45_L4_P2 = new TH1D("fA45L4P2","Good Showers ",100,0,1);
	        fA45_L5_P0 = new TH1D("fA45L5P0","Good Showers ",100,0,1);
	        fA45_L5_P1 = new TH1D("fA45L5P1","Good Showers ",100,0,1);
	        fA45_L5_P2 = new TH1D("fA45L5P2","Good Showers ",100,0,1);

	        fA60_L3_P0 = new TH1D("fA60L3P0","Good Showers ",100,0,1);
	        fA60_L3_P1 = new TH1D("fA60L3P1","Good Showers ",100,0,1);
	        fA60_L3_P2 = new TH1D("fA60L3P2","Good Showers ",100,0,1);
	        fA60_L4_P0 = new TH1D("fA60L4P0","Good Showers ",100,0,1);
	        fA60_L4_P1 = new TH1D("fA60L4P1","Good Showers ",100,0,1);
	        fA60_L4_P2 = new TH1D("fA60L4P2","Good Showers ",100,0,1);
	        fA60_L5_P0 = new TH1D("fA60L5P0","Good Showers ",100,0,1);
	        fA60_L5_P1 = new TH1D("fA60L5P1","Good Showers ",100,0,1);
	        fA60_L5_P2 = new TH1D("fA60L5P2","Good Showers ",100,0,1);

		///////////////////////////////////////////////////////////
	        fA30_L3_P0E = new TH2D("fA30L3P0E","Good Showers ",150,0,600,100,0,1);
	        fA30_L3_P1E = new TH2D("fA30L3P1E","Good Showers ",150,0,600,100,0,1);
	        fA30_L3_P2E = new TH2D("fA30L3P2E","Good Showers ",150,0,600,100,0,1);
	        fA30_L4_P0E = new TH2D("fA30L4P0E","Good Showers ",150,0,600,100,0,1);
	        fA30_L4_P1E = new TH2D("fA30L4P1E","Good Showers ",150,0,600,100,0,1);
	        fA30_L4_P2E = new TH2D("fA30L4P2E","Good Showers ",150,0,600,100,0,1);
	        fA30_L5_P0E = new TH2D("fA30L5P0E","Good Showers ",150,0,600,100,0,1);
	        fA30_L5_P1E = new TH2D("fA30L5P1E","Good Showers ",150,0,600,100,0,1);
	        fA30_L5_P2E = new TH2D("fA30L5P2E","Good Showers ",150,0,600,100,0,1); 

	        fA45_L3_P0E = new TH2D("fA45L3P0E","Good Showers ",150,0,600,100,0,1);
	        fA45_L3_P1E = new TH2D("fA45L3P1E","Good Showers ",150,0,600,100,0,1);
	        fA45_L3_P2E = new TH2D("fA45L3P2E","Good Showers ",150,0,600,100,0,1);
	        fA45_L4_P0E = new TH2D("fA45L4P0E","Good Showers ",150,0,600,100,0,1);
	        fA45_L4_P1E = new TH2D("fA45L4P1E","Good Showers ",150,0,600,100,0,1);
	        fA45_L4_P2E = new TH2D("fA45L4P2E","Good Showers ",150,0,600,100,0,1);
	        fA45_L5_P0E = new TH2D("fA45L5P0E","Good Showers ",150,0,600,100,0,1);
	        fA45_L5_P1E = new TH2D("fA45L5P1E","Good Showers ",150,0,600,100,0,1);
	        fA45_L5_P2E = new TH2D("fA45L5P2E","Good Showers ",150,0,600,100,0,1); 

	        fA60_L3_P0E = new TH2D("fA60L3P0E","Good Showers ",150,0,600,100,0,1);
	        fA60_L3_P1E = new TH2D("fA60L3P1E","Good Showers ",150,0,600,100,0,1);
	        fA60_L3_P2E = new TH2D("fA60L3P2E","Good Showers ",150,0,600,100,0,1);
	        fA60_L4_P0E = new TH2D("fA60L4P0E","Good Showers ",150,0,600,100,0,1);
	        fA60_L4_P1E = new TH2D("fA60L4P1E","Good Showers ",150,0,600,100,0,1);
	        fA60_L4_P2E = new TH2D("fA60L4P2E","Good Showers ",150,0,600,100,0,1);
	        fA60_L5_P0E = new TH2D("fA60L5P0E","Good Showers ",150,0,600,100,0,1.0);
	        fA60_L5_P1E = new TH2D("fA60L5P1E","Good Showers ",150,0,600,100,0,1);
	        fA60_L5_P2E = new TH2D("fA60L5P2E","Good Showers ",150,0,600,100,0,1); 

   return true;
  }
  
  bool GammaContainment::analyze(storage_manager* storage) {
//=================================================
//Define some things 
//=================================================
        auto geom = larutil::GeometryUtilities::GetME();
        auto tservice = larutil::TimeService::GetME();
	auto const& tpc_clock = tservice->TPCClock();
	double tick_offset = tservice->TriggerOffsetTPC() * tpc_clock.Frequency();// Kazu's fix
        unsigned int nplanes = larutil::Geometry::GetME()->Nplanes();
	

        // these things will be filled and used 
	//== How much of the shower is contained in the detector
	double mccontained = -999;
	//==True energy 
	double mcE = -999; 
	//== LarLight vector of hits
		std::vector<larlite::hit> hitsvect;
	//== LarLight vector of charge on plane 
		std::vector<double> chargeplane;

        //== Truth Photon Start Position  
                TLorentzVector StartConePos;
                TLorentzVector StartShowerPos;
        //== Truth Photon Start Dir  
                TLorentzVector StartConeDir;
                TLorentzVector StartShowerDir;
	//== is the cone in tpc?
		bool showerintpc;







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
        //== Make the pxhit  vector by plane for now... using hits
		std::vector<std::vector<larutil::PxHit>> ContainedPxHitsVect(nplanes);
        //== Make the pxhit  vector by plane for now...
		std::vector<std::vector<unsigned int>> ContainedHitsVect(nplanes);
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
                              h.w = hit.WireID().Wire  * geom->WireToCm();
                              h.charge = hit.Integral();
                              h.plane  = hit.View();
                                if( (int)hit.View() ==0) PxHitsVect[0].push_back(h);
                                if( (int)hit.View() ==1) PxHitsVect[1].push_back(h);
                                if( (int)hit.View() ==2) PxHitsVect[2].push_back(h);
                                  }
//  This uses the truth info to get the cone
        auto mcshower = storage->get_data<event_mcshower>("mcreco");
			for(auto const& mcs : *mcshower){
				chargeplane = mcs.Charge();
                                auto SP = mcs.Start();
                                auto ShowerDetProf =  mcs.DetProfile();
				mcE = SP.E();
                                mccontained = ShowerDetProf.E()/SP.E();
			        StartConePos = SP.Position();
                                StartShowerPos = ShowerDetProf.Position();
                                StartConeDir = SP.Momentum();
                                StartShowerDir = ShowerDetProf.Momentum();
		// Check if this cone is inside of the tpc. look at edges. 
	}//mcshower 


// If gamma is truth contained at 90%
	if(mccontained>.95 && mcE>100){ 
		cp0 = 0;
		cp1 = 0;
		cp2 = 0;
		 for(auto const& hit : PxHitsVect[0]) cp0+=hit.charge;
		 for(auto const& hit : PxHitsVect[1]) cp1+=hit.charge;
		 for(auto const& hit : PxHitsVect[2]) cp2+=hit.charge;
	mcsdeposit0 = chargeplane[0];
	mcsdeposit1 = chargeplane[1];
	mcsdeposit2 = chargeplane[2];
	
	// Now find out how much is contained
	// Can we walk all planes?
                try{
                showerintpc = fgeoconic.ConeInTPC(StartShowerPos,StartShowerDir,ConeLength,angle, smoothness);
                }catch(const ::larutil::LArUtilException& e){
                return false;
                showerintpc = false;
                }

	cont0 = -999;
	cont1 = -999;
	cont2 = -999;

	if( showerintpc ) { 
		// find out how much charge we is contained 
		for(unsigned int plane = 0 ; plane<nplanes; plane++){	
                        std::vector<larutil::PxHit> contp;
                        try{
                        auto polyproj = fgeoconic.ConicalFeatures(StartShowerPos, StartShowerDir,ConeLength, angle, plane ,smoothness);
                        contp = fgeoconic.PolyContain(PxHitsVect[plane], polyproj);
			 if(plane==0) for(auto const& hit : contp) cont0+=hit.charge;
			 if(plane==1) for(auto const& hit : contp) cont1+=hit.charge;
			 if(plane==2) for(auto const& hit : contp) cont2+=hit.charge;
                        }catch(const ::larutil::LArUtilException& e){
                        continue;}
		}// for loop over planes
		
		}

	FullTree->Fill();


	}






//=============
//== 3 Rad Lneghts and 30 angle
//=============
	// Fill out a few histograms to get a sense for things
	if(mccontained>.9 && mcE<500){ 
	bool s30tpc = false;
	 try{
                s30tpc = fgeoconic.ConeInTPC(StartShowerPos,StartShowerDir,3*RadLength,30.0, smoothness);
                }catch(const ::larutil::LArUtilException& e){
                s30tpc = false;
                }
	double scont0=0.0;
	double scont1=0.0;
	double scont2=0.0;
	if( s30tpc ) { 
		for(unsigned int plane = 0 ; plane<nplanes; plane++){	
                        std::vector<larutil::PxHit> contp;
                        try{
                        auto polyproj = fgeoconic.ConicalFeatures(StartShowerPos, StartShowerDir,3*RadLength, 30.0, plane ,smoothness);
                        contp = fgeoconic.PolyContain(PxHitsVect[plane], polyproj);
			 if(plane==0) for(auto const& hit : contp) scont0+=hit.charge;
			 if(plane==1) for(auto const& hit : contp) scont1+=hit.charge;
			 if(plane==2) for(auto const& hit : contp) scont2+=hit.charge;
                        }catch(const ::larutil::LArUtilException& e){
                        continue;}
		}// for loop over planes
if(scont0/cp0 >=1.0) scont0--;// this is cheating ever so slightly
if(scont1/cp1 >=1.0) scont1--;// this is cheating ever so slightly
if(scont2/cp2 >=1.0) scont2--;// this is cheating ever so slightly
	//FIll the hists for this plane
	fA30_L3_P0->Fill(scont0/cp0);
	fA30_L3_P1->Fill(scont1/cp1);
	fA30_L3_P2->Fill(scont2/cp2);
	fA30_L3_P0E->Fill(mcE,scont0/cp0);
	fA30_L3_P1E->Fill(mcE,scont1/cp1);
	fA30_L3_P2E->Fill(mcE,scont2/cp2);
		}// if s30tpc
	}// if contained and energy
//=============
//== 4 Rad Lneghts and 30 angle
//=============
	// Fill out a few histograms to get a sense for things
	if(mccontained>.9 && mcE<500){ 
	bool s30tpc = false;
	 try{
                s30tpc = fgeoconic.ConeInTPC(StartShowerPos,StartShowerDir,4*RadLength,30.0, smoothness);
                }catch(const ::larutil::LArUtilException& e){
                s30tpc = false;
                }
	double scont0=0.0;
	double scont1=0.0;
	double scont2=0.0;
	if( s30tpc ) { 
		for(unsigned int plane = 0 ; plane<nplanes; plane++){	
                        std::vector<larutil::PxHit> contp;
                        try{
                        auto polyproj = fgeoconic.ConicalFeatures(StartShowerPos, StartShowerDir,4*RadLength, 30.0, plane ,smoothness);
                        contp = fgeoconic.PolyContain(PxHitsVect[plane], polyproj);
			 if(plane==0) for(auto const& hit : contp) scont0+=hit.charge;
			 if(plane==1) for(auto const& hit : contp) scont1+=hit.charge;
			 if(plane==2) for(auto const& hit : contp) scont2+=hit.charge;
                        }catch(const ::larutil::LArUtilException& e){
                        continue;}
		}// for loop over planes
if(scont0/cp0 >=1.0) scont0--;// this is cheating ever so slightly
if(scont1/cp1 >=1.0) scont1--;// this is cheating ever so slightly
if(scont2/cp2 >=1.0) scont2--;// this is cheating ever so slightly
	//FIll the hists for this plane
	fA30_L4_P0->Fill(scont0/cp0);
	fA30_L4_P1->Fill(scont1/cp1);
	fA30_L4_P2->Fill(scont2/cp2);
	fA30_L4_P0E->Fill(mcE,scont0/cp0);
	fA30_L4_P1E->Fill(mcE,scont1/cp1);
	fA30_L4_P2E->Fill(mcE,scont2/cp2);
		}// if s30tpc
	}// if contained and energy

//=============
//== 5 Rad Lneghts and 30 angle
//=============
	// Fill out a few histograms to get a sense for things
	if(mccontained>.9 && mcE<500){ 
	bool s30tpc = false;
	 try{
                s30tpc = fgeoconic.ConeInTPC(StartShowerPos,StartShowerDir,5*RadLength,30.0, smoothness);
                }catch(const ::larutil::LArUtilException& e){
                s30tpc = false;
                }
	double scont0=0.0;
	double scont1=0.0;
	double scont2=0.0;
	if( s30tpc ) { 
		for(unsigned int plane = 0 ; plane<nplanes; plane++){	
                        std::vector<larutil::PxHit> contp;
                        try{
                        auto polyproj = fgeoconic.ConicalFeatures(StartShowerPos, StartShowerDir,5*RadLength, 30.0, plane ,smoothness);
                        contp = fgeoconic.PolyContain(PxHitsVect[plane], polyproj);
			 if(plane==0) for(auto const& hit : contp) scont0+=hit.charge;
			 if(plane==1) for(auto const& hit : contp) scont1+=hit.charge;
			 if(plane==2) for(auto const& hit : contp) scont2+=hit.charge;
                        }catch(const ::larutil::LArUtilException& e){
                        continue;}
		}// for loop over planes
if(scont0/cp0 >=1.0) scont0--;// this is cheating ever so slightly
if(scont1/cp1 >=1.0) scont1--;// this is cheating ever so slightly
if(scont2/cp2 >=1.0) scont2--;// this is cheating ever so slightly
	//FIll the hists for this plane
	fA30_L5_P0->Fill(scont0/cp0);
	fA30_L5_P1->Fill(scont1/cp1);
	fA30_L5_P2->Fill(scont2/cp2);
	fA30_L5_P0E->Fill(mcE,scont0/cp0);
	fA30_L5_P1E->Fill(mcE,scont1/cp1);
	fA30_L5_P2E->Fill(mcE,scont2/cp2);
		}// if s30tpc
	}// if contained and energy

//=================================================
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//=================================================

//=============
//== 3 Rad Lneghts and 45 angle
//=============
	// Fill out a few histograms to get a sense for things
	if(mccontained>.9 && mcE<500){ 
	bool s45tpc = false;
	 try{
                s45tpc = fgeoconic.ConeInTPC(StartShowerPos,StartShowerDir,3*RadLength,45.0, smoothness);
                }catch(const ::larutil::LArUtilException& e){
                s45tpc = false;
                }
	double scont0=0.0;
	double scont1=0.0;
	double scont2=0.0;
	if( s45tpc ) { 
		for(unsigned int plane = 0 ; plane<nplanes; plane++){	
                        std::vector<larutil::PxHit> contp;
                        try{
                        auto polyproj = fgeoconic.ConicalFeatures(StartShowerPos, StartShowerDir,3*RadLength, 45.0, plane ,smoothness);
                        contp = fgeoconic.PolyContain(PxHitsVect[plane], polyproj);
			 if(plane==0) for(auto const& hit : contp) scont0+=hit.charge;
			 if(plane==1) for(auto const& hit : contp) scont1+=hit.charge;
			 if(plane==2) for(auto const& hit : contp) scont2+=hit.charge;
                        }catch(const ::larutil::LArUtilException& e){
                        continue;}
		}// for loop over planes
if(scont0/cp0 >=1.0) scont0--;// this is cheating ever so slightly
if(scont1/cp1 >=1.0) scont1--;// this is cheating ever so slightly
if(scont2/cp2 >=1.0) scont2--;// this is cheating ever so slightly
	//FIll the hists for this plane
	fA45_L3_P0->Fill(scont0/cp0);
	fA45_L3_P1->Fill(scont1/cp1);
	fA45_L3_P2->Fill(scont2/cp2);
	fA45_L3_P0E->Fill(mcE,scont0/cp0);
	fA45_L3_P1E->Fill(mcE,scont1/cp1);
	fA45_L3_P2E->Fill(mcE,scont2/cp2);
		}// if s45tpc
	}// if contained and energy
//=============
//== 4 Rad Lneghts and 45 angle
//=============
	// Fill out a few histograms to get a sense for things
	if(mccontained>.9 && mcE<500){ 
	bool s45tpc = false;
	 try{
                s45tpc = fgeoconic.ConeInTPC(StartShowerPos,StartShowerDir,4*RadLength,45.0, smoothness);
                }catch(const ::larutil::LArUtilException& e){
                s45tpc = false;
                }
	double scont0=0.0;
	double scont1=0.0;
	double scont2=0.0;
	if( s45tpc ) { 
		for(unsigned int plane = 0 ; plane<nplanes; plane++){	
                        std::vector<larutil::PxHit> contp;
                        try{
                        auto polyproj = fgeoconic.ConicalFeatures(StartShowerPos, StartShowerDir,4*RadLength, 45.0, plane ,smoothness);
                        contp = fgeoconic.PolyContain(PxHitsVect[plane], polyproj);
			 if(plane==0) for(auto const& hit : contp) scont0+=hit.charge;
			 if(plane==1) for(auto const& hit : contp) scont1+=hit.charge;
			 if(plane==2) for(auto const& hit : contp) scont2+=hit.charge;
                        }catch(const ::larutil::LArUtilException& e){
                        continue;}
		}// for loop over planes
if(scont0/cp0 >=1.0) scont0--;// this is cheating ever so slightly
if(scont1/cp1 >=1.0) scont1--;// this is cheating ever so slightly
if(scont2/cp2 >=1.0) scont2--;// this is cheating ever so slightly
	//FIll the hists for this plane
	fA45_L4_P0->Fill(scont0/cp0);
	fA45_L4_P1->Fill(scont1/cp1);
	fA45_L4_P2->Fill(scont2/cp2);
        fA45_L4_P0E->Fill(mcE,scont0/cp0);
        fA45_L4_P1E->Fill(mcE,scont1/cp1);
        fA45_L4_P2E->Fill(mcE,scont2/cp2);
		}// if s45tpc
	}// if contained and energy

//=============
//== 5 Rad Lneghts and 45 angle
//=============
	// Fill out a few histograms to get a sense for things
	if(mccontained>.9 && mcE<500){ 
	bool s45tpc = false;
	 try{
                s45tpc = fgeoconic.ConeInTPC(StartShowerPos,StartShowerDir,5*RadLength,45.0, smoothness);
                }catch(const ::larutil::LArUtilException& e){
                s45tpc = false;
                }
	double scont0=0.0;
	double scont1=0.0;
	double scont2=0.0;
	if( s45tpc ) { 
		for(unsigned int plane = 0 ; plane<nplanes; plane++){	
                        std::vector<larutil::PxHit> contp;
                        try{
                        auto polyproj = fgeoconic.ConicalFeatures(StartShowerPos, StartShowerDir,5*RadLength, 45.0, plane ,smoothness);
                        contp = fgeoconic.PolyContain(PxHitsVect[plane], polyproj);
			 if(plane==0) for(auto const& hit : contp) scont0+=hit.charge;
			 if(plane==1) for(auto const& hit : contp) scont1+=hit.charge;
			 if(plane==2) for(auto const& hit : contp) scont2+=hit.charge;
                        }catch(const ::larutil::LArUtilException& e){
                        continue;}
		}// for loop over planes
if(scont0/cp0 >=1.0) scont0--;// this is cheating ever so slightly
if(scont1/cp1 >=1.0) scont1--;// this is cheating ever so slightly
if(scont2/cp2 >=1.0) scont2--;// this is cheating ever so slightly
	//FIll the hists for this plane
	fA45_L5_P0->Fill(scont0/cp0);
	fA45_L5_P1->Fill(scont1/cp1);
	fA45_L5_P2->Fill(scont2/cp2);
        fA45_L5_P0E->Fill(mcE,scont0/cp0);
        fA45_L5_P1E->Fill(mcE,scont1/cp1);
        fA45_L5_P2E->Fill(mcE,scont2/cp2);

		}// if s45tpc
	}// if contained and energy

//=================================================
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//=================================================

//=============
//== 3 Rad Lneghts and 60 angle
//=============
	// Fill out a few histograms to get a sense for things
	if(mccontained>.9 && mcE<500){ 
	bool s60tpc = false;
	 try{
                s60tpc = fgeoconic.ConeInTPC(StartShowerPos,StartShowerDir,3*RadLength,60.0, smoothness);
                }catch(const ::larutil::LArUtilException& e){
                s60tpc = false;
                }
	double scont0=0.0;
	double scont1=0.0;
	double scont2=0.0;
	if( s60tpc ) { 
		for(unsigned int plane = 0 ; plane<nplanes; plane++){	
                        std::vector<larutil::PxHit> contp;
                        try{
                        auto polyproj = fgeoconic.ConicalFeatures(StartShowerPos, StartShowerDir,3*RadLength, 60.0, plane ,smoothness);
                        contp = fgeoconic.PolyContain(PxHitsVect[plane], polyproj);
			 if(plane==0) for(auto const& hit : contp) scont0+=hit.charge;
			 if(plane==1) for(auto const& hit : contp) scont1+=hit.charge;
			 if(plane==2) for(auto const& hit : contp) scont2+=hit.charge;
                        }catch(const ::larutil::LArUtilException& e){
                        continue;}
		}// for loop over planes
if(scont0/cp0 >=1.0) scont0--;// this is cheating ever so slightly
if(scont1/cp1 >=1.0) scont1--;// this is cheating ever so slightly
if(scont2/cp2 >=1.0) scont2--;// this is cheating ever so slightly
	//FIll the hists for this plane
	fA60_L3_P0->Fill(scont0/cp0);
	fA60_L3_P1->Fill(scont1/cp1);
	fA60_L3_P2->Fill(scont2/cp2);
        fA60_L3_P0E->Fill(mcE,scont0/cp0);
        fA60_L3_P1E->Fill(mcE,scont1/cp1);
        fA60_L3_P2E->Fill(mcE,scont2/cp2);
		}// if s60tpc
	}// if contained and energy
//=============
//== 4 Rad Lneghts and 60 angle
//=============
	// Fill out a few histograms to get a sense for things
	if(mccontained>.9 && mcE<500){ 
	bool s60tpc = false;
	 try{
                s60tpc = fgeoconic.ConeInTPC(StartShowerPos,StartShowerDir,4*RadLength,60.0, smoothness);
                }catch(const ::larutil::LArUtilException& e){
                s60tpc = false;
                }
	double scont0=0.0;
	double scont1=0.0;
	double scont2=0.0;
	if( s60tpc ) { 
		for(unsigned int plane = 0 ; plane<nplanes; plane++){	
                        std::vector<larutil::PxHit> contp;
                        try{
                        auto polyproj = fgeoconic.ConicalFeatures(StartShowerPos, StartShowerDir,4*RadLength, 60.0, plane ,smoothness);
                        contp = fgeoconic.PolyContain(PxHitsVect[plane], polyproj);
			 if(plane==0) for(auto const& hit : contp) scont0+=hit.charge;
			 if(plane==1) for(auto const& hit : contp) scont1+=hit.charge;
			 if(plane==2) for(auto const& hit : contp) scont2+=hit.charge;
                        }catch(const ::larutil::LArUtilException& e){
                        continue;}
		}// for loop over planes
if(scont0/cp0 >=1.0) scont0--;// this is cheating ever so slightly
if(scont1/cp1 >=1.0) scont1--;// this is cheating ever so slightly
if(scont2/cp2 >=1.0) scont2--;// this is cheating ever so slightly
	//FIll the hists for this plane
	fA60_L4_P0->Fill(scont0/cp0);
	fA60_L4_P1->Fill(scont1/cp1);
	fA60_L4_P2->Fill(scont2/cp2);
        fA60_L4_P0E->Fill(mcE,scont0/cp0);
        fA60_L4_P1E->Fill(mcE,scont1/cp1);
        fA60_L4_P2E->Fill(mcE,scont2/cp2);
		}// if s60tpc
	}// if contained and energy

//=============
//== 5 Rad Lneghts and 60 angle
//=============
	// Fill out a few histograms to get a sense for things
	if(mccontained>.9 && mcE<500){ 
	bool s60tpc = false;
	 try{
                s60tpc = fgeoconic.ConeInTPC(StartShowerPos,StartShowerDir,5*RadLength,60.0, smoothness);
                }catch(const ::larutil::LArUtilException& e){
                s60tpc = false;
                }
	double scont0=0.0;
	double scont1=0.0;
	double scont2=0.0;
	if( s60tpc ) { 
		for(unsigned int plane = 0 ; plane<nplanes; plane++){	
                        std::vector<larutil::PxHit> contp;
                        try{
                        auto polyproj = fgeoconic.ConicalFeatures(StartShowerPos, StartShowerDir,5*RadLength, 60.0, plane ,smoothness);
                        contp = fgeoconic.PolyContain(PxHitsVect[plane], polyproj);
			 if(plane==0) for(auto const& hit : contp) scont0+=hit.charge;
			 if(plane==1) for(auto const& hit : contp) scont1+=hit.charge;
			 if(plane==2) for(auto const& hit : contp) scont2+=hit.charge;
                        }catch(const ::larutil::LArUtilException& e){
                        continue;}
		}// for loop over planes
	//FIll the hists for this plane
if(scont0/cp0 >=1.0) scont0--;// this is cheating ever so slightly
if(scont1/cp1 >=1.0) scont1--;// this is cheating ever so slightly
if(scont2/cp2 >=1.0) scont2--;// this is cheating ever so slightly
	fA60_L5_P0->Fill(scont0/cp0);
	fA60_L5_P1->Fill(scont1/cp1);
	fA60_L5_P2->Fill(scont2/cp2);
        fA60_L5_P0E->Fill(mcE,scont0/cp0);
        fA60_L5_P1E->Fill(mcE,scont1/cp1);
        fA60_L5_P2E->Fill(mcE,scont2/cp2);
		}// if s60tpc
	}// if contained and energy












//=================================================
//$$$$$$$$$$$$$$$$$---END---$$$$$$$$$$$$$$$$$$$$$$$
//-------------Bring in info-----------------------
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//=================================================
    return true;
  }

  bool GammaContainment::finalize() {

	if(_fout)
	_fout->cd();
	FullTree->Write();
	fA30_L3_P0->Write();
	fA30_L3_P1->Write();
	fA30_L3_P2->Write();
	fA30_L4_P0->Write();
	fA30_L4_P1->Write();
	fA30_L4_P2->Write();
	fA30_L5_P0->Write();
	fA30_L5_P1->Write();
	fA30_L5_P2->Write();
	////////////////
	fA45_L3_P0->Write();
	fA45_L3_P1->Write();
	fA45_L3_P2->Write();
	fA45_L4_P0->Write();
	fA45_L4_P1->Write();
	fA45_L4_P2->Write();
	fA45_L5_P0->Write();
	fA45_L5_P1->Write();
	fA45_L5_P2->Write();
	////////////////
	fA60_L3_P0->Write();
	fA60_L3_P1->Write();
	fA60_L3_P2->Write();
	fA60_L4_P0->Write();
	fA60_L4_P1->Write();
	fA60_L4_P2->Write();
	fA60_L5_P0->Write();
	fA60_L5_P1->Write();
	fA60_L5_P2->Write();



		//////
	fA30_L3_P0E->Write();
	fA30_L3_P1E->Write();
	fA30_L3_P2E->Write();
	fA30_L4_P0E->Write();
	fA30_L4_P1E->Write();
	fA30_L4_P2E->Write();
	fA30_L5_P0E->Write();
	fA30_L5_P1E->Write();
	fA30_L5_P2E->Write();

	fA45_L3_P0E->Write();
	fA45_L3_P1E->Write();
	fA45_L3_P2E->Write();
	fA45_L4_P0E->Write();
	fA45_L4_P1E->Write();
	fA45_L4_P2E->Write();
	fA45_L5_P0E->Write();
	fA45_L5_P1E->Write();
	fA45_L5_P2E->Write();


	fA60_L3_P0E->Write();
	fA60_L3_P1E->Write();
	fA60_L3_P2E->Write();
	fA60_L4_P0E->Write();
	fA60_L4_P1E->Write();
	fA60_L4_P2E->Write();
	fA60_L5_P0E->Write();
	fA60_L5_P1E->Write();
	fA60_L5_P2E->Write();

    return true;
  }


 void GammaContainment::InitializeAnaTree()
        {
        FullTree = new TTree("FullTree","FullTree");
                FullTree->Branch("cp0",&cp0,"cp0/D");
                FullTree->Branch("cp1",&cp1,"cp1/D");
                FullTree->Branch("cp2",&cp2,"cp2/D");
                FullTree->Branch("mcsdeposit0",&mcsdeposit0,"mcsdeposit0/D");
                FullTree->Branch("mcsdeposit1",&mcsdeposit1,"mcsdeposit1/D");
                FullTree->Branch("mcsdeposit2",&mcsdeposit2,"mcsdeposit2/D");
                FullTree->Branch("cont0",&cont0,"cont0/D");
                FullTree->Branch("cont1",&cont1,"cont1/D");
                FullTree->Branch("cont2",&cont2,"cont2/D");
	}


}
#endif
