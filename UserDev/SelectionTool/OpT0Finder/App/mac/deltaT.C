void deltaZT(){

    TFile * f = new TFile("anaQLL_wo_tc.root"); // ana.root");
    TFile * f_wo = new TFile("anaQLL_w_tc.root"); // ana.root");

    TTree * m = (TTree*)f->Get("flashmatch_tree") ;
    TTree * m_wo = (TTree*)f_wo->Get("flashmatch_tree") ;
    //TTree * ti = (TTree*)f->Get("int_tree") ;

    TCanvas * c0 = new TCanvas("c0","c0");
    TH1D * h0 = new TH1D("h0","MCTruth Flash Matched Time Diff",120,-6000,7000);
    TH1D * h1 = new TH1D("h1","MCTruth Flash Matched Time Diff",120,-6000,7000);
    m->Draw("ft - mct>>h0","");
    m_wo->Draw("ft - mct>>h1","");
    h0->GetXaxis()->SetTitle("Delta T [us]");
    gStyle->SetOptStat(0);
    h0->SetLineColor(kRed) ;
    h0->SetLineWidth(2);
    c0->SetLogy();
    h0->DrawCopy() ;
    h1->SetLineColor(kBlue) ;
    h1->SetLineWidth(2);
    h1->Draw("same") ;
    c0->Print("DeltaT_us.png","png");

//    TCanvas * c4 = new TCanvas("c4","c4");
//    TH1D * h4 = new TH1D("h4","Matched Flash-MCTrack DeltaT[us]",100,-15,15);
//    t->Draw("(ft - mct)>>h4",""); //,"_misID==0");
//    h4->SetFillStyle( 3001);
//    h4->SetFillColor( kBlue);
//    h4->GetXaxis()->SetTitle("Delta T [us]");
//    h4->DrawCopy() ;
//    gStyle->SetOptStat(1);
//    c4->SetLogy();
//    c4->Print("DeltaT_Com.png","png");



}
