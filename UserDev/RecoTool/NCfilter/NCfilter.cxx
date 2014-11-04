#ifndef RECOTOOL_NCFILTER_CXX
#define RECOTOOL_NCFILTER_CXX

#include "NCfilter.h"

namespace larlite {

  bool NCfilter::initialize() {

    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you have a histogram to fill in the event loop, for example,
    // here is a good place to create one on the heap (i.e. "new TH1D"). 
    //
    return true;
  }
  
  bool NCfilter::analyze(storage_manager* storage) {
    // This can go in the header when cleaning up;
    // Need some bool to check about the quality of the protoclusters
    unsigned int nplanes = larutil::Geometry::GetME()->Nplanes();
        
    bool TryFuzzyCluster = false;	
    // This is what the final clusters will be 
    std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>> BestClusters(nplanes);
    // These bools are used to stop when we have a good match and good cluster
    std::vector<bool> FlagGoodPlanes(nplanes,false);
    std::vector<std::pair<double,double>> AvgPairSI(nplanes);	
    std::vector<larlite::cluster> clustervect;
    std::vector<larlite::hit> hitsvect;
    std::vector<bool> QualFlip;
    
    // grab the incoming clusters. 
    //auto Incoming_cluster = (const event_cluster*)(storage->get_data(DATA::DBCluster));
    auto Incoming_cluster = storage->get_data<event_cluster>("dbcluster");
    //auto Incoming_cluster = storage->get_data<event_cluster>("fuzzycluster");
    //auto Incoming_cluster = storage->get_data<event_cluster>("cccluster");

    // First of all create an output
    auto Output_cluster = storage->get_data<event_cluster>("ncfilter");

  TryFuzzy:
    if(TryFuzzyCluster){
      // This is fine... really....
      Incoming_cluster = storage->get_data<event_cluster>("fuzzycluster");
      // Need to Reset everything
      BestClusters.clear();
      BestClusters.resize(3);
      FlagGoodPlanes.clear();
      FlagGoodPlanes.resize(3);
      AvgPairSI.clear();	
      AvgPairSI.resize(3);	
      clustervect.clear();
      hitsvect.clear();
      QualFlip.clear();
      std::cout<<"Trying Fuzzy"<<std::endl;
    }

    auto hit_producers = Incoming_cluster->association_keys(data::kHit);
    if(!(hit_producers.size())) 
      print(msg::kERROR,__FUNCTION__,"No associated hit found!");
    
    auto hits = storage->get_data<event_hit>(hit_producers[0]);
    
    if(!hits || !Incoming_cluster) {
      print(msg::kERROR,__FUNCTION__,"No DBCluster or associated hits found!");
      throw std::exception();
      return false;}
    
    //  std::cout<<"Making it here ?" <<std::endl;
    //std::cout<< " Output_cluster->size() "<<Output_cluster->size()<<std::endl;
    
    // set event variables
    if(Output_cluster->size())
      print(msg::kWARNING,__FUNCTION__,"DATA::RyanCluster is not empty. Clearing it...");
    Output_cluster->clear_data();
    Output_cluster->set_event_id(Incoming_cluster->event_id());
    Output_cluster->set_run(Incoming_cluster->run());
    Output_cluster->set_subrun(Incoming_cluster->subrun());

    if(!(Incoming_cluster->size())){
      print(msg::kWARNING,__FUNCTION__,Form("Event %d has no DBCluster...",Incoming_cluster->event_id()));
      return true;}
    else if(!(hits->size())){
      print(msg::kERROR,__FUNCTION__,Form("Event %d has no hits (but there's DBCluster!!!)",Incoming_cluster->event_id()));
      return false;}

    //============Starting with  first pass alg============
    // make a vector of clusters
    for(auto const& c : *Incoming_cluster) clustervect.push_back(c);
    for(auto const& h : *hits) hitsvect.push_back(h);
    auto const& cluster_hit_ass = Incoming_cluster->association(hits->id());
    AvgPairSI = fDivReg.SplitLineC(clustervect, cluster_hit_ass, hitsvect);
    //====================Ending with alg===================

	
    //=======================
    //=== make the regions===
    //=======================
    // This is making the first set of regions that will be tested
    std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>> FirstRegions = fForceRegions.ForceTwoRegions(hitsvect,AvgPairSI);
    //=======================
    
    //=======================
    //=== Check Bools========
    //=======================
    // do the time prof test...
    std::vector<bool> QualFirst = fPlaneCheckMatch.PlaneCheck( FirstRegions, hits);
	//~~~~~~~~~~~^^~~~~~~~~~~~~~ The output is a vector with relations Q01,Q02,Q12
    std::vector<bool> flip(3,false);
    if(QualFirst[0]) {FlagGoodPlanes[0] = true; FlagGoodPlanes[1] = true;}
    if(QualFirst[1]) {FlagGoodPlanes[0] = true; FlagGoodPlanes[2] = true;}
    if(QualFirst[2]) {FlagGoodPlanes[1] = true; FlagGoodPlanes[2] = true;}
    // also fill in a check for the protovertex
    
    //==============================================
    // Check to see what we need to do next.
    // Should we try another alg or keep theses? 
    //==============================================
    for(unsigned int a = 0 ;a < nplanes;a++){
      if(!FlagGoodPlanes[a]) flip[a]= true;
      // If the plane bools are NOT TRUE  Then turn on the flip bool for that plane
      if(FlagGoodPlanes[a]) BestClusters[a]= FirstRegions[a];
      // If the plane bools are true then fill into the final cluster.. BestClusters[a]
    }// for loop over all the planes
    
    
    // check the rough vertex
    std::vector<std::pair<double,double>> RoughVertex =fQuality.ProtoVertexCW(FirstRegions,hits);
    
    //-----------------------------------------------------------------------------------------------------------------------------------
    //-------------- DONE WITH ALG PASS... MOVING ON-------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------------------------------------
    
    //====================================================
    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //============Starting with  flip pass alg============
    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //====================================================
    // if not all the plane bools are true then do some more work
    for( unsigned int i = 0 ; i<flip.size();i++)
      {
	if(flip[i]){
	  std::vector<std::pair<double, double> > FlipAvgPairSI = fDivReg.FlipLineC(clustervect, cluster_hit_ass, hitsvect);
	  std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>> FlipRegions = fForceRegions.ForceTwoFlipRegions(hitsvect, FlipAvgPairSI);
	  // This being in the if will prevent overwriting the stuff that is already good.
	  //		if(FlipRegions[i].first.size()>0 && FlipRegions[i].second.size()>0){
	  //BestClusters[i] = FlipRegions[i];}
	  BestClusters[i] = FlipRegions[i];
	}// end of if flip[i]
      }// for loop over all the flip stuff
    //=======================
    //=== Check Bools========
    //=======================
    // do the time prof test...
    //	std::cout<<"Size of all the best clusters"<<BestClusters[0].first.size()<<" , "<<BestClusters[0].second.size()<<std::endl<<BestClusters[1].first.size()<<" , "<<BestClusters[1].second.size()<<std::endl<<BestClusters[2].first.size()<<" , "<<BestClusters[2].second.size()<<std::endl;
    
    QualFlip = fPlaneCheckMatch.PlaneCheck(BestClusters, hits);
    ///---------------------------------------------------------------------------------
    // Check to see what we need to do next after the Rotate. Should we try another alg or keep theses? 
    //---------------------------------------------------------------------------------
    std::vector<bool> RotateAlg(3,false);
    if(QualFlip[0]) {FlagGoodPlanes[0] = true; FlagGoodPlanes[1] = true;}
    if(QualFlip[1]) {FlagGoodPlanes[0] = true; FlagGoodPlanes[2] = true;}
    if(QualFlip[2]) {FlagGoodPlanes[1] = true; FlagGoodPlanes[2] = true;}
    for(unsigned int a = 0 ;a < nplanes;a++){
      if(!FlagGoodPlanes[a]) RotateAlg[a]= true;
    }// loop over the planes	
    
    std::vector<std::pair<double,double>> RoughFlipVertex =fQuality.ProtoVertexCW(BestClusters,hits);
    
    //-----------------------------------------------------------------------------------------------------------------------------------
    //-------------- DONE WITH ALG PASS... MOVING ON-------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------------------------------------------------
    
    //====================================================
    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //============Starting with  Rotate alg===============
    //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    //====================================================
    for( unsigned int angle = 0 ; angle < 360; angle+=20){
      // if not all the plane bools are true then do some more work
      for( unsigned int i = 0 ; i<RotateAlg.size();i++)
	{
	  // This is not that efficient... I am doing the loop a few time... RG Come back and clean up
	  if(RotateAlg[i]){
	    std::vector<std::pair<double, double> > RotateAvgPairSI = fDivReg.RotateLineC(clustervect, cluster_hit_ass, hitsvect);
	    std::vector<std::pair<std::vector<unsigned int>,std::vector<unsigned int>>>  RotateRegions = \
	      fForceRegions.ForceTwoRotateRegions(hitsvect, RotateAvgPairSI, angle);
	    // This being in the if will prevent overwriting the stuff that is already good.
	    BestClusters[i] = RotateRegions[i];
	  }// end of if Rotate[i]
	}// for loop over all the Rotate stuff
      //====================Ending with alg===================
      
      //=======================
      //=== Check Bools========
      //=== for Rotation ======
      //=======================
      // do the time prof test...
      std::vector<bool> QualRot = fPlaneCheckMatch.PlaneCheck(BestClusters, hits);
      ///---------------------------------------------------------------------------------
      // Check to see what we need to do next after the Rotate. Should we try another alg or keep theses? 
      //---------------------------------------------------------------------------------
      std::vector<bool> RotateAlg(3,false);
      if(QualRot[0]) {FlagGoodPlanes[0] = true; FlagGoodPlanes[1] = true;}
      if(QualRot[1]) {FlagGoodPlanes[0] = true; FlagGoodPlanes[2] = true;}
      if(QualRot[2]) {FlagGoodPlanes[1] = true; FlagGoodPlanes[2] = true;}
      for(unsigned int a = 0 ;a < nplanes;a++) if(!FlagGoodPlanes[a]) RotateAlg[a]= true;
      
      std::vector<std::pair<double,double>> RoughRotVertex =fQuality.ProtoVertexCW(BestClusters,hits);
      
      if(FlagGoodPlanes[0] && FlagGoodPlanes[1] && FlagGoodPlanes[2]) break;
    }// For loop over the angle 
    
    
    //===================================================================
    //====== THIS IS THE END OF ALL THE ALGS ============================
    //===================================================================

    // Look back trying a different cluster
	

    /*	
	if(FlagGoodPlanes[0]==false && FlagGoodPlanes[1]==false && FlagGoodPlanes[2]==false 
	&&BestClusters[0].first.size()==0 && BestClusters[0].second.size()==0
	&& BestClusters[1].first.size()==0 && BestClusters[1].second.size()==0
	&& BestClusters[2].first.size()==0 && BestClusters[2].second.size()==0 && TryFuzzyCluster==false ){TryFuzzyCluster = true; goto TryFuzzy;} 
    */ 
    
    if(FlagGoodPlanes[0]==false && FlagGoodPlanes[1]==false && FlagGoodPlanes[2]==false && TryFuzzyCluster==false){TryFuzzyCluster = true; goto TryFuzzy;} 
    
    //==================================
    //=== Try to unmerge some things ===
    //==================================
    for(unsigned int a = 0 ; a< 3 ; a++){
      // if this is a good plane there are clusters in it
      if(FlagGoodPlanes[a]){
	std::pair<std::vector<unsigned int>,std::vector<unsigned int>> Nicer_Clusters = fLookBack.ClusterRefine(BestClusters[a],hitsvect);
	// reseting the clusters
	BestClusters[a].first = Nicer_Clusters.first;
	BestClusters[a].second = Nicer_Clusters.second;
      }// if the plane can be looped
    }// for loop over all the planes
    
    
    // RG Can do another time prof or time overlap check here!
    
    //==================================
    //====== Try Refining the Angle ====
    //==================================
    
    // First sort out the Crawler Clusters based on which which hits belong to which cluster. 
    /*
      std::cout<<"protovertex:"<<std::endl;
      std::vector<std::pair<double,double>> pv = fquality.protovertexcw(bestclusters,hits);
      for( int i =0 ;i<3;i++){
      std::cout<< "\t\t "<< pv[i].first<<" , "<<pv[i].second<<std::endl;}
    */	
    
    //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    std::cout<< " \n\n\n"<<std::endl;

    // Prepare association container
    AssSet_t  hit_ass_set;
    AssUnit_t hit_ass;
    hit_ass_set.reserve(BestClusters.size()*2);

    //===================================================================
    //=======Lets fill these hits out into clusters.===================== 
    //===================================================================
    for( unsigned int a=0 ; a<BestClusters.size();a++){
      // need a check to see if we did well or not
      if(FlagGoodPlanes[a]){
	::larlite::cluster lite_cluster;
	// Skip if there's no hit
	if(!(BestClusters[a].first.size()) || !(BestClusters[a].second.size())) continue;
	
	//
	// Save clusters
	//
	
	// Clear data products
	hit_ass.clear();
	hit_ass.reserve(BestClusters[a].first.size());
	lite_cluster.clear_data();
	
	// Make association
	for( auto const& hit : BestClusters[a].first) hit_ass.push_back(hit);

	// Add association
	hit_ass_set.push_back(hit_ass);
	
	// Set cluster ID
	lite_cluster.set_id(Output_cluster->size());
	
	// Set cluster view
	lite_cluster.set_view(hits->at(hit_ass.at(0)).View());
	
	// Add a cluster to the output
	Output_cluster->push_back(lite_cluster);
	
	// Clear data products
	hit_ass.clear();
	hit_ass.reserve(BestClusters[a].second.size());
	lite_cluster.clear_data();
	
	// Make association
	for( auto const& hit : BestClusters[a].second) hit_ass.push_back(hit);
	
	// Add association
	hit_ass_set.push_back(hit_ass);
	
	// Set cluster ID
	lite_cluster.set_id(Output_cluster->size());
	
	// Set cluster view
	lite_cluster.set_view(hits->at(hit_ass.at(0)).View());
	
	// Add a cluster to the output
	Output_cluster->push_back(lite_cluster);
	
      }// if FLagGoodPlanes[a]
    }// loop over Best Clusters
    Output_cluster->set_association(hits->id(),hit_ass_set);
    return true;
  }

  bool NCfilter::finalize() {

    // This function is called at the end of event loop.
    // Do all variable finalization you wish to do here.
    // If you need, you can store your ROOT class instance in the output
    // file. You have an access to the output file through "_fout" pointer.
    //
    // Say you made a histogram pointer h1 to store. You can do this:
    //
    // if(_fout) { _fout->cd(); h1->Write(); }
    //
    // else 
    //   print(msg::kERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
    //
  
    return true;
  }

//---------------------
// Adding some fuctions 
//---------------------









}
#endif
