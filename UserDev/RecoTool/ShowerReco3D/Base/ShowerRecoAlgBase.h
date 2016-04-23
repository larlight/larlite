/**
 * \file ShowerRecoAlgBase.h
 *
 * \ingroup ShowerReco3D
 *
 * \brief Class def header for a class ShowerRecoAlgBase
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerReco3D

    @{*/
#ifndef RECOTOOL_SHOWERRECOALGBASE_H
#define RECOTOOL_SHOWERRECOALGBASE_H

#include <iostream>
#include "ShowerRecoException.h"
#include "ShowerRecoTypes.h"
#include "TTree.h"
#include "TFile.h"
#include "DataFormat/storage_manager.h"



namespace showerreco {

/**
   \class ShowerRecoAlgBase
   User defined class ShowerRecoAlgBase ... these comments are used to generate
   doxygen documentation!
*/
class ShowerRecoAlgBase {

public:

  /// Default constructor
  ShowerRecoAlgBase();

  /// Default destructor
  virtual ~ShowerRecoAlgBase() {}

  /// Function to reset algorithm, to be called @ beginning of each event
  virtual void Reset() = 0;

  /// Function to reconstruct one shower
  virtual Shower_t RecoOneShower(const ::protoshower::ProtoShower & proto_shower) = 0;

  /// Verbosity switch
  void Verbose(bool on = true);

  /// finalize function to write trees
  virtual void Finalize(TFile* fout) {};

  /// initialize function
  virtual void Initialize() {};

  // Pass the storage manager to the reco alg
  void SetStorageManager(larlite::storage_manager * sm) {_storage = sm;}

protected:

  /// Verbosity flag
  bool _verbosity;

  ::larlite::storage_manager * _storage;

};
}

#endif
/** @} */ // end of doxygen group

