/**
 * \file ShowerRecoManager.h
 *
 * \ingroup ShowerReco3D
 * 
 * \brief Class def header for a class ShowerRecoManager
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerReco3D

    @{*/
#ifndef LARLITE_SHOWERRECOMANAGER_H
#define LARLITE_SHOWERRECOMANAGER_H

#include <iostream>
#include "CMTool/CMToolBase/CMatchManager.h"
#include "ShowerRecoException.h"
#include "ShowerRecoAlgBase.h"

namespace showerreco {

  typedef std::vector<std::vector<unsigned int> > ClusterAss_t;

  typedef std::vector< ::larutil::PxHit> PxHitSet_t;

  /**
     \class ShowerRecoManager
     User defined class ShowerRecoManager ... these comments are used to generate
     doxygen documentation!
  */
  class ShowerRecoManager{
    
  public:
    
    /// Default constructor
    ShowerRecoManager();
    
    /// Default destructor
    ~ShowerRecoManager(){}

    void Algo (ShowerRecoAlgBase* alg) { fShowerAlgo = alg; }

    const ShowerRecoAlgBase* Algo() const { return fShowerAlgo; }

    void Reset();

    ClusterAss_t Reconstruct (const std::vector<std::vector<larutil::PxHit> >& clusters,
			      std::vector< ::larlite::shower>& showers);

    void Reconstruct (const std::vector<std::vector<larutil::PxHit> >& clusters,
		      const ClusterAss_t& ass,
		      std::vector< ::larlite::shower>& showers);
    
    ::cmtool::CMatchManager& MatchManager() { return *fMatchMgr; }

    void Finalize() { if (_saveClusterTree) { _clusterTree->Write(); } }

    /// save cluster Ttree
    void SaveClusterTree(bool on) { _saveClusterTree = on; }

  private:
    
    /// Boolean flag to whether or not to run matching
    bool fMatch;

    /// Shower reconstruction algorithm
    ::showerreco::ShowerRecoAlgBase *fShowerAlgo;

    /// Cluster matching code
    ::cmtool::CMatchManager *fMatchMgr;

    void Process(const ClusterAss_t& ass,
		 std::vector< ::larlite::shower>& showers);


    /// Function to fill Cluster info TTree
    void FillClusterTree(const std::vector< ::cluster::ClusterParamsAlg>& cpan_v);
    /// Prepare Cluster info TTree
    void PrepareClusterTree();
    /// Clear Cluster info TTree
    void ClearClusterTree();
    /// Sort function for 3 doubles
    void Sort(double a, double b, double c, double &min, double &med, double &max);
    /// Sort function for 2 doubles
    void Sort(double a, double b, double &min, double &max);


    /// Info for Cluster Tree
    bool _saveClusterTree;
    /// TTree with cluster info
    TTree* _clusterTree;
    // Variables
    double _polyA_min, _polyA_mid, _polyA_max;
    double _polyP_min, _polyP_mid, _polyP_max;
    double _sumQ_min, _sumQ_mid, _sumQ_max;
    double _avgQ_min, _avgQ_mid, _avgQ_max;
    double _angle2d_min, _angle2d_mid, _angle2d_max;
    double _openingAngle_min, _openingAngle_mid, _openingAngle_max;
    double _closingAngle_min, _closingAngle_mid, _closingAngle_max;
    double _eigenvalue_principal_min, _eigenvalue_principal_mid, _eigenvalue_principal_max;
    double _eigenvalue_secondary_min, _eigenvalue_secondary_mid, _eigenvalue_secondary_max;
    double _length_min, _length_mid, _length_max;
    double _width_min, _width_mid, _width_max;
    double _hitD1D_min, _hitD1D_mid, _hitD1D_max;
    double _hitD2D_min, _hitD2D_mid, _hitD2D_max;
    double _modmeancharge_min, _modmeancharge_mid, _modmeancharge_max;
    double _RMS_charge_min, _RMS_charge_mid, _RMS_charge_max;
    double _direction_min, _direction_mid, _direction_max;
    double _showerness_min, _showerness_mid, _showerness_max;
    double _trackness_min, _trackness_mid, _trackness_max;
    double _offaxis_hits_min, _offaxis_hits_mid, _offaxis_hits_max;

    
  };
}

#endif
/** @} */ // end of doxygen group 

