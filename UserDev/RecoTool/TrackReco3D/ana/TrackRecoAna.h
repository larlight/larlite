#ifndef LARLITE_TRACKRECOANA_H
#define LARLITE_TRACKRECOANA_H

#include "Analysis/ana_base.h"

namespace larlite {

  class TrackRecoAna : public ana_base {
  
  public:

    TrackRecoAna();
    ~TrackRecoAna(){}

    bool initialize();
    bool analyze(storage_manager* storage);
    bool finalize();

  private:
    TTree* _tree;
    
    std::string _mc_producer;
    std::string _trk_vtx_prod;

    int _run;   
    int _subrun;
    int _event; 
    int _entry; 

    //
    // reco
    //
    int _vtxid; 

    float _vtx_x;
    float _vtx_y;
    float _vtx_z;
    
    int _ntracks;

    std::vector<int> _npts_v;

    std::vector<float> _dL_v;
    std::vector<float> _sL_v;
    std::vector<float> _EndX_v;
    std::vector<float> _EndY_v;
    std::vector<float> _EndZ_v;

    std::vector<float> _Theta_v;
    std::vector<float> _Phi_v;

    std::vector<std::vector<float> > _dQdx_U_vv;
    std::vector<std::vector<float> > _dQdx_V_vv;
    std::vector<std::vector<float> > _dQdx_Y_vv;
    
    std::vector<std::vector<float> > _Length_vv;
    
    float _TrunkLength;
    
    std::vector<float> _TrunkLength_v;
    std::vector<float> _TrunkdQdx_U_v;
    std::vector<float> _TrunkdQdx_V_v;
    std::vector<float> _TrunkdQdx_Y_v;

    std::vector<float> _TrunkTheta_v;
    std::vector<float> _TrunkPhi_v;

    //
    // mc
    //
    float _MuonStartX;
    float _MuonStartY;
    float _MuonStartZ;

    float _MuonEndX;
    float _MuonEndY;
    float _MuonEndZ;

    float _MuonStartE;
    float _MuonEndE;
    
    float _Muon_dE;
    float _Muon_dL;
    
    float _ProtonStartX;
    float _ProtonStartY;
    float _ProtonStartZ;

    float _ProtonEndX;
    float _ProtonEndY;
    float _ProtonEndZ;

    float _ProtonStartE;
    float _ProtonEndE;

    float _Proton_dE;
    float _Proton_dL;
      
    void ClearEvent();
    void ClearVertex();
    void ResizeVectors(int ntracks);
      
  };
}
#endif

