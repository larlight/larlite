#ifndef ERTOOL_ALGOPRIMARYFINDER_CXX
#define ERTOOL_ALGOPRIMARYFINDER_CXX

#include "AlgoPrimaryFinder.h"

namespace ertool {

  AlgoPrimaryFinder::AlgoPrimaryFinder() : AlgoBase()
  {

    _name       = "AlgoPrimaryFinder";
    _verbose = false;
    _useRadLength = false;

  }

  void AlgoPrimaryFinder::Reset(){}
  
  void AlgoPrimaryFinder::ProcessBegin()
  {
    return;
  }

  void AlgoPrimaryFinder::LoadParams(std::string fname, size_t version)
  {
    return;
  }

  ParticleSet AlgoPrimaryFinder::Reconstruct(const EventData &data){
    
    // ParticleSet where to store primaries found
    ParticleSet primaries;

    //res = emParticles;
    
    if (_verbose) { 
      std::cout << "*********** BEGIN PrimaryFinder RECONSTRUCTION ************" << std::endl;
    }

    // loop over showers
    for (size_t sh=0; sh < data.Shower().size(); sh++){
      
      // Ok, we have a shower.
      // make sure it does not:
      // 1) come from another shower
      // 2) come from a track

      auto const& thisShower = data.Shower(sh);

      if (_verbose) { std::cout << "This shower: (" << sh << ")" << "\tE: " << thisShower._energy << std::endl; }
      bool primary = true;
      // loop over other showers and check 1) and 2)
      for (size_t s=0; s < data.Shower().size(); s++){
	auto const& thatShower(data.Shower(s));
	geoalgo::Point_t vtx(3);
	// make sure we don't use thisShower in the loop
	if (thatShower.ID() == thisShower.ID())
	  continue;
	if (_verbose) { std::cout << "Comparing with shower (" << s << ")" << std::endl; }
	// compare the two showers -> make sure thisShower does not come from thatShower
	// if thisShower comes from somewhere along the second shower's trunk
	// then it is not primary
	double thatShowerLength = thatShower.Length();
	// create a segment starting at thatShower start
	// and going for the entire length in shower dir
	::geoalgo::LineSegment_t thatShowerTrunk(thatShower.Start(),thatShower.Start()+thatShower.Dir()*thatShowerLength);
	double IP = _findRel.FindClosestApproach(thisShower,thatShower,vtx);
	_VsTrack = 0;
	_thatE   = thatShower._energy;
	_IP = IP;
	_IPthisStart = vtx.Dist(thisShower.Start());
	_IPthatStart = vtx.Dist(thatShower.Start());
	_IPthatTrunk = sqrt(_geoAlgo.SqDist(vtx,thatShowerTrunk));
	if (_verbose)
	  std::cout << "\tImpact Parameter      : " << _IP << std::endl
		    << "\tIP to other Shr Start : " << _IPthatStart << std::endl
		    << "\tIP to other Shr Trunk : " << _IPthatTrunk << std::endl
		    << "\tIP to this Shr Start  : " << _IPthisStart << std::endl;
	if ( (IP < _maxIP)                             // good correlation
	     && ( _IPthisStart < _vtxToTrkDist)        // this shower's start point close to IP
	     && ( _IPthatTrunk < _vtxToTrkDist) )      // vtx close to thatShower's trunk
	  {
	    primary = false;
	    //std::cout << "Removed here!" << std::endl;
	    if (_verbose) { std::cout << "NOT primary" << std::endl; }
	    break;
	  }
	
      }//loop over showers
      
      
      // now loop over tracks if still primary
      if (primary){
	for (size_t t=0; t < data.Track().size(); t++){
	  auto const& thatTrack(data.Track(t));
	  if (thatTrack.size() < 2)
	    continue;
	  if (_verbose) { std::cout << "Comparing with track (" << t << ")" << std::endl; }
	  geoalgo::Point_t vtx(3);
	  // compare the two tracks
	  double IP =  _findRel.FindClosestApproach(thisShower,thatTrack,vtx);
	  _VsTrack = 1;
	  _thatE   = thatTrack._energy;
	  _IP = IP;
	  _IPthisStart = vtx.Dist(thisShower.Start());
	  _IPthatStart = vtx.Dist(thatTrack.front());
	  _IPtrkBody = sqrt(_geoAlgo.SqDist(vtx,thatTrack));
	  if (_verbose)
	    std::cout << "\tImpact Parameter: " << _IP << std::endl
		      << "\tIP to Trk Start : " << _IPthatStart << std::endl
		      << "\tIP to Trk Body  : " << _IPtrkBody << std::endl
		      << "\tIP to Shr Start : " << _IPthisStart << std::endl;
	  primary = true;
	  if ( (IP < _maxIP)                                              // good correlation
	       && (vtx.Dist(thatTrack.front()) > _vtxToTrkStartDist)      // vertex far enough away from track start
	       && (sqrt(_geoAlgo.SqDist(vtx,thatTrack)) < _vtxToTrkDist)  // vertex close to track body
	       && (vtx.Dist(thisShower.Start()) < _vtxToShrStartDist) )   // vertex not unreasonably far from shower start
	    {
	      // our shower comes from t -> not interested
	      primary = false;
	      if (_verbose) { std::cout << "NOT primary" << std::endl; }
	      break;
	    }
	}// for all tracks
      }// if primary after checking correlations with showers

      // if still primary append to list of primaries
      // create new particle
      if (primary){
	// for now append a dummy particle of PDG = 0 and mass = 0
	Particle thisP(0,0);
	thisP.RecoObjInfo(sh,Particle::RecoObjType_t::kShower);
	primaries.push_back(thisP);
      }
      
    }// for all showers
    
    return primaries;
  }
  
  void AlgoPrimaryFinder::ProcessEnd(TFile* fout){
    
    if(fout){
      fout->cd();
    }

    return;
  }

}

#endif
