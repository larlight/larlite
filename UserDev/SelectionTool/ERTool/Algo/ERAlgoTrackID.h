/**
 * \file ERAlgoTrackID.h
 *
 * \ingroup Algo
 * 
 * \brief Class def header for a class ERAlgoTrackID
 *
 * @author rmurrell
 */

/** \addtogroup Algo

    @{*/

#ifndef ERTOOL_ERALGOTRACKID_H
#define ERTOOL_ERALGOTRACKID_H

#include "ERTool/Base/AlgoBase.h"

#include <fstream>

#include "TGraph.h"
#include "TH1D.h"
#include "TTree.h"

#include "GeoAlgo/GeoAABox.h"

namespace ertool {

  /**
     \class ERAlgoTrackID
     User custom Algorithm class made by kazuhiro
   */
  class ERAlgoTrackID : public AlgoBase {

    TH1D * propro;
    TH1D * mupro;
    TH1D * pipro;
    TH1D * kapro;

    geoalgo::AABox volume;

    TTree * tree;

    int tpid;
    double tredchi2pro;
    double tredchi2pro_flat;
    double tredchi2mu;
    double tredchi2mu_flat;
    double tredchi2pi;
    double tredchi2ka;

    TGraph * track_dedx;
    Int_t counter = 0;

    TH1D * ReadTextFile(std::ifstream && f,
			char const * name,
			char const * title);

    void Chi2Test(std::vector<Double_t> const & rr,
		  std::vector<Double_t> const & dedxr);
    
  public:

    struct FitTrackResults {
      
      FitTrackResults() {
	
	Reset();
	
      }
      
      Int_t tnpt;
      Double_t tredchi2pro;
      Double_t tredchi2ka;
      Double_t tredchi2mu;
      Double_t tredchi2pi;
      double tredchi2pro_flat;
      double tredchi2mu_flat;
      
      void Reset() {
	
	tnpt = 0;
	tredchi2pro = 0;
	tredchi2ka = 0;
	tredchi2mu = 0;
	tredchi2pi = 0;
	tredchi2pro_flat = 0;
	tredchi2mu_flat = 0;
	
      }
    
    };

    /// Default constructor
    ERAlgoTrackID(const std::string& name="ERAlgoTrackID");

    /// Default destructor
    virtual ~ERAlgoTrackID(){};

    /// Reset function
    void Reset();

    /// Function to accept fclite::PSet
    void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Called @ before processing the first event sample
    void ProcessBegin();

    void GetTrackInfo(Track const & t,
		      std::vector<Double_t> & rr,
		      std::vector<Double_t> & dedxr);

    FitTrackResults FitTrack(Track const & t, Bool_t const reverse = false);
      
    /// Function to evaluate input showers and determine a score
    bool Reconstruct(const EventData &data, ParticleGraph& graph);

    /// Called after processing the last event sample
    void ProcessEnd(TFile* fout=nullptr);
    
  private:

    FitTrackResults ftr;

  };
}
#endif

/** @} */ // end of doxygen group 
