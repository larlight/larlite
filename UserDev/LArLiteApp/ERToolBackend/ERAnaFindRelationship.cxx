#ifndef ERANAFINDRELATIONSHIP_CXX
#define ERANAFINDRELATIONSHIP_CXX

#include "ERAnaFindRelationship.h"

namespace larlite {

  ERAnaFindRelationship::ERAnaFindRelationship()
    : ERToolAnaBase()
    , _ana_tree(nullptr) 
  { 
    _name="ERAnaFindRelationship"; 

  }

  bool ERAnaFindRelationship::initialize() {

    // Setup Tree to hold Quality Check information
    if (_ana_tree) { delete _ana_tree; }
    _ana_tree = new TTree("_ana_tree","Ana Tree");
    _ana_tree->Branch("_dot/D",&_dot,"dot/D");
    _ana_tree->Branch("_thisToVtx",&_thisToVtx,"thisToVtx/D");
    _ana_tree->Branch("_thatToVtx",&_thatToVtx,"thatToVtx/D");
    _ana_tree->Branch("_thisPDG",&_thisPDG,"thisPDG/I");
    _ana_tree->Branch("_thisID",&_thisID,"thisID/I");
    _ana_tree->Branch("_thatPDG",&_thatPDG,"thatPDG/I");
    _ana_tree->Branch("_thatID",&_thatID,"thatID/I");
    _ana_tree->Branch("_thisProcess","std::string",&_thisProcess);

    return ERToolAnaBase::initialize();
  }
  
  bool ERAnaFindRelationship::analyze(storage_manager* storage) {

    // Goal: Find Pi0 + muon Track!

    // Vertex from Pi0:
    geoalgo::Point_t Pi0Vtx(3);
    bool isPi0 = false;
    // Vertex from Pi0 and muon track intersection
    geoalgo::Point_t EvtVtx(3);
    // Make a LineSegment object for the Pi0 so that
    // it can be used to trace back with the tracks
    // to find a common vertex
    geoalgo::HalfLine_t pi0Segment(1,1,1,1,1,1);

    auto status = ERToolAnaBase::analyze(storage);
    if(!status) return status;

    // Search data
    
    // First search for a Pi0 amongst the showers
    auto const& data = _mgr.EventData();
    auto& ps = _mgr.ParticleSetWriteable();

    for (size_t s1=0; s1 < data.Shower().size(); s1++){
      for (size_t s2=s1+1; s2 < data.Shower().size(); s2++){
	// only search for pi0 if not found yet
	if (!isPi0){
	  ::ertool::Shower shower1 = data.Shower(s1);
	  ::ertool::Shower shower2 = data.Shower(s2);
	  isPi0 = _findRel.haveCommonOrigin(shower1,shower2,Pi0Vtx);
	  if (isPi0){
	    ::ertool::Particle gamma1(22,0.);
	    gamma1.Vertex(shower1.Start());
	    gamma1.Momentum(shower1.Dir()*shower1._energy);
	    gamma1.RecoObjInfo(s1,::ertool::Particle::RecoObjType_t::kShower);
	    ps.push_back(gamma1);
	    ::ertool::Particle gamma2(22,0.);
	    gamma2.Vertex(shower2.Start());
	    gamma2.Momentum(shower2.Dir()*shower2._energy);
	    gamma2.RecoObjInfo(s2,::ertool::Particle::RecoObjType_t::kShower);
	    ps.push_back(gamma2);
	    ::ertool::Particle Pi0(111,134.97);
	    Pi0.Vertex(Pi0Vtx);
	    Pi0.Momentum(shower1.Dir()*shower1._energy+shower2.Dir()*shower2._energy);
	    Pi0.AddDaughter(gamma1);
	    Pi0.AddDaughter(gamma2);
	    ps.push_back(Pi0);
	    pi0Segment.Start(Pi0Vtx);
	    std::cout << "1" << std::endl;
	    pi0Segment.Dir(Pi0.Momentum());
	    std::cout << "2" << std::endl;
	    std::cout << std::endl << "Pi0 Created!" << std::endl;
	  }// if we found the pi0
	    
	}
      }//secon loop
    }//first loop

    int muons = 0;
    int muonIdx = 0;
    ::ertool::Track muonTrk;
    for (size_t t1=0; t1 < data.Track().size(); t1++){
      // compare tracks with Pi0 and find common matches
      // only find one...we want one muon + Pi0 only!
      ::ertool::Track track = data.Track(t1);
      if (_findRel.haveCommonOrigin(track,pi0Segment,EvtVtx)){
	muons += 1;
	muonIdx = t1;
	muonTrk = track;
      }
    }//first loop

    if (muons == 1){
      ::ertool::Particle muon(13,105.66);
      muon.Vertex(muonTrk.front());
      ::geoalgo::Vector_t muonMom(muonTrk.back()-muonTrk.front());
      muonMom.Normalize();
      muon.Momentum(muonMom*muonTrk._energy);
      muon.RecoObjInfo(muonIdx,::ertool::Particle::RecoObjType_t::kTrack);
      ps.push_back(muon);
      std::cout << std::endl << "Muon Created!" << std::endl;
    }

    
    return true;  
  }
    
  bool ERAnaFindRelationship::finalize() {

    _ana_tree->Write();

    return ERToolAnaBase::finalize();
  }

}
#endif
