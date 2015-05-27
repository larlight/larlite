#ifndef HITCALIBRATION_CXX
#define HITCALIBRATION_CXX

#include "HitCalibration.h"

namespace larlite {

  bool HitCalibration::initialize() {
   
    _simch_map.clear();

    _allQs.clear();
    _allEs.clear();

    _ADCs_Pl0.clear();
    _IDEs_Pl0.clear();
    _ADCs_Pl1.clear();
    _IDEs_Pl1.clear();
    _ADCs_Pl2.clear();
    _IDEs_Pl2.clear();

    _hADCsVSIDEs_Pl0 = new TH2D("hADCsVSIDEs_Pl0", "Hit ADC vs IDEs e- Matched; Pulse Area ADCs; Simch num. e- - Pl 0", 100, 0, 1000, 100, 0, 100000);
    _hADCsVSIDEs_Pl1 = new TH2D("hADCsVSIDEs_Pl1", "Hit ADC vs IDEs e- Matched; Pulse Area ADCs; Simch num. e- - Pl 1", 100, 0, 1000, 100, 0, 100000);
    _hADCsVSIDEs_Pl2 = new TH2D("hADCsVSIDEs_Pl2", "Hit ADC vs IDEs e- Matched; Pulse Area ADCs; Simch num. e- - Pl 2", 100, 0, 1000, 100, 0, 100000);

    _hRecoVSMcIDEs_Pl0 = new TH2D("hRecoVSMcIDEs_Pl0", "Hit ADC vs IDEs e- Matched; Pulse Area ADCs; Simch num. e- - Pl 0", 100, 0, 100000, 100, 0, 100000);
    _hRecoDivMcIDEs_Pl0 = new TH1D("hRecoDivMcIDEs_Pl0", "Reco e- / MC e-", 100, -3, 3);
    _hRecoVSMcIDEs_Pl1 = new TH2D("hRecoVSMcIDEs_Pl1", "Hit ADC vs IDEs e- Matched; Pulse Area ADCs; Simch num. e- - Pl 1", 100, 0, 100000, 100, 0, 100000);
    _hRecoDivMcIDEs_Pl1 = new TH1D("hRecoDivMcIDEs_Pl1", "Reco e- / MC e-", 100, -3, 3);
    _hRecoVSMcIDEs_Pl2 = new TH2D("hRecoVSMcIDEs_Pl2", "Hit ADC vs IDEs e- Matched; Pulse Area ADCs; Simch num. e- - Pl 2", 100, 0, 100000, 100, 0, 100000);
    _hRecoDivMcIDEs_Pl2 = new TH1D("hRecoDivMcIDEs_Pl2", "Reco e- / MC e-", 100, -3, 3);
    _hRecoHitElectrons = new TH1D("hRecoHitElectrons", "Reco e- from each hit; Hit.Integral() / ADCtoElectrons(Hit.Plane())", 100, 0, 500000);
    _hSimchQvsE_noLifetime = new TH2D("hSimchQvsE_noLifetime", "Q Deposited Collected vs. E deposited for all IDEs", 300, 0, 100000, 300, 0, 10);
    _hSimchQvsE = new TH2D("hSimchQvsE", "Q Deposited (lifetime-adjusted) vs. E deposited for all IDEs", 300, 0, 100000, 300, 0, 10);
    _hHitQvsE = new TH2D("hHitQvsE", "Q Deposited (lifetime-adjusted) vs. E deposited for all Hits", 300, 0, 300000, 300, 0, 30);
    _hHitRecoEvsE = new TH1D("hHitRecoEvsE", "Reconstructed Hit Energy - Deposited Energy; Energy [ MeV ]", 100, -15, 15);

    return true;
  }
  
  bool HitCalibration::analyze(storage_manager* storage) {
  
    std::vector< double > fCalAreaConstants;
    static const double kCalAreaConstants_UB[3] = {7.488e-3, 5.110e-3, 1.008e-2};
    fCalAreaConstants.push_back(kCalAreaConstants_UB[0]);
    fCalAreaConstants.push_back(kCalAreaConstants_UB[1]);
    fCalAreaConstants.push_back(kCalAreaConstants_UB[2]);

    //Get All hits in events (focus on gaushit)
    auto evt_hit = storage->get_data<event_hit>("gaushit");
    if (!evt_hit){
      std::cout << "No hits with this producer in file..." << std::endl;
      return false;
    }

    //Get All SimCh objects
    auto evt_simch = storage->get_data<event_simch>("largeant");

    //Make a map for the simchannels and the LAr Chs
    _simch_map.clear();
    for(auto simCh : *evt_simch)
      _simch_map.insert(std::pair<UShort_t,simch>(simCh.Channel(),simCh));

    for (auto h : *evt_hit){

      //Find the simchannels on the hit's channel number
      auto simch_iter = _simch_map.find( h.Channel() );
      if( simch_iter == _simch_map.end() ) 
	std::cerr<<"Hit has no matched simchannel!!"<<std::endl;

      if (_verbose) { std::cout << "Hit time boundaries: [" << h.StartTick() << ", " << h.EndTick() << "]" << std::endl; }

      //Find IDEs on this simch
      double sigmaTime = h.PeakTime() - h.StartTick();
      std::vector<larlite::ide> matchedIDEs((*simch_iter).second.TrackIDsAndEnergies((h.PeakTime()-3200)-sigmaTime*2,(h.PeakTime()-3200)+sigmaTime*2));

      //if matchedIDEs is empty no IDEs in the time-range of the hit on that channel
      if ( matchedIDEs.size() == 0 ){
	std::cerr << "Hit with no associated IDEs..." << std::endl;
	return true;
      }


      //else loop through found IDEs and and add charge in them      
      else{

	double numElectrons = 0;
	double energyDep = 0;
	for ( auto const &IDE : matchedIDEs ){
	  numElectrons += IDE.numElectrons;
	  energyDep    += IDE.energy;
	  _hSimchQvsE->Fill(IDE.numElectrons*_CaloAlg.LifetimeCorrection(h.PeakTime()), IDE.energy);
	  _hSimchQvsE_noLifetime->Fill(IDE.numElectrons, IDE.energy);
	}
	_hHitQvsE->Fill(numElectrons*_CaloAlg.LifetimeCorrection(h.PeakTime()), energyDep);
	_hHitRecoEvsE->Fill( numElectrons*_CaloAlg.LifetimeCorrection(h.PeakTime())*1.e3/(::larutil::kGeVToElectrons) - energyDep );
	
	//Find plane:
	int pl = int(h.View());
	std::cout << "Plane: " << pl << std::endl;
	if ( pl == 0 ){
	  _hADCsVSIDEs_Pl0->Fill( h.Integral(), numElectrons );
	  _ADCs_Pl0.push_back( h.Integral() );
	  _IDEs_Pl0.push_back( numElectrons );
	  _hRecoVSMcIDEs_Pl0->Fill( numElectrons, (h.Integral()/fCalAreaConstants.at(0)) );
	  _hRecoDivMcIDEs_Pl0->Fill( (h.Integral()/fCalAreaConstants.at(2))/numElectrons );
	}
	if ( pl == 1 ){
	  _hADCsVSIDEs_Pl1->Fill( h.Integral(), numElectrons );
	  _ADCs_Pl1.push_back( h.Integral() );
	  _IDEs_Pl1.push_back( numElectrons );
	  _hRecoVSMcIDEs_Pl1->Fill( numElectrons, (h.Integral()/fCalAreaConstants.at(1)) );
	  _hRecoDivMcIDEs_Pl1->Fill( (h.Integral()/fCalAreaConstants.at(2))/numElectrons );
	}
	if ( pl == 2 ){
	  _hADCsVSIDEs_Pl2->Fill( h.Integral(), numElectrons );
	  _ADCs_Pl2.push_back( h.Integral() );
	  _IDEs_Pl2.push_back( numElectrons );
	  _hRecoVSMcIDEs_Pl2->Fill( numElectrons, (h.Integral()/fCalAreaConstants.at(2)) );
	  _hRecoDivMcIDEs_Pl2->Fill( (h.Integral()/fCalAreaConstants.at(2))/numElectrons );
	}
	
	_hRecoHitElectrons->Fill( h.Integral()/fCalAreaConstants.at(pl) );

      }


    }//end loop over hits
  
    return true;
  }

  bool HitCalibration::finalize() {

    _hADCsVSIDEs_Pl0->Write();  
    _hADCsVSIDEs_Pl1->Write();  
    _hADCsVSIDEs_Pl2->Write();  

    _hRecoVSMcIDEs_Pl0->Write();
    _hRecoVSMcIDEs_Pl1->Write();
    _hRecoVSMcIDEs_Pl2->Write();

    _hRecoHitElectrons->Write();

    _hSimchQvsE->Write();
    _hSimchQvsE_noLifetime->Write();
    _hHitQvsE->Write();
    _hHitRecoEvsE->Write();

    //Fill TGraphs:
    const Int_t Hits_Pl0 = _ADCs_Pl0.size();
    Double_t ADCs_Pl0[Hits_Pl0];
    Double_t IDEs_Pl0[Hits_Pl0];
    for ( int u = 0; u < Hits_Pl0; u++){
      ADCs_Pl0[u] = _ADCs_Pl0.at(u);
      IDEs_Pl0[u] = _IDEs_Pl0.at(u);
    }
    const Int_t Hits_Pl1 = _ADCs_Pl1.size();
    Double_t ADCs_Pl1[Hits_Pl1];
    Double_t IDEs_Pl1[Hits_Pl1];
    for ( int v = 0; v < Hits_Pl1; v++){
      ADCs_Pl1[v] = _ADCs_Pl1.at(v);
      IDEs_Pl1[v] = _IDEs_Pl1.at(v);
    }
    const Int_t Hits_Pl2 = _ADCs_Pl2.size();
    Double_t ADCs_Pl2[Hits_Pl2];
    Double_t IDEs_Pl2[Hits_Pl2];
    for ( int y = 0; y < Hits_Pl2; y++){
      ADCs_Pl2[y] = _ADCs_Pl2.at(y);
      IDEs_Pl2[y] = _IDEs_Pl2.at(y);
    }

    TGraph *gADCsVSIDEs_Pl0 = new TGraph(Hits_Pl0, ADCs_Pl0, IDEs_Pl0);
    gADCsVSIDEs_Pl0->SetTitle("Pl0");
    gADCsVSIDEs_Pl0->GetXaxis()->SetTitle("Hit Peak Amplitude");
    gADCsVSIDEs_Pl0->GetYaxis()->SetTitle("e- in [start point, end point] hit range");
    gADCsVSIDEs_Pl0->Write();
    TGraph *gADCsVSIDEs_Pl1 = new TGraph(Hits_Pl1, ADCs_Pl1, IDEs_Pl1);
    gADCsVSIDEs_Pl1->SetTitle("Pl1");
    gADCsVSIDEs_Pl1->GetXaxis()->SetTitle("Hit Peak Amplitude");
    gADCsVSIDEs_Pl1->GetYaxis()->SetTitle("e- in [start point, end point] hit range");
    gADCsVSIDEs_Pl1->Write();
    TGraph *gADCsVSIDEs_Pl2 = new TGraph(Hits_Pl2, ADCs_Pl2, IDEs_Pl2);
    gADCsVSIDEs_Pl2->SetTitle("Pl2");
    gADCsVSIDEs_Pl2->GetXaxis()->SetTitle("Hit Peak Amplitude");
    gADCsVSIDEs_Pl2->GetYaxis()->SetTitle("e- in [start point, end point] hit range");
    gADCsVSIDEs_Pl2->Write();

    //Now fit the tgraphs
    auto f_Pl0 = new TF1("f_Pl0","[0]+x*[1]", 0, 4000);
    auto f_Pl1 = new TF1("f_Pl1","[0]+x*[1]", 0, 4000);
    auto f_Pl2 = new TF1("f_Pl2","[0]+x*[1]", 0, 4000);

    gADCsVSIDEs_Pl0->Fit("f_Pl0");
    double pl0_b = f_Pl0->GetParameter(0);
    double pl0_s = f_Pl0->GetParameter(1);
    double th_Pl0 = -atan(pl0_s);
    double sin_Pl0 = sin(th_Pl0);
    double cos_Pl0 = cos(th_Pl0);

    gADCsVSIDEs_Pl1->Fit("f_Pl1");
    double pl1_b = f_Pl1->GetParameter(0);
    double pl1_s = f_Pl1->GetParameter(1);
    double th_Pl1 = -atan(pl1_s);
    double sin_Pl1 = sin(th_Pl1);
    double cos_Pl1 = cos(th_Pl1);

    gADCsVSIDEs_Pl2->Fit("f_Pl2");
    double pl2_b = f_Pl2->GetParameter(0);
    double pl2_s = f_Pl2->GetParameter(1);
    double th_Pl2 = -atan(pl2_s);
    double sin_Pl2 = sin(th_Pl2);
    double cos_Pl2 = cos(th_Pl2);

    //translate and rotate points so that y-coordinate will be distance from line
    for ( size_t u = 0; u < _ADCs_Pl0.size(); u++){
      double x = _ADCs_Pl0.at(u);
      double y = _IDEs_Pl0.at(u) - pl0_b;
      _ADCs_Pl0.at(u) = x*cos_Pl0 - y*sin_Pl0;
      _IDEs_Pl0.at(u) = x*sin_Pl0 + y*cos_Pl0;
    }
      
    for ( size_t v = 0; v < _ADCs_Pl1.size();v++){
      double x = _ADCs_Pl1.at(v);
      double y = _IDEs_Pl1.at(v) - pl1_b;
      _ADCs_Pl1.at(v) = x*cos_Pl1 - y*sin_Pl1;
      _IDEs_Pl1.at(v) = x*sin_Pl1 + y*cos_Pl1;
    }

    for ( size_t Y = 0; Y < _ADCs_Pl2.size(); Y++){
      double x = _ADCs_Pl2.at(Y);
      double y = _IDEs_Pl2.at(Y) - pl2_b;
      _ADCs_Pl2.at(Y) = x*cos_Pl2 - y*sin_Pl2;
      _IDEs_Pl2.at(Y) = x*sin_Pl2 + y*cos_Pl2;
    }

    auto Pl0_resid = new TH1D("Pl0_resid", "Pl0 Residuals", 100, -1000, 1000);
    auto Pl1_resid = new TH1D("Pl1_resid", "Pl1 Residuals", 100, -1000, 1000);
    auto Pl2_resid = new TH1D("Pl2_resid", "Pl2 Residuals", 100, -1000, 1000);

    for (size_t x=0; x < _IDEs_Pl0.size(); x++)
      Pl0_resid->Fill( _IDEs_Pl0.at(x) );
    for (size_t y=0; y < _IDEs_Pl1.size(); y++)
      Pl1_resid->Fill( _IDEs_Pl1.at(y) );
    for (size_t z=0; z < _IDEs_Pl2.size(); z++)
      Pl2_resid->Fill( _IDEs_Pl2.at(z) );

    Pl0_resid->Write();
    Pl1_resid->Write();
    Pl2_resid->Write();

    _hRecoDivMcIDEs_Pl0->Write();
    _hRecoDivMcIDEs_Pl1->Write();
    _hRecoDivMcIDEs_Pl2->Write();
    

    return true;
  }

}
#endif
