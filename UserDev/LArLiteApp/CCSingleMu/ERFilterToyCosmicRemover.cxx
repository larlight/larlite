#ifndef ERTOOL_ERFILTERTOYCOSMICREMOVER_CXX
#define ERTOOL_ERFILTERTOYCOSMICREMOVER_CXX

#include "ERFilterToyCosmicRemover.h"

namespace ertool {

  ERFilterToyCosmicRemover::ERFilterToyCosmicRemover() : FilterBase()
						       , fTPC(10.,-100.,10.,240.,100.,1050.)
  {
    _name = "ERFilterToyCosmicRemover";

    hFilterAllTrackCtr  = nullptr;
    hFilteredTrackCtr   = nullptr;
    hTrackRejectionRate = nullptr;

    hFilterAllShowerCtr  = nullptr;
    hFilteredShowerCtr   = nullptr;
    hShowerRejectionRate = nullptr;

    hFilteredTrackLength = nullptr;
    hUnfilteredTrackLength = nullptr;
  }

  void ERFilterToyCosmicRemover::ProcessBegin()
  {
    if(!hFilterAllTrackCtr)
      hFilterAllTrackCtr = new TH1D("hFilterAllTrackCtr",
				    "Track Counts; Counts; Process",
				    100,0,100);
    hFilterAllTrackCtr->Reset();

    if(!hFilteredTrackCtr)
      hFilteredTrackCtr = new TH1D("hFilteredTrackCtr",
				   "Filtered Track Counts; Counts; Process",
				   100,0,100);
    hFilteredTrackCtr->Reset();

    if(!hTrackRejectionRate)
      hTrackRejectionRate = new TH1D("hTrackRejectionRate",
				     "Rejection Rate; Rate; Process",
				     101,-0.005,1.005);
    hTrackRejectionRate->Reset();

    if(!hFilterAllShowerCtr)
      hFilterAllShowerCtr = new TH1D("hFilterAllShowerCtr",
				    "Shower Counts; Counts; Process",
				    100,0,100);
    hFilterAllShowerCtr->Reset();

    if(!hFilteredShowerCtr)
      hFilteredShowerCtr = new TH1D("hFilteredShowerCtr",
				   "Filtered Shower Counts; Counts; Process",
				   100,0,100);
    hFilteredShowerCtr->Reset();

    if(!hShowerRejectionRate)
      hShowerRejectionRate = new TH1D("hShowerRejectionRate",
				     "Rejection Rate; Rate; Process",
				     101,-0.005,1.005);
    hShowerRejectionRate->Reset();

    if(!hUnfilteredTrackLength)
      hUnfilteredTrackLength = new TH1D("hUnfilteredTrackLength",
					"Track Length of Remaining Ones; Length [cm];Tracks",
					400,0,400);
    hUnfilteredTrackLength->Reset();

    if(!hFilteredTrackLength)
      hFilteredTrackLength = new TH1D("hFilteredTrackLength",
					"Track Length of Filtered Ones; Length [cm];Tracks",
					400,0,400);
    hFilteredTrackLength->Reset();

  }

  void ERFilterToyCosmicRemover::Filter(EventData& data)
  { 
    // Search for cosmic-like penetrating tracks
    for(auto const& trk : data.Track()) {

      if(trk->size()<2) data.FilterTrack(trk->ID());

      auto const& start = trk->front();
      auto const& end   = trk->back();

      if(!fTPC.Contain(start) || !fTPC.Contain(end)) {
	data.FilterTrack(trk->ID());
	_sec_filter.RegisterSeed(trk->ID());
      }
    }

    _sec_filter.Filter(data);

    hFilterAllTrackCtr->Fill(data.AllTrack().size());
    hFilteredTrackCtr->Fill(data.AllTrack().size() - data.Track().size());
    hTrackRejectionRate->Fill((double)(data.AllTrack().size()-data.Track().size()) / (double)(data.AllTrack().size()));

    hFilterAllShowerCtr->Fill(data.AllShower().size());
    hFilteredShowerCtr->Fill(data.AllShower().size() - data.Shower().size());
    hShowerRejectionRate->Fill((double)(data.AllShower().size()-data.Shower().size()) / (double)(data.AllShower().size()));

    for(auto const& trk : data.AllTrack()) {
      if(trk.IsFiltered()) hFilteredTrackLength->Fill(trk.Length());
      else hUnfilteredTrackLength->Fill(trk.Length());
    }
  }

  void ERFilterToyCosmicRemover::ProcessEnd(TFile* fout)
  {
    if(fout) {
      fout->cd();
      hFilterAllTrackCtr->Write();
      hFilteredTrackCtr->Write();
      hTrackRejectionRate->Write();
      hFilterAllShowerCtr->Write();
      hFilteredShowerCtr->Write();
      hShowerRejectionRate->Write();
      hUnfilteredTrackLength->Write();
      hFilteredTrackLength->Write();
    }
  }

}

#endif
