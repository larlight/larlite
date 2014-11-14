/**
 * \file PDGSelection.h
 *
 * \ingroup ShowerStudy
 * 
 * \brief Class def header for a class PDGSelection
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerStudy

    @{*/

#ifndef PDGSELECTION_H
#define PDGSELECTION_H

#include "ana_base.h"
#include "ShowerAnaException.h"

namespace larlite {
  /**
     \class PDGSelection
     User custom analysis class made by kazuhiro
   */
  class PDGSelection : public ana_base{
  
  public:

    enum PARTICLE_GROUP {
      kGENERATOR,
      kG4PRIMARY,
      kG4SECONDARY,
      kPARTICLE_GROUP_MAX
    };

    /// Default constructor
    PDGSelection();

    /// Default destructor
    virtual ~PDGSelection(){};

    /**
       Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /**
       Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /**
       Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    /// Setter for what kind of particle to pass the event filter
    void Select(int const pdg_code, 
		PDGSelection::PARTICLE_GROUP part_type=kG4PRIMARY,
		int count=-1);
    
    /// Method to reset internal variables + user defined variables
    void Reset();

  protected:

    size_t _nevent_analyzed;

    size_t _nevent_selected;

    std::vector<std::map<int,int> > _request_list;

    std::vector<std::map<int,int> > _found_list;

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
