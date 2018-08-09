#ifndef __TAGGER_DUMP_H__
#define __TAGGER_DUMP_H__

#include "Analysis/ana_base.h"

namespace larlite {

  class TaggerDump : public ana_base {
  public:
    TaggerDump();
    ~TaggerDump() {};


    bool initialize();
    bool analyze(storage_manager* storage);
    bool finalize();
    
    TTree *fEventTree;

    int _event;
    int _subrun;
    int _run;
    int _entry;
    
    int _pass;
    double _vetoPE;
    double _beamPE;
    double _maxFrac;
    
    std::string _pass_str;
    std::string _vetoPE_str;
    std::string _beamPE_str;
    std::string _maxFrac_str;


  public:
    
    void FillEvent(storage_manager *sto);
    void AttachEvent(TTree* tree);
    void ClearEvent();

  };

}

#endif
