/**
 * \file TrackMomentumCalculator.h
 *
 * \ingroup LArSoftImport
 *
 * \brief  TrackMomentumCalculator.h
 *         author  sowjanyag@phys.ksu.edu
 *
 * @author davidkaleko
 */

/** \addtogroup LArSoftImport

    @{*/
#ifndef TRACKMOMENTUMCALCULATOR_H
#define TRACKMOMENTUMCALCULATOR_H

#include <iostream>
#include "TVector3.h"
#include "TPolyLine3D.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "DataFormat/mctrack.h"
#include "DataFormat/track.h"
#include "TMatrixDSym.h"
#include "TMatrixDSymEigen.h"
#include "TAxis.h"
// #include "Minuit2/Minuit2Minimizer.h"
#include "TMath.h"

/**
   \class TrackMomentumCalculator
   User defined class TrackMomentumCalculator ... these comments are used to generate
   doxygen documentation!
*/

namespace larlite {

    class TrackMomentumCalculator {

        // Global variables/input
        // A. ---> for the TMinuit2 chi^2 minimization !
        Double_t xmeas[30]; Double_t ymeas[30]; Double_t eymeas[30]; Int_t nmeas;
        // B. ---> For the LLHD raster scan !
        // ..

        Int_t n;
        Double_t x[50000]; Double_t y[50000]; Double_t z[50000];
        Int_t n_reco;
        Float_t x_reco[50000]; Float_t y_reco[50000]; Float_t z_reco[50000];
        Float_t seg_size; Float_t seg_stop; Int_t n_seg;
        Float_t x_seg[50000]; Float_t y_seg[50000]; Float_t z_seg[50000];
        TVector3 basex; TVector3 basey; TVector3 basez;
        std::vector<Float_t> segx; std::vector<Float_t> segy; std::vector<Float_t> segz;
        std::vector<Float_t> segnx; std::vector<Float_t> segny; std::vector<Float_t> segnz;
        std::vector<Float_t> segL;
        Double_t find_angle( Double_t vz, Double_t vy );
        Float_t steps_size; Int_t n_steps; std::vector<Float_t> steps;
        Float_t steps_size2;
        Float_t kcal;
        std::vector<Float_t> dthij; std::vector<Float_t> dEi; std::vector<Float_t> dEj; std::vector<Float_t> ind;

    public:

        // Constructor and destructor  //
        TrackMomentumCalculator();

        virtual ~TrackMomentumCalculator() {};

        void SetMinLength(double minlen) { minLength = minlen; }

        //    double GetTrackMomentum(double trkrange, int pdg);

        TPolyLine3D *gr_xyz; TGraph *gr_xy; TGraph *gr_yz; TGraph *gr_xz;

        //    Int_t GetTracks( const std::vector<Float_t> &xxx, const std::vector<Float_t> &yyy, const std::vector<Float_t> &zzz );

        TPolyLine3D *gr_reco_xyz; TGraph *gr_reco_xy; TGraph *gr_reco_yz; TGraph *gr_reco_xz;

        Int_t GetRecoTracks( const std::vector<Float_t> &xxx, const std::vector<Float_t> &yyy, const std::vector<Float_t> &zzz );

        TPolyLine3D *gr_seg_xyz; TGraph *gr_seg_xy; TGraph *gr_seg_yz; TGraph *gr_seg_xz;

        Int_t GetSegTracks2( const std::vector<Float_t> &xxx, const std::vector<Float_t> &yyy, const std::vector<Float_t> &zzz );

        //    void GetDeltaThetaRMS( Double_t &mean, Double_t &rms, Double_t &rmse, Double_t thick );

        TGraphErrors *gr_meas;

        //    Double_t GetMomentumMultiScatterChi2( const larlite::mctrack &trk );

        Double_t p_mcs; Double_t p_mcs_e; Double_t chi2;

        Int_t GetDeltaThetaij( std::vector<Float_t> &ei, std::vector<Float_t> &ej, std::vector<Float_t> &th, Double_t thick, std::vector<Float_t> &ind );

        Double_t my_g( Double_t xx, Double_t Q, Double_t s );

        Double_t my_mcs_llhd( Double_t x0, Double_t x1 );

        Double_t GetMomentumMultiScatterLLHD( const larlite::mctrack &trk );
        Double_t GetMomentumMultiScatterLLHD( const larlite::track   &trk, bool flip = false );
        // Double_t GetMomentumMultiScatterChi2( const larlite::mctrack &trk );

        Double_t p_mcs_2; Double_t LLbf;

        //    Double_t GetMuMultiScatterLLHD( const larlite::mctrack &trk );

        Double_t GetMuMultiScatterLLHD2( const larlite::mctrack &trk );

        Double_t minLength;

        Double_t maxLength;

    };
} // end namespace larlite


#endif
/** @} */ // end of doxygen group

