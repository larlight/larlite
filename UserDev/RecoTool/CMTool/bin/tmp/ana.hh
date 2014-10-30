/**
 * \file Ana_Class_Name.hh
 *
 * \ingroup Working_Package
 * 
 * \brief Class def header for a class Ana_Class_Name
 *
 * @author USER_NAME
 */

/** \addtogroup Working_Package

    @{*/

#ifndef ANA_CLASS_NAME_HH
#define ANA_CLASS_NAME_HH

#include "ana_base.hh"

namespace larlight {
  /**
     \class Ana_Class_Name
     User custom analysis class made by andrzej
   */
  class Ana_Class_Name : public ana_base{
  
  public:

    /// Default constructor
    Ana_Class_Name(){ _name="Ana_Class_Name"; _fout=0;};

    /// Default destructor
    virtual ~Ana_Class_Name(){};

    /** IMPLEMENT in Ana_Class_Name.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in Ana_Class_Name.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in Ana_Class_Name.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    protected:

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
