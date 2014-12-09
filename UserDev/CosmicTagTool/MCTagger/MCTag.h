/**
 * \file MCTag.h
 *
 * \ingroup MCTagger
 * 
 * \brief Class def header for a class MCTag
 *
 * @author David Caratelli
 */

/** \addtogroup MCTagger

    @{*/

#ifndef LARLITE_MCTAG_H
#define LARLITE_MCTAG_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class MCTag
     User custom analysis class made by David Caratelli
   */
  class MCTag : public ana_base{
  
  public:

    /// Default constructor
    MCTag(){ _name="MCTag"; _fout=0;};

    /// Default destructor
    virtual ~MCTag(){};

    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

    void SetDataType(std::string dtype) { _dataType = dtype; }
    
  protected:
    
    std::string _dataType;
    
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
