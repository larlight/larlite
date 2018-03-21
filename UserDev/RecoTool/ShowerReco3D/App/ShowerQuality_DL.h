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
    TTree *fEventTree;
    TTree *fShowerTree;
    TTree *fAndyTree;

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
    void ResizeVectors(int nshowers);
    void ClearEvent();
    void ClearVertex();
    void AttachAndy(TTree* tree);
    void ResetAndy();
    void FillAndy(storage_manager *sto);

    

    //
    // Andy Related
    //
    double MCFlux_NuPosX;
    double MCFlux_NuPosY;
    double MCFlux_NuPosZ;
    double MCFlux_NuMomX;
    double MCFlux_NuMomY;
    double MCFlux_NuMomZ;
    double MCFlux_NuMomE;
    int MCFlux_ntype;
    int MCFlux_ptype;
    double MCFlux_nimpwt;
    double MCFlux_dk2gen;
    double MCFlux_nenergyn;
    double MCFlux_tpx;
    double MCFlux_tpy;
    double MCFlux_tpz;
    int MCFlux_tptype;
    double MCFlux_vx;
    double MCFlux_vy;
    double MCFlux_vz;

    int MCTruth_NParticles;
    std::vector<int> MCTruth_particles_TrackId;
    std::vector<int> MCTruth_particles_PdgCode;
    std::vector<int> MCTruth_particles_Mother;
    std::vector<int> MCTruth_particles_StatusCode;
    std::vector<int>  MCTruth_particles_NDaughters;    
    std::vector<std::vector<int> > MCTruth_particles_Daughters;
     std::vector<double> MCTruth_particles_Gvx;
    std::vector<double> MCTruth_particles_Gvy;
    std::vector<double> MCTruth_particles_Gvz;
    std::vector<double> MCTruth_particles_Gvt;
    std::vector<double> MCTruth_particles_px0;
    std::vector<double> MCTruth_particles_py0;
    std::vector<double> MCTruth_particles_pz0;
    std::vector<double> MCTruth_particles_e0;
    std::vector<int> MCTruth_particles_Rescatter;
    std::vector<double> MCTruth_particles_polx;
    std::vector<double> MCTruth_particles_poly;
    std::vector<double> MCTruth_particles_polz;
    int MCTruth_neutrino_CCNC;
    int MCTruth_neutrino_mode;
    int MCTruth_neutrino_interactionType;
    int MCTruth_neutrino_target;
    int MCTruth_neutrino_nucleon;
    int MCTruth_neutrino_quark;
    double MCTruth_neutrino_W;
    double MCTruth_neutrino_X;
    double MCTruth_neutrino_Y;
    double MCTruth_neutrino_Q2;

    int GTruth_ProbePDG;
    int GTruth_IsSeaQuark;
    int GTruth_tgtPDG;
    double GTruth_weight;
    double GTruth_probability;
    double GTruth_Xsec;
    double GTruth_fDiffXsec;
    double GTruth_vertexX;
    double GTruth_vertexY;
    double GTruth_vertexZ;
    double GTruth_vertexT;
    int GTruth_Gscatter;
    int GTruth_Gint;
    int GTruth_ResNum;
    int GTruth_NumPiPlus;
    int GTruth_NumPi0;
    int GTruth_NumPiMinus;
    int GTruth_NumProton;
    int GTruth_NumNeutron;
    int GTruth_IsCharm;
    double GTruth_gX;
    double GTruth_gY;
    double GTruth_gZ;
    double GTruth_gT;
    double GTruth_gW;
    double GTruth_gQ2;
    double GTruth_gq2;
     double GTruth_ProbeP4x;
    double GTruth_ProbeP4y;
    double GTruth_ProbeP4z;
    double GTruth_ProbeP4E;
    double GTruth_HitNucP4x;
    double GTruth_HitNucP4y;
    double GTruth_HitNucP4z;
    double GTruth_HitNucP4E;
    double GTruth_FShadSystP4x;
    double GTruth_FShadSystP4y;
    double GTruth_FShadSystP4z;
    double GTruth_FShadSystP4E;

  };
}
#endif
