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
    _tree->Branch("_dQdx0_smooth",&_dQdx0_smooth,"dQdx_Plane0_smooth/D");
    _tree->Branch("_dQdx1_smooth",&_dQdx1_smooth,"dQdx_Plane1_smooth/D");
    _tree->Branch("_dQdx2_smooth",&_dQdx2_smooth,"dQdx_Plane2_smooth/D");
    _tree->Branch("_BestdQdx_smooth",&_BestdQdx_smooth,"BestdQdx_smooth/D");    
    _tree->Branch("_mean_dQdx",&_mean_dQdx,"mean_dQdx/D");
    _tree->Branch("_rms_dQdx",&_rms_dQdx,"rms_dQdx/D");
    _tree->Branch("_Nhits",&_Nhits,"Nhits/I");
    _tree->Branch("_Nhits_smooth",&_Nhits_smooth,"Nhits_smooth/I");
    _tree->Branch("_pitch",&_pitch,"pitch/D");
    //    _tree->Branch("HitdeltaQdeltax_v0",&HitdeltaQdeltax_v[0]);
    // _tree->Branch("HitdeltaQdeltax_v1",&HitdeltaQdeltax_v[1]);
    // _tree->Branch("HitdeltaQdeltax_v2",&HitdeltaQdeltax_v[2]);
    
    std::cout << "tree created" << std::endl;

    return;
  }


  void dQdx2DModule::do_reconstruction(const ShowerClusterSet_t & inputShowers, Shower_t & resultShower){
    // This function takes the shower cluster set and computes the dQdx in the beginning of the shower
    // and then assigns it to the shower
    Len.clear();
    dQdx.clear();
    dQdx_smooth.clear();
    HitDist_toStart.clear();
    HitCharge.clear();
    IndexSort.clear();
    HitdeltaQdeltax_v.clear();

    dQdx.resize(3);
    dQdx_smooth.resize(3);
    Len.resize(3);
    HitdeltaQdeltax_v.resize(3);

    auto geomHelper = larutil::GeometryHelper::GetME();

    //Grab 3D direction already in the shower product
    auto const& dir3D = resultShower.fDCosStart;
    
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
	dist = 2.4;
      }

      Len[pl] = dist;
      
      double pitch = (geomHelper->GetPitch(dir3D,pl));
      _pitch = pitch;
      pitch /= 0.3;
      
      if(pitch == 0){
	std::cout << "no pitch" << std::endl; 
	pitch = 1;
      }
      //Setting correct 3D length
      Len[pl] /= pitch;

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
      //with SMOOTHING!
      double _rms_dQdx_temp = 0;
      _mean_dQdx = 0;
      _rms_dQdx_temp = 0;
      _Nhits = 0;
      _Nhits_smooth = 0;

      for(auto h : StartHits){
	dQdx.at(pl) += cluster.hit_vector.at(h).charge;	       
	_Nhits++;
	_mean_dQdx += cluster.hit_vector.at(h).charge;
	_rms_dQdx_temp += cluster.hit_vector.at(h).charge*cluster.hit_vector.at(h).charge;
	HitDist_toStart.push_back(geomHelper->Get2DDistance(cluster.start_point,cluster.hit_vector.at(h)));
	HitCharge.push_back(cluster.hit_vector.at(h).charge);
      }
      
      if(HitDist_toStart.size() != 0){
	IndexSort.resize(HitDist_toStart.size());
	int n = 0;

	std::generate(IndexSort.begin(),  IndexSort.end(), [&]{ return n++; });
	std::sort(IndexSort.begin(),IndexSort.end(),[&](int i1, int i2) {return HitDist_toStart[i1] < HitDist_toStart[i2];});

	HitdeltaQdeltax_v[pl].resize(HitDist_toStart.size()-1);      

	for(int i = 0; i < HitDist_toStart.size()-1; i++){

	  if( (HitDist_toStart.at(IndexSort[i]) - HitDist_toStart.at(IndexSort[i+1])) != 0){
	    HitdeltaQdeltax_v[pl].push_back( 
					    (HitCharge.at(IndexSort[i]) - HitCharge.at(IndexSort[i+1]))/
					    (HitDist_toStart.at(IndexSort[i]) - HitDist_toStart.at(IndexSort[i+1])));
	    
	    //    std::cout << HitdeltaQdeltax_v[pl].back() << std::endl;
	  }
	}
      }

      _mean_dQdx /= StartHits.size();
      _rms_dQdx_temp /= StartHits.size();
      _rms_dQdx = sqrt(_rms_dQdx_temp);

      double sigma = sqrt(_rms_dQdx*_rms_dQdx-_mean_dQdx*_mean_dQdx);

      if(StartHits.size() < 5)
	sigma = 0;

      for(auto h : StartHits){
	if(cluster.hit_vector.at(h).charge < (_mean_dQdx + sigma) &&
	   cluster.hit_vector.at(h).charge > (_mean_dQdx - sigma) ){	  
	  dQdx_smooth.at(pl) += cluster.hit_vector.at(h).charge;
	  _Nhits_smooth++;
	}
      }
      if(_Nhits_smooth < 5){
	dQdx_smooth.at(pl) = dQdx.at(pl);
	_Nhits_smooth = _Nhits;
      }

      if(pl == 0)
	_Q0 = dQdx[0];

      if(pl == 1)
	_Q1 = dQdx[1];

      if(pl == 2)
	_Q2 = dQdx[2];

      //Calculate the dQdx!
      if(dist > 0){
	dQdx.at(pl) /= Len[pl];
	dQdx_smooth.at(pl) /= Len[pl];
      }
      else{
	dQdx.at(pl) = 0;
	dQdx_smooth.at(pl) = 0;
      }
      
      if(pl == 0)
	{
	_dQdx0 = dQdx[0];
	_dQdx0_smooth = dQdx_smooth[0];
	}

      if(pl == 1)
	{
	_dQdx1 = dQdx[1];
	_dQdx1_smooth = dQdx_smooth[1];
	}

      if(pl == 2)
	{
	_dQdx2 = dQdx[2];
	_dQdx2_smooth = dQdx_smooth[2];
	}
     

       resultShower.fdQdx = dQdx;
       _tree->Fill(); 
   
    }
    
    _BestdQdx = dQdx[std::distance(Len.begin(),std::max_element(Len.begin(),Len.end()))];
    _BestdQdx_smooth = dQdx_smooth[std::distance(Len.begin(),std::max_element(Len.begin(),Len.end()))];

    /// Trying to guard against selecting a bad plane, this does a second pass just to make sure
    /// we are using the best value. 
    if(_BestdQdx == 0){
      std::vector< double> Len2;
      for( int L = 0; L < Len.size(); L++){
	if(Len.at(L) != *std::max_element(Len.begin(),Len.end()))
	  Len2.push_back(Len.at(L));	
      }
      _BestdQdx = dQdx[std::distance(Len2.begin(),std::max_element(Len2.begin(),Len2.end()))];
    }

    if(_BestdQdx_smooth == 0){
      std::vector< double> Len2;
      for( int L = 0; L < Len.size(); L++){
	if(Len.at(L) != *std::max_element(Len.begin(),Len.end()))
	  Len2.push_back(Len.at(L));	
      }
      _BestdQdx_smooth = dQdx_smooth[std::distance(Len2.begin(),std::max_element(Len2.begin(),Len2.end()))];
    }


    resultShower.fBestdQdx = _BestdQdx;
    _tree->Fill();
       
    return;
    
  }


} //showerreco

#endif
