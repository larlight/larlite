/**
 * \file RemoveTrkLikeHits.h
 *
 * \ingroup HitRemoval
 * 
 * \brief Class def header for a class RemoveTrkLikeHits
 *
 * @author david
 */

/** \addtogroup HitRemoval

    @{*/

#ifndef LARLITE_REMOVETRKLIKEHITS_H
#define LARLITE_REMOVETRKLIKEHITS_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class RemoveTrkLikeHits
     User custom analysis class made by SHELL_USER_NAME
   */
  class RemoveTrkLikeHits : public ana_base{
  
  public:

    /// Default constructor
    RemoveTrkLikeHits(){ _name="RemoveTrkLikeHits"; _fout=0;}

    /// Default destructor
    virtual ~RemoveTrkLikeHits(){}

    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

    // set pfpart producer
    void setPFPartProducer(std::string s) { _pfpart_producer = s; }
    // set roi producer
    void setROIProducer(std::string s) { _roi_producer = s; }

  protected:

    std::string _pfpart_producer;
    std::string _roi_producer;
    
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
