/**
 * \file RmTrksNearVtx.h
 *
 * \ingroup HitRemoval
 * 
 * \brief Class def header for a class RmTrksNearVtx_fromPFPart
 *
 * @author david caratelli
 */

/** \addtogroup HitRemoval

    @{*/

#ifndef LARLITE_RMTRKSNEARVTX_NEARCLUS_H
#define LARLITE_RMTRKSNEARVTX_NEARCLUS_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class RmTrksNearVtx_fromPFPart
     User custom analysis class made by SHELL_USER_NAME
   */
  class RmTrksNearVtx_fromPFPart : public ana_base{
  
  public:

    /// Default constructor
    RmTrksNearVtx_fromPFPart(){ _name="RmTrksNearVtx_fromPFPart"; _fout=0;}

    /// Default destructor
    virtual ~RmTrksNearVtx_fromPFPart(){}

    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

    void setPFPartProducer(std::string s) { _pfpart_producer = s; }
    void setVtxProducer(std::string s)    { _vtx_producer = s;    }

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
