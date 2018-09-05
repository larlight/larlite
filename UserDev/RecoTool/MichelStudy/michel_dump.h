#ifndef LARLITE_MICHEL_DUMP_H
#define LARLITE_MICHEL_DUMP_H

#include "Analysis/ana_base.h"
#include "LArUtil/SpaceChargeMicroBooNE.h"

namespace larlite {

  class michel_dump : public ana_base{
  
  public:

  michel_dump() : _outtree(nullptr) { _name="michel_dump"; _fout=0;}
    virtual ~michel_dump(){}

    virtual bool initialize();
    virtual bool analyze(storage_manager* storage);
    virtual bool finalize();

    TTree* _outtree;

  protected:

    int _run;
    int _subrun;
    int _event;

    int _has_michel;

    float _michel_x;
    float _michel_y;
    float _michel_z;

    float _michel_sx;
    float _michel_sy;
    float _michel_sz;

    float _muon_e;
    float _michel_e;

    ::larutil::SpaceChargeMicroBooNE _sce;    
  };
}
#endif
