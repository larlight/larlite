/**
 * \file Calibration_File.h
 *
 * \ingroup Calibration_File
 *
 * \brief Class def header for a class Calibration_File
 *
 * @author hourlier
 */

/** \addtogroup dEdxConversion

 @{*/

#ifndef LARLITE_CALIBRATIONFILE_H
#define LARLITE_CALIBRATIONFILE_H

#include "Analysis/ana_base.h"
#include "DataFormat/track.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryHelper.h"
#include "TGraph2D.h"
#include "TH3D.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TImage.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TFile.h"
#include "TTree.h"
#include "TSpline.h"
#include <sys/stat.h>
#include <unistd.h>
#include <vector>
#include <string>

namespace larlite {
    /**
     \class Calibration_File
     User custom analysis class made by SHELL_USER_NAME
     */
    class Calibration_File : public ana_base{

        public:

        Calibration_File(){ _name="Calibration_File"; _fout=0;}
        virtual ~Calibration_File(){}
        virtual bool initialize();
        virtual bool analyze(storage_manager* storage);
        virtual bool finalize();

        void LoadSelectionFile();

        void SetCalibrationFilePath(std::string s){CalibrationFilePath = s;}
        void SetCalibrationFileName(std::string s){CalibrationFileName = s;}
        void SetdEdxSplineFile(     std::string s){SplineFileName = s;}
        void SetSelectionFile(      std::string s){SelectionFileName = s;}
        void SetSpatialCalibration(TH3D* h){hSpatialCorr_INPUT=h;}

        void FillSpatialCalibHistos();
        void FillRecombinationHistos();
        void Make2planesCorrectionMaps();
        void Make3DCorrectionMap();

        bool InVolume(TVector3 point, TVector3 FVmargin);
        void SelectedVertex(int _run, int _subrun, int _event, int _vtxid);

        void DefineTreeBranches();
        void GetSpatialCorrectionFactor(TVector3 point,geo::View_t view=geo::kUnknown);
        void Get_dEdx_Splines();
        void MakedQdxCurves();

        protected:

        int run, subrun, event, vtx_id,plane;
        bool useTruncateddQdx;
        bool selectedProton;
        bool selectedCosmic;

        double alpha_box;
        double beta_box;
        double C_dqdx_calib;
        double sigma_alpha_box;
        double sigma_beta_box;
        double sigma_C_dqdx_calib;
        double SpatialCalib;
        double Wion;
        double Efield;

        double xmax;
        double xmin;
        double ymax;
        double ymin;
        double zmax;
        double zmin;

        double avg_Ion_YZ;
        double avg_Ion_X;
        double avg_Ion_3D;

        double MinimumTrackLength;
        
        std::vector<double> Avg_dEdx;
        std::vector<double> Avg_dEdx_5cm;
        std::vector<double> Avg_dEdx_10cm;

        std::vector<double> Avg_corrdQdx;
        std::vector<double> Avg_corrdQdx_5cm;
        std::vector<double> Avg_corrdQdx_10cm;

        std::vector<double> chi2_p_hypothesis;
        std::vector<double> chi2_m_hypothesis;

        std::vector<double> chi2_invert_p_hypothesis;
        std::vector<double> chi2_invert_m_hypothesis;

        std::vector<std::vector<int> > SelectEvtID_proton;
        std::vector<std::vector<int> > SelectEvtID_cosmic;

        std::vector<std::vector<double> >CollectionCorrectionMap;
        std::vector<larlite::track> thisVertex;
        std::vector<std::vector<larlite::track> > AllVertex_m;
        std::vector<std::vector<larlite::track> > AllVertex_p;
        larlite::geo::View_t views[3];
        std::string SelectionFileName;
        std::string CalibrationFilePath;
        std::string CalibrationFileName;
        std::string SplineFileName;
        std::string dirName;

        TSpline3 sMuonRange2dEdx;
        TSpline3 sProtonRange2dEdx;

        TH3D *hSpatialCorr;
        TH3D *hSpatialCorr_INPUT;
        TFile *fCalib;
        TTree *RecombinationParameters;

        TH1D *hSumLength;

        TH2D *hIon_YZ_plane;
        TH2D *hIon_corrected_YZ_plane;
        TH2D *hHitMap_YZ_plane;
        TH2D *hMean_YZ_plane;
        TH2D *hCorrectionMap_YZ_plane;
        TH1D *hIon_X_dir;
        TH1D *hIon_corrected_X_dir;
        TH1D *hHitMap_X_dir;
        TH1D *hMean_X_dir;
        TH1D *hCorrectionMap_X_dir;
        TH1D *hUncorrected_YZ_plane;
        TH1D *hUncorrected_X_dir;

        TH2D *hRecombination_raw;

        TH2D *hUncorrecteddQdx;
        TH2D *hCorrecteddQdx;

        TH3D *hHitMap3D;
        TH3D *hRawCharge3D;
        TH3D *hMeanCharge3D;
        TH3D *hCorrectionMap3D;

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
