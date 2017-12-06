/**
 * \file DropSingles.h
 *
 * \ingroup merging
 * 
 * \brief Class def header for a class DropSingles
 *
 * @author cadams
 */

/** \addtogroup merging

    @{*/

#ifndef LARLITE_DROPSINGLES_H
#define LARLITE_DROPSINGLES_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class DropSingles
     User custom analysis class made by SHELL_USER_NAME
   */
  class DropSingles : public ana_base{
  
  public:

    /// Default constructor
    DropSingles(){ _name="DropSingles"; _fout=0;}

    /// Default destructor
    virtual ~DropSingles(){}

    /** IMPLEMENT in DropSingles.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in DropSingles.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in DropSingles.cc! 
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
