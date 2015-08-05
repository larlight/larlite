/**
 * \file ERAlgoNGamma.h
 *
 * \ingroup Algo
 * 
 * \brief Class def header for a class ERAlgoNGamma
 *
 * @author ber9
 */

/** \addtogroup Algo

    @{*/

#ifndef ERTOOL_ERALGONGAMMA_H
#define ERTOOL_ERALGONGAMMA_H

#include "ERTool/Base/AlgoBase.h"
#include "ERTool/Algo/AlgoEMPart.h"
#include "ERTool/Algo/AlgoFindRelationship.h"
#include "GeoAlgo/GeoAlgo.h"
#include "GeoAlgo/GeoAABox.h"
#include <algorithm>
#include <utility>
#include <TTree.h>
namespace ertool {

  /**
     \class ERAlgoNGamma
     User custom Algorithm class made by kazuhiro
   */
  class ERAlgoNGamma : public AlgoBase {
  
  public:

    /// Default constructor
    ERAlgoNGamma(const std::string& name="ERAlgoNGamma");

    /// Default destructor
    virtual ~ERAlgoNGamma(){};

    /// Reset function
    void Reset();

    /// Function to accept fclite::PSet
    void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Called @ before processing the first event sample
    void ProcessBegin();

    /// Function to evaluate input showers and determine a score
    bool Reconstruct(const EventData &data, ParticleGraph& graph);

    /// Set verbosity
    void setVerbose(bool on)
    {
      _verbose = on;
      _findRel.setDebug(on);
    }
    /// Called after processing the last event sample
    void ProcessEnd(TFile* fout=nullptr);

    /// from EMPart
    void setMaxIP(double d){_maxIP = d;}
    void setEThreshold(double E){_Ethreshold = E;}
  private:

    ::geoalgo::AABox fTPC;

    /// Clear tree
    void ClearTree();

  protected:

    /// Verbose flag
    bool _verbose;

    /// Gamma mass 
    double _gamma_mass;
    
    /// Maximum impact parameter allowed between two objects
    double _maxIP;
    
    /// Energy threshold
    double _Ethreshold;

    /// Other algorithms in use
    AlgoEMPart _alg_emp;
    AlgoFindRelationship _findRel;
    ::geoalgo::GeoAlgo _geoAlgo;

    /// Fill entry in tree every time EMPart LL function is called
    TTree* _empart_tree;
    double _dedx;
    double _radlen;
    int _pdg;

    /// Fill entry in tree every time reference shower-object comparison
    TTree* _alg_tree;
    double _E; /// energy of shower
    double _thatE; /// energy of object (compared shower or track)
    int _PDG; /// PDG code assigned by AlgoEMPart
    double _dEdx;
    int _VsTrack; /// comparing vs track (==1) <-- investigate meaning
    double _IP; /// impact parameter with other object
    double _IPthisStart; /// distance from IP to this shower start point
    double _IPthatStart; /// distance from IP to that shower/track start point
    double _IPtrkBody; /// distance from IP to body of track (if comparig w trk)
  };
}
#endif

/** @} */ // end of doxygen group 
