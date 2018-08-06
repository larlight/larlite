#ifndef __SEGMENT_DUMP_H__
#define __SEGMENT_DUMP_H__

#include "Analysis/ana_base.h"
#include "LArUtil/SpaceChargeMicroBooNE.h"

namespace larlite {

  class SegmentDump : public ana_base {
  public:
    SegmentDump();
    ~SegmentDump() {};


    bool initialize();
    bool analyze(storage_manager* storage);
    bool finalize();
    
    TTree *fEventTree;
    TTree *fAndyTree;

    int _event;
    int _subrun;
    int _run;
    int _entry;

    bool _has_mc;

    struct aparticle{
      int pdg;
      int trackid;
      int parenttrackid;
      int ancestortrackid;
      float depeng;

      bool primary() const {
	return ((this->trackid == this->parenttrackid) && 
		(this->trackid == this->ancestortrackid));
      }

      bool daughterof (const aparticle& particle) const
      { return (this->parenttrackid == particle.trackid); }
      
      bool ancestorof ( const aparticle& particle) const
      { return (this->ancestortrackid == particle.trackid); }

    };

    int _selected;
    int _parent_pdg;
    
    double _energy_init;

    double _parent_x;
    double _parent_y;
    double _parent_z;

    double _parent_sce_x;
    double _parent_sce_y;
    double _parent_sce_z;

    double _parent_px;
    double _parent_py;
    double _parent_pz;

    int _interaction_mode;
    int _interaction_type;

    double _dep_sum_lepton;
    double _dep_sum_proton;

    double _proton_Px;
    double _proton_Py;
    double _proton_Pz;

    double _lepton_Px;
    double _lepton_Py;
    double _lepton_Pz;

    std::vector<double> _daughter_energydep_v;
    std::vector<int> _daughter_pdg_v;

    std::vector<double> _daughterX_v;
    std::vector<double> _daughterY_v;
    std::vector<double> _daughterZ_v;

    std::vector<double> _daughterX_end_v;
    std::vector<double> _daughterY_end_v;
    std::vector<double> _daughterZ_end_v;

    std::vector<double> _daughterPx_v;
    std::vector<double> _daughterPy_v;
    std::vector<double> _daughterPz_v;
    
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
    
    ::larutil::SpaceChargeMicroBooNE _sce;    


    void FillAndy(storage_manager *sto);
    void FillSegment(storage_manager *sto);
    
    void AttachEvent(TTree* tree);
    void AttachAndy(TTree* tree);

    void ClearEvent();
    void ResetAndy();

  };

}

#endif
