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
    std::cout << "tree created" << std::endl;

    return;
  }


  void dQdx2DModule::do_reconstruction(const ShowerClusterSet_t & inputShowers, Shower_t & resultShower){
    // This function takes the shower cluster set and computes the dQdx in the beginning of the shower
    // and then assigns it to the shower
    
    dQdx.resize(3);

    auto geomHelper = larutil::GeometryHelper::GetME();
    
    //iterate through the selected clusters
    for(auto const & cluster : inputShowers){

      //Which plane does the cluster live in
      auto const& pl = cluster.plane_id.Plane;

      //      std::cout << " \t \t \t Cluster in Plane " << pl << std::endl;
      
      // measure the distance between the start point and showering point
      
      double dist; 
      if(cluster.showering_point.w !=0 && cluster.showering_point.t !=0){
	dist = geomHelper->Get2DDistance(cluster.start_point, cluster.showering_point);      
      }
      else{dist = 2.4;}
 
      _StartW = cluster.start_point.w;
      _StartT = cluster.start_point.t;
      _ShowW = cluster.showering_point.w;
      _ShowT = cluster.showering_point.t;

      //      std::cout << "Distance : " << dist << std::endl; 

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

      auto StartHits = geomHelper->SelectLocalPointList( cluster.hit_vector,
							 strtHit,
							 dist,
							 2*dist*TMath::Tan((cluster.opening_angle_charge_wgt)/2),
							 slope,
							 avg );      
      
      for(auto h : StartHits){
	dQdx.at(pl) += cluster.hit_vector.at(h).charge;	       
      }

      //      std::cout << "Charge " << dQdx.at(pl) << std::endl;

      if(pl == 0)
	_Q0 = dQdx[0];
      
      if(pl == 1)
	_Q1 = dQdx[1];

      if(pl == 2)
	_Q2 = dQdx[2];


      if(dist != 0){
	dQdx.at(pl) /= dist;
      }
      else{
	std::cout << " How your dist = 0 foo! " << std::endl;
      }
      
      //      std::cout << " dQdx !!!! " << dQdx[pl] << std::endl;

      if(pl == 0)
	_dQdx0 = dQdx[0];
      
      if(pl == 1)
	_dQdx1 = dQdx[1];

      if(pl == 2)
	_dQdx2 = dQdx[2];

       _tree->Fill();

       resultShower.fdQdx = dQdx;

    }
    
    
    
    return;
    
  }


} //showerreco

#endif
