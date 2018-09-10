#ifndef LARLITE_CALIBRATIONFILE_CXX
#define LARLITE_CALIBRATIONFILE_CXX

#include "Calibration_File.h"
#include "TVector3.h"
#include "TRandom3.h"
#include "TLine.h"
#include "TLatex.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "DataFormat/hit.h"
#include "DataFormat/track.h"
#include "DataFormat/vertex.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryHelper.h"
#include <iostream>
#include <fstream>
#include <vector>

namespace larlite {

    bool Calibration_File::initialize() {

        std::cout << "Hello World" << std::endl;
        views[0] = larlite::geo::kU;
        views[1] = larlite::geo::kV;
        views[2] = larlite::geo::kZ;
        if(AllVertex_p.size()!=0)AllVertex_p.clear();
        if(AllVertex_m.size()!=0)AllVertex_m.clear();
        std::cout << "views set" << std::endl;

        alpha_box = 1;
        beta_box = 1;
        C_dqdx_calib = 1;
        SpatialCalib = 1;

        Wion = 23.6e-6;// MeV (i.e. 23.6eV = ionization energy)
        Efield = 0.273;// kV per cm

        MinimumTrackLength = 5;
        plane = 2;

        avg_Ion_YZ = 1;
        avg_Ion_X  = 1;

        // Set detector dimensions
        xmax =  256.25;
        xmin =  0;
        ymax =  116.5;
        ymin = -116.5;
        zmax =  1036.8;
        zmin =  0;

        DefineTreeBranches();
        Get_dEdx_Splines();
        LoadSelectionFile();

        hSumLength = new TH1D("hSumLength","hSumLength;L_{1}+L_{2}",100,0,1000);

        hIon_YZ_plane    = new TH2D("hIon_YZ_plane"   ,"hIon_YZ_plane;Z(cm);Y(cm)"   ,100,zmin,zmax,100,ymin,ymax);
        hHitMap_YZ_plane = new TH2D("hHitMap_YZ_plane","hHitMap_YZ_plane;Z(cm);Y(cm)",
                                    hIon_YZ_plane->GetNbinsX(),hIon_YZ_plane->GetXaxis()->GetXmin(),hIon_YZ_plane->GetXaxis()->GetXmax(),
                                    hIon_YZ_plane->GetNbinsY(),hIon_YZ_plane->GetYaxis()->GetXmin(),hIon_YZ_plane->GetYaxis()->GetXmax());
        hMean_YZ_plane   = new TH2D("hMean_YZ_plane","hMean_YZ_plane;Z(cm);Y(cm)",
                                  hIon_YZ_plane->GetNbinsX(),hIon_YZ_plane->GetXaxis()->GetXmin(),hIon_YZ_plane->GetXaxis()->GetXmax(),
                                  hIon_YZ_plane->GetNbinsY(),hIon_YZ_plane->GetYaxis()->GetXmin(),hIon_YZ_plane->GetYaxis()->GetXmax());

        hCorrectionMap_YZ_plane = new TH2D("hCorrectionMap_YZ_plane","hCorrectionMap_YZ_plane;Z(cm);Y(cm)",
                                           hIon_YZ_plane->GetNbinsX(),hIon_YZ_plane->GetXaxis()->GetXmin(),hIon_YZ_plane->GetXaxis()->GetXmax(),
                                           hIon_YZ_plane->GetNbinsY(),hIon_YZ_plane->GetYaxis()->GetXmin(),hIon_YZ_plane->GetYaxis()->GetXmax());

        hIon_corrected_YZ_plane = new TH2D("hIon_corrected_YZ_plane","hIon_corrected_YZ_plane;Z(cm);Y(cm)",
                                           hIon_YZ_plane->GetNbinsX(),hIon_YZ_plane->GetXaxis()->GetXmin(),hIon_YZ_plane->GetXaxis()->GetXmax(),
                                           hIon_YZ_plane->GetNbinsY(),hIon_YZ_plane->GetYaxis()->GetXmin(),hIon_YZ_plane->GetYaxis()->GetXmax());


        hIon_X_dir       = new TH1D("hIon_X_dir","hIon_X_dir;X(cm)",50,xmin, xmax);
        hHitMap_X_dir    = new TH1D("hHitMap_X_dir","hHitMap_X_dir;X(cm)",
                                    hIon_X_dir->GetNbinsX(),hIon_X_dir->GetXaxis()->GetXmin(),hIon_X_dir->GetXaxis()->GetXmax());
        hMean_X_dir = new TH1D("hMean_X_dir","hMean_X_dir;X(cm)",
                               hIon_X_dir->GetNbinsX(),hIon_X_dir->GetXaxis()->GetXmin(),hIon_X_dir->GetXaxis()->GetXmax());
        hCorrectionMap_X_dir = new TH1D("hCorrectionMap_X_dir","hCorrectionMap_X_dir;X(cm)",
                                    hIon_X_dir->GetNbinsX(),hIon_X_dir->GetXaxis()->GetXmin(),hIon_X_dir->GetXaxis()->GetXmax());
        hIon_corrected_X_dir = new TH1D("hIon_corrected_X_dir","hIon_corrected_X_dir;X(cm)",
                                       hIon_X_dir->GetNbinsX(),hIon_X_dir->GetXaxis()->GetXmin(),hIon_X_dir->GetXaxis()->GetXmax());

        hUncorrected_YZ_plane = new TH1D("hUncorrected_YZ_plane","hUncorrected_YZ_plane;Z(cm);Y(cm)",1000,0,1000);
        hUncorrected_X_dir = new TH1D("hUncorrected_X_dir","hUncorrected_X_dir;X(cm)",100,0,1000);

        hSpatialCorr = new TH3D("hSpatialCorr","hSpatialCorr;X(cm);Y(cm);Z(cm)",
                                hIon_X_dir->GetNbinsX(),hIon_X_dir->GetXaxis()->GetXmin(),hIon_X_dir->GetXaxis()->GetXmax(),
                                hIon_YZ_plane->GetNbinsY(),hIon_YZ_plane->GetYaxis()->GetXmin(),hIon_YZ_plane->GetYaxis()->GetXmax(),
                                hIon_YZ_plane->GetNbinsX(),hIon_YZ_plane->GetXaxis()->GetXmin(),hIon_YZ_plane->GetXaxis()->GetXmax());

        std::cout << "initialized" << std::endl;
        return true;
    }
    //=====================================================
    bool Calibration_File::analyze(storage_manager* storage) {

        //const larlite::event_track  *track_v  = (larlite::event_track*)storage->get_data<larlite::event_track>("trackReco");
        const larlite::event_vertex *vertex_v = (larlite::event_vertex*)storage->get_data<larlite::event_vertex>("trackReco");

        run    = storage->run_id();
        subrun = storage->subrun_id();
        event  = storage->event_id();


        vtx_id = -1;

        // check if enry is empty
        if(vertex_v->size() == 0){return true;}

        larlite::event_track* ev_trk=nullptr;
        auto const& vtx_to_trk = storage->find_one_ass(vertex_v->id(), ev_trk, vertex_v->id().second);
        if(!ev_trk) throw larlite::DataFormatException("Could not find associated track data product!");

        for(int vertex_index=0;vertex_index<vertex_v->size();vertex_index++){
            vtx_id = vertex_index;
            selectedProton = false;
            selectedCosmic = false;

            SelectedVertex(run, subrun,event,vtx_id);

            //if(!selectedCosmic)continue;
            if(!selectedProton && !selectedCosmic)continue;

            if(thisVertex.size()!=0)thisVertex.clear();
            double sumLength = 0;
            for(auto const& trk_index : vtx_to_trk[vertex_index]) {
                thisVertex.push_back( (*ev_trk)[trk_index]);
                sumLength+=(*ev_trk)[trk_index].Length();
            }
            if(selectedCosmic){AllVertex_m.push_back(thisVertex);}
            if(selectedProton){AllVertex_p.push_back(thisVertex);}
            hSumLength->Fill(sumLength);
        }// vtx_id


        return true;

    }
    //=====================================================
    bool Calibration_File::finalize() {

        FillSpatialCalibHistos();
        Make2planesCorrectionMaps();
        Make3DCorrectionMap();
        MakedQdxCurves();

        RecombinationParameters->Fill();
        std::cout << "Bye World" << std::endl;
        return true;
    }
    //=====================================================
    void Calibration_File::LoadSelectionFile(){
        std::cout << "LoadSelectionFile : " << SelectionFileName << std::endl;
        TFile *fSelection = TFile::Open(Form("%s",SelectionFileName.c_str()),"READ");
        if(!(fSelection->IsOpen())){
            std::cout << "ERROR : could not open selection file!" << std::endl;
        }
        std::cout << "selection file opened" << std::endl;
        if(SelectEvtID_proton.size()!=0)SelectEvtID_proton.clear();
        if(SelectEvtID_cosmic.size()!=0)SelectEvtID_cosmic.clear();

        TTree *NuMuVertexVariable = (TTree*)fSelection->Get("NuMuVertexVariables");
        if(NuMuVertexVariable==nullptr)std::cout << "ERROR, no ttree loaded " << std::endl;
        std::cout << "ttree loaded with " <<NuMuVertexVariable->GetEntries() << " entries" << std::endl;


        int run_sel, subrun_sel,event_sel, vtxid_sel,NTracks_sel,N5cmTracks_sel,InFiducial_sel;
        int goodReco_sel;
        float cosmicLL_sel, Xreco_sel,Yreco_sel,Zreco_sel;
        std::vector<double> *TrackLength_v_sel = 0;
        std::vector<double> *dQdx_v_sel = 0;

        NuMuVertexVariable->SetBranchAddress("run",&run_sel);
        NuMuVertexVariable->SetBranchAddress("subrun",&subrun_sel);
        NuMuVertexVariable->SetBranchAddress("event",&event_sel);
        NuMuVertexVariable->SetBranchAddress("vtxid",&vtxid_sel);
        NuMuVertexVariable->SetBranchAddress("Good3DReco",&goodReco_sel);
        NuMuVertexVariable->SetBranchAddress("CosmicLL",&cosmicLL_sel);
        NuMuVertexVariable->SetBranchAddress("Xreco",&Xreco_sel);
        NuMuVertexVariable->SetBranchAddress("Yreco",&Yreco_sel);
        NuMuVertexVariable->SetBranchAddress("Zreco",&Zreco_sel);
        NuMuVertexVariable->SetBranchAddress("InFiducial",&InFiducial_sel);
        NuMuVertexVariable->SetBranchAddress("NTracks",&NTracks_sel);
        NuMuVertexVariable->SetBranchAddress("N5cmTracks",&N5cmTracks_sel);
        NuMuVertexVariable->SetBranchAddress("TrackLength_v",&TrackLength_v_sel);
        NuMuVertexVariable->SetBranchAddress("dQdx_v",&dQdx_v_sel);

        for(Long64_t i=0;i<NuMuVertexVariable->GetEntries();i++){
            NuMuVertexVariable->GetEntry(i);
            if(InFiducial_sel!=1 || goodReco_sel!=1 || N5cmTracks_sel!=2)continue;
            if(TrackLength_v_sel->size() != 2)continue;

            if(TrackLength_v_sel->at(0) > 35 && TrackLength_v_sel->at(1) > 35 /* remove tracks that could be Michels */
               && TrackLength_v_sel->at(0)+TrackLength_v_sel->at(1) > 200     /* sum of the track lengths are about the height of the detector */
               && cosmicLL_sel < 3){                                          /*remove non-cosmic-looking*/
                std::vector<int> info;
                info.push_back(run_sel);
                info.push_back(subrun_sel);
                info.push_back(event_sel);
                info.push_back(vtxid_sel);
                SelectEvtID_cosmic.push_back(info);
            }

            // select well reconstructed protons
            if(cosmicLL_sel > 4){
                std::vector<int> info;
                info.push_back(run_sel);
                info.push_back(subrun_sel);
                info.push_back(event_sel);
                info.push_back(vtxid_sel);
                SelectEvtID_proton.push_back(info);
            }
        }
        std::cout << SelectEvtID_cosmic.size() << " & " << SelectEvtID_proton.size() << std::endl;
        std::cout << "tree looped through" << std::endl;

    }
    //=====================================================
    void Calibration_File::GetSpatialCorrectionFactor(TVector3 point,geo::View_t view){
        int iBin = hSpatialCorr->FindBin(point.X(),point.Y(),point.Z());
        SpatialCalib = hSpatialCorr->GetBinContent(iBin);
    }
    //=====================================================
    void Calibration_File::DefineTreeBranches(){
        std::cout << "DefineTreeBranches" << std::endl;
        RecombinationParameters = new TTree("RecombinationParameters","RecombinationParameters");

        // what view is this calibration for?
        RecombinationParameters->Branch("view",&plane);
        // Recombination variables
        RecombinationParameters->Branch("alpha",&alpha_box);
        RecombinationParameters->Branch("beta",&beta_box);
        RecombinationParameters->Branch("C",&C_dqdx_calib);
        //error on calibration variables
        RecombinationParameters->Branch("sigma_alpha",&sigma_alpha_box);
        RecombinationParameters->Branch("sigma_beta",&sigma_beta_box);
        RecombinationParameters->Branch("sigma_C",&sigma_C_dqdx_calib);
    }
    //=====================================================
    void Calibration_File::Get_dEdx_Splines(){
        std::cout << "Get_dEdx_Splines" << std::endl;
        TFile *fSplines = TFile::Open(Form("%s",SplineFileName.c_str()),"READ");
        sMuonRange2dEdx   = (*(TSpline3*)fSplines->Get("sMuonRange2dEdx"));
        sProtonRange2dEdx = (*(TSpline3*)fSplines->Get("sProtonRange2dEdx"));
    }
    //=====================================================
    bool Calibration_File::InVolume(TVector3 point, TVector3 FVmargin){
        // ---------------------------------------------------------------------------------------#
        // Function uses detector active volume coordinates in conjuction with an input parameter
        // for distance from edge of active volume to define a fiducial volume and return 0/1 if
        // the given coordinates are contained in the so defined fiducial volume, default 10 cm
        // ---------------------------------------------------------------------------------------#

        if ( point.X()   < (xmax - FVmargin.X())
            && point.X() > (xmin + FVmargin.X())
            && point.Y() < (ymax - FVmargin.Y())
            && point.Y() > (ymin + FVmargin.Y())
            && point.Z() < (zmax - FVmargin.Z())
            && point.Z() > (zmin + FVmargin.Z()) )return true;
        else return false;
    }
    //=====================================================
    void Calibration_File::SelectedVertex(int _run, int _subrun, int _event, int _vtxid){
        selectedProton = false;
        selectedCosmic = false;
        for(size_t i = 0;i<SelectEvtID_cosmic.size();i++){
            if(_run!= SelectEvtID_cosmic[i][0])continue;
            if(_subrun!=SelectEvtID_cosmic[i][1])continue;
            if(_event!=SelectEvtID_cosmic[i][2])continue;
            if(_vtxid!=SelectEvtID_cosmic[i][3])continue;
            selectedCosmic=true;
        }

        for(size_t i = 0;i<SelectEvtID_proton.size();i++){
            if(_run!= SelectEvtID_proton[i][0])continue;
            if(_subrun!=SelectEvtID_proton[i][1])continue;
            if(_event!=SelectEvtID_proton[i][2])continue;
            if(_vtxid!=SelectEvtID_proton[i][3])continue;
            selectedProton=true;
        }

        return;
    }
    //=====================================================
    void Calibration_File::FillSpatialCalibHistos(){
        for(size_t ivertex = 0;ivertex<AllVertex_m.size();ivertex++){
            for(size_t itrack=0;itrack<AllVertex_m[ivertex].size();itrack++){
                for(size_t iNode = 0;iNode<AllVertex_m[ivertex][itrack].NumberTrajectoryPoints();iNode++){
                    if(AllVertex_m[ivertex][itrack].DQdxAtPoint(iNode,views[plane]) == 111)continue; // remove pixel values added for dead wire recovery

                    hIon_YZ_plane->Fill(   AllVertex_m[ivertex][itrack].LocationAtPoint(iNode).Z(),AllVertex_m[ivertex][itrack].LocationAtPoint(iNode).Y(),AllVertex_m[ivertex][itrack].DQdxAtPoint(iNode,views[plane]));
                    hHitMap_YZ_plane->Fill(AllVertex_m[ivertex][itrack].LocationAtPoint(iNode).Z(),AllVertex_m[ivertex][itrack].LocationAtPoint(iNode).Y());
                    hIon_X_dir->Fill(   AllVertex_m[ivertex][itrack].LocationAtPoint(iNode).X(),AllVertex_m[ivertex][itrack].DQdxAtPoint(iNode,views[plane]));
                    hHitMap_X_dir->Fill(AllVertex_m[ivertex][itrack].LocationAtPoint(iNode).X());
                    if(AllVertex_m[ivertex][itrack].DQdxAtPoint(iNode,views[plane])!=0)hUncorrected_YZ_plane->Fill(AllVertex_m[ivertex][itrack].DQdxAtPoint(iNode,views[plane]));
                    
                }
            }
        }
    }
    //=====================================================
    void Calibration_File::Make2planesCorrectionMaps(){
        int i_YZ = 0;
        int i_X = 0;

        //--------------------------------------------------
        // Get averaged values, per cell and the global ones
        //--------------------------------------------------
        for(int i=0;i<hIon_YZ_plane->GetNbinsX();i++){
            for(int j=0;j<hIon_YZ_plane->GetNbinsY();j++){
                if(hHitMap_YZ_plane->GetBinContent(i+1,j+1) < 10)continue;
                hMean_YZ_plane->SetBinContent(i+1,j+1,hIon_YZ_plane->GetBinContent(i+1,j+1)/hHitMap_YZ_plane->GetBinContent(i+1,j+1));
                i_YZ++;
                avg_Ion_YZ += hIon_YZ_plane->GetBinContent(i+1,j+1)/hHitMap_YZ_plane->GetBinContent(i+1,j+1);
            }
        }
        for(int i = 0;i<hIon_X_dir->GetNbinsX();i++){
            if(hHitMap_X_dir->GetBinContent(i+1) < 10)continue;
            hMean_X_dir->SetBinContent(i+1,hIon_X_dir->GetBinContent(i+1)/hHitMap_X_dir->GetBinContent(i+1));
            i_X++;
            avg_Ion_X+= hIon_X_dir->GetBinContent(i+1)/hHitMap_X_dir->GetBinContent(i+1);
        }

        avg_Ion_YZ/=i_YZ;
        avg_Ion_X/=i_X;

        //-----------------------------------------------------------------
        // estimate the correction factors based on the found avraged value
        //-----------------------------------------------------------------
        for(int i=0;i<hMean_YZ_plane->GetNbinsX();i++){
            for(int j=0;j<hMean_YZ_plane->GetNbinsY();j++){
                if(hMean_YZ_plane->GetBinContent(i+1,j+1)==0){hCorrectionMap_YZ_plane->SetBinContent(i+1,j+1,1);continue;}
                hCorrectionMap_YZ_plane->SetBinContent(i+1,j+1,avg_Ion_YZ/hMean_YZ_plane->GetBinContent(i+1,j+1));
            }
        }

        for(int i = 0;i<hMean_X_dir->GetNbinsX();i++){
            if(hMean_X_dir->GetBinContent(i+1) == 0)continue;
            hCorrectionMap_X_dir->SetBinContent(i+1,avg_Ion_X/hMean_X_dir->GetBinContent(i+1));
        }

        for(int i=0;i<hSpatialCorr->GetNbinsX();i++){
            for(int j=0;j<hSpatialCorr->GetNbinsY();j++){
                for(int k=0;k<hSpatialCorr->GetNbinsZ();k++){
                    hSpatialCorr->SetBinContent(i+1,j+1,k+1, hCorrectionMap_X_dir->GetBinContent(i+1)*hCorrectionMap_YZ_plane->GetBinContent(k+1,j+1));
                }
            }
        }

        //------------------------
        // correct ionization maps
        //------------------------
        for(int i=0;i<hCorrectionMap_YZ_plane->GetNbinsX();i++){
            for(int j=0;j<hCorrectionMap_YZ_plane->GetNbinsY();j++){
                hIon_corrected_YZ_plane->SetBinContent(i+1,j+1,hIon_YZ_plane->GetBinContent(i+1,j+1)*hCorrectionMap_YZ_plane->GetBinContent(i+1,j+1));
            }
        }

        for(int i = 0;i<hMean_X_dir->GetNbinsX();i++){
           hIon_corrected_X_dir->SetBinContent(i+1,hIon_X_dir->GetBinContent(i+1)*hCorrectionMap_X_dir->GetBinContent(i+1));
        }


    }
    //=====================================================
    void Calibration_File::MakedQdxCurves(){
        hUncorrecteddQdx = new TH2D("hUncorrecteddQdx","Uncorrected, uncalibrated;res. Length (cm);uncalibrated dQdx (A.U.)",400,0,400,300,0,300);
        hCorrecteddQdx   = new TH2D("hCorrecteddQdx","Corrected, uncalibrated;res. Length (cm);uncalibrated dQdx (A.U.)"    ,400,0,400,300,0,300);
        hRecombination_raw = new TH2D("hRecombination_raw","hRecombination_raw;expected dEdx;uncalibrates, uncorrected dQdx",100,0,15,100,0,300);

        double avg_ion[2]={0,0};
        for(size_t ivertex=0; ivertex<AllVertex_p.size();ivertex++){
            for(size_t itrack=0;itrack<AllVertex_p[ivertex].size();itrack++){
                for(size_t iNode=0;iNode<AllVertex_p[ivertex][itrack].NumberTrajectoryPoints();iNode++){
                    double raw_ion = AllVertex_p[ivertex][itrack].DQdxAtPoint(iNode,views[plane]);
                    TVector3 point = AllVertex_p[ivertex][itrack].LocationAtPoint(iNode);
                    double correction = hCorrectionMap3D->GetBinContent(hCorrectionMap3D->FindBin(point.X(),point.Y(),point.Z()));

                    //double corrected_ion = raw_ion*hSpatialCorr->GetBinContent(hSpatialCorr->FindBin(point.X(),point.Y(),point.Z()));
                    double corrected_ion = raw_ion*correction;
                    /*std::cout << "raw_ion " << raw_ion << std::endl;
                    std::cout << "correction " << correction << std::endl;
                    std::cout << "corrected_ion " << corrected_ion << std::endl;
                    std::cout << "avg_Ion_3D " << avg_Ion_3D << std::endl;*/
                    double reslength = AllVertex_p[ivertex][itrack].Length(iNode);
                    if(raw_ion <= 0 || raw_ion == 111)continue;
                    avg_ion[itrack]+=raw_ion;
                    hUncorrecteddQdx->Fill(reslength , raw_ion);
                    hCorrecteddQdx->Fill(reslength, corrected_ion);
                }
                avg_ion[itrack]/=AllVertex_p[ivertex][itrack].NumberTrajectoryPoints();
            }
            //find only the proton track
            int kProton = 0;
            if(AllVertex_p[ivertex][0].Length() < 5 || AllVertex_p[ivertex][1].Length() < 5)continue;
            if(AllVertex_p[ivertex][0].Length() > AllVertex_p[ivertex][1].Length() || avg_ion[0] < avg_ion[1]){kProton=1;}
            for(size_t iNode=0;iNode<AllVertex_p[ivertex][kProton].NumberTrajectoryPoints();iNode++){
                double raw_ion = AllVertex_p[ivertex][kProton].DQdxAtPoint(iNode,views[plane]);
                double reslength = AllVertex_p[ivertex][kProton].Length(iNode);
                if(raw_ion <= 0 || raw_ion == 111 || reslength < 2)continue;
                hRecombination_raw->Fill(sProtonRange2dEdx.Eval(reslength),raw_ion);

            }
        }
    }
    //=====================================================
    void Calibration_File::Make3DCorrectionMap(){
        int Nx(10), Ny(10), Nz(10);
        hHitMap3D = new TH3D("hHitMap3D","hHitMap3D",Nx,xmin, xmax, Ny,ymin, ymax, Nz, zmin, zmax);
        hRawCharge3D = new TH3D("hRawCharge3D","hRawCharge3D",Nx,xmin, xmax, Ny,ymin, ymax, Nz, zmin, zmax);
        hMeanCharge3D = new TH3D("hMeanCharge3D","hMeanCharge3D",Nx,xmin, xmax, Ny,ymin, ymax, Nz, zmin, zmax);
        hCorrectionMap3D = new TH3D("hCorrectionMap3D","hCorrectionMap3D",Nx,xmin, xmax, Ny,ymin, ymax, Nz, zmin, zmax);

        for(size_t ivertex = 0;ivertex<AllVertex_m.size();ivertex++){
            for(size_t itrack=0;itrack<AllVertex_m[ivertex].size();itrack++){
                for(size_t iNode = 0;iNode<AllVertex_m[ivertex][itrack].NumberTrajectoryPoints();iNode++){
                    if(AllVertex_m[ivertex][itrack].DQdxAtPoint(iNode,views[plane]) == 111)continue; // remove pixel values added for dead wire recovery
                    if(AllVertex_m[ivertex][itrack].DQdxAtPoint(iNode,views[plane]) == 0  )continue; // remove empty points
                    TVector3 point = AllVertex_m[ivertex][itrack].LocationAtPoint(iNode);

                    hRawCharge3D->Fill(point.X(),point.Y(),point.Z(), AllVertex_m[ivertex][itrack].DQdxAtPoint(iNode,views[plane]));
                    hHitMap3D->Fill(point.X(),point.Y(),point.Z());

                }
            }
        }

        avg_Ion_3D=0;
        int nentries=0;
        for(int iX = 0;iX < hRawCharge3D->GetNbinsX();iX++){
            for(int iY = 0;iY < hRawCharge3D->GetNbinsY();iY++){
                for(int iZ = 0;iZ < hRawCharge3D->GetNbinsZ();iZ++){
                    if(hHitMap3D->GetBinContent(iX+1,iY+1,iZ+1)==0)continue;
                    std::cout << "hHitMap3D : " << hHitMap3D->GetBinContent(iX+1,iY+1,iZ+1) << std::endl;
                    hMeanCharge3D->SetBinContent(iX+1,iY+1,iZ+1, hRawCharge3D->GetBinContent(iX+1,iY+1,iZ+1)/hHitMap3D->GetBinContent(iX+1,iY+1,iZ+1));
                }
            }
        }

        for(int iX = 0;iX < hRawCharge3D->GetNbinsX();iX++){
            for(int iY = 0;iY < hRawCharge3D->GetNbinsY();iY++){
                for(int iZ = 0;iZ < hRawCharge3D->GetNbinsZ();iZ++){
                    avg_Ion_3D+=hMeanCharge3D->GetBinContent(iX+1,iY+1,iZ+1);
                    nentries++;
                }
            }
        }
        if(nentries!=0)avg_Ion_3D/=nentries;
        std::cout << "avg_Ion_3D : " << avg_Ion_3D << std::endl;

        for(int iX = 0;iX < hMeanCharge3D->GetNbinsX();iX++){
            for(int iY = 0;iY < hMeanCharge3D->GetNbinsY();iY++){
                for(int iZ = 0;iZ < hMeanCharge3D->GetNbinsZ();iZ++){
                    hCorrectionMap3D->SetBinContent(iX+1,iY+1,iZ+1,avg_Ion_3D/hMeanCharge3D->GetBinContent(iX+1,iY+1,iZ+1));
                }
            }
        }

    }
    //=====================================================

}
#endif
