/**
 * \file ConvertdEdx.h
 *
 * \ingroup dEdxConversion
 *
 * \brief Class def header for a class ConvertdEdx
 *
 * @author hourlier
 */

/** \addtogroup dEdxConversion

 @{*/

#ifndef LARLITE_CONVERTDEDX_H
#define LARLITE_CONVERTDEDX_H

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
     \class ConvertdEdx
     User custom analysis class made by SHELL_USER_NAME
     */
    class ConvertdEdx : public ana_base{

    public:

        ConvertdEdx(){ _name="ConvertdEdx"; _fout=0;}
        virtual ~ConvertdEdx(){}
        virtual bool initialize();
        virtual bool analyze(storage_manager* storage);
        virtual bool finalize();

        void ChoseCalibration();

        void SetCalibrationFilePath(std::string s){CalibrationFilePath = s;}
        void SetCalibrationFileName(std::string s){CalibrationFileName = s;}
        void SetdEdxSplineFile(     std::string s){SplineFileName = s;}

        bool LoadCalibrationFile();

        void DefineTreeBranches();
        void GetSpatialCorrectionFactor(TVector3 point,geo::View_t view=geo::kUnknown);
        void Get_dEdx_Splines();

        TTree* GetAnaTree(){return dEdxTree;}

    protected:

        int run, subrun, event, vtx_id;
        bool useTruncateddQdx;

        double alpha_box;
        double beta_box;
        double C_dqdx_calib;
        double SpatialCalib;
        double Wion;
        double Efield;

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

        std::vector<std::vector<double> >CollectionCorrectionMap;
        larlite::geo::View_t views[3];
        std::string CalibrationFilePath;
        std::string CalibrationFileName;
        std::string SplineFileName;
        std::string dirName;

        TSpline3 sMuonRange2dEdx;
        TSpline3 sProtonRange2dEdx;

        //TH2D *hdEdxVresLength;
        TH3D *hSpatialCorr;
        TFile *fCalib;
        TTree *dEdxTree;


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
