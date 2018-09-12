#ifndef LARLITE_CALIBRATIONFILE_CXX
#define LARLITE_CALIBRATIONFILE_CXX

#include "Calibration_File.h"
#include "TVector3.h"
#include "TRandom3.h"
#include "TLine.h"
#include "TLatex.h"
#include "TH2D.h"
#include "TVirtualFitter.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TProfile.h"
#include "DataFormat/hit.h"
#include "DataFormat/track.h"
#include "DataFormat/vertex.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryHelper.h"
#include <iostream>
#include <fstream>
#include <vector>

double RecombinationFunc(double *x, double *par){
    double dqdx=0;
    double alpha = par[0];
    double beta = par[1];
    double C = par[2];
    double W = par[3];
    double rho = par[4];
    double epsilon = par[5];
    double dedx = x[0];
    dqdx = (1./C)*((rho*epsilon)/(beta*W))*log(dedx*(beta/(rho*epsilon))+alpha);
    return dqdx;
}
double dEdxFunc(double *x, double *par){
    double dedx=0;
    double alpha = par[0];
    double beta = par[1];
    double C = par[2];
    double W = par[3];
    double rho = par[4];
    double epsilon = par[5];
    double dqdx = x[0];
    dedx = ((rho*epsilon)/beta)*(exp(C*dqdx*((beta*W)/(rho*epsilon)))-alpha);
    return dedx;
}
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

        alpha_expect = 0.93;
        beta_expect = 0.212;
        C_dqdx_expect = 1000;

        Wion = 23.6e-6;// MeV (i.e. 23.6eV = ionization energy)
        Efield = 0.273;// kV per cm
        LArDensity = 1.38;

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

        isdEdxReady=false;

        DefineTreeBranches();
        Get_dEdx_Splines();
        LoadSelectionFile();

        hSumLength = new TH1D("hSumLength","hSumLength;L_{1}+L_{2}",100,0,1000);
        hUncorrecteddQdx = new TH2D("hUncorrecteddQdx","Uncorrected, uncalibrated;res. Length (cm);uncalibrated dQdx (A.U.)",400,0,400,300,0,300);
        hCorrecteddQdx   = new TH2D("hCorrecteddQdx","Corrected, uncalibrated;res. Length (cm);calibrated dQdx (e^{-}/cm)"    ,400,0,400,300,0,300000);
        hCalibrateddEdx   = new TH2D("hCalibrateddEdx","Corrected, Calibrated;res. Length (cm);Calibrated dE/dx (MeV/cm)"      ,400,0,400,300,0,30);

        hCalibrateddEdx_p   = new TH2D("hCalibrateddEdx_p","protons, Calibrated;res. Length (cm);Calibrated dE/dx (MeV/cm)"      ,400,0,400,300,0,30);
        hCalibrateddEdx_m   = new TH2D("hCalibrateddEdx_m","muons, Calibrated;res. Length (cm);Calibrated dE/dx (MeV/cm)"      ,400,0,400,300,0,30);

        hRecombination_raw = new TH2D("hRecombination_raw","hRecombination_raw;expected dEdx;uncalibrated, uncorrected dQdx",100,0,15,100,0,300);
        hRecombination_cor = new TH2D("hRecombination_cor","hRecombination_cor;expected dEdx (MeV/cm);corrected dQdx (e^{-}/cm)"  ,60,0,15,50,0,500000);

        fRecombinationExpected = new TF1("fRecombinationExpected",RecombinationFunc,0,1000,6);
        fRecombinationExpected->SetParameters(alpha_expect,beta_expect,1,Wion,LArDensity,Efield);
        fRecombinationExpected->SetParNames("alpha","beta","C","W_{ion}","#rho_{LAr}","E_{field}");
        fRecombinationExpected->FixParameter(0, alpha_expect);
        fRecombinationExpected->FixParameter(1, beta_expect);
        fRecombinationExpected->FixParameter(3, Wion);
        fRecombinationExpected->FixParameter(4,LArDensity);
        fRecombinationExpected->FixParameter(5,Efield);
        std::cout << "Expected dQdx for MIP : " << fRecombinationExpected->Eval(2.104) << std::endl;

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
            if(selectedCosmic){AllVertex_m.push_back(thisVertex);hSumLength->Fill(sumLength);}
            if(selectedProton){AllVertex_p.push_back(thisVertex);}

        }// vtx_id


        return true;

    }
    //=====================================================
    bool Calibration_File::finalize() {
        std::cout << "finalize()" << std::endl;

        Make3DCorrectionMap();
        MakedQdxCurves();
        FitRecombinationPlot();
        MakedQdxCurves();
        TCanvas *cdEdx = new TCanvas("cdEdx","cdEdx",800,600);
        hCalibrateddEdx->Draw("colz");
        sProtonRange2dEdx->Draw("same");
        sMuonRange2dEdx->Draw("same");

        RecombinationParameters->Fill();

        std::cout << "About to save histograms" << std::endl;
        _fout->cd();
        RecombinationParameters->Write();
        //hSpatialCorr->Write();
        hSumLength->Write();
        //hIon_YZ_plane->Write();
        //hIon_corrected_YZ_plane->Write();
        //hHitMap_YZ_plane->Write();
        //hMean_YZ_plane->Write();
        //hCorrectionMap_YZ_plane->Write();
        //hIon_X_dir->Write();
        //hIon_corrected_X_dir->Write();
        //hHitMap_X_dir->Write();
        //hMean_X_dir->Write();
        //hCorrectionMap_X_dir->Write();
        hUncorrected_dQdx_1D->Write();
        hCorrected_dQdx_1D->Write();
        hRecombination_raw->Write();
        hRecombination_cor->Write();
        hUncorrecteddQdx->Write();
        hCorrecteddQdx->Write();
        hCalibrateddEdx->Write();
        hCalibrateddEdx_p->Write();
        hCalibrateddEdx_m->Write();
        hHitMap3D->Write();
        hRawCharge3D->Write();
        hMeanCharge3D->Write();
        hCorrectionMap3D->Write();
        fRecombination->Write();
        fdEdxConvert->Write();
        sProtonRange2dEdx->Write();
        sMuonRange2dEdx->Write();

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


        int run_sel, subrun_sel,event_sel, vtxid_sel,NTracks_sel,N5cmTracks_sel,InFiducial_sel,PassCuts_sel;
        int goodReco_sel;
        float cosmicLL_sel, Xreco_sel,Yreco_sel,Zreco_sel, WallDist_sel;
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
        NuMuVertexVariable->SetBranchAddress("PassCuts",&PassCuts_sel);
        NuMuVertexVariable->SetBranchAddress("WallDist",&WallDist_sel);


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
            if(cosmicLL_sel > 4 && PassCuts_sel == 1){
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
        sMuonRange2dEdx   = (TSpline3*)fSplines->Get("sMuonRange2dEdx");
        sProtonRange2dEdx = (TSpline3*)fSplines->Get("sProtonRange2dEdx");
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
    void Calibration_File::MakedQdxCurves(){
        std::cout << "MakedQdxCurves()" << std::endl;
        if(hUncorrecteddQdx->GetEntries()!=0)hUncorrecteddQdx->Reset();
        if(hCorrecteddQdx->GetEntries()!=0)hCorrecteddQdx->Reset();
        if(hCalibrateddEdx->GetEntries()!=0)hCalibrateddEdx->Reset();
        if(hCalibrateddEdx_m->GetEntries()!=0)hCalibrateddEdx_m->Reset();
        if(hCalibrateddEdx_p->GetEntries()!=0)hCalibrateddEdx_p->Reset();
        if(hRecombination_raw->GetEntries()!=0)hRecombination_raw->Reset();
        if(hRecombination_cor->GetEntries()!=0)hRecombination_cor->Reset();

        double avg_ion[2]={0,0};
        for(size_t ivertex=0; ivertex<AllVertex_p.size();ivertex++){
            for(size_t itrack=0;itrack<AllVertex_p[ivertex].size();itrack++){
                for(size_t iNode=0;iNode<AllVertex_p[ivertex][itrack].NumberTrajectoryPoints();iNode++){
                    double raw_ion = AllVertex_p[ivertex][itrack].DQdxAtPoint(iNode,views[plane]);
                    TVector3 point = AllVertex_p[ivertex][itrack].LocationAtPoint(iNode);
                    double correction = hCorrectionMap3D->GetBinContent(hCorrectionMap3D->FindBin(point.X(),point.Y(),point.Z()));
                    double corrected_ion = raw_ion*correction;
                    double dEdx = corrected_ion;
                    if(isdEdxReady)dEdx = fdEdxConvert->Eval(corrected_ion);
                    double reslength = AllVertex_p[ivertex][itrack].Length(iNode);
                    if(reslength < 5)continue;
                    if(raw_ion <= 0 || raw_ion == 111)continue;
                    avg_ion[itrack]+=raw_ion;
                    hUncorrecteddQdx->Fill(reslength , raw_ion);
                    if(corrected_ion < 1000)continue;
                    hCorrecteddQdx->Fill(reslength, corrected_ion);
                    if(dEdx < 0.5)continue;
                    hCalibrateddEdx->Fill(reslength,dEdx);
                }
                avg_ion[itrack]/=AllVertex_p[ivertex][itrack].NumberTrajectoryPoints();
            }
            //find only the proton track
            int kProton = 0;
            int kMuon = 1;
            if(AllVertex_p[ivertex][kProton].Length() < 5 || AllVertex_p[ivertex][kMuon].Length() < 5)continue;
            if(AllVertex_p[ivertex][kProton].Length() > AllVertex_p[ivertex][kMuon].Length())continue;
            if(avg_ion[kProton] < avg_ion[kMuon])continue;
            for(size_t iNode=0;iNode<AllVertex_p[ivertex][kProton].NumberTrajectoryPoints();iNode++){
                double raw_ion = AllVertex_p[ivertex][kProton].DQdxAtPoint(iNode,views[plane]);
                TVector3 point = AllVertex_p[ivertex][kProton].LocationAtPoint(iNode);
                double correction = hCorrectionMap3D->GetBinContent(hCorrectionMap3D->FindBin(point.X(),point.Y(),point.Z()));
                double corrected_ion = raw_ion*correction;
                double dEdx = corrected_ion;
                if(isdEdxReady)dEdx = fdEdxConvert->Eval(corrected_ion);
                double reslength = AllVertex_p[ivertex][kProton].Length(iNode);
                if(raw_ion <= 0)continue;
                if(raw_ion == 111)continue;
                if(reslength < 2)continue;
                if(raw_ion*correction < 2000)continue;
                hRecombination_raw->Fill(sProtonRange2dEdx->Eval(reslength),raw_ion);
                hRecombination_cor->Fill(sProtonRange2dEdx->Eval(reslength),corrected_ion);
                hCalibrateddEdx_p->Fill(reslength, dEdx);
            }
            for(size_t iNode=0;iNode<AllVertex_p[ivertex][kMuon].NumberTrajectoryPoints();iNode++){
                double raw_ion = AllVertex_p[ivertex][kMuon].DQdxAtPoint(iNode,views[plane]);
                TVector3 point = AllVertex_p[ivertex][kMuon].LocationAtPoint(iNode);
                double correction = hCorrectionMap3D->GetBinContent(hCorrectionMap3D->FindBin(point.X(),point.Y(),point.Z()));
                double corrected_ion = raw_ion*correction;
                double dEdx = corrected_ion;
                if(isdEdxReady)dEdx = fdEdxConvert->Eval(corrected_ion);
                double reslength = AllVertex_p[ivertex][kMuon].Length(iNode);
                if(raw_ion <= 0)continue;
                if(raw_ion == 111)continue;
                if(reslength < 2)continue;
                if(raw_ion*correction < 2000)continue;
                hCalibrateddEdx_m->Fill(reslength,dEdx);

            }
        }
    }
    //=====================================================
    void Calibration_File::Make3DCorrectionMap(){
        std::cout << "Make3DCorrectionMap()" << std::endl;
        const int Nx(5), Ny(10), Nz(50);
        TH1D* hRawChargeCell[Nx][Ny][Nz];
        for(int ix=0;ix<Nx;ix++){
            for(int iy=0;iy<Ny;iy++){
                for(int iz=0;iz<Nz;iz++){
                    hRawChargeCell[ix][iy][iz] = new TH1D(Form("hRawChargeCell_%02d_%02d_%02d",ix,iy,iz),Form("hRawChargeCell_%02d_%02d_%02d",ix,iy,iz),100,0,200);
                }
            }
        }

        //TCanvas *cCells= new TCanvas("cCells","cCells",800,600);
        hHitMap3D = new TH3D("hHitMap3D","hHitMap3D",Nx,xmin, xmax, Ny,ymin, ymax, Nz, zmin, zmax);
        hRawCharge3D = new TH3D("hRawCharge3D","hRawCharge3D",Nx,xmin, xmax, Ny,ymin, ymax, Nz, zmin, zmax);
        hMeanCharge3D = new TH3D("hMeanCharge3D","hMeanCharge3D",Nx,xmin, xmax, Ny,ymin, ymax, Nz, zmin, zmax);
        hCorrectionMap3D = new TH3D("hCorrectionMap3D","hCorrectionMap3D",Nx,xmin, xmax, Ny,ymin, ymax, Nz, zmin, zmax);

        hUncorrected_dQdx_1D = new TH1D("hUncorrected_dQdx_1D","hUncorrected_dQdx_1D;dQdx",1000,0,1000);
        hCorrected_dQdx_1D = new TH1D("hCorrected_dQdx_1D","hCorrected_dQdx_1D;dQdx",1000,0,-1);hCorrected_dQdx_1D->SetLineColor(2);

        for(size_t ivertex = 0;ivertex<AllVertex_m.size();ivertex++){
            for(size_t itrack=0;itrack<AllVertex_m[ivertex].size();itrack++){
                for(size_t iNode = 0;iNode<AllVertex_m[ivertex][itrack].NumberTrajectoryPoints();iNode++){
                    if(AllVertex_m[ivertex][itrack].DQdxAtPoint(iNode,views[plane]) == 111)continue; // remove pixel values added for dead wire recovery
                    if(AllVertex_m[ivertex][itrack].DQdxAtPoint(iNode,views[plane]) >  75)continue; // remove pixel from obviously non MIPs parts
                    if(AllVertex_m[ivertex][itrack].DQdxAtPoint(iNode,views[plane]) == 0  )continue; // remove empty points
                    if(AllVertex_m[ivertex][itrack].Length(iNode) < 20)continue; // remove points near Bragg peak

                    TVector3 point = AllVertex_m[ivertex][itrack].LocationAtPoint(iNode);
                    if(point.X() < xmin || point.X() > xmax)continue;
                    if(point.Y() < ymin || point.Y() > ymax)continue;
                    if(point.Z() < zmin || point.Z() > zmax)continue;
                    if(AllVertex_m[ivertex][itrack].DQdxAtPoint(iNode,views[plane])!=0)hUncorrected_dQdx_1D->Fill(AllVertex_m[ivertex][itrack].DQdxAtPoint(iNode,views[plane]));
                    hRawCharge3D->Fill(point.X(),point.Y(),point.Z(), AllVertex_m[ivertex][itrack].DQdxAtPoint(iNode,views[plane]));
                    hHitMap3D->Fill(point.X(),point.Y(),point.Z());
                    int binx = (int)((point.X()-xmin)*(Nx/(xmax-xmin)));
                    int biny = (int)((point.Y()-ymin)*(Ny/(ymax-ymin)));
                    int binz = (int)((point.Z()-zmin)*(Nz/(zmax-zmin)));
                    //if(binx>=Nx)binx=Nx-1;
                    //if(biny>=Ny)biny=Ny-1;
                    //if(binz>=Nz)binz=Nz-1;
                    if(binx>=Nx)std::cout << xmin << " : " << point.X() << " : " << xmax << std::endl;
                    if(biny>=Ny)std::cout << ymin << " : " << point.Y() << " : " << ymax << std::endl;
                    if(binz>=Nz)std::cout << zmin << " : " << point.Z() << " : " << zmax << std::endl;
                    //std::cout << binx << "/" << Nx <<"\t" << biny<<"/"<<Ny << "\t" <<binz<<"/" << Nz<< std::endl;
                    hRawChargeCell[binx][biny][binz]->Fill(AllVertex_m[ivertex][itrack].DQdxAtPoint(iNode,views[plane]));
                }
            }
        }

        avg_Ion_3D=0;
        int nentries=0;
        /*for(int iX = 0;iX < hRawCharge3D->GetNbinsX();iX++){
            for(int iY = 0;iY < hRawCharge3D->GetNbinsY();iY++){
                for(int iZ = 0;iZ < hRawCharge3D->GetNbinsZ();iZ++){
                    if(hHitMap3D->GetBinContent(iX+1,iY+1,iZ+1)<=10)continue;
                    //std::cout << "hHitMap3D : " << hHitMap3D->GetBinContent(iX+1,iY+1,iZ+1) << std::endl;
                    hMeanCharge3D->SetBinContent(iX+1,iY+1,iZ+1, hRawCharge3D->GetBinContent(iX+1,iY+1,iZ+1)/hHitMap3D->GetBinContent(iX+1,iY+1,iZ+1));
                }
            }
        }*/

        for(int ix=0;ix<Nx;ix++){
            for(int iy=0;iy<Ny;iy++){
                for(int iz=0;iz<Nz;iz++){
                    //hRawChargeCell[ix][iy][iz]->Draw();cCells->Modified();cCells->Update();cCells->SaveAs(Form("%s.pdf",hRawChargeCell[ix][iy][iz]->GetTitle()));
                    if(hRawChargeCell[ix][iy][iz]->GetEntries()<10){hMeanCharge3D->SetBinContent(ix+1,iy+1,iz+1,0);continue;}
                    else hMeanCharge3D->SetBinContent(ix+1,iy+1,iz+1,hRawChargeCell[ix][iy][iz]->GetBinCenter(hRawChargeCell[ix][iy][iz]->GetMaximumBin()));
                }
            }
        }

        for(int iX = 0;iX < hRawCharge3D->GetNbinsX();iX++){
            for(int iY = 0;iY < hRawCharge3D->GetNbinsY();iY++){
                for(int iZ = 0;iZ < hRawCharge3D->GetNbinsZ();iZ++){
                    if(hMeanCharge3D->GetBinContent(iX+1,iY+1,iZ+1) <= 0 || hMeanCharge3D->GetBinContent(iX+1,iY+1,iZ+1) > 200)continue;
                    avg_Ion_3D+=hMeanCharge3D->GetBinContent(iX+1,iY+1,iZ+1);
                    nentries++;
                }
            }
        }
        if(nentries!=0)avg_Ion_3D/=nentries;
        std::cout << "avg_Ion_3D : " << avg_Ion_3D << std::endl;
        std::cout << "Expected dQdx for MIP : " << fRecombinationExpected->Eval(2.104) << std::endl;
        avg_Ion_3D = fRecombinationExpected->Eval(2.104);

        for(int iX = 0;iX < hMeanCharge3D->GetNbinsX();iX++){
            for(int iY = 0;iY < hMeanCharge3D->GetNbinsY();iY++){
                for(int iZ = 0;iZ < hMeanCharge3D->GetNbinsZ();iZ++){
                    if(hMeanCharge3D->GetBinContent(iX+1,iY+1,iZ+1)<=0){hCorrectionMap3D->SetBinContent(iX+1,iY+1,iZ+1,1);continue;}
                    hCorrectionMap3D->SetBinContent(iX+1,iY+1,iZ+1,avg_Ion_3D/hMeanCharge3D->GetBinContent(iX+1,iY+1,iZ+1));
                }
            }
        }

        for(size_t ivertex = 0;ivertex<AllVertex_m.size();ivertex++){
            for(size_t itrack=0;itrack<AllVertex_m[ivertex].size();itrack++){
                for(size_t iNode = 0;iNode<AllVertex_m[ivertex][itrack].NumberTrajectoryPoints();iNode++){
                    if(AllVertex_m[ivertex][itrack].DQdxAtPoint(iNode,views[plane]) == 111)continue; // remove pixel values added for dead wire recovery
                    if(AllVertex_m[ivertex][itrack].DQdxAtPoint(iNode,views[plane]) == 0  )continue; // remove empty points
                    if(AllVertex_m[ivertex][itrack].Length(iNode) < 20)continue; // remove points near Bragg peak
                    TVector3 point = AllVertex_m[ivertex][itrack].LocationAtPoint(iNode);
                    double correction = hCorrectionMap3D->GetBinContent(hCorrectionMap3D->FindBin(point.X(),point.Y(),point.Z()));
                    //std::cout << correction << "\t" << AllVertex_m[ivertex][itrack].DQdxAtPoint(iNode,views[plane])*correction << std::endl;
                    if(correction==0)correction=1;
                    hCorrected_dQdx_1D->Fill(AllVertex_m[ivertex][itrack].DQdxAtPoint(iNode,views[plane])*correction);
                }
            }
        }
        std::cout << "Expected dQdx for MIP : " << fRecombinationExpected->Eval(2.104) << std::endl;

    }
    //=====================================================
    void Calibration_File::FitRecombinationPlot(){
        std::cout << "FitRecombinationPlot()" << std::endl;
        fRecombination = new TF1("fRecombination",RecombinationFunc,0,1000,6);
        fRecombination->SetParameters(alpha_expect,beta_expect,1,Wion,LArDensity,Efield);
        fRecombination->SetParNames("alpha","beta","C","W_{ion}","#rho_{LAr}","E_{field}");
        //fRecombination->SetParLimits(0,0.5,1.5);
        //fRecombination->SetParLimits(1,0,0.5);
        //fRecombination->SetParLimits(2,0.5,1.5);

        //fRecombination->FixParameter(0, alpha_expect);
        //fRecombination->FixParameter(1,beta_expect);
        fRecombination->FixParameter(2,1);
        fRecombination->FixParameter(3, Wion);
        fRecombination->FixParameter(4,LArDensity);
        fRecombination->FixParameter(5,Efield);


        TCanvas *cRecombination = new TCanvas("cRecombination","cRecombination",800,600);
        TF1 *fgaus = new TF1("fgaus","gaus(0)",0,1000);
        fgaus->SetParameters(100,200,20,10,200,100);

        TProfile *P = hRecombination_cor->ProfileX("P");
        P->Fit(fRecombination,"re","e",2.75,15);
        hRecombination_cor->Draw("colz");

        TH1D *hint = new TH1D("hint",".68 conf.band", hRecombination_cor->GetNbinsX(),hRecombination_cor->GetXaxis()->GetXmin(),hRecombination_cor->GetXaxis()->GetXmax());
        (TVirtualFitter::GetFitter())->GetConfidenceIntervals(hint,0.68);
        hint->SetStats(kFALSE);
        hint->SetFillColor(2);
        hint->SetFillStyle(3001);
        hint->Draw("e3 same");
        fRecombination->Draw("same");
        P->SetLineColor(1);
        P->SetMarkerStyle(20);
        P->Draw("sames");
        //fRecombinationExpected->SetParameter(2,fRecombination->GetParameter(2));
        fRecombinationExpected->SetLineColor(4);
        fRecombinationExpected->Draw("same");

        alpha_box = fRecombination->GetParameter(0);
        beta_box = fRecombination->GetParameter(1);
        C_dqdx_calib = fRecombination->GetParameter(2);
        sigma_alpha_box = fRecombination->GetParError(0);
        sigma_beta_box = fRecombination->GetParError(1);
        sigma_C_dqdx_calib = fRecombination->GetParError(2);

        fdEdxConvert = new TF1("fdEdxConvert",dEdxFunc,0,1000000,6);
        fdEdxConvert->SetParameters(alpha_box,beta_box,C_dqdx_calib,Wion,LArDensity,Efield);
        isdEdxReady=true;
    }
    //=====================================================

}
#endif
