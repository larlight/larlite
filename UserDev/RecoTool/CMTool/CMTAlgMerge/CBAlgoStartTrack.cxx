#ifndef RECOTOOL_CBALGOSTARTTRACK_CXX
#define RECOTOOL_CBALGOSTARTTRACK_CXX

#include "CBAlgoStartTrack.h"

namespace cmtool {

  //-------------------------------------------------------
  CBAlgoStartTrack::CBAlgoStartTrack() : CBoolAlgoBase()
  //-------------------------------------------------------
  {
    
    SetMinHits(10);
    SetMinWidth(1.00000);
    SetMinOpeningAngle(0.15000);
    SetMinEP(0.99000);
    SetDebug(false);

  }

  //-----------------------------
  void CBAlgoStartTrack::Reset()
  //-----------------------------
  {

  }

  //------------------------------------------------------------------------------------------
  //void CBAlgoStartTrack::EventBegin(const std::vector<cluster::ClusterParamsAlg> &clusters)
  //------------------------------------------------------------------------------------------
  //{
  //
  //}

  //-------------------------------
  //void CBAlgoStartTrack::EventEnd()
  //-------------------------------
  //{
  //
  //}

  //-----------------------------------------------------------------------------------------------
  //void CBAlgoStartTrack::IterationBegin(const std::vector<cluster::ClusterParamsAlg> &clusters)
  //-----------------------------------------------------------------------------------------------
  //{
  //
  //}

  //------------------------------------
  //void CBAlgoStartTrack::IterationEnd()
  //------------------------------------
  //{
  //
  //}
  
  //----------------------------------------------------------------
  bool CBAlgoStartTrack::Bool(const ::cluster::ClusterParamsAlg &cluster1,
			       const ::cluster::ClusterParamsAlg &cluster2)
  //----------------------------------------------------------------
  {
    
    if(cluster1.GetHitVector().size() < _min_hits ||
       cluster2.GetHitVector().size() < _min_hits )
      return false;
    
    bool FirstTrackSecondBlob = false;
    bool SecondTrackFirstBlob = false;

    if(IsStartTrack(cluster1) && IsOverlappingBlob(cluster2))
      FirstTrackSecondBlob = true;
    if(IsStartTrack(cluster2) && IsOverlappingBlob(cluster1))
      SecondTrackFirstBlob = true;

    if(_debug){
      if(FirstTrackSecondBlob){
	std::cout<<"*********************************************"<<std::endl;
	std::cout<<"First track second blob! First has params:"<<std::endl;
	std::cout<<"Angle 2D = "<<cluster1.GetParams().angle_2d<<std::endl;
	std::cout<<"Opening Angle = "<<cluster1.GetParams().opening_angle<<std::endl;
	std::cout<<"Sum charge = "<<cluster1.GetParams().sum_charge<<std::endl;
	std::cout<<"Length = "<<cluster1.GetParams().length<<std::endl;
	std::cout<<"Width = "<<cluster1.GetParams().width<<std::endl;
	std::cout<<"N_Hits = "<<cluster1.GetHitVector().size()<<std::endl;
	std::cout<<"eigenvalue_principal = "<<cluster1.GetParams().eigenvalue_principal<<std::endl;
	std::cout<<"Second has params:"<<std::endl;
	std::cout<<"Angle 2D = "<<cluster2.GetParams().angle_2d<<std::endl;
	std::cout<<"Opening Angle = "<<cluster2.GetParams().opening_angle<<std::endl;
	std::cout<<"Sum charge = "<<cluster2.GetParams().sum_charge<<std::endl;
	std::cout<<"Length = "<<cluster2.GetParams().length<<std::endl;
	std::cout<<"Width = "<<cluster2.GetParams().width<<std::endl;
	std::cout<<"N_Hits = "<<cluster2.GetHitVector().size()<<std::endl;
	std::cout<<"eigenvalue_principal = "<<cluster2.GetParams().eigenvalue_principal<<std::endl;

      }
		 

    }

    //if cluster1 looks like a track and cluster2 looks like a blob
    if (FirstTrackSecondBlob)
      {
	std::pair<float,float> trackEndPoint = 
	  std::pair<float,float>( cluster1.GetParams().end_point.w, cluster1.GetParams().end_point.t );

	//if the tracklike cluster's endpoint is inside polygon of blob
	if(cluster2.GetParams().PolyObject.PointInside(trackEndPoint))
	  return true;
	else
	  return false;
      }

    //if cluster2 looks like a track and cluster1 looks like a blob
    else if (SecondTrackFirstBlob)
      {
	std::pair<float,float> trackEndPoint = 
	  std::pair<float,float>( cluster2.GetParams().end_point.w, cluster2.GetParams().end_point.t );

	//if the tracklike cluster's endpoint is inside polygon of blob
	if(cluster1.GetParams().PolyObject.PointInside(trackEndPoint))
	  return true;
	else
	  return false;
      }

    else
      return false;
       
    //the reason for not just using (one track one blob) and 
    //polyoverlap for this is that 
    //this was merging BNB events where the track is clearly in a different
    //direction than the blob
    
    //this will fail when the tracklike cluster is reconstructed backwards
    //(looks like it's a track going into a blob, rather than exiting)
  
    
  }

  //------------------------------
  void CBAlgoStartTrack::Report()
  //------------------------------
  {

  }
    

  bool CBAlgoStartTrack::IsStartTrack(const ::cluster::ClusterParamsAlg &cluster)
  {
    if(cluster.GetParams().eigenvalue_principal > _min_EP) 
      return true;
    else
      return false;
     
  }

  bool CBAlgoStartTrack::IsOverlappingBlob(const ::cluster::ClusterParamsAlg &cluster)
  {
    if(cluster.GetParams().width > _min_width && 
       cluster.GetParams().opening_angle > _min_opening_angle &&
       cluster.GetParams().eigenvalue_principal < _min_EP)
      return true;
    else
      return false;
  }
}
#endif
