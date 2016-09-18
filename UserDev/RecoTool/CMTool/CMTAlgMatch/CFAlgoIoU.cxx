#ifndef RECOTOOL_CFALGOIOU_CXX
#define RECOTOOL_CFALGOIOU_CXX

#include "CFAlgoIoU.h"

namespace cmtool {

  //-------------------------------------------------------
  CFAlgoIoU::CFAlgoIoU() : CFloatAlgoBase()
  //-------------------------------------------------------
  {
    SetDebug(false) ;
    SetVerbose(false) ;
    RequireThreePlanes(true) ;
  }

  //-----------------------------
  void CFAlgoIoU::Reset()
  //-----------------------------
  {

  }

  //----------------------------------------------------------------------------------------------
  float CFAlgoIoU::Float(const std::vector<const cluster::cluster_params*> &clusters)
  //----------------------------------------------------------------------------------------------
  {
    
    // Code-block by Kazu starts
    // This ensures the algorithm works only if # clusters is > 2 (and not =2)
    // You may take out this block if you want to allow matching using clusters from only 2 planes.
    if(_require_3planes && clusters.size()==2) return -1;
    // Code-block by Kazu ends

    double t_min_abs = 9600; // smallest start point of the 3
    double t_max_abs = 0;    // largest start point of the three

    
    for(auto const& c : clusters){

      double min,max;

      getMinMaxTime(c,min,max);

      if ( min < t_min_abs )
	t_min_abs = min;
      if ( max   > t_max_abs )
	t_max_abs = max;
      
    }
    
    if (clusters.size() < 2) return -1;

    // do the clusters overlap at all?
    bool overlap = true;

    double t_min_common, t_max_common;

    getMinMaxTime(clusters[0],t_min_common,t_max_common);

    for (size_t i=1; i < clusters.size(); i++){

      double t_min, t_max; // min and max for current cluster
      getMinMaxTime(clusters[i],t_min,t_max);

      // now find overlap
      if (t_max < t_min_common){
	overlap = false;
	break;
      }

      if (t_min > t_max_common){
	overlap = false;
	break;
      }

      if (t_min > t_min_common) t_min_common = t_min;
      if (t_max < t_max_common) t_max_common = t_max;

    }// for all clusters

    if (overlap = false) return -1;

    // calculate overlap

    double iou = (t_max_common - t_min_common) / (t_max_abs - t_min_abs);

    return iou;
  }
  
  
  //------------------------------
  void CFAlgoIoU::Report()
  //------------------------------
  {
  }
  
  
  void CFAlgoIoU::getMinMaxTime(const cluster::cluster_params* cluster, double& min, double& max)
  {
    
    auto const& hits = cluster->hit_vector;

    min = 9600;
    max = 0;

    for (auto const& hit : hits){

      if (hit.t > max) max = hit.t;
      if (hit.t < min) min = hit.t;

    }// fo rall hits
    
    return;
  }
  
    
}
#endif
