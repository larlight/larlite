/**
 * \file DrawMCTruth.h
 *
 * \ingroup TruthViewer
 *
 * \brief Class def header for a class DrawMCTruth
 *
 * @author cadams
 */

/** \addtogroup TruthViewer

    @{*/

#ifndef LARLITE_DRAWMCTRUTH_H
#define LARLITE_DRAWMCTRUTH_H

#include "Analysis/ana_base.h"
#include "DataFormat/mctruth.h"
#include "TVector3.h"

namespace evd {
/**
   \class DrawMCTruth
   User custom analysis class made by SHELL_USER_NAME
 */

class MCInfoSummary {
public:
  TVector3 vertex() {return _vertex;}
  int incoming_lepton_pdg() {return _incoming_lepton_pdg;}
  int outgoing_lepton_pdg() {return _outgoing_lepton_pdg;}
  int target_pdg() {return _target_pdg;}
  std::vector<int> outgoing_pdg_list() {return _outgoing_pdg_list;}

  TVector3 _vertex;
  int _incoming_lepton_pdg;
  int _outgoing_lepton_pdg;
  int _target_pdg;
  std::vector<int> _outgoing_pdg_list;

};


class DrawMCTruth : public larlite::ana_base {

public:

  /// Default constructor
  DrawMCTruth() { _name = "DrawMCTruth"; _fout = 0;}

  /// Default destructor
  virtual ~DrawMCTruth() {}

  /** IMPLEMENT in DrawMCTruth.cc!
      Initialization method to be called before the analysis event loop.
  */
  virtual bool initialize();

  /** IMPLEMENT in DrawMCTruth.cc!
      Analyze a data event-by-event
  */
  virtual bool analyze(larlite::storage_manager* storage);

  /** IMPLEMENT in DrawMCTruth.cc!
      Finalize method to be called after all events processed.
  */
  virtual bool finalize();

  void setProducer(std::string s){_producer = s;}

  MCInfoSummary getData() {return _this_info;}

protected:

  MCInfoSummary _this_info;

  std::string _producer;
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
