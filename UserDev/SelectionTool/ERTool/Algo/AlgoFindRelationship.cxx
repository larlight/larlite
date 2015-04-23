#ifndef ERTOOL_FINDRELATIONSHIP_CXX
#define ERTOOL_FINDRELATIONSHIP_CXX

#include "AlgoFindRelationship.h"

namespace ertool{

  AlgoFindRelationship::AlgoFindRelationship() : AlgoBase()
  {
    _name   = "AlgoFindRelationship";
    _debug  = false;

    //Track-particle masses (convert to MEV)
    _pi_mass = 1000.* TDatabasePDG().GetParticle(211)->Mass();
    _pr_mass = 1000.* TDatabasePDG().GetParticle(2212)->Mass();
    _mu_mass = 1000.* TDatabasePDG().GetParticle(13)->Mass();
    _ka_mass = 1000.* TDatabasePDG().GetParticle(311)->Mass();
    _uk_mass = 0.;
  }
  
  void AlgoFindRelationship::Reset()
  {}


  double AlgoFindRelationship::FindClosestApproach(const geoalgo::HalfLine_t& shr1,
						   const geoalgo::HalfLine_t& shr2,
						   geoalgo::Point_t& vtx) const
  {
    // Find mininum impact parameter between a two showers
    // flip their directions and look backwards
    
    // Create a half-line pointing backwards from the shower
    geoalgo::HalfLine_t shr1Bkwd(shr1.Start(), shr1.Dir()*(-1));
    geoalgo::HalfLine_t shr2Bkwd(shr2.Start(), shr2.Dir()*(-1));
    
    // coordinates for closest points on the two objects
    geoalgo::Point_t PtShr1(3);
    geoalgo::Point_t PtShr2(3);
    double IP = _geoAlgo.SqDist(shr1Bkwd, shr2Bkwd, PtShr1, PtShr2);
    // build candidate vertex
    vtx = (PtShr1+PtShr2)/2.;
    
    return sqrt(IP);
  }


  double AlgoFindRelationship::FindClosestApproach(const geoalgo::HalfLine_t& shr,
						   const geoalgo::Trajectory_t& trk,
						   geoalgo::Point_t& vtx) const
  {
    // Find mininum impact parameter between a shower and a track
    // Search for a point of origin, either common to the two objects
    // or on one of the two objects (that gave rise to the second)
    // along the shower direction, starting from the start point and
    // going backwards along the shower direction
    // *** ADD ALONG SHOWER DIR FOR ITS LENGTH - NOT IMPLEMENTED NOW ***
    // and along the entire segment track as well as backwards along the
    // track direction
    
    // Create a half-line pointing backwards from the shower
    geoalgo::HalfLine_t shrBkwd(shr.Start(), shr.Dir()*(-1));
    
    // 1) Search for whether the shower might originate
    // from the track
    // get IP for this shower and the entire track
    // but constrain the search along the track
    // coordinates for closest points on the two objects
    geoalgo::Point_t PtShr(3);
    geoalgo::Point_t PtTrk(3);
    double IP = _geoAlgo.SqDist(shrBkwd, trk, PtShr, PtTrk);
    // build candidate vertex
    vtx = (PtShr+PtTrk)/2.;
    
    return sqrt(IP);
  }



  double AlgoFindRelationship::FindClosestApproach(const geoalgo::Trajectory_t& trk2,
						   const geoalgo::Trajectory_t& trk1,
						   geoalgo::Point_t& vtx) const
  {
    // Find mininum impact parameter between two tracks
    // Search for a point of origin, either common to the two objects
    // or on one of the two objects (that gave rise to the second)
    // compare the points on one track to the backwards direction of
    // the other searching if A came from B
    // then flip the roles and search if B comes from A

    // holders for closest approach points
    geoalgo::Point_t pt1(3);
    geoalgo::Point_t pt2(3);
    
    // keep track of smallest quantity
    double IP = 1036.;
    // and temporary holders
    double IPtmp = 0;
    geoalgo::Point_t vtxtmp(3);
    geoalgo::Point_t pttmp(3);

    //std::cout << "\t\t\t\tTrk A Start: " << trk1.front() << std::endl; 
    //std::cout << "\t\t\t\tTrk B Start: " << trk2.front() << std::endl; 

    // Search for IP looping through segments of one track
    // and using start-point of other track
    pttmp = _geoAlgo.ClosestPt(trk2.front(), trk1);
    vtxtmp = (trk2.front()+pttmp)/2.;
    IPtmp = pttmp.SqDist(trk2.front());
    //std::cout << "\t\t\t\tTrk A body and Trk B start. IP: " << IPtmp << " Vtx: " << vtxtmp << std::endl; 
    if (IPtmp < IP) { IP = IPtmp; vtx = vtxtmp; }
    // same but flip roles of tracks
    pttmp = _geoAlgo.ClosestPt(trk1.front(), trk2);
    vtxtmp = (trk1.front()+pttmp)/2.;
    IPtmp = pttmp.SqDist(trk1.front());
    //std::cout << "\t\t\t\tTrk B body and Trk A start. IP: " << IPtmp << " Vtx: " << vtxtmp <<  std::endl; 
    if (IPtmp < IP) { IP = IPtmp; vtx = vtxtmp; }
    // Search for IP looping through segments of one track
    // and using end point of other track
    pttmp = _geoAlgo.ClosestPt(trk2.back(), trk1);
    vtxtmp = (trk2.back()+pttmp)/2.;
    IPtmp = pttmp.SqDist(trk2.back());
    //std::cout << "\t\t\t\tTrk A body and Trk B end. IP: " << IPtmp << " Vtx: " << vtxtmp <<  std::endl; 
    if (IPtmp < IP) { IP = IPtmp; vtx = vtxtmp; }
    // same but flip roles of tracks
    pttmp = _geoAlgo.ClosestPt(trk1.back(), trk2);
    vtxtmp = (trk1.back()+pttmp)/2.;
    IPtmp = pttmp.SqDist(trk1.back());
    //std::cout << "\t\t\t\tTrk B body and Trk A end. IP: " << IPtmp << " Vtx: " << vtxtmp <<  std::endl; 
    if (IPtmp < IP) { IP = IPtmp; vtx = vtxtmp; }

    return sqrt(IP);
  }


  ParticleSet AlgoFindRelationship::FindTrackHierarchy(const std::vector<const ertool::Track*> &tracks)
  {

    //std::cout << "Starting with " << tracks.size() << " tracks" << std::endl;

    // Purpose of this function:
    // To read in a list of tracks in the event
    // and sort them into a "hierarchical" order
    // Example interaction:
    // nu -> p + n + pi+
    //               pi+ -> mu+

    // Create a ParticleSet.
    // Initially each track is an independent particle
    // as relationships are found sort by "genealogy"
    ParticleSet trackParts;
    for (size_t t=0; t < tracks.size(); t++){
      Track tr = *(tracks.at(t));
      //std::cout << "\tTrack " << t << "\tVtx: " << tr[0] << std::endl;
      Particle thisp = GetPDG(tr);
      thisp.Vertex(tr.at(0));
      double mom = tr._energy - thisp.Mass();
      if (mom < 0) { mom = 1; }
      geoalgo::Vector_t dir(tr.at(1) - tr.at(0));
      dir.Normalize();
      thisp.Momentum(dir*mom);
      thisp.RecoObjInfo(t,Particle::RecoObjType_t::kTrack);
      trackParts.push_back(thisp);
    }
    // create a list that keeps track of whether a particle
    // is "primary" or not. Primary == does not have a mother track
    // primary = 1. Not = 0.
    std::vector<int> trackPrimary(trackParts.size(),1);

    // Map to keep track of siblings
    std::map<int, std::vector<int> > siblingMap;
    // format:
    // [ 0 : 1,2 ]
    // [ 1 : 0,2 ]
    // [ 2 : 0,1 ]
    // [ 3 : 4 ]
    // [ 4 : 3 ]
    // 1st element is position of particle
    // 2nd element is list of position of siblings
    // Now loop over particles and form hierarchy
    for (size_t t1=0; t1 < trackParts.size(); t1++){
      for (size_t t2=t1+1; t2 < trackParts.size(); t2++){
	Track tr1 = *(tracks.at(trackParts.at(t1).RecoObjID()));
	Track tr2 = *(tracks.at(trackParts.at(t2).RecoObjID()));
	// require both tracks to be longer than some distance (1 cm)
	//if ( (tr1.Length() > 1) || (tr2.Length() > 0.1) )
	//  continue;
	// now find the relationship between the two tracks
	geoalgo::Point_t vtx;
	//std::cout << "Compare " << t1 << " and " << t2 << std::endl;
	double IP = FindClosestApproach(tr1,tr2,vtx);
	// 3 cases:
	// tracks are uncorrelated (IP > something)
	// track 1 comes from track 2 (or vice-versa)
	// track 1 and 2 are "siblings"
	//std::cout << "\t IP: " << IP << std::endl;
	if (IP > 1)
	  continue;
	// calculate distance from IP to track bodies and start points
	//std::cout << "\tVtx: " << vtx << std::endl;
	double vtx_t1S = vtx.Dist(tr1.at(0));
	double vtx_t2S = vtx.Dist(tr2.at(0));
	double vtx_t1B = sqrt(_geoAlgo.SqDist(vtx,tr1));
	double vtx_t2B = sqrt(_geoAlgo.SqDist(vtx,tr2));
	//std::cout << "\t\tVtx-1 Start: " << vtx_t1S
	//	    << "\t\tVtx-2 Start: " << vtx_t2S
	//	    << "\t\tVtx-1 Body : " << vtx_t1B
	//	    << "\t\tVtx-2 Body : " << vtx_t2B << std::endl;
	// check if 2 comes form 1
	if ( (vtx_t1B < 1) && (vtx_t1S > 1) && (vtx_t2S < 1) ){
	  trackParts.at(t1).AddDaughter(trackParts.at(t2));
	  trackPrimary.at(t2) = 0;
	  //std::cout << "Track " << t2 << " daughter of " << t1 << std::endl;
	}
	// check if 1 comes form 2
	if ( (vtx_t2B < 1) && (vtx_t2S > 1) && (vtx_t1S < 1) ){
	  trackParts.at(t2).AddDaughter(trackParts.at(t1));
	  trackPrimary.at(t1) = 0;
	  //std::cout << "Track " << t1 << " daughter of " << t2 << std::endl;
	}
	// check if siblings
	if ( (vtx_t1S < 1) && (vtx_t2S < 1) ){
	  // now what???
	  //std::cout << "Tracks " << t1 << "and " << t2 << " are siblings" << std::endl;
	  siblingMap[t1].push_back(t2);
	  siblingMap[t2].push_back(t1);
	}
      }
    }

    int primary = 0;
    for (size_t n=0; n < trackPrimary.size(); n++){
      if (trackPrimary[n] == 1)
	primary += 1;
    }
    //std::cout << "Primaries left: " << primary << std::endl;

    // Create a ParticleSet where to store output
    ParticleSet thisset;
    // At the end, go through primary tracks
    // for those that have siblings, create a single unknown ancestor
    for (size_t t = 0; t < trackParts.size(); t++){
      if (trackPrimary.at(t) == 1){
	// it si primary!
	// create unknown particle and add this track & siblings
	Particle unknown(999,_uk_mass);
	// temporary holder for the vertex
	geoalgo::Point_t thisvtx = trackParts[t].Vertex();
	// how many particles at this vertex?
	int numP = 1;
	unknown.AddDaughter(trackParts[t]);
	//std::cout << "Found primary " << t;
	if ( siblingMap.find(t) != siblingMap.end() ){
	  //std::cout << "\tWith siblings: ";
	  // has siblings. Add them to unknown as daughters
	  for (auto &s : siblingMap.find(t)->second){
	    //std::cout <<  s << "\t";
	    unknown.AddDaughter(trackParts[s]);
	    thisvtx += trackParts[s].Vertex();
	    numP += 1;
	    // erase this particle's "primary" status to avoid double counting
	    trackPrimary.at(s) = 0;
	  }
	}// if has siblings
	//std::cout << std::endl;
	// get Vtx by averaging over vertices of all particles that are siblings
	thisvtx /= numP;
	unknown.Vertex(thisvtx);
	thisset.push_back(unknown);
      }// if track is primary
    }// for all tracks

    return thisset;
  }


  ParticleSet AlgoFindRelationship::FindHierarchy(const std::vector<const ertool::Track*>  &tracks,
						  const std::vector<const ertool::Shower*> &showers)
  {
    
    // First, find the hierarchy of all tracks
    ParticleSet trackHierarchy = FindTrackHierarchy(tracks);

    // Compare showers with tracks. First with vertices of each primary
    // group of tracks, then with all the tracks themselves
    for (size_t s=0; s < showers.size(); s++){
      Shower shr = *(showers.at(s));
      Particle unknownShower(0,0);
      // boolean to know if this shower was added as daughter somewhere
      bool daughter = false;
      
      // loop over trackHierarchy vector
      for (size_t i=0; i < trackHierarchy.size(); i++){
	
	// if the shower start point is close to the vertex
	if (trackHierarchy.at(i).Vertex().Dist(shr.Start()) < 1.){

	  trackHierarchy.at(i).AddDaughter(unknownShower);
	  daughter = true;
	}

	// assigned to a vertex? good! we are done with this shower!
	if (daughter)
	  continue;

	// otherwise, compare track-by-track (including daughters of tracks)
	//CompareShrWithPartBranch(shr, tracks, trackHierarchy.at(i) );
	
      }// for all "vertices"

      // if shower was not assigned somewhere -> Add as independent
      if (!daughter)
	trackHierarchy.push_back(unknownShower);
      
    }// for all showers
		
    return trackHierarchy;
  }

  /*
    bool AlgoFindRelationship::CompareShrWithPartBranch(const Shower& shr,
    const std::vector<const ertool::Track*>  &tracks,
    Particle& part)
    {
    
    for (auto &p : part._daughters){
    
    Track tr = *tracks.at(p.RecoObjID());
    
    geoalgo::Point_t vtx(3);
    double IP = FindClosestApproach(shr, tr, vtx);
      
      // if IP small enough
      if (IP < 1.){
	
	// if vtx close enough to both objects
	if ( (vtx.Dist(shr.Start()) < 50.) &&
	     (sqrt(_geoAlgo.SqDist(vtx,tr)) < 1.) ){
	  
	  // if close to track start:
	  // should this ever happen?
	  // in theory we checked this already...
	  // this CAN happen...but do we want to handle
	  // this case in a special way????
	  if ( vtx.Dist(tr[0]) < 1.)
	    part.AddDaughter(unknownShower);
	  // otherwise shower comes from track
	  else
	    p.AddDaughter(unknownShower);
	  
	  daughter = true;
	  return true;
	  
	}// if close enough to track body & shower start
      }// if IP is small enough

      }// for all particles at this "vertex"
    
    return false;
  }
  */  
  Particle AlgoFindRelationship::GetPDG(const Track &trk)
  {

    switch(trk._pid){
    case Track::TrackPartID_t::kUnknown : return Particle(999,_uk_mass);
    case Track::TrackPartID_t::kPIDA    : return Particle(999,_uk_mass);
    case Track::TrackPartID_t::kProton  : return Particle(2212,_pr_mass);
    case Track::TrackPartID_t::kKaon    : return Particle(311,_ka_mass);
    case Track::TrackPartID_t::kPion    : return Particle(211,_pi_mass);
    case Track::TrackPartID_t::kMuon    : return Particle(13,_mu_mass);
    }

    return Particle(999,_uk_mass);
  }

};

#endif
