#ifndef RECOTOOL_CBALGOPOLYSHORTESTDIST_CXX
#define RECOTOOL_CBALGOPOLYSHORTESTDIST_CXX

#include "CBAlgoPolyShortestDist.h"

namespace cmtool {

  //-------------------------------------------------------
  CBAlgoPolyShortestDist::CBAlgoPolyShortestDist() : CBoolAlgoBase()
  //-------------------------------------------------------
  {
    SetMinDistSquared(1.);
    SetMinNumHits(0);
    SetMaxNumHits(99999);
    SetDebug(false);
  }

  //-----------------------------
  void CBAlgoPolyShortestDist::Reset()
  //-----------------------------
  {

  }

  //------------------------------------------------------------------------------------------
  void CBAlgoPolyShortestDist::EventBegin(const std::vector<cluster::cluster_params> &clusters)
  //------------------------------------------------------------------------------------------
  {
    if(clusters.size())
      tmp_min_dist = 99999;
  }

  //-------------------------------
  //void CBAlgoPolyShortestDist::EventEnd()
  //-------------------------------
  //{
  //
  //}

  //-----------------------------------------------------------------------------------------------
  //void CBAlgoPolyShortestDist::IterationBegin(const std::vector<cluster::cluster_params> &clusters)
  //-----------------------------------------------------------------------------------------------
  //{
  //
  //}

  //------------------------------------
  //void CBAlgoPolyShortestDist::IterationEnd()
  //------------------------------------
  //{
  //
  //}
  
  //----------------------------------------------------------------
  bool CBAlgoPolyShortestDist::Bool(const ::cluster::cluster_params &cluster1,
                                    const ::cluster::cluster_params &cluster2)
  //----------------------------------------------------------------
  {
    if( (cluster1.hit_vector.size() < _min_hits) ||
        (cluster2.hit_vector.size() < _min_hits) )
      return false;

    if( (cluster1.hit_vector.size() > _max_hits) ||
        (cluster2.hit_vector.size() > _max_hits) )
      return false;

    //if either has < 3 sides do not merge!
    if ( (cluster1.PolyObject.Size() < 2) or
         (cluster2.PolyObject.Size() < 2) ){
      return false;
    }

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
        
        if(distsqrd < tmp_min_dist) tmp_min_dist = distsqrd;

        if(_debug){
          std::cout<<"two polygon points dist2 is "<<distsqrd<<std::endl;
          std::cout<<"minimum dist was "<<tmp_min_dist<<std::endl;
        }
        if(distsqrd<_dist_sqrd_cut)
          return true;
      }

    }

    return false;
  }

  //------------------------------
  void CBAlgoPolyShortestDist::Report()
  //------------------------------
  {

  }
    
}
#endif
