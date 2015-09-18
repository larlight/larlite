#ifndef RECOTOOL_CBALGOMERGETINYWITHBIG_CXX
#define RECOTOOL_CBALGOMERGETINYWITHBIG_CXX

#include "CBAlgoMergeTinyWithBig.h"

namespace cmtool {

  //-------------------------------------------------------
  CBAlgoMergeTinyWithBig::CBAlgoMergeTinyWithBig() : CBoolAlgoBase()
  //-------------------------------------------------------
  {
    SetMinHitsBig(50);
    SetMaxHitsBig(99999);
    SetMinHitsSmall(0);
    SetMaxHitsSmall(15);
    SetMinDistSquared(0);

    SetDebug(false);
  }

  //-----------------------------
  void CBAlgoMergeTinyWithBig::Reset()
  //-----------------------------
  {

  }

  //----------------------------------------------------------------
  bool CBAlgoMergeTinyWithBig::Bool(const ::cluster::cluster_params &cluster1,
                                    const ::cluster::cluster_params &cluster2)
  //----------------------------------------------------------------
  {

    if(_debug)
      std::cout<<"MergeTinyWithBig. One cluster has "
               <<cluster1.hit_vector.size()<<" hits, the other has "
               <<cluster2.hit_vector.size()<<" hits."<<std::endl;
        
    
    bool is_1_small = false;
    bool is_2_small = false;
    bool is_1_big = false;
    bool is_2_big = false;

    //if the first cluster counts as "small"
    if(cluster1.hit_vector.size() > _min_hits_small &&
       cluster1.hit_vector.size() < _max_hits_small)
      is_1_small = true;
    //if the second cluster counts as "small"
    if(cluster2.hit_vector.size() > _min_hits_small &&
       cluster2.hit_vector.size() < _max_hits_small)
      is_2_small = true;
    if(cluster1.hit_vector.size() > _min_hits_big &&
       cluster1.hit_vector.size() < _max_hits_big)
      is_1_big = true;
    if(cluster2.hit_vector.size() > _min_hits_big &&
       cluster2.hit_vector.size() < _max_hits_big)
      is_2_big = true;
    
    if(_debug)
      std::cout<<"is_1_small, is_1_big, is_2_small, is_2_big are: "
               <<is_1_small<<", "<<is_1_big<<", "
               <<is_2_small<<", "<<is_2_big<<std::endl;

    //if neither of the clusters is small don't merge
    if(!is_1_small && !is_2_small)
      return false;
    //if neither of the clusters is big, don't merge
    if(!is_1_big && !is_2_big)
      return false;
    //if both are small, don't merge
    if(is_1_small && is_2_small)
      return false;
    //if both are big, don't merge
    if(is_1_big && is_2_big)
      return false;

    if(_debug)
      std::cout<<"Looks like one of them is big, and one is small."<<std::endl;
    //god this code is ugly

    //now we know which one of them is big & the other is small.

    //loop over the points on the first polygon and calculate
    //distance to each point on the second polygon
    //if any two points are close enough to each other,
    //merge the two clusters

    unsigned int npoints1 = cluster1.PolyObject.Size();
    unsigned int npoints2 = cluster2.PolyObject.Size();
    //loop over points on first polygon
    for(unsigned int i = 0; i < npoints1; ++i){
      float pt1w = cluster1.PolyObject.Point(i).first;
      float pt1t = cluster1.PolyObject.Point(i).second;
      //loop over points on second polygon
      for(unsigned int j = 0; j < npoints2; ++j){
        float pt2w = cluster2.PolyObject.Point(j).first;
        float pt2t = cluster2.PolyObject.Point(j).second;
        double distsqrd = pow(pt2w-pt1w,2)+pow(pt2t-pt1t,2);
        
        if(_debug){
          std::cout<<"two polygon points dist2 is "<<distsqrd<<std::endl;
        }
        if(distsqrd<_dist_sqrd_cut)
          return true;
      }

    }

    return false;

  }

  //------------------------------
  void CBAlgoMergeTinyWithBig::Report()
  //------------------------------
  {

  }
    
}
#endif
