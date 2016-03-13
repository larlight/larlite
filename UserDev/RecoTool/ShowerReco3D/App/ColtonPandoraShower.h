/**
 * \file ColtonPandoraShower.h
 *
 * \ingroup filter
 *
 * \brief Class def header for a class ColtonPandoraShower
 *
 * @author cadams
 */

/** \addtogroup filter

    @{*/

#ifndef COLTONPANDORASHOWER_H
#define COLTONPANDORASHOWER_H

#include "Analysis/ana_base.h"
#include "ClusterRecoUtil/Base/CRUHelper.h"


namespace showerreco {
/**
   \class ColtonPandoraShower
   User custom analysis class made by SHELL_USER_NAME
 */
class ColtonPandoraShower : public larlite::ana_base {

public:

  /// Default constructor
  ColtonPandoraShower() { _name = "ColtonPandoraShower"; _fout = 0;}

  /// Default destructor
  virtual ~ColtonPandoraShower() {}

  /** IMPLEMENT in ColtonPandoraShower.cc!
      Initialization method to be called before the analysis event loop.
  */
  virtual bool initialize();

  /** IMPLEMENT in ColtonPandoraShower.cc!
      Analyze a data event-by-event
  */
  virtual bool analyze(larlite::storage_manager* storage);

  /** IMPLEMENT in ColtonPandoraShower.cc!
      Finalize method to be called after all events processed.
  */
  virtual bool finalize();

  void SetInputProducer(std::string s) {_input_producer = s;}
  void SetOutputProducer(std::string s) {_output_producer = s;}

protected:
  std::string _input_producer;
  std::string _output_producer;
  ::cluster::CRUHelper _cru_helper;


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
