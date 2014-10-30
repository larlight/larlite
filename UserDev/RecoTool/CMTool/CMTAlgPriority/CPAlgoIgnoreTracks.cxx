#ifndef RECOTOOL_CPALGOIGNORETRACKS_CXX
#define RECOTOOL_CPALGOIGNORETRACKS_CXX

#include "CPAlgoIgnoreTracks.h"

namespace cmtool {

  //----------------------------------------------------------
  CPAlgoIgnoreTracks::CPAlgoIgnoreTracks() : CPriorityAlgoBase()
  //----------------------------------------------------------
  {
    SetMinHits(20);
    SetMinModHitDens(1.4);
    SetMinMHitWires(3.5);
    //"fPrincipal" is log(1-eigenvalue_principal)
    // >-7 means EP > 0.99908
    SetMinPrincipal(-6.);
  }

  //-----------------------------
  void CPAlgoIgnoreTracks::Reset()
  //-----------------------------
  {

  }

  //------------------------------------------------------------------------------
  float CPAlgoIgnoreTracks::Priority(const ::cluster::ClusterParamsAlg &cluster)
  //------------------------------------------------------------------------------
  {
    //return -1 for tracks, otherwise return nhits
    auto nhit = cluster.GetNHits();
    
    //NOTE! default return value for non-track clusters is nhits
    //but if you want to sort by a different way, use a separate
    //CPAlgo in an array with this one, and use the kLastAlgo option
    //to sort by that algo and still remove tracks!
    return ( IsTrack(cluster) ? -1 : (float)nhit );
  }
  
  //------------------------------
  void CPAlgoIgnoreTracks::Report()
  //------------------------------
  {

  }
    

  bool CPAlgoIgnoreTracks::IsTrack(const ::cluster::ClusterParamsAlg &cluster)
  {
    double fPrincipal = TMath::Log(1-cluster.GetParams().eigenvalue_principal);

    bool isTrack = false;
    if(cluster.GetNHits() > _min_hits &&
       cluster.GetParams().modified_hit_density < _min_mod_hit_dens &&
       cluster.GetParams().multi_hit_wires < _min_multihit_wires &&
       fPrincipal < _min_principal)
      isTrack = true;

    return isTrack;
  }
}
#endif
