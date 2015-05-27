/**
 * \file HitToCluster.h
 *
 * \ingroup merging
 * 
 * \brief Class def header for a class HitToCluster
 *
 * @author cadams
 */

/** \addtogroup merging

    @{*/

#ifndef LARLITE_HITTOCLUSTER_H
#define LARLITE_HITTOCLUSTER_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class HitToCluster
     User custom analysis class made by SHELL_USER_NAME
   */
  class HitToCluster : public ana_base{
  
  public:

    /// Default constructor
    HitToCluster(){ _name="HitToCluster"; _fout=0;}

    /// Default destructor
    virtual ~HitToCluster(){}

    /** IMPLEMENT in HitToCluster.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in HitToCluster.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in HitToCluster.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void SetInputProducer(std::string s){_input_producer = s;}
    void SetOutputProducer(std::string s){_output_producer = s;}

  protected:
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
