#ifndef ERTOOL_ERALGOTRACKID_CXX
#define ERTOOL_ERALGOTRACKID_CXX

#include "ERAlgoTrackID.h"

#include <sstream>
#include <algorithm>

#include "TCanvas.h"

namespace ertool {



  TH1D * ERAlgoTrackID::ReadTextFile
  (std::ifstream && f,
   char const * name,
   char const * title) {
    
    if(f.is_open()) {

      std::cout << "Loading " << name << "\n";

      std::vector<Double_t> dedx;
      std::vector<Double_t> rrbins;
      std::vector<Double_t> errors;
      std::string line;

      std::getline(f, line);
      std::stringstream ss;
      ss << line;
      Double_t temprr = -1;
      ss >> temprr;
      rrbins.push_back(temprr);

      while(std::getline(f, line)) {
	
	ss.str("");
	ss.clear();
	ss << line;
	Double_t dedxe = -1;
	ss >> dedxe;
	Double_t rr = -1;
	ss >> rr;
	Double_t error = -1;
	ss >> error;
	dedx.push_back(dedxe);
	rrbins.push_back(rr);
	errors.push_back(error);

      }

      Double_t arr[rrbins.size()];
      for(size_t i = 0; i < rrbins.size(); ++i) arr[i] = rrbins.at(i);

      TH1D * pro = new TH1D(name, title, rrbins.size()-1, arr);

      for(int i = 1; i <= pro->GetNbinsX(); ++i) {
	pro->SetBinContent(i, dedx.at(i-1));
	pro->SetBinError(i, errors.at(i-1));
      }

      return pro;

    }

    else {
      std::cout << "File not open\n";
      exit(0);
    }

  }



  ERAlgoTrackID::ERAlgoTrackID(const std::string& name) :
    AlgoBase(name) {

    kapro = ReadTextFile
      (std::ifstream("/uboone/data/users/rmurrell/dedx_range_ka.txt"),
       "kapro", "Kaon Profile");
    mupro = ReadTextFile
      (std::ifstream("/uboone/data/users/rmurrell/dedx_range_mu.txt"),
       "mupro", "Muon Profile");
    propro = ReadTextFile
      (std::ifstream("/uboone/data/users/rmurrell/dedx_range_pro.txt"),
       "propro", "Proton Profile");
    pipro = ReadTextFile
      (std::ifstream("/uboone/data/users/rmurrell/dedx_range_pi.txt"),
       "pipro", "Pion Profile");

  }



  void ERAlgoTrackID::Reset(){}



  void ERAlgoTrackID::AcceptPSet(const ::fcllite::PSet& cfg){}



  void ERAlgoTrackID::ProcessBegin() {

    tree = new TTree("tree", "");
    tree->Branch("pid", &tpid, "pid/I");
    tree->Branch("redchi2pro", &tredchi2pro, "tredchi2pro/D");
    tree->Branch("redchi2pro_flat", &tredchi2pro_flat, "tredchi2pro_flat/D");
    tree->Branch("redchi2mu", &tredchi2mu, "tredchi2mu/D");  
    tree->Branch("redchi2mu_flat", &tredchi2mu_flat, "tredchi2mu_flat/D");
    tree->Branch("redchi2pi", &tredchi2pi, "tredchi2pi/D");
    tree->Branch("redchi2ka", &tredchi2ka, "tredchi2ka/D");

    track_dedx = new TGraph();
    track_dedx->SetName("track_dedx");
    
    volume.Min(1.3, -114, 1.5);
    volume.Max(255, 114, 1035);

  }



  void ERAlgoTrackID::Chi2Test(std::vector<Double_t> const & rr,
			       std::vector<Double_t> const & dedx) {
    
    Int_t npt = 0;
    Double_t chi2pro = 0;
    Double_t chi2ka = 0;
    Double_t chi2pi = 0;
    Double_t chi2mu = 0;

    double proflat = 2.5;
    double muflat = 1.9;
    double chi2proflat = 0;
    double chi2muflat = 0;

    double proflaterr = 0;
    size_t probinno = 5;
    for(int i = propro->GetNbinsX()-probinno+1;i<=propro->GetNbinsX();++i)
      proflaterr += propro->GetBinError(i);
    proflaterr /= probinno;
    
    double muflaterr = 0;
    size_t mubinno = 5;
    for(int i = mupro->GetNbinsX()-mubinno+1;i<=mupro->GetNbinsX();++i)
      muflaterr += mupro->GetBinError(i);
    muflaterr /= mubinno;

    for(size_t i = 0; i < dedx.size(); ++i) {

      if(dedx.at(i) > 1000) continue; //protect against large pulse height
      Int_t bin = propro->FindBin(rr.at(i));
      if(bin >= 1 && bin <= propro->GetNbinsX()) {

	Double_t bincpro = propro->GetBinContent(bin);
	if(bincpro < 1e-6) {
	  bincpro = (propro->GetBinContent(bin-1) + 
		     propro->GetBinContent(bin+1)) / 2;
	}
	Double_t bincka = kapro->GetBinContent(bin);
	if(bincka < 1e-6) {
	  bincka = (kapro->GetBinContent(bin-1) + 
		    kapro->GetBinContent(bin+1)) / 2;
	}
	Double_t bincpi = pipro->GetBinContent(bin);
	if(bincpi < 1e-6) {
	  bincpi = (pipro->GetBinContent(bin-1) + 
		    pipro->GetBinContent(bin+1)) / 2;
	}
	Double_t bincmu = mupro->GetBinContent(bin);
	if(bincmu < 1e-6) {
	  bincmu = (mupro->GetBinContent(bin-1) + 
		    mupro->GetBinContent(bin+1)) / 2;
	}
	Double_t binepro = propro->GetBinError(bin);
	if(binepro < 1e-6) {
	  binepro = (propro->GetBinError(bin-1) +
		     propro->GetBinError(bin+1)) / 2;
	}
	Double_t bineka = kapro->GetBinError(bin);
	if(bineka < 1e-6) {
	  bineka = (kapro->GetBinError(bin-1) +
		    kapro->GetBinError(bin+1)) / 2;
	}
	Double_t binepi = pipro->GetBinError(bin);
	if(binepi < 1e-6) {
	  binepi = (pipro->GetBinError(bin-1) +
		    pipro->GetBinError(bin+1)) / 2;
	}
	Double_t binemu = mupro->GetBinError(bin);
	if(binemu < 1e-6) {
	  binemu = (mupro->GetBinError(bin-1) +
		    mupro->GetBinError(bin+1)) / 2;
	}
	Double_t errdedx = 0.04231 + 0.0001783*dedx.at(i)*dedx.at(i);
	errdedx *= dedx.at(i);
	chi2pro +=
	  pow((dedx.at(i)-bincpro)/sqrt(pow(binepro,2)+pow(errdedx,2)),2);
	chi2ka +=
	  pow((dedx.at(i)-bincka)/sqrt(pow(bineka,2)+pow(errdedx,2)),2);
	chi2pi +=
	  pow((dedx.at(i)-bincpi)/sqrt(pow(binepi,2)+pow(errdedx,2)),2);
	chi2mu +=
	  pow((dedx.at(i)-bincmu)/sqrt(pow(binemu,2)+pow(errdedx,2)),2);

	chi2proflat +=
	  pow((dedx.at(i)-proflat)/sqrt(pow(proflaterr,2)+pow(errdedx,2)),2);
	chi2muflat +=
	  pow((dedx.at(i)-muflat)/sqrt(pow(muflaterr,2)+pow(errdedx,2)),2);

	++npt;

      }

    }

    if(npt) {
      ftr.tnpt = npt;
      ftr.tredchi2pro = chi2pro / npt;
      ftr.tredchi2ka = chi2ka / npt;
      ftr.tredchi2pi = chi2pi / npt;
      ftr.tredchi2mu = chi2mu / npt;
      ftr.tredchi2pro_flat = chi2proflat / npt;
      ftr.tredchi2mu_flat = chi2muflat / npt;
    }

  }



  void ERAlgoTrackID::GetTrackInfo
  (Track const & t,
   std::vector<Double_t> & rr, 
   std::vector<Double_t> & dedxr) {

    //Full dedx
    std::vector<Double_t> const & dedx = t._dedx;

    Double_t sum = 0;
   
    //Backwards Loop through the track
    for(Size_t i = dedx.size() - 1; i >= 0; --i) {

      //Measures each step-to-step length
      geoalgo::Point_t const vec(t.at(i) - t.at(i+1));

      //Builds the residule range
      sum += sqrt(vec*vec);
   
      if(dedx.at(i)) {   
	rr.push_back(sum);
	dedxr.push_back(dedx.at(i));
      }
    
    }//Backwards Loop through Track

  }


  
  ERAlgoTrackID::FitTrackResults ERAlgoTrackID::FitTrack
  (Track const & t, Bool_t const reverse) {
    
    ftr.Reset();

    std::vector<Double_t> rr;
    std::vector<Double_t> dedxr;

    GetTrackInfo(t, rr, dedxr);

    if(rr.size()) {

      if(!reverse) {
	std::reverse(rr.begin(), rr.end());
	std::reverse(dedxr.begin(), dedxr.end());
	
	for(Size_t i = 0; i < rr.size(); ++i) {

	  track_dedx->SetPoint(counter++, rr.at(i), dedxr.at(i));

	}

      }

      Chi2Test(rr, dedxr);

    }

    return ftr;

  }



  bool ERAlgoTrackID::Reconstruct(const EventData &data, ParticleGraph& graph){

    for(NodeID_t const n : graph.GetParticleNodes(kTrack)) {
      Particle & p = graph.GetParticle(n);
      Track const & t = data.Track(p.RecoID());
      
      if(t._pid != ertool::Track::kProton) continue;

      FitTrackResults const results = FitTrack(t);

      if(results.tnpt) {
 	
	Double_t const redchi2pro = results.tredchi2pro;
	Double_t const redchi2ka = results.tredchi2ka;
	Double_t const redchi2mu = results.tredchi2mu;
	Double_t const redchi2pi = results.tredchi2pi;
	double const redchi2pro_flat = results.tredchi2pro_flat;
	double const redchi2mu_flat = results.tredchi2mu_flat;

	Double_t test = 1e20;
	Int_t id = 0;
	if(redchi2pro < test) {
	  test = redchi2pro;
	  id = 1;
	}
	if(redchi2pro_flat < test) {
	  test = redchi2pro_flat;
	  id = 2;
	}
	if(redchi2mu < test) {
	  test = redchi2mu;
	  id = 3;
	}
	if(redchi2pi < test) {
	  test = redchi2pi;
	  id = 3;	
	}
	if(redchi2mu_flat < test) {
	  test = redchi2mu_flat;
	  id = 4;
	}

	if(id == 1 || 2) p.SetParticleInfo(2212);
	if(id == 3 || 4) p.SetParticleInfo(Track::kMIPy);

	tpid = id;
	
	tredchi2pro = redchi2pro;
	tredchi2pro_flat = redchi2pro_flat;
	tredchi2mu = redchi2mu;
	tredchi2mu_flat = redchi2mu_flat;
	tredchi2pi = redchi2pi;	
	tredchi2ka = redchi2ka;

	tree->Fill();

      }

    }
    
    return true;

  }



  void ERAlgoTrackID::ProcessEnd(TFile* fout) {

    if(fout) {

      fout->cd();

      TCanvas * c1 = new TCanvas("track_dedx");
      track_dedx->Draw("ap");
      c1->Write();
     
      TCanvas * c = new TCanvas("track_dedx_sup");
      propro->Draw();
      track_dedx->Draw("p");
      propro->Draw("same");
      c->Write();

      tree->Write();
     
    }

  }



}

#endif
