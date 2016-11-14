/**
 * \file FlashMatch2D.h
 *
 * \ingroup App
 * 
 * \brief Class def header for a class FlashMatch2D
 *
 * @author kazuhiro
 */

/** \addtogroup App

    @{*/

#ifndef LARLITE_FLASHMATCH2D_H
#define LARLITE_FLASHMATCH2D_H

#include "Analysis/ana_base.h"
#include "MCQCluster.h"
#include "OpT0Finder/Base/FlashMatchManager.h"

namespace larlite {
  /**
     \class FlashMatch2D
     User custom analysis class made by SHELL_USER_NAME
   */
  class FlashMatch2D : public ana_base{
  
  public:

    /// Default constructor
    FlashMatch2D(){ _name="FlashMatch2D"; _fout=0;}

    /// Default destructor
    virtual ~FlashMatch2D(){}

    /** IMPLEMENT in FlashMatch2D.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in FlashMatch2D.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in FlashMatch2D.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    ::flashana::FlashMatchManager& Manager() { return _mgr;}
    
    const std::vector<flashana::FlashMatch_t> MatchResult() const { return _result; }
    
    void SetConfigFile(std::string name) { _config_file = name; }

    size_t SubVolumeWires() const { return _num_wires; }

    size_t SubVolumeTicks() const { return _num_ticks; }

  protected:

    ::flashana::FlashMatchManager _mgr;
    
    std::vector<flashana::FlashMatch_t> _result;

    // Configurable params
    std::string _config_file;
    std::string _wire_producer;
    std::string _opflash_producer_beam;
    std::string _opflash_producer_cosmic;
    double _flash_trange_start;
    double _flash_trange_end;
    size_t _num_wires;
    size_t _num_ticks;
    std::vector<double> _gain_correction;

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
