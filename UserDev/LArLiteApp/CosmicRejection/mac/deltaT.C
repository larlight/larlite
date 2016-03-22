
void deltaT(){

    //The two input files to this function should be cosmic files run 
    //with TimeCompat Alg and those run without

    TFile * f = new TFile("cos_TC.root");// ana_mc_w_tc_v2.root");
    TFile * f_wo = new TFile("cos_no_TC.root"); //ana_mc_wo_tc_v2.root");

    TTree * m = (TTree*)f->Get("match_tree") ;
    TTree * p = (TTree*)f->Get("prim_tree") ;

    TTree * m_wo = (TTree*)f_wo->Get("match_tree") ;

    TCanvas * c0 = new TCanvas("c0","c0");
    TH1D * h0 = new TH1D("h0","MCTruth Flash Matched Time Diff",120,-6000,7000);
    TH1D * h1 = new TH1D("h1","MCTruth Flash Matched Time Diff",120,-6000,7000);
    m->Draw("_ft - _mct>>h0","");
    m_wo->Draw("_ft - _mct>>h1","");
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

    TCanvas * c4 = new TCanvas("c4","c4");
    TH1D * h3 = new TH1D("h3","MCTruth Flash Matched Time Diff",100,0,120);
    TH1D * h4 = new TH1D("h4","MCTruth Flash Matched Time Diff",100,0,120);
    m->Draw("1000*(ft - mct)>>h3",""); //,"_misID==0");
    m_wo->Draw("1000*(ft - mct)>>h4",""); //,"_misID==0");
    h3->SetLineColor(kRed) ;
    h3->SetLineWidth(2);
    h3->GetXaxis()->SetTitle("Delta T [ns]");
    h4->SetLineColor(kBlue) ;
    h4->SetLineWidth(2);
    h4->DrawCopy();
    h3->Draw("same") ;
    gStyle->SetOptStat(0);
    c4->Print("DeltaT_ns.png","png");

}
