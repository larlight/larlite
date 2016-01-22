#ifndef ERTOOL_ERALGOTRACKID_CXX
#define ERTOOL_ERALGOTRACKID_CXX

#include "ERAlgoTrackID.h"

#include "TF1.h"

namespace ertool {



  ERAlgoTrackID::ERAlgoTrackID(const std::string& name) : AlgoBase(name){}



  void ERAlgoTrackID::Reset(){}



  void ERAlgoTrackID::AcceptPSet(const ::fcllite::PSet& cfg){}



  void ERAlgoTrackID::ProcessBegin() {

    contained_chi2 =
      new TH1D("hist_contained_redchi2", "Contained RedChi2", 100, 0, 10);
    uncontained_chi2 =
      new TH1D("hist_uncontained_redchi2", "Uncontained RedChi2", 100, 0, 10);

    volume.Min(1.3, -114, 1.5);
    volume.Max(255, 114, 1035);

  }



  Double_t ERAlgoTrackID::Chi2Test(TGraph * g) {

    TF1 * f1 =
      new TF1("f1","2.2",g->GetXaxis()->GetXmin(),g->GetXaxis()->GetXmax());

    return g->Chisquare(f1);

  }



  void ERAlgoTrackID::FitTrack(Track const & t, Particle & p) {

    std::vector<Double_t> rr;
    std::vector<Double_t> dedxr;
    std::vector<Double_t> const & dedx = t._dedx;

    Double_t sum = 0;

    for(Size_t i = dedx.size() - 1; i >= 0; --i) {

      geoalgo::Point_t const vec(t.at(i) - t.at(i+1));

      sum += sqrt(vec*vec);

      rr.push_back(sum);
      dedxr.push_back(dedx.at(i));

    }



    TGraph * g = new TGraph(rr.size(), &rr[0], &dedxr[0]);
    Double_t const redchi2 = Chi2Test(g) / rr.size();    
    delete g;

    if(volume.Contain(t.front()) && volume.Contain(t.front()))
      contained_chi2->Fill(redchi2);
    else
      uncontained_chi2->Fill(redchi2);

  }



  bool ERAlgoTrackID::Reconstruct(const EventData &data, ParticleGraph& graph){

    for(NodeID_t const n : graph.GetParticleNodes(kTrack)) {
      Particle & p = graph.GetParticle(n);
      FitTrack(data.Track(p.RecoID()), p);
    }
    
    return true;

  }



  void ERAlgoTrackID::ProcessEnd(TFile* fout) {

    contained_chi2->Write();
    uncontained_chi2->Write();

  }



}

#endif
