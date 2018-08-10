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


    hTaggedUntaggedDist = new TH1D("hTaggedUntaggedDist","Distance From Untagged Tracks to their Nearest Tagged Track; Dist [cm]; Tracks",
			 500,0,1060);
    hProjDist = new TH1D("hProjDist","Distance Projected to TPC Boundaries; Dist [cm]; Tracks",
			 500,0,1060);
    hPerpDist = new TH1D("hPerpDist","Distance Perpendicular to TPC Boundaries; Dist [cm]; Tracks",
			 500,0,1060);

    hPerpDistXBoundary = new TH1D("hPerpDistXBoundary","Distance Perpendicular to TPC X Boundary; Dist [cm]; Tracks",
			 500,0,1060);

    hPerpDistYBoundary = new TH1D("hPerpDistYBoundary","Distance Perpendicular to TPC Y Boundary; Dist [cm]; Tracks",
			 500,0,1060);

    hPerpDistZBoundary = new TH1D("hPerpDistZBoundary","Distance Perpendicular to TPC Z Boundary; Dist [cm]; Tracks",
			 500,0,1060);

    hPerpDistToABoundary = new TH1D("hPerpDistToABoundary","Distance Perpendicular to Closest TPC Boundary; Dist [cm]; Tracks",
			 500,0,1060);

    hTrackLength = new TH1D("hTrackLength","Track Length; Dist [cm]; Tracks",
			 500,0,1060);

    hLongestTrackLength = new TH1D("hLongestTrackLength","Longest Track Length; Dist [cm]; Tracks",
			 500,0,1060);

    hMuonTrackLength = new TH1D("hMuonTrackLength","Muon Track Length; Length [cm]; Tracks",
			 500,0,1060);

    hMuonTrackPhi = new TH1D("hMuonTrackPhi","Muon Track Phi; Phi; Tracks",
			 100,-3.14,3.14);

    hTrackPhi = new TH1D("hTrackPhi","Track Phi; Phi; Tracks",
			 100,-3.14,3.14);

    hTrackTheta = new TH1D("hTrackTheta","Track Theta; Theta; Tracks",
			 500,0,1060);

    hPerpDistVsTrackLength = new TH2D("hPerpDistVsTrackLength","Distance Perpendicular to TPC Boundaries vs Track Length; Length [cm]; Dist[cm];Tracks", 500,0,1060, 500,0,1060);

    hProjDistVsTrackLength = new TH2D("hProjDistVsTrackLength","Distance Projected to TPC Boundaries vs Track Length; Length [cm]; Dist[cm];Tracks", 500,0,1060, 500,0,1060);

    hTaggedUntaggedDistVsUntaggedTrackLength = new TH2D("hTaggedUntaggedDistVsUntaggedTrackLength ","Distance From Untagged Tracks to their Nearest Tagged Track vs Untagged Track Length; Length [cm]; Dist[cm];Tracks", 500,0,1060, 500,0,1060);

  }

  void FilterFidVolume::Filter(EventData& data)
  { 

    // Grab the longest track parameters
    double longestTrackLength = 0;
    double longestTrackPhi = 0;

    // Loop over tracks
    for(auto const& t : data.Track()) {

      // If this track has only 1 trajectory point, skip
      if(t->size()<2) continue;

      auto const& start = t->front();
      auto const& end   = t->back();

      // If this track (start & end) is NOT contained within a box, filter & continue
      if(!fTPC.Contain(start) || !fTPC.Contain(end)) {
	data.FilterTrack(t->ID());
	continue;
      }

      // Check for untagged tracks, t, close to the tagged track track ct
      double shortestTaggedUntaggedDist = 999999;
      double untaggedLength = 0;
      if(t->_cosmogenic < 0.35 && t->_cosmogenic > -99){
	for(auto const& ct : data.Track()){
	  if(t == ct)
	    continue;
	  if(ct->_cosmogenic >= 0.35 || ct->_cosmogenic <= -99){
	    for(auto const& p: *ct){
	      double taggedUntaggedDist = fGeoAlgo.SqDist(*t,p);
	      if(taggedUntaggedDist < shortestTaggedUntaggedDist){
		shortestTaggedUntaggedDist = taggedUntaggedDist;
		untaggedLength = t->Length();
	      }
	    }
	  }
	}
      }
      if(shortestTaggedUntaggedDist < 999999){
	hTaggedUntaggedDist->Fill(shortestTaggedUntaggedDist);
	hTaggedUntaggedDistVsUntaggedTrackLength->Fill(untaggedLength,shortestTaggedUntaggedDist);
      }

      // Reject tracks based on cosmic score
      if(t->_cosmogenic >= 0.35 || t->_cosmogenic <= -99)
	continue;

      // To reevaluate the cosmic tag, change the cut to 10 cm instead of the default 5 cm
      double y_dist = std::abs(fTPC.Min()[1]-start[1]);
      if(y_dist > std::abs(fTPC.Min()[1]-end[1])) y_dist = std::abs(fTPC.Min()[1]-end[1]);
      if(y_dist > std::abs(fTPC.Max()[1]-start[1])) y_dist = std::abs(fTPC.Max()[1]-start[1]);
      if(y_dist > std::abs(fTPC.Max()[1]-end[1])) y_dist = std::abs(fTPC.Max()[1]-end[1]);
      if(y_dist < 10)
	continue;

      // Fill track histograms
      hTrackLength->Fill(t->Length());
      hTrackPhi->Fill(start.Phi());

      // See if this is the longest track so far
      if( longestTrackLength < t->Length()){
	longestTrackLength = t->Length();
	longestTrackPhi = start.Phi();
      }

      // Further analysis for tracks contained within TPC...
      
      double min_dist = 0;
      //
      // Simple perpendicular distance
      // (0) Take the minimum distance between start-to-wall and end-to-wall
      min_dist = std::min( sqrt(fGeoAlgo.SqDist(fTPC,start)),
			   sqrt(fGeoAlgo.SqDist(fTPC,end)) );
      hPerpDist->Fill(min_dist);
    
      // Fill the 2D histograms
      hPerpDistVsTrackLength->Fill(t->Length(),min_dist);

      //
      // Perpendicular distance to X, Y, and Z boundaries 
      //  (0) Distance to Y Boundary 
      //  (1) Distance to X Boundary 
      //  (2) Distance to Z Boundary 
     
      // Step (0)
      // Consider Distance to Y Boundary 
      double min_y_dist = std::abs(fTPC.Min()[1]-start[1]);
      if(min_y_dist > std::abs(fTPC.Min()[1]-end[1])) min_y_dist = std::abs(fTPC.Min()[1]-end[1]);
      if(min_y_dist > std::abs(fTPC.Max()[1]-start[1])) min_y_dist = std::abs(fTPC.Max()[1]-start[1]);
      if(min_y_dist > std::abs(fTPC.Max()[1]-end[1])) min_y_dist = std::abs(fTPC.Max()[1]-end[1]);
      hPerpDistYBoundary->Fill(min_y_dist);

      // Step (1)
      // Consider Distance to X Boundary 
      double min_x_dist = std::abs(fTPC.Min()[0]-start[0]);
      if(min_x_dist > std::abs(fTPC.Min()[0]-end[0])) min_x_dist = std::abs(fTPC.Min()[0]-end[0]);
      if(min_x_dist > std::abs(fTPC.Max()[0]-start[0])) min_x_dist = std::abs(fTPC.Max()[0]-start[0]);
      if(min_x_dist > std::abs(fTPC.Max()[0]-end[0])) min_x_dist = std::abs(fTPC.Max()[0]-end[0]);
      hPerpDistXBoundary->Fill(min_x_dist);

      // Step (2)
      // Consider Distance to Z Boundary 
      double min_z_dist = std::abs(fTPC.Min()[2]-start[2]);
      if(min_z_dist > std::abs(fTPC.Min()[2]-end[2])) min_z_dist = std::abs(fTPC.Min()[2]-end[2]);
      if(min_z_dist > std::abs(fTPC.Max()[2]-start[2])) min_z_dist = std::abs(fTPC.Max()[2]-start[2]);
      if(min_z_dist > std::abs(fTPC.Max()[2]-end[2])) min_z_dist = std::abs(fTPC.Max()[2]-end[2]);
      hPerpDistZBoundary->Fill(min_z_dist);


      hPerpDistToABoundary->Fill(std::min(min_z_dist, std::min(min_y_dist,min_x_dist)));

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
      // min_dist = std::min(start_xs_v[0].Dist(start),
			  // end_xs_v[0].Dist(end));
      // hProjDist->Fill(min_dist);

      // // Fill the 2D histograms
      // hProjDistVsTrackLength->Fill(t->Length(),min_dist);

    }

    if(longestTrackLength > 0)
      hLongestTrackLength->Fill(longestTrackLength);

    if(longestTrackLength > 20){
      hMuonTrackLength->Fill(longestTrackLength);
      hMuonTrackPhi->Fill(longestTrackPhi);
    }

  }

  void FilterFidVolume::ProcessEnd(TFile* fout) {

    if(fout) {
      fout->cd();
      hPerpDist->Write();
      hPerpDistXBoundary->Write();
      hPerpDistYBoundary->Write();
      hPerpDistZBoundary->Write();
      hProjDist->Write();
      hPerpDistVsTrackLength->Write();
      hProjDistVsTrackLength->Write();
      hTrackLength->Write();
      hTrackPhi->Write();
      hTrackTheta->Write();
      hLongestTrackLength->Write();
      hMuonTrackLength->Write();
      hMuonTrackPhi->Write();
      hTaggedUntaggedDist->Write();
      hTaggedUntaggedDistVsUntaggedTrackLength->Write();
      hPerpDistToABoundary->Write();
    }
  }

}

#endif
