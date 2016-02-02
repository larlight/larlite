/**
 * \file ShowerAna.h
 *
 * \ingroup analysis
 * 
 * \brief Class def header for a class ShowerAna
 *
 * @author cadams
 */

/** \addtogroup analysis

    @{*/

#ifndef LARLITE_SHOWERANA_H
#define LARLITE_SHOWERANA_H

#include "Analysis/ana_base.h"

#include <TTree.h>

namespace larlite {
  /**
     \class ShowerAna
     User custom analysis class made by SHELL_USER_NAME
   */
  class ShowerAna : public ana_base{
  
  public:

    /// Default constructor
    ShowerAna();

    /// Default destructor
    virtual ~ShowerAna(){}

    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

    void setShowerProducer(std::string s) { _producer = s; }

  protected:

    std::string _producer;
    
    TTree* _shower_tree;
    
    double _energy_U, _energy_V, _energy_Y;
    double _dedx_U, _dedx_V, _dedx_Y;
    double _dqdx_U, _dqdx_V, _dqdx_Y;
    double _x, _y, _z;
    double _px, _py, _pz;
    double _theta, _phi;
    double _opening_angle;
    double _length;

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
