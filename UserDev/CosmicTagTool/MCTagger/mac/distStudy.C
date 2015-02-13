
void distStudy(){

	TFile * _file0 = new TFile("tagShowersSmall.root","READ"); //mcshowerBkgdDepE.root","READ") ;
	TTree * t = ( TTree *)_file0->Get("tag_tree");

	 // get number of events from histogram
	  Int_t Nevents;
	  TH1D *hEvents = (TH1D*)_file0->Get("numberOfEvents");
	  Nevents = hEvents->GetEntries();
	  Double_t Time = Nevents*(6.4E-3); //in seconds
	  Double_t ExperimentTime = (1.6E-6)*(6.6E20)/(5E12);
	std::cout<<"Events: "<<Nevents<<std::endl;
	std::cout<<"Scaling by: "<<ExperimentTime/Time <<std::endl;

	//Photon Conversions (no fiducial cut): All Comptons and pair productions in the detector, parent gamma out of detector
	TH1D * h = new TH1D("h","Min dist shower to track fiducial",200,0,250);
	TCanvas * c = new TCanvas("c","c");
	t->Draw("_X>>h","_E>200 ") ;//&& _X > 25 && _X < 231 && _Y > -91 && _Y < 91 && _Z > 30& _Z < 936 ");
	h->GetXaxis()->SetTitle("Distance to track (cm)");
//	h->Scale(ExperimentTime/Time);
	c->SetLogy();
	h->Draw();
//\"compt\" \"conv\"
//	TH1D * h1 = new TH1D("h1","Min dist shower to track active",200,0.08,500);
//	TCanvas * c1 = new TCanvas("c1","c1");
//	t->Draw("_minMuDist>>h1","_inActiveVolume");
//	h1->GetXaxis()->SetTitle("Distance to track (cm)");
// 	h1->Scale(1/h1->Integral()) ;
//	h1->Fit("gaus");
////	c1->SetLogy();



//	TLegend *l = new TLegend(0.6,0.6,0.7,0.75) ;
//	l->AddEntry(h10,"Z, No Energy Cut","fl");   // h1 and h2 are histogram pointers
//	l->Draw();

} //end testScript.C
