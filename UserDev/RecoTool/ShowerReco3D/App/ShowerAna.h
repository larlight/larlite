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

#include "TH1F.h"
#include "TVector3.h"

namespace larlite {
  /**
     \class ShowerAna
     User custom analysis class made by SHELL_USER_NAME
   */
  class ShowerAna : public ana_base{
  
  public:

    /// Default constructor
    ShowerAna(){ _name="ShowerAna"; _fout=0;}

    /// Default destructor
    virtual ~ShowerAna(){}

    /** IMPLEMENT in ShowerAna.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in ShowerAna.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in ShowerAna.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    bool isFiducial(const TVector3 & vertex);


  protected:
    
    // Histograms to compare vertex information, dE/dx, direction:
    TH1F * vertex_X;
    TH1F * vertex_Y;
    TH1F * vertex_Z;
    TH1F * vertex_abs;

    TH1F * direction_X;
    TH1F * direction_Y;
    TH1F * direction_Z;
    TH1F * direction_angle;

    TH1F * dEdx;
    TH1F * dEdx_fid;

    std::vector<int> _good_event_list;
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
