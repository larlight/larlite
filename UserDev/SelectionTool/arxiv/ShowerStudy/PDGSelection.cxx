#ifndef SELECTIONTOOL_PDGSELECTION_CXX
#define SELECTIONTOOL_PDGSELECTION_CXX

#include "PDGSelection.h"

namespace larlite {

  PDGSelection::PDGSelection() : 
    _request_list(PDGSelection::kPARTICLE_GROUP_MAX,std::map<int,int>()),
    _found_list(PDGSelection::kPARTICLE_GROUP_MAX,std::map<int,int>())
  { 
    _name="PDGSelection"; 
    _fout=0;
    Reset();
  }

  void PDGSelection::Reset()
  {

    for(auto& list : _request_list) list.clear();
    for(auto& list : _found_list) list.clear();

    _nevent_analyzed = 0;
    _nevent_selected = 0;

  }

  void PDGSelection::Select(int const pdg_code, 
			    PDGSelection::PARTICLE_GROUP part_type,
			    int count)
  {
    if(part_type == PDGSelection::kPARTICLE_GROUP_MAX) 
      
      throw ::showerana::ShowerAnaException(Form("<<%s>> Invalid particle type: %d",
						 __FUNCTION__,
						 part_type)
					    );

    _request_list.at(part_type).insert(std::make_pair(pdg_code,count));

  }

  bool PDGSelection::initialize() {

    _nevent_analyzed = 0;

    _nevent_selected = 0;

    return true;
  }
  
  bool PDGSelection::analyze(storage_manager* storage) {

    // Clear found list
    for(auto& list : _found_list) list.clear();

    // Log # of analyze() function calls
    ++_nevent_analyzed;

    // Process if a user specified to look into a primary particle list
    if(_request_list.at(kGENERATOR).size()) {

      // Pull out a particle list to which we store particle count
      auto& gen_list = _found_list.at(kGENERATOR);

      // Pull out larlight::event_mctruth data product
      auto ev_mct = storage->get_data<event_mctruth>("generator");

      // Check if this is a valid data product
      if(!ev_mct) 
	
	throw ::showerana::ShowerAnaException(Form("<<%s>> MCTruth information not found!",
						   __FUNCTION__)
					      );

      // Loop over a vector of larlight::mctruth data product
      for(auto const& mct : *ev_mct) {
	
	// Loop over a vector of larlight::mcpart that is contained in this larlight::mctruth
	for(auto const& mcp : mct.GetParticles()) {

	  // Record the count of this PDG code
	  auto iter = gen_list.find(mcp.PdgCode());
	  if(iter != gen_list.end()) (*iter).second += 1;
	  else gen_list.insert(std::make_pair(mcp.PdgCode(),1));
	  
	}
      }
    }

    // Process if a user specified to look into a secondary particle list
    if(_request_list.at(kG4PRIMARY).size() || _request_list.at(kG4SECONDARY).size()) {

      // Save typing by making a local reference to the subject list of PDG code
      auto& primary_list = _found_list.at(kG4PRIMARY);
      auto& secondary_list = _found_list.at(kG4SECONDARY);

      // Pull out larlight::event_mcpart data product
      auto ev_mcp = storage->get_data<event_mcpart>("largeant");

      // Check data product validity
      if(!ev_mcp) 

	throw ::showerana::ShowerAnaException(Form("<<%s>> MCTruth information not found!",
						   __FUNCTION__)
					      );

      // Loop over a vector of larlight::mcpart data proudct
      for(auto const& mcp : *ev_mcp) {

	// Record the count of this PDG code
	if( mcp.Mother() ) {

	  auto iter = primary_list.find(mcp.PdgCode());
	  if(iter != primary_list.end()) (*iter).second +=1;
	  else primary_list.insert(std::make_pair(mcp.PdgCode(),1));

	}else{

	  auto iter = secondary_list.find(mcp.PdgCode());
	  if(iter != secondary_list.end()) (*iter).second +=1;
	  else secondary_list.insert(std::make_pair(mcp.PdgCode(),1));

	}
      }
    }

    // Check if this event should be selected or not
    bool status = true;
    for(size_t index=0; index< _request_list.size(); ++index) {

      if(!_request_list[index].size()) continue;

      auto const& request_list = _request_list.at(index);
      auto const& found_list   = _found_list.at(index);

      for(auto const& pdg_ctr_pair : request_list) {

	auto const& pdg = pdg_ctr_pair.first;
	auto const& ctr = pdg_ctr_pair.second;

	if(ctr) {
	  auto const& iter = found_list.find(pdg);
	  if(iter == found_list.end()) status = false;
	  else if(ctr>0 && (*iter).second != ctr) status = false;	  
	}
	else if(found_list.find(pdg) != found_list.end()) {
	  status = false;
	}
	if(!status) break;
      }
      if(!status) break;
    }

    if(status) _nevent_selected++;

    return status;
  }

  bool PDGSelection::finalize() {

    print(msg::kNORMAL,__FUNCTION__,Form("Selected %zu out of %zu events...",
					 _nevent_selected,
					 _nevent_analyzed)
	  );

    return true;
  }

}
#endif
