/**
 * \file ERFilterToyCosmicRemover.h
 *
 * \ingroup CCSingleMu
 * 
 * \brief Class def header for a class ERFilterToyCosmicRemover
 *
 * @author kazuhiro
 */

/** \addtogroup CCSingleMu

    @{*/

#ifndef ERTOOL_ERFILTERTOYCOSMICREMOVER_H
#define ERTOOL_ERFILTERTOYCOSMICREMOVER_H

#include "ERTool/Base/FilterBase.h"
#include "ERFilterSecondaries.h"
#include "GeoAlgo/GeoAABox.h"
#include <TH1D.h>
namespace ertool {

  /**
     \class ERFilterToyCosmicRemover
     User custom SPAFilter class made by kazuhiro
   */
  class ERFilterToyCosmicRemover : public FilterBase {
  
  public:

    /// Default constructor
    ERFilterToyCosmicRemover();

    /// Default destructor
    virtual ~ERFilterToyCosmicRemover(){};

    /// Select function
    virtual void Filter (EventData& data);

    void ProcessBegin();

    void ProcessEnd(TFile* fout=nullptr);

  private:

    ::ertool::ERFilterSecondaries _sec_filter;
    ::geoalgo::AABox fTPC;
    TH1D* hFilterAllTrackCtr;
    TH1D* hFilteredTrackCtr;
    TH1D* hTrackRejectionRate;
    TH1D* hFilterAllShowerCtr;
    TH1D* hFilteredShowerCtr;
    TH1D* hShowerRejectionRate;
    TH1D* hUnfilteredTrackLength;
    TH1D* hFilteredTrackLength;
  };
}
#endif

/** @} */ // end of doxygen group 
