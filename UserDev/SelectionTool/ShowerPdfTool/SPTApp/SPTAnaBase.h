/**
 * \file SPTAnaBase.h
 *
 * \ingroup ShowerPdfTool
 * 
 * \brief Class def header for a class SPTAnaBase
 *
 * @author kazuhiro
 */

/** \addtogroup ShowerPdfTool

    @{*/

#ifndef SELECTIONTOOL_SPTANABASE_H
#define SELECTIONTOOL_SPTANABASE_H

#include "Analysis/ana_base.h"
#include "SPTBase/SPAHelper.h"

namespace larlite {
  /**
     \class SPTAnaBase
     @brief Base analysis unit class for SPT fmwk. It has features to make SPAData.
     This class implements features to generate SPAData. More elaborate analysis       \n
     unit class may be derived from this class. See sptool::ExampleSPFit for instance. \n
   */
  class SPTAnaBase : public ana_base{
  
  public:

    /// Default constructor
    SPTAnaBase();

    /// Default destructor
    virtual ~SPTAnaBase(){};

    /// Initialization method to be called before the analysis event loop.
    virtual bool initialize();

    /// Analyze a data event-by-event  
    virtual bool analyze(storage_manager* storage);

    /// Finalize method to be called after all events processed.
    virtual bool finalize();

    /**
       @brief Setter for MC info producer names.
       First argument is the generator's name, and the second is MCShower/MCTrack producer name. \n
       It's ok to provide an empty generator name (you will lose vertex MC info as a cost), but  \n
       providing an empty MCShower/MCTrack producer name will result in exception throw.         \n
       Calling this function automatically disables the usage of reco info.
    */
    void MCProducer(const std::string gen, const std::string mcst);

    /**
       @brief Setter for reco info producer names.
       First argument is track producer's name, and the second is shower producer's name.        \n
       It is OK to set an empty string for the track producer's name, but doing so for the shower\n
       producer name will cause an exception throw. Calling this function automatically disables \n
       the usage of mc info.
    */
    void RecoProducer(const std::string track, const std::string shower);

  protected:

    ::sptool::SPAData   _data;   ///< Data to be filled by this module (main purpose!)
    ::sptool::SPAHelper _helper; ///< SPAHelper helps generating SPAData

  private:
    std::string _name_generator; ///< Generator's producer name
    std::string _name_mcreco;    ///< MCShower/MCTrack producer name
    std::string _name_track;     ///< 3D track producer name
    std::string _name_shower;    ///< 3D shower producer name

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
