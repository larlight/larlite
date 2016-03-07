/**
 * \file EREffCorrectAna.h
 *
 * \ingroup Pi0Ana
 * 
 * \brief Class def header for a class EREffCorrectAna
 *
 * @author ryan
 */

/** \addtogroup Pi0Ana

    @{*/

#ifndef ERTOOL_EREFFCORRECTANA_H
#define ERTOOL_EREFFCORRECTANA_H

#include "ERTool/Base/AnaBase.h"
#include "ERTool/Base/ParticleGraph.h"
#include "ERTool/Base/Particle.h"
#include "ERTool/Base/EventData.h"
#include <math.h>       /* cos */
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"

#include "LArUtil/Geometry.h"




namespace ertool {

  /**
     \class EREffCorrectAna
     User custom Analysis class made by kazuhiro
   */
  class EREffCorrectAna : public AnaBase {
  
  public:

    /// Default constructor
    EREffCorrectAna(const std::string& name="EREffCorrectAna");

    /// Default destructor
    virtual ~EREffCorrectAna(){}

    /// Reset function
    virtual void Reset();

    /// Function to accept fclite::PSet
    void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Called @ before processing the first event sample
    void ProcessBegin();

    /// Function to evaluate input showers and determine a score
    //bool Analyze(const EventData &data, const ParticleGraph &ps);
    bool Analyze(const EventData &data, const ParticleGraph &graph);

    /// Called after processing the last event sample
    void ProcessEnd(TFile* fout=nullptr);

    void SetReco(bool recon ){_recon = recon;}

    void SetFid(double fid ){_fid = fid;}

	private: 
	
	bool _recon = true;

	//--Perm. Histos--
	TH1D* _ProdNeutrino;
	TH1D* _ProdNeutrinoX;
	TH1D* _1pi0MassReco;
	TH1D* _1pi0MomentumReco;
	TH1D* _1pi0MomentumRecoX;
	TH1D* _1pi0MomentumRecoY;
	TH1D* _1pi0MomentumRecoZ;
	TH1D* _1pi0CosBeamReco;

	TH1D* _1pi0MassMC;
	TH1D* _1pi0MomentumMC;
	TH1D* _1pi0CosBeamMC;


	TH1D* _1pi0MomentumEFF;
	TH1D* _1pi0CosBeamEFF;
	TTree* _nutree;
	int _nutype;
	double _nux;
	double _nuy;
	double _nuz;
	TTree* _atree;
	double _pi0x;
	double _pi0y;
	double _pi0z;

	//---------------
     int _nEvents = 0;
     int _nGoodEvents = 0;
     int _nRecoEvents = 0;

     double _fid = 0.;


  };
}
#endif

/** @} */ // end of doxygen group 
