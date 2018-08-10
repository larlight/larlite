/**
 * \file ERAlgoTrackDresser.h
 *
 * \ingroup Algo
 * 
 * \brief Class def header for a class ERAlgoTrackDresser
 *
 * @author jzennamo
 */

/** \addtogroup Algo

    @{*/

#ifndef ERTOOL_ERALGOTRACKDRESSER_H
#define ERTOOL_ERALGOTRACKDRESSER_H

#include "ERTool/Base/AlgoBase.h"
#include "GeoAlgo/GeoAlgo.h"


namespace ertool {

  /**
     \class ERAlgoTrackDresser
     User custom Algorithm class made by kazuhiro
   */
  class ERAlgoTrackDresser : public AlgoBase {
  
  public:

    /// Default constructor
    ERAlgoTrackDresser(const std::string& name="ERAlgoTrackDresser");

    /// Default destructor
    virtual ~ERAlgoTrackDresser(){};

    /// Reset function
    void Reset();

    /// Function to accept fclite::PSet
    void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Called @ before processing the first event sample
    void ProcessBegin();

    /// Function to evaluate input showers and determine a score
    bool Reconstruct(const EventData &data, ParticleGraph& graph);

    /// Called after processing the last event sample
    void ProcessEnd(TFile* fout=nullptr);

    void setMinShwToTrackDist(double dist){ _minDist = dist;  }
    void setMinTrkStartDist  (double dist){ _strtDist = dist; }
    void setMinTrkEndDist    (double dist){ _endDist = dist;  }
    void setMinTrkDeltaDist  (double dist){ _deltaDist = dist;}

  protected:
    // GeoAlgo Tool
    ::geoalgo::GeoAlgo _geoAlgo;
    
    double _minDist;
    double _strtDist;
    double _endDist;
    double _deltaDist;

  };
}
#endif

/** @} */ // end of doxygen group 
