#ifndef RECOTOOL_FORCEREGIONS_CXX
#define RECOTOOL_FORCEREGIONS_CXX

#include "Quality.h"

namespace cluster {


  double Quality::TimeProf( std::vector<larutil::PxHit> hita , std::vector<larutil::PxHit> hitb) {

//    double T2CM = larutil::GeometryUtilities::GetME()->TimeToCm();
  //  double W2CM = larutil::GeometryUtilities::GetME()->WireToCm();
   int nplanes = 3;
   double ks = 0.0;
   std::vector< std::vector<TH1D*> > signals(nplanes);
   std::vector< std::vector<TH1D*> > pulses(nplanes);

double time_diff = ( larutil::DetectorProperties::GetME()->GetXTicksOffset(hita.at(0).plane) -
                        larutil::DetectorProperties::GetME()->GetXTicksOffset(hitb.at(0).plane) ) * larutil::GeometryUtilities::GetME()->TimeToCm();

   // First go look for the min & max of hits 
   double min_time = larutil::DetectorProperties::GetME()->NumberTimeSamples()*larutil::GeometryUtilities::GetME()->TimeToCm();
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
     //time -= larutil::DetectorProperties::GetME()->GetXTicksOffset(ha.plane)*larutil::GeometryUtilities::GetME()->TimeToCm();
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
     //time -= larutil::DetectorProperties::GetME()->GetXTicksOffset(hb.plane)*larutil::GeometryUtilities::GetME()->TimeToCm();
     double charge = hb.charge;
     int bin = histo_b.FindBin(time);
     histo_b.SetBinContent(bin,histo_b.GetBinContent(bin)+charge);
     for (int j = bin; j<=histo_b.GetNbinsX(); ++j){
       histo_intb.SetBinContent(j,histo_intb.GetBinContent(j)+charge);
     }
   }


//   std::cout
  //   << "siga: "<< histo_a.GetEntries() << std::endl
    // << "sigb: "<< histo_b.GetEntries() << std::endl
//     << "siginta: "<<histo_inta.GetEntries() << std::endl
  //   << "sigintb: "<<histo_intb.GetEntries() << std::endl
    // << std::endl;

   if (histo_intb.Integral()) histo_intb.Scale(1./histo_intb.GetBinContent(histo_intb.GetNbinsX()));
   ks = histo_inta.KolmogorovTest(&histo_intb);

//   std::cout<<"\033[91m This is the Quality Check :: (0,1) "<<ks<<std::endl;
//<<"\033[99m"<<std::endl;
   return ks;
	}

//-----------------------------------------------------------------------------------------------------------------------

  //std::pair<double,double> Quality::ProtoVertex( std::vector<larutil::PxHit> hita , std::vector<larutil::PxHit> hitb) {
  std::vector<std::pair<double,double>> Quality::ProtoVertexCW( const std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>> Regions, const larlite::event_hit* hits ) {


	unsigned int nplanes =3;
	std::vector<std::pair<double,double>> returnPV(nplanes);

//    double T2CM = larutil::GeometryUtilities::GetME()->TimeToCm();
  //  double W2CM = larutil::GeometryUtilities::GetME()->WireToCm();


	for(unsigned int k=0; k<nplanes;k++) {

	// make the vectors of hits
            std::vector<larutil::PxHit> hita;
            std::vector<larutil::PxHit> hitb;
	   if(Regions[k].first.size()>15 && Regions[k].second.size()>15)
		{
           fCRUHelper.GeneratePxHit(Regions[k].first, hits, hita);
           fCRUHelper.GeneratePxHit(Regions[k].second, hits, hitb);

	

	// clusters are comming in as Pxhit so we are already in CM 
        double aroughslope=0;
        double aroughcept=0;
		// Why is this a problem ? ? ? RG
        double broughslope = -987654321;
        double broughcept = -987654321;
	// first calculate the slopes and cepts of each cluster( Region ) 
		// cluster a
	double atotcharge  = 0;
        double aavgcharge = 0;
        double awiretime=  0;
        double awire =  0;
        double atime =  0;
        double awirewire = 0;
        int na=0;
	if(hita.size()>15){
              	for(auto const& thehit : hita) {
			atotcharge+=thehit.charge;}
			aavgcharge=atotcharge/hita.size();
        	for(auto const& thehit : hita) {
                	if(thehit.charge>aavgcharge){
				na+=1;
				awiretime += thehit.w * thehit.t;
				awire += thehit.w;
				atime += thehit.t;
				awirewire += thehit.w * thehit.w;
                                        	}//if charge
                                   	}//end of loop over the hits
		aroughslope = (na * awiretime - awire * atime)/(na*awirewire-awire*awire);
		aroughcept= atime/na - aroughslope *(awire/na);
		}// If hits are more than min

	double btotcharge  = 0;
        double bavgcharge = 0;
        double bwiretime=  0;
        double bwire =  0;
        double btime =  0;
        double bwirewire = 0;
        int nb=0;
	if(hitb.size()>15){
              	for(auto const& thehit : hitb) {
			btotcharge+=thehit.charge;}
			bavgcharge=btotcharge/hitb.size();
        	for(auto const& thehit : hitb) {
                	if(thehit.charge>bavgcharge){
				nb+=1;
				bwiretime += thehit.w * thehit.t;
				bwire += thehit.w;
				btime += thehit.t;
				bwirewire += thehit.w * thehit.w;
                                        	}//if charge
                                   	}//end of loop over the hits
	
		broughslope = (nb * bwiretime - bwire * btime)/(nb*bwirewire-bwire*bwire);
		broughcept= btime/nb - broughslope *(bwire/nb);
		}// If hits are more than min

	// calculate the protovertex

	double protowirevert= (broughcept-aroughcept)/(aroughslope-broughslope);
	double prototimevert= aroughslope *protowirevert + aroughcept;
	std::pair<double,double> wiretime(protowirevert,prototimevert);
	returnPV[k] = wiretime;
	}// if we pass the min hits for both cluster... should always happen with proper input 
	else{std::pair<double,double> bad(-999,-999); returnPV[k]= bad;}
	


}// looping over planes



	

   	return returnPV;

	}

//------------------------------------------------------------------------------------------------------------------------------
  std::pair<double,double> Quality::ProtoVertexcw( std::vector<larutil::PxHit> hita , std::vector<larutil::PxHit> hitb) {

	// clusters are comming in as Pxhit so we are already in CM 
        double aroughslope;
        double aroughcept;
        double broughslope;
        double broughcept;

	// first calculate the slopes and cepts of each cluster( Region ) 

		// cluster a
//	double ahighcharge  = 0;
        //double aavgcharge = 0;
        double awiretime=  0;
        double awire =  0;
        double atime =  0;
        double awirewire = 0;
        int na=0;
	if(hita.size()>15){
 //             	for(auto const& thehit : hita) {
//			if(thehit.charge>ahighcharge) ahighcharge = thehit.charge;}
        	for(auto const& thehit : hita) {
		// this can be done better
			for( int i = 0 ; i<thehit.charge; i++){
				na+=1;
				awiretime += thehit.w * thehit.t;
				awire += thehit.w;
				atime += thehit.t;
				awirewire += thehit.w * thehit.w;
                                        	}//looping over charge a lot
                                   	}//end of loop over the hits
		aroughslope = (na * awiretime - awire * atime)/(na*awirewire-awire*awire);
		aroughcept= atime/na - aroughslope *(awire/na);
		}// If hits are more than min

//	double bhighcharge  = 0;
       // double bavgcharge = 0;
        double bwiretime=  0;
        double bwire =  0;
        double btime =  0;
        double bwirewire = 0;
        int nb=0;
	if(hitb.size()>15){
 //             	for(auto const& thehit : hitb) {
//			if(thehit.charge>bhighcharge) bhighcharge = thehit.charge;}
        	for(auto const& thehit : hitb) {
			// this can be done better
			for( int i = 0 ; i<thehit.charge; i++){
				nb+=1;
				bwiretime += thehit.w * thehit.t;
				bwire += thehit.w;
				btime += thehit.t;
				bwirewire += thehit.w * thehit.w;
                                        	}//looping over charge a lot
                                   	}//end of loop over the hits
		broughslope = (nb * bwiretime - bwire * btime)/(nb*bwirewire-bwire*bwire);
		broughcept= btime/nb - broughslope *(bwire/nb);
		}// If hits are more than min




	// calculate the protovertex

	if(hita.size()>15 &&hitb.size()>15){
	double protowirevert= (broughcept-aroughcept)/(aroughslope-broughslope);
	double prototimevert= aroughslope *protowirevert + aroughcept;
	std::pair<double,double> wiretime(protowirevert,prototimevert);
   	return wiretime;
	}// if we pass the min hits for both cluster... should always happen with proper input 
	else{std::pair<double,double> bad(-999,-999); return bad;}
	}








//-----------------------------------------------------------------------------------------------------------------------
/*
  std::vector<bool> Quality::PlaneCheck(std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>> Regions, std::vector<std::vector<larlite::hit>> hits  ) {

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
                double aa = Quality.TimeProf(pxfa_v0,pxfa_v1);
                double ab = Quality.TimeProf(pxfa_v0,pxfb_v1);
                double ba = Quality.TimeProf(pxfb_v0,pxfa_v1);
                double bb = Quality.TimeProf(pxfb_v0,pxfb_v1);
                        if(aa>QlevelHi && bb>QlevelHi && ab<QlevelLo && ba<QlevelLo) Q01 = true;
                        if(aa<QlevelLo && bb<QlevelLo && ab>QlevelHi && ba>QlevelHi) Q01 = true;
                        if(aa>QlevelVert && bb>QlevelVert && ab>QlevelVert && ba>QlevelVert) Q01 = false;// need a switch... vertical showers   
                                        }//plane 1

                if(Regions[2].first.size() > 15 && Regions[2].second.size()>15){
                    fCRUHelper.GeneratePxHit(Regions[2].first, hits, pxfa_v2);
                    fCRUHelper.GeneratePxHit(Regions[2].second, hits, pxfb_v2);
                std::cout<< "\t \033[97m Plane 02:"<<std::endl;
                double aa = Quality.TimeProf(pxfa_v0,pxfa_v2);
                double ab = Quality.TimeProf(pxfa_v0,pxfb_v2);
                double ba = Quality.TimeProf(pxfb_v0,pxfa_v2);
                double bb = Quality.TimeProf(pxfb_v0,pxfb_v2);
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
                double aa = Quality.TimeProf(pxfa_v1,pxfa_v2);
                double ab = Quality.TimeProf(pxfa_v1,pxfb_v2);
                double ba = Quality.TimeProf(pxfb_v1,pxfa_v2);
                double bb = Quality.TimeProf(pxfb_v1,pxfb_v2);
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


