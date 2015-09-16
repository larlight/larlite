#ifndef DQDX2DMODULE_CXX
#define DQDX2DMODULE_CXX

#include <iomanip>

#include "dQdx2DModule.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryHelper.h"

namespace showerreco{

  void dQdx2DModule::initialize()
  {

    std::cout << "creating tree" << std::endl;
    if (_tree) delete _tree;
    _tree = new TTree(_name.c_str(),"dQdx2D Info Tree");
    _tree->Branch("_dQdx0",&_dQdx0,"dQdx_Plane0/D");
    _tree->Branch("_dQdx1",&_dQdx1,"dQdx_Plane1/D");
    _tree->Branch("_dQdx2",&_dQdx2,"dQdx_Plane2/D");
    _tree->Branch("_Q0",&_Q0,"Q_Plane0/D");
    _tree->Branch("_Q1",&_Q1,"Q_Plane1/D");
    _tree->Branch("_Q2",&_Q2,"Q_Plane2/D");
    _tree->Branch("_StartW",&_StartW,"StartW/D");
    _tree->Branch("_StartT",&_StartT,"StartT/D");
    _tree->Branch("_ShowW",&_ShowW,"ShowW/D");
    _tree->Branch("_ShowT",&_ShowT,"ShowT/D");
    _tree->Branch("_length",&_length,"length/D");
    _tree->Branch("_BestdQdx",&_BestdQdx,"BestdQdx/D");
    std::cout << "tree created" << std::endl;

    return;
  }


  void dQdx2DModule::do_reconstruction(const ShowerClusterSet_t & inputShowers, Shower_t & resultShower){
    // This function takes the shower cluster set and computes the dQdx in the beginning of the shower
    // and then assigns it to the shower
    Len.clear();
    dQdx.resize(3);
    Len.resize(3);
    auto geomHelper = larutil::GeometryHelper::GetME();
    
    //iterate through the selected clusters
    for(auto const & cluster : inputShowers){

      //Which plane does the cluster live in
      auto const& pl = cluster.plane_id.Plane;

      // measure the distance between the start point and showering point      
      // Lots of failure modes to remove
      double dist = 0; 
      if(cluster.showering_point.w != 0 && cluster.showering_point.t != 0 &&
	 cluster.start_point.w != 0 && cluster.start_point.t != 0 && 
	 cluster.start_point.w != cluster.showering_point.w){
	dist = geomHelper->Get2DDistance(cluster.start_point, cluster.showering_point);      
      }
      else{
	//Major failure mode is ShoweringPoint is == 0, when that happens pull the "default" value
	dist = -999;
      }
      Len[pl] = dist;
 
      _StartW = cluster.start_point.w;
      _StartT = cluster.start_point.t;
      _ShowW = cluster.showering_point.w;
      _ShowT = cluster.showering_point.t;

      resultShower.fShoweringLength[pl] = dist;      
      _length = dist;

      // set the energy for this plane
      // Because GeoHelper needs these...will fix soon
      Hit2D strtHit;
      Hit2D avg;
      
      strtHit.w = cluster.start_point.w; 
      strtHit.t = cluster.start_point.t; 

      double slope;

      if(cluster.start_dir.at( 0) != 0){
	slope = cluster.start_dir.at(1)/cluster.start_dir.at(0);
      }
      else{
	slope = cluster.slope_2d;
      }


      // Calculate which hits are in the beginning of the shower (from start->showering point)
      // and fall within a rectangle with this distance as its length and the width is based
      // on the opening angle of the cluster with a slope as defined above
      // Possible improvement: Turn this into a triangle! 
      auto StartHits = geomHelper->SelectLocalPointList( cluster.hit_vector,
							 strtHit,
							 dist,
							 2*dist*TMath::Tan((cluster.opening_angle_charge_wgt)/2),
							 slope,
							 avg );      



      //Caculation of the Charge deposited at start of shower
      //UNSMOOTHED! Possible improvement 
      for(auto h : StartHits){
	dQdx.at(pl) += cluster.hit_vector.at(h).charge;	       
      }


      if(pl == 0)
	_Q0 = dQdx[0];
      
      if(pl == 1)
	_Q1 = dQdx[1];

      if(pl == 2)
	_Q2 = dQdx[2];

      //Calculate the dQdx!
      if(dist > 0){
	dQdx.at(pl) /= dist;
      }
      else{
	dQdx.at(pl) = kDOUBLE_MIN;
      }
      
      if(pl == 0)
	_dQdx0 = dQdx[0];
      
      if(pl == 1)
	_dQdx1 = dQdx[1];

      if(pl == 2)
	_dQdx2 = dQdx[2];

     

       resultShower.fdQdx = dQdx;
       _tree->Fill(); 
   
    }
    
    _BestdQdx = dQdx[std::distance(Len.begin(),std::max_element(Len.begin(),Len.end()))];
    resultShower.fBestdQdx = _BestdQdx;
    _tree->Fill();
       
    return;
    
  }


} //showerreco

#endif
