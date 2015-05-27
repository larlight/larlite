#ifndef RECOTOOL_MCSHOWERCLUSTERER_CXX
#define RECOTOOL_MCSHOWERCLUSTERER_CXX

#include "MCShowerClusterer.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/simch.h"
#include "DataFormat/cluster.h"
#include "DataFormat/hit.h"
//tmp debug
#include <algorithm>

namespace larlite {

  bool MCShowerClusterer::initialize() {

    _nplanes = ::larutil::Geometry::GetME()->Nplanes();

    //all hits will have the same width
    //guessing at this value now from looking at fEndTime-fStartTime for single e- shower hits
    _hitwidth = 3.;

    return true;
  }
  
  bool MCShowerClusterer::analyze(storage_manager* storage) {

    auto geo = ::larutil::Geometry::GetME();
    //auto geoutil = ::larutil::GeometryUtilities::GetME();

    // Get MCShower objects from storage
    auto mcshower_v = (event_mcshower* )(storage->get_data(data::kMCShower,"mcshower") );

    auto simch_v = (event_simch*)(storage->get_data(data::kSimChannel,"largeant") );

    // Check MCShower object exists
    if( !mcshower_v ) {
      print(msg::kERROR,__FUNCTION__,"MCShower object DNE! Skipping event...");
      return false;
    }
    if( !simch_v ) {
      print(msg::kERROR,__FUNCTION__,"SimChannel object DNE! Skipping event...");
      return false;
    }

    // Output hits
    auto out_hit_v = (event_hit*)(storage->get_data(data::kHit,"mcshower"));
    if(!out_hit_v) {
      print(msg::kERROR,__FUNCTION__,"data::kHit could not be retrieved!");
      return false;
    }
    if(out_hit_v->size())
      print(msg::kWARNING,__FUNCTION__,"data::kHit is not empty. Clearing it...");
    out_hit_v->clear_data();

    // Output clusters
    auto out_clus_v = (event_cluster*)(storage->get_data(data::kCluster,"mcshower"));
    if(!out_clus_v) {
      print(msg::kERROR,__FUNCTION__,"data::kCluster could not be retrieved!");
      return false;
    }
    if(out_clus_v->size())
      print(msg::kWARNING,__FUNCTION__,"data::kCluster is not empty. Clearing it...");
    out_clus_v->clear_data();
    out_clus_v->reserve(mcshower_v->size() * _nplanes);

    // finished creating new products -> set ID
    storage->set_id(mcshower_v->run(),mcshower_v->subrun(),mcshower_v->event_id());

    // Loop over MCShowers
    AssSet_t  assn_cs_v;
    AssSet_t  assn_hc_v;
    for(size_t shower_index = 0; shower_index < mcshower_v->size(); ++shower_index) {
      
      // Get shower object
      auto const& ishower = mcshower_v->at(shower_index);

      // Get a set of daughter track IDs
      std::set<unsigned int> daughters;
      for(auto const& id : ishower.DaughterTrackID())
	daughters.insert(id);

      // Create an output cluster container
      std::vector<larlite::cluster> clusters(geo->Nplanes(),larlite::cluster());
      for(size_t plane=0; plane<geo->Nplanes(); ++plane)
	clusters.at(plane).set_view(geo->PlaneToView(plane));

      // Create an output cluster-hit association container
      std::vector<AssUnit_t> assn_hc(geo->Nplanes(),AssUnit_t());

      // Loop over channels
      for(auto const& sch : *simch_v) {

	// Get channel number
	auto const& ch = sch.Channel();

	auto const& w  = geo->ChannelToWireID(ch);

	auto const& plane = geo->ChannelToPlane(ch);

	auto const& view  = geo->PlaneToView(plane);

	// Retrieve data for all energy deposition in this channel
	auto& tdc_ide_map = sch.TDCIDEMap();

	// Loop over all energy deposition data
	for(auto const& tdc_ide : tdc_ide_map) {

	  // Get timing
	  auto const& tdc = tdc_ide.first;

	  double hitq = 0;
	  bool   hit_found = false;
	  // Loop over all energy deposition @ this timing
	  for(auto const& this_ide : tdc_ide.second) {

	    unsigned int daughter_track = 0;
	    if(this_ide.trackID > 0) daughter_track = this_ide.trackID;
	    else daughter_track = (-1 * this_ide.trackID);

	    // If nothing to do with this MCShower, ignore.
	    if(daughters.find(daughter_track) == daughters.end()) continue;
	    
	    hit_found = true;

	    //hard-coding fit functions DC has figured out to 
	    //convert number of electrons to charge
	    //USING AMPLITUDE FOR NOW
	    //http://www.nevis.columbia.edu/~dcaratelli/showandtell/U_Wire_MaxWire_vs_Qin_078_20.png
	    //CAN DO SIMILAR WITH AREA IF YOU WANT
	    if(view==::larlite::geo::kU)
	      hitq += (this_ide.numElectrons/1000)*1.33 - 0.443;
	    else if(view==::larlite::geo::kV)
	      hitq += (this_ide.numElectrons/1000)*.938 - 0.222;
	    else if(view==::larlite::geo::kW)
	      hitq += (this_ide.numElectrons/1000)*1.79 + 0.507;
	    else {std::cout<<"wtf"<<std::endl;return false;}

	  }


	  if(!hit_found) continue;


	  //instead of merging hits, try cutting on amplitude
	  //wes says cut on 6 for collection and 3 for induction
	  if( (view == ::larlite::geo::kU && hitq < 3) ||
	      (view == ::larlite::geo::kV && hitq < 3) ||
	      (view == ::larlite::geo::kW && hitq < 6) ) 
	    continue;

	  // Create a hit (and association)
	  // ONLY IF IT DOESN'T OVERLAP AN EXISTING HIT, ELSE MODIFY THAT EXISTING HIT
	  int overlap_index = DoesHitOverlapExisting(out_hit_v,ch,tdc-(_hitwidth/2),tdc+(_hitwidth/2));
	  if(overlap_index == -1){

	    ::larlite::hit h;
	    h.set_wire    ( w    );
	    h.set_channel ( ch   );

	    double start_time = tdc - (_hitwidth/2);
	    if(start_time<0) start_time = 0;

	    h.set_time_range ( start_time, tdc + (_hitwidth/2) );
	    
	    h.set_time_peak( tdc, 0 );

	    h.set_view    ( view );

	    h.set_integral  ( hitq, 0 );

	    h.set_amplitude  ( hitq, 0 );
	    
	    switch(view) {
	      
	    case ::larlite::geo::kU:
	    case ::larlite::geo::kV:
	      h.set_signal_type( ::larlite::geo::kInduction   );
	      break;
	    case ::larlite::geo::kW:
	      //case ::larlite::geo::kZ:
	      h.set_signal_type( ::larlite::geo::kCollection  );
	      break;
	    default:
	      h.set_signal_type( ::larlite::geo::kMysteryType );
	    }
	    
	    out_hit_v->push_back(h);
	    
	    assn_hc.at(plane).push_back(out_hit_v->size()-1);
	    
	  } //kaleko end doesoverlap
	  
	  //if this hit overlaps an existing hit, modify the existing hit
	  else{
	    Double_t newstart = std::min(tdc-(_hitwidth/2),double(out_hit_v->at(overlap_index).StartTick()));
	    Double_t newend   = std::max(tdc+(_hitwidth/2),double(out_hit_v->at(overlap_index).EndTick()));
	    //to-do: implement weighted average by charge here
	    Double_t newpeak  = newstart + ((newend-newstart)/2);
	    Double_t newq     = out_hit_v->at(overlap_index).Integral() + hitq;
	    out_hit_v->at(overlap_index).set_time_range(newstart,newend);
	    out_hit_v->at(overlap_index).set_time_peak(newpeak,0);
	    out_hit_v->at(overlap_index).set_integral(newq,0);
	    out_hit_v->at(overlap_index).set_amplitude(newq,0);
	  }


	  //whether or not hit overlaps other hits, add to cluster charge
	  clusters.at(plane).set_integral(clusters.at(plane).Integral() + hitq, 0,
					  clusters.at(plane).Integral() + hitq);

	}
      }
      
      //
      // Store
      //

      // MCShower->Cluster association
      AssUnit_t assn_cs;
      assn_cs.reserve(geo->Nplanes());

      for(size_t plane = 0; plane<geo->Nplanes(); ++plane) {

	if(!(assn_hc.at(plane).size())) continue;

	clusters.at(plane).set_id(out_clus_v->size());

	clusters.at(plane).set_view(geo->PlaneToView(plane));
	
	assn_cs.push_back(out_clus_v->size());

	assn_hc_v.push_back(assn_hc[plane]);

	out_clus_v->push_back(clusters.at(plane));

      }
      assn_cs_v.push_back(assn_cs);
    }

    // store association
    auto out_event_ass = (event_ass*)(storage->get_data(data::kAssociation,"cmtool"));
    out_event_ass->set_association(product_id(data::kCluster,out_clus_v->name()),product_id(data::kHit,"mcshower"),assn_hc_v);
    out_event_ass->set_association(product_id(data::kMCShower,mcshower_v->name()),product_id(data::kCluster,"mcshower"),assn_cs_v);
    //out_clus_v->set_association(data::kHit,"mcshower",assn_hc_v);
    //mcshower_v->set_association(data::kCluster,"mcshower",assn_cs_v);

    return true;
  }

  bool MCShowerClusterer::finalize() {

    return true;
  }

  int MCShowerClusterer::DoesHitOverlapExisting(event_hit* original_hit_v,UInt_t channel,Double_t start,Double_t end) {


    if(!_group_overlapping_hits) return -1;

    //check if there are overlapping hits on the same channel with this fancy c++11 find_if thing
    //(if existing hit duration includes the current hit start time, or
    // if existing hit duration includes the current hit end time)

    auto it = std::find_if(original_hit_v->begin(),
			   original_hit_v->end(),
			   [&channel,&start,&end](const ::larlite::hit &ihit) {return ihit.Channel() == channel && ( (ihit.StartTick()<start && ihit.EndTick()>start) || (ihit.StartTick()<end && ihit.EndTick()>end) );});


    //if overlapping hit found, return the index in original_hit_v of that hit
    if(it != original_hit_v->end())
      return (int)(it-original_hit_v->begin());

    //return -1 if hit does not overlap any existing hits
    return -1;
  }
}
#endif
