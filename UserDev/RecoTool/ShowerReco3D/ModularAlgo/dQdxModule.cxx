#ifndef DQDXMODULE_CXX
#define DQDXMODULE_CXX

#include "dQdxModule.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryHelper.h"
#include "ClusterRecoUtil/Base/ClusterParams.h"
#include "ClusterRecoUtil/Base/Polygon2D.h"
#include "math.h"

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
    _tree->Branch("_n_hits_radius",&_n_hits_radius,"n_hits_radius/I");
    _tree->Branch("_n_hits_poly",&_n_hits_poly,"n_hits_poly/I");
    _tree->Branch("_shrs_w",&_shrs_w,"shrs_w/D");
    _tree->Branch("_shrs_t",&_shrs_t,"shrs_t/D");
    _tree->Branch("_length",&_length,"length/D");
    _tree->Branch("_pl",&_pl,"pl/I");
    _tree->Branch("_pitch",&_pitch,"pitch/D");
    _tree->Branch("_dQ",&_dQ,"dQ/D");    
    _tree->Branch("_dQdx",&_dQdx,"dQdx/D");
    _tree->Branch("_dQdx_pitch",&_dQdx_pitch,"dQdx_p/D");
    _tree->Branch("_dQdx_pitch_pl2",&_dQdx_pitch_pl2,"dQdx_p_pl2/D");
    _tree->Branch("_rms",&_rms,"rms/D");
    _tree->Branch("_pl_best",&_pl_best,"pl_best/I");
    
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
    double dQdx[3]={};
    double dQdx_pitch[3] ={};
    int n_hits[3]={};
    int n_hits_radius[3] = {};
    int n_hits_poly[3]={};
    double rms[3];//charge rms
    
    //// loop over all input cluster -> calculate a dQdx per plane
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
      auto const& clu_an = inputShowers.at(n).opening_angle;
      
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
       n_hits[pl] = hits.size();       
       double factor;
             
       //_n_hits =n_hits[pl];//hits in cluster
       
       _pl = pl;//plane ID
       _pitch = pitch;//pitch
       factor = pitch/0.3;//pitch correction factor
       //_length = trunk_length[pl];//trunk length
       dx[pl]=trunk_length[pl];//trunk length
       dx_p[pl]=std::abs((shr_start.w - start.w)*factor);//3D length abs w/ pitch correction
       _length=dx_p[pl];
       _shrs_w=shr_start.w;
       _shrs_t=shr_start.t;
       double sum = 0; ///sum of charge square
       
       dQ[pl]=0;
       //std::cout<<n_hits<<std::endl;
       for (int i=0;i<n_hits[pl];i++){
	 hit_length = sqrt((hits[i].w-start.w)*(hits[i].w-start.w)+
			   (hits[i].t-start.t)*(hits[i].t-start.t));
	 dx[pl]=hit_length;
	 //Radius limit
	 if (hit_length<trunk_length[pl]){
	   n_hits_radius[pl]++;
	   //Interior_angle should be smaller thaner half of cluster open angle
	   std::vector<std::pair<float,float>> p_hit;//define triangle
	   p_hit.resize(3);
	   p_hit.at(0).first=start.w;
	   p_hit.at(0).second=start.t;
	   p_hit.at(1).first=shr_start.w;
	   p_hit.at(1).second=shr_start.t;
	   p_hit.at(2).first=hits[i].w;
	   p_hit.at(2).second=hits[i].t;	  
	   //remove hits at clu/shr start points
	   float check_l1,check_l2;
	   check_l1=sqrt((p_hit.at(2).second-p_hit.at(0).second)*(p_hit.at(2).second-p_hit.at(0).second)+
			 (p_hit.at(2).first-p_hit.at(0).first)*(p_hit.at(2).first-p_hit.at(0).first));
	   check_l2=sqrt((p_hit.at(2).second-p_hit.at(1).second)*(p_hit.at(2).second-p_hit.at(1).second)+
			 (p_hit.at(2).first-p_hit.at(1).first)*(p_hit.at(2).first-p_hit.at(1).first));
	   
	   //Get the Triangle
	   Polygon2D poly(p_hit);
	   
	   if(check_l1*check_l2>0){float in_an=poly.InteriorAngle(0);///retrive interior angle      
	     //std::cout<<"in_an"<<in_an<<"\n";
	     //std::cout<<"cluster_an"<<poly.Size()<<"\n";
	     if(tan(in_an)<=tan(clu_an)){
	       double Q = hits[i].charge * _charge_conversion;
	       sum += Q*Q;
	       dQ[pl] += Q;
	       n_hits_poly[pl]++;
	     }
	   }	
	 }
       }
       rms[pl]=sqrt(sum/n_hits_poly[pl]);
       //dQdx
       dQdx[pl]=dQ[pl]/n_hits_poly[pl];///dx[pl];
       
       //ddQdx w/ pitch correction
       dQdx_pitch[pl]=dQ[pl]/pitch/n_hits_poly[pl];///dx_p[pl];
       //find best plane w/ longest trunk length
       
       
       if(pl==2){
	 int pl_best;
	 
	 if(trunk_length[pl]>trunk_length[pl-1])pl_best=pl;
	 else pl_best = pl-1;
	 
	 if(trunk_length[pl_best]<trunk_length[pl-2])pl_best = pl-2;
	 _pl_best =pl_best;//tree viriable
	 
	 resultShower.fBestdQdxPlane = pl_best;///best plane go to showerquality
       }      
       
       ///Best Plane
       _dQ   = dQ[_pl_best];
       _dQdx = dQdx[_pl_best];
       _dQdx_pitch =dQdx_pitch[_pl_best];
       _dQdx_pitch_pl2= dQdx_pitch[2];
       _rms = rms[_pl_best];
       _n_hits =n_hits[_pl_best];
       _n_hits_radius = n_hits_radius[_pl_best];
       _n_hits_poly= n_hits_poly[_pl_best];
       
       resultShower.fdQdx[pl] = dQdx_pitch[_pl_best];///dQdx go to showerquality      
       if(_verbose)std::cout<<"#########################################################\n";
       
       if(pl==2){///for best plane
	 //std::cout<<_dQdx;
	 _tree->Fill();
       }
       
       ///test------------------------------------------------------------------------
       //      if (_verbose) 
       //play polygon
       
       
    }
    return;
  }
} //showerreco

#endif
