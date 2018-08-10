/**
 * \file SampleSelIIAAna.h
 *
 * \ingroup CCInclusive
 *
 * \brief Ana module that uses SelIIA to find 1mu1p events then makes histograms about them
 *
 *     track: pandoraNuPMA
 *     vtx: pmtrack
 *     calorimetry: pandoraNuPMAcalo
 *
 *
 * @author davidkaleko
 */

/** \addtogroup CCInclusive

    @{*/

#ifndef LARLITE_SAMPLESELIIAANA_H
#define LARLITE_SAMPLESELIIAANA_H

#include "Analysis/ana_base.h"
#include "DataFormat/track.h"
#include "DataFormat/vertex.h"
#include "DataFormat/calorimetry.h"
#include "GeoAlgo/GeoVector.h"
#include "GeoAlgo/GeoSphere.h"
#include "GeoAlgo/GeoAABox.h"
#include "SelIIA.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "CCInclConstants.h"
#include "CCFidVolBox.h"


namespace larlite {
  /**
     \class SampleSelIIAAna
     User custom analysis class made by SHELL_USER_NAME
   */

  class SampleSelIIAAna : public ana_base {

  public:

    /// Default constructor
    SampleSelIIAAna() {
      _name = "SampleSelIIAAna";
      _fout = 0;
      _hcorrect_ID = 0;
      _running_on_data = false;
      _filetype = kINPUT_FILE_TYPE_MAX;
      _vtx_sphere_radius = 4.;
    }

    /// Default destructor
    virtual ~SampleSelIIAAna() {}

    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();

    void setRunningOnData(bool flag) { _running_on_data = flag; }

    void setInputType(InputFileType_t filetype) { _filetype = filetype; }

    void setVtxSphereRadius(double myradius) { _vtx_sphere_radius = myradius; }

    bool setBGWTimes();

  protected:

    SelIIA _nu_finder;

    double _vtx_sphere_radius;

    InputFileType_t _filetype;

    size_t total_events;
    size_t passed_events;

    double BGW_mintime;
    double BGW_maxtime;

    // Fiducial volume box
    geoalgo::AABox _fidvolBox;

    // TPC box (smaller by ~1 cm)
    geoalgo::AABox _tpcBox;

    // Whether the neutrino was correctly identified (based on MCTruth)
    TH1F *_hcorrect_ID;

    // Whether to try to access/use MCTruth information
    bool _running_on_data;

  };
}
#endif

/** @} */ // end of doxygen group
