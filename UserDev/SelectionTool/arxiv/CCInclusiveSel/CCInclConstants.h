/**
 * \file CCInclusiveConstants.h
 *
 * \ingroup CCInclusive
 *
 * \brief Class def header for a class CCInclusiveConstants
 *
 * @author davidkaleko
 */

/** \addtogroup CCInclusive

    @{*/
#ifndef CCINCLUSIVECONSTANTS_H
#define CCINCLUSIVECONSTANTS_H

#include <iostream>
#include "DataFormat/vertex.h"
#include "DataFormat/track.h"

/**
   \class CCInclusiveConstants
   User defined class CCInclusiveConstants ... these comments are used to generate
   doxygen documentation!
 */
namespace larlite {

  /// Category of input file (for different BGW times)
  enum InputFileType_t {
    kOnBeam,
    kOffBeam,
    kCorsikaInTime,
    kBNBOnly,
    kBNBCosmic,
    kINPUT_FILE_TYPE_MAX
  };

  /// My own custom-defined "PID" for individual tracks
  enum KalekoPID_t {
    kKalekoProton,
    kKalekoMuon,
    kKalekoChargedPion,
    kKalekoUnknown,
    kKALEKO_PID_MAX
  };

  /// My reconstructed neutrino interactions are a vector of tracks associated with a vertex
  /// And each track has a computed "PID" defined in the enum below.
  typedef std::pair<larlite::vertex, std::vector< std::pair<larlite::KalekoPID_t, larlite::track> > > CCNuItxn_t;

}

#endif
/** @} */ // end of doxygen group

