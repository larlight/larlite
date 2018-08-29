#ifndef LARLITE_NC_DUMP_H
#define LARLITE_NC_DUMP_H

#include "Analysis/ana_base.h"

namespace larlite {

  class nc_dump : public ana_base{
  
  public:

  nc_dump() : _outtree(nullptr) { _name="nc_dump"; _fout=0;}
    virtual ~nc_dump(){}

    virtual bool initialize();
    virtual bool analyze(storage_manager* storage);
    virtual bool finalize();

    TTree* _outtree;

  private:

    int _run;
    int _subrun;
    int _event;


    std::vector<int> _shower_pdg_v;
    std::vector<int> _shower_trackid_v;
    std::vector<int> _is_primary_v;
    std::vector<int> _is_pi0_v;
    std::vector<float> _shower_edep_v;

    float _pi0_energy;
    
    void ClearEvent();

  };
}
#endif
