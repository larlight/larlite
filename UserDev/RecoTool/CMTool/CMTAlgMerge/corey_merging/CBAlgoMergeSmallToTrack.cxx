#ifndef RECOTOOL_CBALGOMERGESMALLTOTRACK_CXX
#define RECOTOOL_CBALGOMERGESMALLTOTRACK_CXX

#include "CBAlgoMergeSmallToTrack.h"

namespace cmtool {

  CBAlgoMergeSmallToTrack::CBAlgoMergeSmallToTrack() : CBoolAlgoBase()
  {
    // Nothing to be done in the base class
    SetDebug(true);

    // Set parameters for track-like clusters
    SetMinHits(15);
    SetMinModHitDens(15);
    SetMinMHitWires(3.5);
    SetMaxWidth(5);
    SetMinLength(8);
        
    SetMinCharge(100);
    //"fPrincipal" is log(1-eigenvalue_principal)
    // >-7 means EP > 0.99908
    SetMinPrincipal(0.993);
    
    // Set parameters for small clusters
    SetMaxCharge(100);
    SetMaxLength(9);
    SetMaxWidth(9);
    SetMaxHit(16);

    // Set merging criteria
    SetMaxClosestDist(2.5);

  }


  
  bool CBAlgoMergeSmallToTrack::Bool(
      const ::cluster::ClusterParamsAlg &cluster1,
      const ::cluster::ClusterParamsAlg &cluster2)
  {

    // Figure out which cluster is the track and which is the small
    // If both are track or both are small, return false

    // return true;

    const ::cluster::ClusterParamsAlg * track;
    const ::cluster::ClusterParamsAlg * small;

    if (isTrack(cluster1)){
      if (isTrack(cluster2)){
        // both are tracks, bail
        return false;
      }
      track = &cluster1;
    }
    else if (isTrack(cluster2)){
      track =  &cluster2;
    }
    // else neither are tracks, bail
    else return false;

    // Figure out which is small
    if (isSmall(cluster1)){
      if (isSmall(cluster2)){
        // both are small, bail
        return false;
      }
      // verify that track and small aren't the same
      if (track == &cluster1) return false;
      small = &cluster1;
    }
    else if (isSmall(cluster2)){
      if (track == &cluster2) return false;
      small =  &cluster2;
    }
    // else neither are small, bail
    else return false;

    // Now make the final decision based on distances:
    // Must have the closest approach be less than the max
    // Must have the distance to start and end be greater than the min

    if (closestApproach(*track, *small) < _max_closest_dist){
      // In this space, fill in the computation 
      // to get the distance to the start and end points

      if(_debug) std::cout<<"Merged"<<std::endl;
      return true;
    }

    return false;
  }

/*---------------------------------------------------------------------*/
// Determine is a cluster is a track based on some parameters.

  bool CBAlgoMergeSmallToTrack::isTrack(
    const ::cluster::ClusterParamsAlg &cluster)
  {

      size_t N_Hits = cluster.GetHitVector().size();
      auto start_point = cluster.GetParams().start_point;
      // double angle_2d = cluster.GetParams().angle_2d;
      // double opening_angle = cluster.GetParams().opening_angle;
      Polygon2D PolyObject = cluster.GetParams().PolyObject;
      double length = cluster.GetParams().length;
      double width = cluster.GetParams().width;
      double plane = cluster.GetParams().start_point.plane;
      double start_x = cluster.GetParams().start_point.w;
      double start_y = cluster.GetParams().start_point.t;
      double end_x = cluster.GetParams().end_point.w;
      double end_y = cluster.GetParams().end_point.t;
      double ep = cluster.GetParams().eigenvalue_principal;
      double hit_density = cluster.GetParams().modified_hit_density;
      double mhit_wire = cluster.GetParams().multi_hit_wires;

      if( ((N_Hits > _min_hits) &&
          (length > _min_length)  &&
          (ep > _min_principal))  ||
          (ep > (_min_principal+0.006)) ){

          if(_debug){

            std::cout<<"------------------------------------------------------------------------------"<<std::endl;
            std::cout<<"cluster plane: "<<plane<<"  cluster start: ("<<start_x<<","<<start_y<<")"<<
            " cluster end: ("<<end_x<<","<<end_y<<")"<<std::endl;
            std::cout<<"N_Hits: "<<N_Hits<<"  min_hits: "<<_min_hits<<std::endl;
            std::cout<<"EP: "<<ep<<"  min_ep: "<<_min_principal<<std::endl;
            // std::cout<<"width: "<<width<<"  max_width: "<<_max_width<<std::endl;
            std::cout<<"length: "<<length<<"  min_length: "<<_min_length<<std::endl;
            std::cout<<"density: "<<hit_density<<" mod_hit_density: "<<_min_mod_hit_dens<<std::endl;
            // std::cout<<"Multi_hit wire: "<<mhit_wire<<"  min_multihit_wires: "<<_min_multihit_wires<<std::endl;
            std::cout<<"This is a track!"<<std::endl;

          }
          return true;
      }

      if (_debug){

        std::cout<<"------------------------------------------------------------------------------"<<std::endl;
        std::cout<<"cluster plane: "<<plane<<"  cluster start: ("<<start_x<<","<<start_y<<")"<<
        " cluster end: ("<<end_x<<","<<end_y<<")"<<std::endl;
        std::cout<<"N_Hits: "<<N_Hits<<"  min_hits: "<<_min_hits<<std::endl;
        std::cout<<"EP: "<<ep<<"  min_ep: "<<_min_principal<<std::endl;
        // std::cout<<"width: "<<width<<"  max_width: "<<_max_width<<std::endl;
        std::cout<<"length: "<<length<<"  min_length: "<<_min_length<<std::endl;
        std::cout<<"density: "<<hit_density<<" mod_hit_density: "<<_min_mod_hit_dens<<std::endl;
        // std::cout<<"Multi_hit wire: "<<mhit_wire<<"  min_multihit_wires: "<<_min_multihit_wires<<std::endl;
        std::cout<<"This is not a track"<<std::endl;
      }

    return false;
  }

/*---------------------------------------------------------------------*/
// Determine if a cluster is small enough

  bool CBAlgoMergeSmallToTrack::isSmall(
    const ::cluster::ClusterParamsAlg &cluster)
  {

      size_t N_Hits = cluster.GetHitVector().size();
      auto start_point = cluster.GetParams().start_point;
      // double angle_2d = cluster.GetParams().angle_2d;
      // double opening_angle = cluster.GetParams().opening_angle;
      Polygon2D PolyObject = cluster.GetParams().PolyObject;
      double length = cluster.GetParams().length;
      double width = cluster.GetParams().width;
      // double charge = cluster.GetParams().sum_charge;
      double plane = cluster.GetParams().start_point.plane;
      double start_x = cluster.GetParams().start_point.w;
      double start_y = cluster.GetParams().start_point.t;
      double end_x = cluster.GetParams().end_point.w;
      double end_y = cluster.GetParams().end_point.t;
      double hit_density = cluster.GetParams().modified_hit_density;


      if( (N_Hits < _max_hits) &&
          (hit_density < _min_mod_hit_dens) &&
          (length < _max_length)        ){

          if(_debug){

            std::cout<<"------------------------------------------------------------------------------"<<std::endl;
            std::cout<<"cluster plane: "<<plane<<"  cluster start: ("<<start_x<<","<<start_y<<")"<<
            " cluster end: ("<<end_x<<","<<end_y<<")"<<std::endl;
            std::cout<<"N_Hits: "<<N_Hits<<"  max_hits: "<<_max_hits<<std::endl;
            // std::cout<<"width: "<<width<<"  max_width: "<<_max_width<<std::endl;
            std::cout<<"length: "<<length<<"  max_length: "<<_max_length<<std::endl;
            // std::cout<<"charge: "<<length<<"  max_charge: "<<_max_charge<<std::endl;
            std::cout<<"density: "<<hit_density<<" mod_hit_density: "<<_min_mod_hit_dens<<std::endl;
            std::cout<<"This is a small!"<<std::endl;

          }

            return true;
      }

      if (_debug){

        std::cout<<"------------------------------------------------------------------------------"<<std::endl;
        std::cout<<"cluster plane: "<<plane<<"  cluster start: ("<<start_x<<","<<start_y<<")"<<
        " cluster end: ("<<end_x<<","<<end_y<<")"<<std::endl;  
        std::cout<<"N_Hits: "<<N_Hits<<"  max_hits: "<<_max_hits<<std::endl;
        // std::cout<<"width: "<<width<<"  max_width: "<<_max_width<<std::endl;
        std::cout<<"length: "<<length<<"  max_length: "<<_max_length<<std::endl;
        // std::cout<<"charge: "<<charge<<"  max_charge: "<<_max_charge<<std::endl;
        std::cout<<"density: "<<hit_density<<" mod_hit_density: "<<_min_mod_hit_dens<<std::endl;
        std::cout<<"This is not a small"<<std::endl;
      }

    return false;
  }

  float CBAlgoMergeSmallToTrack::closestApproach(
    const ::cluster::ClusterParamsAlg &cluster1,
    const ::cluster::ClusterParamsAlg &cluster2)
  {

      unsigned int npoints1 = cluster1.GetParams().PolyObject.Size();
      unsigned int npoints2 = cluster2.GetParams().PolyObject.Size();
      float bound = 10000;

      //loop over points on first polygon
      for(unsigned int i = 0; i < npoints1; ++i){

        float pt1w = cluster1.GetParams().PolyObject.Point(i).first;
        float pt1t = cluster1.GetParams().PolyObject.Point(i).second;
        
        //loop over points on second polygon
        for(unsigned int j = 0; j < npoints2; ++j){

          float pt2w = cluster2.GetParams().PolyObject.Point(j).first;
          float pt2t = cluster2.GetParams().PolyObject.Point(j).second;
          float dist = pow((pow(pt2w-pt1w,2)+pow(pt2t-pt1t,2)),0.5);
          
          if(_debug){
              std::cout<<"#################################"<<std::endl;
              std::cout<<"Distance: "<<dist<<"  max_dist: "<<_max_closest_dist<<std::endl;
          }

          if(dist<bound) bound = dist;
        }
      }

      return bound;
    }
}

#endif
