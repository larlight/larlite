#ifndef DQDXMODULE_CXX
#define DQDXMODULE_CXX

#include "dQdxModule.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryHelper.h"
#include "ClusterRecoUtil/Base/ClusterParams.h"

namespace showerreco{

  dQdxModule::dQdxModule()
  {
    _name = "dQdxModule";
  }

  void dQdxModule::initialize()
  {

    if (_tree) delete _tree;
    _tree = new TTree(_name.c_str(),"dQdx Info Tree");
    _tree->Branch("_n_hits",&_n_hits,"n_hits/I");
    _tree->Branch("_shrs_w",&_shrs_w,"shrs_w/D");
    _tree->Branch("_shrs_t",&_shrs_t,"shrs_t/D");
    _tree->Branch("_length",&_length,"length/D");
    _tree->Branch("_pl",&_pl,"pl/I");
    _tree->Branch("_pitch",&_pitch,"pitch/D");
    _tree->Branch("_dQ",&_dQ,"dQ/D");    
    _tree->Branch("_dQdx",&_dQdx,"dQdx/D");
    _tree->Branch("_dQdx_pitch",&_dQdx_pitch,"dQdx_p/D");
    
    
    _fC_to_e = 6250.; // a fC in units of the electron charge
    _ADC_to_mV = 0.5; // ADC -> mV conversion from gain measurements
    // to go from mV to fC the ASIC gain and Shaping time
    // must be considered
    // fC -> mV *= ( shaping time * ASIC gain )
    _shp_time  = 2.; // in usec
    _asic_gain = 7.8; // in mV/fC
    _charge_conversion = _ADC_to_mV * _fC_to_e / ( _shp_time * _asic_gain ) ;

    return;
  }

  void dQdxModule::do_reconstruction(const ShowerClusterSet_t & inputShowers, Shower_t & resultShower){
    

    auto geom = larutil::Geometry::GetME();
    auto geomHelper = larutil::GeometryHelper::GetME();
    
    // get the 3D direction reconstructed hopefully in a previous step
    auto const& dir3D = resultShower.fDCosStart;
    double dQ[3]={};
    double dx[3]={};
    double dx_p[3]={};
    double trunk_length[3]={};
    // loop over all input cluster -> calculate a dQdx per plane
    for (size_t n=0; n < inputShowers.size(); n++){
      
      // get the hits associated with this cluster
      auto const& hits = inputShowers.at(n).hit_vector;
      
      // get the plane associated with this cluster
      auto const& pl = inputShowers.at(n).plane_id.Plane;

      // get the start point for this cluster
      auto const& start = inputShowers.at(n).start_point;

      // get the showering point for this cluster
      auto const& shr_start = inputShowers.at(n).showering_point;

      // cluster open angle
      auto const& opena_cluster = inputShowers.at(n).opening_angle;

      // start direction
      auto const& start_dir = inputShowers.at(n).start_dir;

      //*** we need to calculate a pitch for this plane
      double pitch = geomHelper->GetPitch(dir3D,(int)pl);

      // BEGIN ALGORITHM DEVELOPMENT
      
      // STEP 1 : find list of hits that are along the trunk of
      // the shower. Find these hits by looking around the segment
      // connecting the start point and the beginning of the
      // showering point along the start_dir direction

       trunk_length[pl] = sqrt ( (shr_start.w - start.w) * (shr_start.w - start.w) + 
				   (shr_start.t - start.t) * (shr_start.t - start.t) );
      double hit_length;
      int n_hits = hits.size();
      double dQdx, dQdx_pitch =0;
      double factor;
      
      _n_hits =n_hits;
      _pl = pl;
      _pitch = pitch;
      factor = pitch/0.3;
      _length = trunk_length[pl];
      dx[pl]=trunk_length[pl];
      dx_p[pl]=(shr_start.w - start.w)*factor;//3D length
      _shrs_w=shr_start.w;
      _shrs_t=shr_start.t;
      
      dQ[pl]=0;
      for (int i=0;i<n_hits;i++){
	hit_length = sqrt((hits[i].w-start.w)*(hits[i].w-start.w)+
			  (hits[i].t-start.t)*(hits[i].t-start.t));
	dx[pl]=hit_length;

	if (hit_length<trunk_length[pl]){
	  double Q = hits[i].charge * _charge_conversion;
	    dQ[pl] += Q;
	}
	
      }
      dQdx=dQ[pl]/dx[pl];
      dQdx_pitch=dQ[pl]/dx_p[pl];
      
      _dQ   = dQ[pl];
      _dQdx = dQdx;
      _dQdx_pitch =dQdx_pitch;
      
      resultShower.fdQdx[pl] = dQdx_pitch;
      
      if (_verbose) std::cout<<"_pitch="<<_pitch<<"\n";
      //if (trunk_length>0)
      _tree->Fill();
      std::cout<<"plane:"<<pl<<","<<opena_cluster<<"\n";
    }
    
    return;
  }

  
} //showerreco

#endif
