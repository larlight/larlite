#ifndef MAKEPARTICLETREE_CXX
#define MAKEPARTICLETREE_CXX

#include "MakeParticleTree.h"

namespace larlite {


  void MakeParticleTree::MakeTree(const event_mcpart& evt_part,
				  event_mctree& evt_tree)
  {

    // first need to make map
    setParticleMap(evt_tree, evt_part);

    // now can go through the map and add trees to evt_tree
    setTrees(evt_tree, evt_part);

    // set event number etc
    evt_tree.set_subrun(evt_part.subrun());
    evt_tree.set_run(evt_part.run());
    evt_tree.set_event_id(evt_part.event_id());

    return;
  }

  void MakeParticleTree::setParticleMap(event_mctree& evt_tree,
					const event_mcpart& evt_part){

    // if evt_part empty return
    if (evt_part.size() == 0){
      std::cout << "Event Part not filled...exit" << std::endl;
      return;
    }

    //clear module's map
    _ParticleMap.clear();

    //clear map before filling it
     //Fill the Map for particles and trackIDs
    for (size_t i=0; i < evt_part.size(); i++){
      _ParticleMap[evt_part.at(i).TrackId()] = i;
    }//for all particles in evt_part
    
    // set this map for the evt_tree object
    evt_tree.setMap(_ParticleMap);

    return;
  } 


  int MakeParticleTree::searchParticleMap(const int trackID){
    
    try{
      return _ParticleMap.at(trackID);
    }
    catch (const std::out_of_range& oor){
      //       std::cerr << "Out of Range Error: Mother TrackID not found. Likely a Primary particle." << std::endl;
    }
    return -1;
  }


  void MakeParticleTree::setTrees(event_mctree& evt_tree,
				  const event_mcpart& evt_part){

     // if evt_part empty return
     if (evt_part.size() == 0){
       std::cout << "Event Part not filled...exit" << std::endl;
       return;
     }

     for (size_t i=0; i < evt_part.size(); i++){
       mcpart part = evt_part.at(i);
       if (part.Mother() == 0){
	 mctree node(part.TrackId());
	 node.setPrimary(true);
	 node.setAncestorId(part.TrackId());
	 // add all sub-nodes for this root node
	 AddNodes(part, node, part.TrackId(), evt_part);
	 evt_tree.push_back(node);
       }// if the process is primary -> new root node
     }// for all particles

    return;
  }


  void MakeParticleTree::AddNodes(mcpart part,
				  treenode& parentnode,
				  int ancestor,
				  const event_mcpart& evt_part){

    std::set<Int_t> daughters = part.Daughters();
    //loop over daugher iterator -> add daughters to node
    for (std::set<int>::iterator it=daughters.begin(); it!=daughters.end(); ++it){
      mcpart tmpPart = evt_part.at(this->searchParticleMap(it));
      ::treenode tmpnode(tmpPart.TrackId());
      tmpnode.setParentId(part.TrackId());
      tmpnode.setAncestorId(ancestor);
      tmpnode.setPrimary(false);
      //use recursion to fill this node's children as well
      AddNodes(tmpPart, tmpnode, ancestor, evt_part);
      parentnode.AddChild(tmpnode);
    }

    return;
  }



}

#endif
