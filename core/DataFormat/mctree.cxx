#ifndef LARLITE_MCTREE_CXX
#define LARLITE_MCTREE_CXX

#include "mctree.h"

namespace larlite {

  mctree::mctree(int trackId)
    : data_base(data::kMCTree), treenode::treenode(trackId)
  {
    clear_data();
  }
    
  void mctree::clear_data() 
  {}

  int event_mctree::searchParticleMap(int trackId){
    
    try{
      return _particleMap.at(trackId);
    }
    catch(const std::exception& oor){
    //catch (const std::out_of_range& oor){
      //       std::cerr << "Out of Range Error: Mother TrackID not found. Likely a Primary particle." << std::endl;
    }
    return -1;
  }


}

#endif
