#ifndef LARLITE_COPYDL2DLRAW_H
#define LARLITE_COPYDL2DLRAW_H

#include "Analysis/ana_base.h"

namespace larlite {

  class CopyDL2DLRaw : public ana_base{
  
  public:

    CopyDL2DLRaw() {
      _name="CopyDL2DLRaw"; 
      _fout=0;
      _input_producer = "";
      _output_producer = "";
    }
       
    ~CopyDL2DLRaw(){}

    bool initialize() { return true; }
    bool analyze(storage_manager* storage);
    bool finalize() { return true; }
    
    void set_input_producer(const std::string& prod)
    { _input_producer = prod; }

    void set_output_producer(const std::string& prod)
    { _output_producer = prod; }
    
  private:
    std::string _input_producer;
    std::string _output_producer;
  };
}
#endif

//**************************************************************************
// 
// For Analysis framework documentation, read Manual.pdf here:
//
// http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
//
//**************************************************************************

/** @} */ // end of doxygen group 
