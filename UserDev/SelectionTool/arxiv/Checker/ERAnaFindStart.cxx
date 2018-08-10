#ifndef ERTOOL_ERANAFINDSTART_CXX
#define ERTOOL_ERANAFINDSTART_CXX

#include "ERAnaFindStart.h"

namespace ertool {

  ERAnaFindStart::ERAnaFindStart(const std::string& name) : AnaBase(name), _tracks(nullptr) {
      PrepareTreeVariables();
  }

  void ERAnaFindStart::Reset()
  {}

  void ERAnaFindStart::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAnaFindStart::ProcessBegin()
  {
  }

  bool ERAnaFindStart::Analyze(const EventData &data, const ParticleGraph &ps) {

      for (auto const& p : ps.GetParticleNodes(RecoType_t::kTrack)) {
          auto const& track = data.Track(ps.GetParticle(p).RecoID());
          _track_x =  track.front().at(0);
          _tracks->Fill();
          _n_tracks++;

          //std::cout << "start of track: "<< _track_x << std::endl;
      }

      for (auto const& t : ps.GetParticleNodes(RecoType_t::kShower)) {
          auto const &shower = data.Shower(ps.GetParticle(t).RecoID());
          _shower_x = shower.Start().at(0);
          _showers->Fill();
      }

      return true;

  }
    void ERAnaFindStart::ProcessEnd(TFile* fout){

        std::cout << "number of tracks: "<< _n_tracks << std::endl;

        if (fout) {
            fout->cd();
            _tracks->Write();
            _showers->Write();
        }
        return;
    }

    void ERAnaFindStart::PrepareTreeVariables() {

        if (_tracks) { delete _tracks; }

        _tracks = new TTree("Tracks", "Result Tree");
        _tracks->Branch("track_x", &_track_x);

        _showers = new TTree("Showers", "Result Tree");
        _showers->Branch("shower_x", &_shower_x);

        _tracks->SetName(Form("Tracks"));//, _treename.c_str()));
        _showers->SetName(Form("Showers"));//, _treename.c_str()));


        return;
    }

    void ERAnaFindStart::ResetTreeVariables() {

        _shower_x = -999.;
        _track_x = -999.;

        return;

    }

}

#endif
