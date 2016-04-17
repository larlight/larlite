/**
 * \file RmTrksNearVtx.h
 *
 * \ingroup HitRemoval
 * 
 * \brief Class def header for a class RmTrksNearVtx
 *
 * @author david
 */

/** \addtogroup HitRemoval

    @{*/

#ifndef LARLITE_RMTRKSNEARVTX_H
#define LARLITE_RMTRKSNEARVTX_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class RmTrksNearVtx
     User custom analysis class made by SHELL_USER_NAME
   */
  class RmTrksNearVtx : public ana_base{
  
  public:

    /// Default constructor
    RmTrksNearVtx(){ _name="RmTrksNearVtx"; _fout=0;}

    /// Default destructor
    virtual ~RmTrksNearVtx(){}

    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

    void setVtxProducer(std::string s) { _vtx_producer = s; }
    void setPFPartProducer(std::string s) { _pfpart_producer = s; }

  protected:

    std::string _pfpart_producer;
    std::string _vtx_producer;

    std::vector<double> vtx_w_cm;
    std::vector<double> vtx_t_cm;
    
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
