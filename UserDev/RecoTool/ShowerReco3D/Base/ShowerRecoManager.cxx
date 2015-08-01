#ifndef SHOWERRECOMANAGER_CXX
#define SHOWERRECOMANAGER_CXX

#include "ShowerRecoManager.h"

namespace showerreco {

  ShowerRecoManager::ShowerRecoManager()
    : fShowerAlgo(nullptr)
    , fMatchMgr(nullptr)
    , _clusterTree(nullptr)
  {
    fMatch    = true;
    auto geom = ::larutil::Geometry::GetME();
    fMatchMgr = new ::cmtool::CMatchManager(geom->Nplanes());
    PrepareClusterTree();
    // default -> do not save cluster tree
    _saveClusterTree = false;
  }

  void ShowerRecoManager::Reset()
  {
    if(fShowerAlgo) fShowerAlgo->Reset();
    fMatchMgr->Reset();
  }

  ClusterAss_t ShowerRecoManager::Reconstruct (const std::vector<std::vector<larutil::PxHit> >& clusters,
					       std::vector< ::larlite::shower>& showers)
  {
    showers.clear();
    fMatchMgr->SetClusters(clusters);

    ClusterAss_t res_ass;
    // Run matching & retrieve matched cluster indices
    try{
      fMatchMgr->Process();
    }catch( ::cmtool::CMTException &e){
      e.what();
      return res_ass;
    }
    res_ass = fMatchMgr->GetBookKeeper().GetResult();
  
    Process(res_ass, showers);

    return res_ass;
  }
  
  void ShowerRecoManager::Reconstruct (const std::vector<std::vector<larutil::PxHit> >& clusters,
				       const ClusterAss_t& ass,
				       std::vector< ::larlite::shower>& showers)
  {
    showers.clear();
    fMatchMgr->SetClusters(clusters);

    Process(ass,showers);
  }
  
  void ShowerRecoManager::Process(const ClusterAss_t& ass,
				  std::vector< ::larlite::shower>& showers)
  {
    
    for(auto const& pair : ass) {
      
      std::vector< ::cluster::ClusterParamsAlg> cpans;
      
      cpans.reserve(pair.size());
      
      for(auto const& index : pair)

	cpans.push_back(fMatchMgr->GetInputClusters()[index]);

      fShowerAlgo->AppendInputClusters(cpans);
      if (_saveClusterTree)
	FillClusterTree(cpans);
    }

    // Run shower reco
    showers = fShowerAlgo->Reconstruct();
  }

  void ShowerRecoManager::PrepareClusterTree(){

    if (_clusterTree) { delete _clusterTree; }
    _clusterTree = new TTree("_clusterTree","Cluster Tree");

    _clusterTree->Branch("_polyA_min",&_polyA_min,"polyA_min/D");
    _clusterTree->Branch("_polyA_mid",&_polyA_mid,"polyA_mid/D");
    _clusterTree->Branch("_polyA_max",&_polyA_max,"polyA_max/D");
    _clusterTree->Branch("_polyP_min",&_polyP_min,"polyP_min/D");
    _clusterTree->Branch("_polyP_mid",&_polyP_mid,"polyP_mid/D");
    _clusterTree->Branch("_polyP_max",&_polyP_max,"polyP_max/D");
    _clusterTree->Branch("_sumQ_min",&_sumQ_min,"sumQ_min/D");
    _clusterTree->Branch("_sumQ_mid",&_sumQ_mid,"sumQ_mid/D");
    _clusterTree->Branch("_sumQ_max",&_sumQ_max,"sumQ_max/D");
    _clusterTree->Branch("_avgQ_min",&_avgQ_min,"avgQ_min/D");
    _clusterTree->Branch("_avgQ_mid",&_avgQ_mid,"avgQ_mid/D");
    _clusterTree->Branch("_avgQ_max",&_avgQ_max,"avgQ_max/D");
    _clusterTree->Branch("_angle2d_min",&_angle2d_min,"angle2d_min/D");
    _clusterTree->Branch("_angle2d_mid",&_angle2d_mid,"angle2d_mid/D");
    _clusterTree->Branch("_angle2d_max",&_angle2d_max,"angle2d_max/D");
    _clusterTree->Branch("_openingAngle_min",&_openingAngle_min,"openingAngle_min/D");
    _clusterTree->Branch("_openingAngle_mid",&_openingAngle_mid,"openingAngle_mid/D");
    _clusterTree->Branch("_openingAngle_max",&_openingAngle_max,"openingAngle_max/D");
    _clusterTree->Branch("_closingAngle_min",&_closingAngle_min,"closingAngle_min/D");
    _clusterTree->Branch("_closingAngle_mid",&_closingAngle_mid,"closingAngle_mid/D");
    _clusterTree->Branch("_closingAngle_max",&_closingAngle_max,"closingAngle_max/D");
    _clusterTree->Branch("_eigenvalue_principal_min",&_eigenvalue_principal_min,"eigenvalue_principal_min/D");
    _clusterTree->Branch("_eigenvalue_principal_mid",&_eigenvalue_principal_mid,"eigenvalue_principal_mid/D");
    _clusterTree->Branch("_eigenvalue_principal_max",&_eigenvalue_principal_max,"eigenvalue_principal_max/D");
    _clusterTree->Branch("_eigenvalue_secondary_min",&_eigenvalue_secondary_min,"eigenvalue_secondary_min/D");
    _clusterTree->Branch("_eigenvalue_secondary_mid",&_eigenvalue_secondary_mid,"eigenvalue_secondary_mid/D");
    _clusterTree->Branch("_eigenvalue_secondary_max",&_eigenvalue_secondary_max,"eigenvalue_secondary_max/D");
    _clusterTree->Branch("_length_min",&_length_min,"length_min/D");
    _clusterTree->Branch("_length_mid",&_length_mid,"length_mid/D");
    _clusterTree->Branch("_length_max",&_length_max,"length_max/D");
    _clusterTree->Branch("_width_min",&_width_min,"width_min/D");
    _clusterTree->Branch("_width_mid",&_width_mid,"width_mid/D");
    _clusterTree->Branch("_width_max",&_width_max,"width_max/D");
    _clusterTree->Branch("_hitD1D_min",&_hitD1D_min,"hitD1D_min/D");
    _clusterTree->Branch("_hitD1D_mid",&_hitD1D_mid,"hitD1D_mid/D");
    _clusterTree->Branch("_hitD1D_max",&_hitD1D_max,"hitD1D_max/D");
    _clusterTree->Branch("_hitD2D_min",&_hitD2D_min,"hitD2D_min/D");
    _clusterTree->Branch("_hitD2D_mid",&_hitD2D_mid,"hitD2D_mid/D");
    _clusterTree->Branch("_hitD2D_max",&_hitD2D_max,"hitD2D_max/D");
    _clusterTree->Branch("_modmeancharge_min",&_modmeancharge_min,"modmeancharge_min/D");
    _clusterTree->Branch("_modmeancharge_mid",&_modmeancharge_mid,"modmeancharge_mid/D");
    _clusterTree->Branch("_modmeancharge_max",&_modmeancharge_max,"modmeancharge_max/D");
    _clusterTree->Branch("_RMS_charge_min",&_RMS_charge_min,"RMS_charge_min/D");
    _clusterTree->Branch("_RMS_charge_mid",&_RMS_charge_mid,"RMS_charge_mid/D");
    _clusterTree->Branch("_RMS_charge_max",&_RMS_charge_max,"RMS_charge_max/D");
    _clusterTree->Branch("_direction_min",&_direction_min,"direction_min/D");
    _clusterTree->Branch("_direction_mid",&_direction_mid,"direction_mid/D");
    _clusterTree->Branch("_direction_max",&_direction_max,"direction_max/D");
    _clusterTree->Branch("_showerness_min",&_showerness_min,"showerness_min/D");
    _clusterTree->Branch("_showerness_mid",&_showerness_mid,"showerness_mid/D");
    _clusterTree->Branch("_showerness_max",&_showerness_max,"showerness_max/D");
    _clusterTree->Branch("_trackness_min",&_trackness_min,"trackness_min/D");
    _clusterTree->Branch("_trackness_mid",&_trackness_mid,"trackness_mid/D");
    _clusterTree->Branch("_trackness_max",&_trackness_max,"trackness_max/D");
    _clusterTree->Branch("_offaxis_hits_min",&_offaxis_hits_min,"offaxis_hits_min/D");
    _clusterTree->Branch("_offaxis_hits_mid",&_offaxis_hits_mid,"offaxis_hits_mid/D");
    _clusterTree->Branch("_offaxis_hits_max",&_offaxis_hits_max,"offaxis_hits_max/D");

    return;
  }

  void ShowerRecoManager::ClearClusterTree(){

    _polyA_min = _polyA_mid = _polyA_max = 0;
    _polyP_min = _polyP_mid = _polyP_max = 0;
    _sumQ_min = _sumQ_mid = _sumQ_max = 0;
    _avgQ_min = _avgQ_mid = _avgQ_max = 0;
    _angle2d_min = _angle2d_mid = _angle2d_max = 0;
    _openingAngle_min = _openingAngle_mid = _openingAngle_max = 0;
    _closingAngle_min = _closingAngle_mid = _closingAngle_max = 0;
    _eigenvalue_principal_min = _eigenvalue_principal_mid = _eigenvalue_principal_max = 0;
    _eigenvalue_secondary_min = _eigenvalue_secondary_mid = _eigenvalue_secondary_max = 0;
    _length_min = _length_mid = _length_max = 0;
    _width_min = _width_mid = _width_max = 0;
    _hitD1D_min = _hitD1D_mid = _hitD1D_max = 0;
    _hitD2D_min = _hitD2D_mid = _hitD2D_max = 0;
    _modmeancharge_min = _modmeancharge_mid = _modmeancharge_max = 0;
    _RMS_charge_min = _RMS_charge_mid = _RMS_charge_max = 0;
    _direction_min = _direction_mid = _direction_max = 0;
    _showerness_min = _showerness_mid = _showerness_max = 0;
    _trackness_min = _trackness_mid = _trackness_max = 0;
    _offaxis_hits_min = _offaxis_hits_mid = _offaxis_hits_max = 0;

    return;
  }

  void ShowerRecoManager::FillClusterTree(const std::vector< ::cluster::ClusterParamsAlg>& cpan_v)
  {

    ClearClusterTree();

    if (cpan_v.size() == 3){
      auto cl0 = cpan_v[0];
      auto cl1 = cpan_v[1];
      auto cl2 = cpan_v[2];
      auto p0 = cl0.GetParams();
      auto p1 = cl1.GetParams();
      auto p2 = cl2.GetParams();
      Sort(p0.PolyObject.Area(),p1.PolyObject.Area(),p2.PolyObject.Area(),_polyA_min,_polyA_mid,_polyA_max);
      Sort(p0.PolyObject.Perimeter(),p1.PolyObject.Perimeter(),p2.PolyObject.Perimeter(),_polyP_min,_polyP_mid,_polyP_max);
      Sort(p0.sum_charge,p1.sum_charge,p2.sum_charge,_sumQ_min,_sumQ_mid,_sumQ_max);
      Sort(p0.mean_charge,p1.mean_charge,p2.mean_charge,_avgQ_min,_avgQ_mid,_avgQ_max);
      Sort(p0.angle_2d,p1.angle_2d,p2.angle_2d,_angle2d_min,_angle2d_mid,_angle2d_max);
      Sort(p0.opening_angle,p1.opening_angle,p2.opening_angle,_openingAngle_min,_openingAngle_mid,_openingAngle_max);
      Sort(p0.closing_angle,p1.closing_angle,p2.closing_angle,_closingAngle_min,_closingAngle_mid,_closingAngle_max);
      Sort(p0.eigenvalue_principal,p1.eigenvalue_principal,p2.eigenvalue_principal,_eigenvalue_principal_min,_eigenvalue_principal_mid,_eigenvalue_principal_max);
      Sort(p0.eigenvalue_secondary,p1.eigenvalue_secondary,p2.eigenvalue_secondary,_eigenvalue_secondary_min,_eigenvalue_secondary_mid,_eigenvalue_secondary_max);
      Sort(p0.length,p1.length,p2.length,_length_min,_length_mid,_length_max);
      Sort(p0.width,p1.width,p2.width,_width_min,_width_mid,_width_max);
      Sort(p0.hit_density_1D,p1.hit_density_1D,p2.hit_density_1D,_hitD1D_min,_hitD1D_mid,_hitD1D_max);
      Sort(p0.hit_density_2D,p1.hit_density_2D,p2.hit_density_2D,_hitD2D_min,_hitD2D_mid,_hitD2D_max);
      Sort(p0.modmeancharge,p1.modmeancharge,p2.modmeancharge,_modmeancharge_min,_modmeancharge_mid,_modmeancharge_max);
      Sort(p0.RMS_charge,p1.RMS_charge,p2.RMS_charge,_RMS_charge_min,_RMS_charge_mid,_RMS_charge_max);
      Sort(p0.direction,p1.direction,p2.direction,_direction_min,_direction_mid,_direction_max);
      Sort(p0.showerness,p1.showerness,p2.showerness,_showerness_min,_showerness_mid,_showerness_max);
      Sort(p0.trackness,p1.trackness,p2.trackness,_trackness_min,_trackness_mid,_trackness_max);
      Sort(p0.offaxis_hits,p1.offaxis_hits,p2.offaxis_hits,_offaxis_hits_min,_offaxis_hits_mid,_offaxis_hits_max);
    }      


    if (cpan_v.size() == 2){
      auto cl0 = cpan_v[0];
      auto cl1 = cpan_v[1];
      auto p0 = cl0.GetParams();
      auto p1 = cl1.GetParams();
      Sort(p0.PolyObject.Area(),p1.PolyObject.Area(),_polyA_min,_polyA_max);
      Sort(p0.PolyObject.Perimeter(),p1.PolyObject.Perimeter(),_polyP_min,_polyP_max);
      Sort(p0.sum_charge,p1.sum_charge,_sumQ_min,_sumQ_max);
      Sort(p0.mean_charge,p1.mean_charge,_avgQ_min,_avgQ_max);
      Sort(p0.angle_2d,p1.angle_2d,_angle2d_min,_angle2d_max);
      Sort(p0.opening_angle,p1.opening_angle,_openingAngle_min,_openingAngle_max);
      Sort(p0.closing_angle,p1.closing_angle,_closingAngle_min,_closingAngle_max);
      Sort(p0.eigenvalue_principal,p1.eigenvalue_principal,_eigenvalue_principal_min,_eigenvalue_principal_max);
      Sort(p0.eigenvalue_secondary,p1.eigenvalue_secondary,_eigenvalue_secondary_min,_eigenvalue_secondary_max);
      Sort(p0.length,p1.length,_length_min,_length_max);
      Sort(p0.width,p1.width,_width_min,_width_max);
      Sort(p0.hit_density_1D,p1.hit_density_1D,_hitD1D_min,_hitD1D_max);
      Sort(p0.hit_density_2D,p1.hit_density_2D,_hitD2D_min,_hitD2D_max);
      Sort(p0.modmeancharge,p1.modmeancharge,_modmeancharge_min,_modmeancharge_max);
      Sort(p0.RMS_charge,p1.RMS_charge,_RMS_charge_min,_RMS_charge_max);
      Sort(p0.direction,p1.direction,_direction_min,_direction_max);
      Sort(p0.showerness,p1.showerness,_showerness_min,_showerness_max);
      Sort(p0.trackness,p1.trackness,_trackness_min,_trackness_max);
      Sort(p0.offaxis_hits,p1.offaxis_hits,_offaxis_hits_min,_offaxis_hits_max);
    }      

    _clusterTree->Fill();
    
    return;
  }

  void ShowerRecoManager::finalize(TFile* fout){
    if (!fout)
      return;
    if (_saveClusterTree) { _clusterTree->Write(); }
    TTree* algtree = fShowerAlgo->getTree();
    if (algtree) { algtree->Write(); }
    return;
  }
  
  void ShowerRecoManager::Sort(double a, double b, double c, double &min, double &med, double &max){

    if ( (a < b) && (a < c) ){
      min = a;
      if ( b < c ){
	med = b;
	max = c;
      }
      else{
	med = c;
	max = b;
      }
    }
    else if ( (b < a) && (b < c) ){
      min = b;
      if ( a < c ){
	med = a;
	max = c;
      }
      else{
	med = c;
	max = a;
      }
    }
    else{
      min = c;
      if ( a < b ){
	med = a;
	max = b;
      }
      else{
	med = b;
	max = a;
      }
    }

    return;
  }


  void ShowerRecoManager::Sort(double a, double b, double &min, double &max){

    if (a < b){
      min = a;
      max = b;
    }
    else{
      min = b;
      max = a;
    }

    return;
  }



}

#endif
