#ifndef RECOTOOL_CFALGOIOU_CXX
#define RECOTOOL_CFALGOIOU_CXX

#include "CFAlgoIoU.h"

namespace cmtool {

  //-------------------------------------------------------
  CFAlgoIoU::CFAlgoIoU() : CFloatAlgoBase()
  //-------------------------------------------------------
  {
    _iou_min = 0.5;
  }

  //-----------------------------
  void CFAlgoIoU::Reset()
  //-----------------------------
  {

  }

  //----------------------------------------------------------------------------------------------
  float CFAlgoIoU::Float(const std::vector<const cluster::Cluster*> &clusters)
  //----------------------------------------------------------------------------------------------
  {


    if (_verbose)
      std::cout << "Matching " << clusters.size() << " clusters" << std::endl;
    
    // if 3 clusters -> skip
    if (clusters.size() != 2) return -1;

    // require collection plane
    if ( (clusters[0]->_plane != 2) && (clusters[1]->_plane != 2) ) return -1;

    double t_min_abs = 9600; // smallest start point of the 3
    double t_max_abs = 0;    // largest start point of the three

    
    for(auto const& c : clusters){

      double min,max;

      getMinMaxTime(c,min,max);

      if (_verbose) {
	std::cout << "cluster bounds :  [ " << min << ", " << max << "]" 
		  << " cluster size   : " << c->size() 
		  << std::endl;
      }

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

      if (_verbose)
	std::cout << "PLANES " << clusters[0]->_plane
		  << " AND " << clusters[i]->_plane
		  << std::endl;

      double t_min, t_max; // min and max for current cluster
      getMinMaxTime(clusters[i],t_min,t_max);

      if (_verbose) {
	std::cout << "Current t_min,t_max    = [" <<  t_min_common << ", " << t_max_common << " ]"<< std::endl
		  << "Current cluster bounds = [" <<  t_min        << ", " << t_max        << " ]"<< std::endl;
	  }
      
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
      
      if (_verbose) {
	std::cout << "updated t_min,t_max    = [" <<  t_min_common << ", " << t_max_common << " ]"<< std::endl;
      }
      
    }// for all clusters
    
    if (overlap == false) return -1;
    
    if (_verbose) {
      std::cout << "T common interval : [" <<  t_min_common << ", " << t_max_common << " ]"<< std::endl
		<< "T total  interval : [" <<  t_min_abs    << ", " << t_max_abs    << " ]"<< std::endl;
    }	
    
    // calculate overlap
    double iou = (t_max_common - t_min_common) / (t_max_abs - t_min_abs);

    if (_verbose)
      std::cout << "Cluster IoU : " << iou
		<< std::endl << std::endl;

    if (iou < _iou_min) return -1;

    return iou;
  }
  
  
  //------------------------------
  void CFAlgoIoU::Report()
  //------------------------------
  {
  }
  
  
  void CFAlgoIoU::getMinMaxTime(const cluster::Cluster* cluster, double& min, double& max)
  {
    
    auto const& hits = cluster->GetHits();

    min = 9600;
    max = 0;

    for (auto const& hit : hits){

      if (hit._t > max) max = hit._t;
      if (hit._t < min) min = hit._t;

    }// fo rall hits
    
    return;
  }
  
    
}
#endif
