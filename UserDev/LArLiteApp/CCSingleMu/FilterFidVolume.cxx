#ifndef ERTOOL_FILTERFIDVOLUME_CXX
#define ERTOOL_FILTERFIDVOLUME_CXX

#include "FilterFidVolume.h"

namespace ertool {

  FilterFidVolume::FilterFidVolume() 
    : FilterBase()
    , fTPC(0.,-116.,0.,256.,116.,1060.)
  {
    _name = "FilterFidVolume";
  }

  void FilterFidVolume::ProcessBegin() {
    
    hProjDist = new TH1D("hProjDist","Distance Projected to TPC Boundaries; Dist [cm]; Tracks",
			 500,0,1060);
    hPerpDist = new TH1D("hPerpDist","Distance Perpendicular to TPC Boundaries; Dist [cm]; Tracks",
			 500,0,1060);

  }

  void FilterFidVolume::Filter(EventData& data)
  { 
    // Loop over tracks
    for(auto const& t : data.Track()) {

      // If this track has only 1 trajectory point, skip
      if(t->size()<2) continue;

      auto const& start = t->front();
      auto const& end   = t->back();

      // If this track (start & end) is NOT contained within a box, filter & continue
      if(!fTPC.Contain(start) || !fTPC.Contain(end)) {
	//t->Filter();
	continue;
      }
      
      // Further analysis for tracks contained within TPC...
      
      double min_dist = 0;
      //
      // Simple perpendicular distance
      // (0) Take the minimum distance between start-to-wall and end-to-wall
      min_dist = std::min( sqrt(fGeoAlgo.SqDist(fTPC,start)),
			   sqrt(fGeoAlgo.SqDist(fTPC,end)) );
      hPerpDist->Fill(min_dist);
      
      //
      // Projected distance to the wall
      // (0) Define a half-line from start to the wall & end to the wall
      // (1) Find intersection of such half-line and AABox (TPC boundary)
      // (2) Take the minimum distance between two.
      
      // Step (0)
      auto const start_dir = (*t)[1] - start;
      auto const end_dir   = end - (*t)[t->size()-2];
      
      // Get distance btw. start & box
      ::geoalgo::HalfLine start_to_wall ( start, start_dir*-1.);
      ::geoalgo::HalfLine end_to_wall   ( end,   end_dir      );
      
      // Step (1)
      // Get a vector of intersection points: the return should be length 1
      auto start_xs_v = fGeoAlgo.Intersection( fTPC, start_to_wall );
      auto end_xs_v   = fGeoAlgo.Intersection( fTPC, end_to_wall   );

      // Step (2)
      // Find the minimum distance
      min_dist = std::min(start_xs_v[0].Dist(start),
			  end_xs_v[0].Dist(end));
      hProjDist->Fill(min_dist);
      
    }
  }

  void FilterFidVolume::ProcessEnd(TFile* fout) {

    if(fout) {
      fout->cd();
      hPerpDist->Write();
      hProjDist->Write();
    }
  }

}

#endif
