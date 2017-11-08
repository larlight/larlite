#ifndef LARLITE_SHOWERQUALITY_DL_H
#define LARLITE_SHOWERQUALITY_DL_H

#include "Analysis/ana_base.h"
#include "ShowerReco3D/Base/ShowerRecoException.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/shower.h"
#include "MCSingleShowerCompAlg.h"

namespace larlite {

  class ShowerQuality_DL : public ana_base {

  public:

    ShowerQuality_DL();
    ~ShowerQuality_DL() {};

    void SetVertexProducer(const std::string name)
    { fVertexProducer = name; }

    void SetShowerProducer(const std::string name)
    { fShowerProducer = name; }

    bool initialize();
    bool analyze(storage_manager* storage);
    bool finalize();
    
  private:


    /// Vertex Producer's Name
    std::string fVertexProducer;

    /// Shower Producers name
    std::string fShowerProducer;
    
    /// Analysis TTree. Filled once per reconstructed shower.
    TTree *fShowerTree;
    int _event;
    int _subrun;
    int _run;
    int _entry;


    bool _has_mc;
    
    //
    // reco
    //
    int _vtx_id;
    int _nshowers;
    std::vector<double> _reco_x_v;
    std::vector<double> _reco_y_v;
    std::vector<double> _reco_z_v;
    std::vector<double> _reco_dcosx_v;
    std::vector<double> _reco_dcosy_v;
    std::vector<double> _reco_dcosz_v;
    std::vector<double> _reco_phi_v;
    std::vector<double> _reco_theta_v;
    std::vector<double> _reco_energy_U_v;
    std::vector<double> _reco_energy_V_v;
    std::vector<double> _reco_energy_Y_v;
    std::vector<double> _reco_dedx_v;
    std::vector<double> _reco_dedx_U_v;
    std::vector<double> _reco_dedx_V_v;
    std::vector<double> _reco_dedx_Y_v;
    std::vector<double> _reco_dqdx_v;
    std::vector<double> _reco_dqdx_U_v;
    std::vector<double> _reco_dqdx_V_v;
    std::vector<double> _reco_dqdx_Y_v;
    std::vector<double> _reco_length_v;
    std::vector<double> _reco_width1_v;
    std::vector<double> _reco_width2_v;

    std::vector<double> _mc_reco_anglediff_v;
    std::vector<double> _mc_reco_dist_v;
    std::vector<double> _cluster_eff_U_v;
    std::vector<double> _cluster_eff_V_v;
    std::vector<double> _cluster_eff_Y_v;
    std::vector<double> _cluster_pur_U_v;
    std::vector<double> _cluster_pur_V_v;
    std::vector<double> _cluster_pur_Y_v;
    
    //
    // mc
    //
    int _mcinfoInteractionType;
    int _mcinfoMode;
    int _mc_pdg_code;
    double _mc_x;
    double _mc_y;
    double _mc_z;
    double _mc_dcosx;
    double _mc_dcosy;
    double _mc_dcosz;
    double _mc_energy;

    double _mc_dedx;
    double _mc_dqdx_U;
    double _mc_dqdx_V;
    double _mc_dqdx_Y;
    double _mc_charge_U;
    double _mc_charge_V;
    double _mc_charge_Y;

    double _mcs_E;
    double _mc_containment;
    double _mc_length;
    double _mc_wildlength;
    
    //
    // eff & purity alg
    //
    MCSingleShowerCompAlg _alg;

    //
    // clear
    //
    void ClearEvent();
    void ClearVertex();
    void ResizeVectors(int nshowers);
    
  };
}
#endif
