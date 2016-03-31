/**
 * \file ERAlgoNCPi0.h
 *
 * \ingroup Algo
 * 
 * \brief Class def header for a class ERAlgoNCPi0
 *
 * @author ryan
 */

/** \addtogroup Algo

    @{*/

#ifndef ERTOOL_ERALGONCPI0_H
#define ERTOOL_ERALGONCPI0_H

#include "ERTool/Base/AlgoBase.h"
#include "GeoAlgo/GeoAlgo.h"

#include "LArUtil/Geometry.h"


#include <TTree.h>


 namespace ertool {

  /**
     \class ERAlgoNCPi0
     User custom Algorithm class made by kazuhiro
   */
     class ERAlgoNCPi0 : public AlgoBase {

     public:

    /// Default constructor
        ERAlgoNCPi0(const std::string& name="ERAlgoNCPi0");

    /// Default destructor
        virtual ~ERAlgoNCPi0(){};

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

    // set min shower Energy 
        void setMinShrEnergy(double E) { _energy_min = E; }
    // set max shower Energy 
        void setMaxShrEnergy(double Em) { _energy_max = Em; }

    // set min shower Energy 
        void setMinShrEnergySum(double EE) { _energy_sum_min = EE; }
    // set max shower Energy 
        void setMaxShrEnergySum(double EEm) { _energy_sum_max = EEm; }

    // set min shower Energy 
        void setMinEnergyAsy(double EEASY) { _energy_asy_min = EEASY; }
    // set max shower Energy 
        void setMaxEnergyAsy(double EEASYm) { _energy_asy_max = EEASYm; }

    // Set Max Opening angle
        void setMaxOpeningAngle(double OAM) { _OpeningAnglemax = OAM; }
    // Set Min Opening angle
        void setMinOpeningAngle(double OAMi) { _OpeningAnglemin = OAMi; }

    // Set minimum IP Between showers 
        void setMinIP(double IPMin) { _IPmin = IPMin; }
    // Set maximum IP Between showers 
        void setMaxIP(double IPM) { _IPmax = IPM; }

    // Set minimum RadLenth ( Distance from vtx to start of showers )
        void setMinRadLength(double RLMi) { _RadL_min = RLMi; }
    // Set maximum RadLenth ( Distance from vtx to start of showers )
        void setMaxRadLength(double RLMa) { _RadL_max = RLMa; }


    // Set maximum RadLenth ( Distance from vtx to start of showers )
        void setPi0DirXMin(double PDXMi) { _PDX_min = PDXMi; }
        void setPi0DirXMax(double PDXMa) { _PDX_max = PDXMa; }
        void setPi0DirYMin(double PDYMi) { _PDY_min = PDYMi; }
        void setPi0DirYMax(double PDYMa) { _PDY_max = PDYMa; }
        void setPi0DirZMin(double PDZMi) { _PDZ_min = PDZMi; }
        void setPi0DirZMax(double PDZMa) { _PDZ_max = PDZMa; }


    // Set maximum RadLenth ( Distance from vtx to start of showers )
        void setFidXmin(double fxmin) { _fidx_min = fxmin; }
        void setFidYmin(double fymin) { _fidy_min = fymin; }
        void setFidZmin(double fzmin) { _fidz_min = fzmin; }
        void setFidXmax(double fxmax) { _fidx_max = fxmax; }
        void setFidYmax(double fymax) { _fidy_max = fymax; }
        void setFidZmax(double fzmax) { _fidz_max = fzmax; }






    protected: 

    /// GeoAlgo instance for geometric computations
        geoalgo::GeoAlgo _geoAlgo;

        double _fidx_min =0.;
        double _fidx_max =0.;
        double _fidy_min =0.;
        double _fidy_max =0.;
        double _fidz_min =0.;
        double _fidz_max =0.;

        double _energy_min =0;
        double _energy_max =100000000000000;
        double _energy_sum_min =0;
        double _energy_sum_max =100000000000000;
        double _energy_asy_min =0;
        double _energy_asy_max =1;

        double _vtx_IP;
        double _IPmax =2000;
        double _IPmin =0.;
        double _OpeningAnglemax = 3.14159265;
        double _OpeningAnglemin = 0. ;
        double _RadL_min =0;
        double _RadL_max =100000000000000;

        double _PDX_min =-1.;
        double _PDX_max =1.;
        double _PDY_min =-1.;
        double _PDY_max =1.;
        double _PDZ_min =-1.;
        double _PDZ_max =1.;

        TTree *_pi0_tree;
        double _Rmass;
        double _RE_A;
        double _RE_B;
        double _RIP;
        double _ROangle;
        double _Rnshowers;
        double _Anshowers;
        double _Rvtx_X;
        double _Rvtx_Y;
        double _Rvtx_Z;
        double _RadL_A;
        double _RadL_B;
        TTree *_pi0_selected_tree;
        double _Rmass_S;
        double _RE_A_S;
        double _RE_B_S;
        double _RIP_S;
        double _ROangle_S;
        double _Rvtx_X_S;
        double _Rvtx_Y_S;
        double _Rvtx_Z_S;

        TTree * CandTree;
        int    _Candsize;
        double _C_X;
        double _C_Y;
        double _C_Z;
        double _CE_A;
        double _CE_B;
        double _COangle;
        double _CIP;
        double _CVxtDist_A;
        double _CVxtDist_B;
        double _CVDistToEdge_A;
        double _CSDistToEdge_A;
        double _CVDistToEdge_B;
        double _CSDistToEdge_B;

    };
}
#endif

/** @} */ // end of doxygen group 
