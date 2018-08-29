#ifndef LARLITE_CONVERTDEDX_CXX
#define LARLITE_CONVERTDEDX_CXX

#include "ConvertdEdx.h"
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
#include "RooDouble.h"

namespace larlite {

    bool ConvertdEdx::initialize() {

        std::cout << "Hello World" << std::endl;


        views[0] = larlite::geo::kU; views[1] = larlite::geo::kV; views[2] = larlite::geo::kZ;

        useTruncateddQdx=true;
        alpha_box = 1;
        beta_box = 1;
        C_dqdx_calib = 1;
        SpatialCalib = 1;
        
        Wion = 23.6e-6;// MeV (i.e. 23.6eV = ionization energy)
        Efield = 0.273;// kV per cm

        DefineTreeBranches();
        Get_dEdx_Splines();

        //hdEdxVresLength = new TH2D("hdEdxVresLength","hdEdxVresLength",200,0,-200,500,0,-10);
        LoadCalibrationFile();
        return true;
    }
    //=====================================================
    bool ConvertdEdx::analyze(storage_manager* storage) {

        const larlite::event_track  *track_v  = (larlite::event_track*)storage->get_data<larlite::event_track>("trackReco");
        const larlite::event_vertex *vertex_v = (larlite::event_vertex*)storage->get_data<larlite::event_vertex>("trackReco");

        run    = storage->run_id();
        subrun = storage->subrun_id();
        event  = storage->event_id();

        if(Avg_dEdx.size()!=0     ){Avg_dEdx.clear();     }
        if(Avg_dEdx_5cm.size()!=0 ){Avg_dEdx_5cm.clear(); }
        if(Avg_dEdx_10cm.size()!=0){Avg_dEdx_10cm.clear();}

        if(Avg_corrdQdx.size()!=0     ){Avg_corrdQdx.clear();     }
        if(Avg_corrdQdx_5cm.size()!=0 ){Avg_corrdQdx_5cm.clear(); }
        if(Avg_corrdQdx_10cm.size()!=0){Avg_corrdQdx_10cm.clear();}

        if(chi2_p_hypothesis.size()!=0){chi2_p_hypothesis.clear();}
        if(chi2_m_hypothesis.size()!=0){chi2_m_hypothesis.clear();}
        if(chi2_invert_p_hypothesis.size() !=0){chi2_invert_p_hypothesis.clear();}
        if(chi2_invert_m_hypothesis.size()!=0 ){chi2_invert_m_hypothesis.clear();}

        vtx_id = -1;

        // check if enry is empty
        if(vertex_v->size() == 0){return true;}

        storage->set_id(run,subrun,event);
        larlite::event_track  *corr_track_v = (larlite::event_track*)storage->get_data<larlite::event_track>("trackRecoCalibrated");

        larlite::event_track* ev_trk=nullptr;
        auto const& vtx_to_trk = storage->find_one_ass(vertex_v->id(), ev_trk, vertex_v->id().second);
        if(!ev_trk) throw larlite::DataFormatException("Could not find associated track data product!");

        for(int vertex_index=0;vertex_index<vertex_v->size();vertex_index++){
            vtx_id = vertex_index;
            if(Avg_dEdx.size()!=0){Avg_dEdx.clear();          }
            if(Avg_dEdx_5cm.size()!=0 ){Avg_dEdx_5cm.clear(); }
            if(Avg_dEdx_10cm.size()!=0){Avg_dEdx_10cm.clear();}

            if(Avg_corrdQdx.size()!=0     ){Avg_corrdQdx.clear();     }
            if(Avg_corrdQdx_5cm.size()!=0 ){Avg_corrdQdx_5cm.clear(); }
            if(Avg_corrdQdx_10cm.size()!=0){Avg_corrdQdx_10cm.clear();}

            if(chi2_p_hypothesis.size() !=0){chi2_p_hypothesis.clear();}
            if(chi2_m_hypothesis.size() !=0){chi2_m_hypothesis.clear();}
            if(chi2_invert_p_hypothesis.size() !=0){chi2_invert_p_hypothesis.clear();}
            if(chi2_invert_m_hypothesis.size() !=0){chi2_invert_m_hypothesis.clear();}

            std::vector<larlite::track> TracksAtVertex;
            for(auto const& trk_index : vtx_to_trk[vertex_index]) {
                TracksAtVertex.push_back( (*ev_trk)[trk_index]);
            }

            for(size_t iTrack=0;iTrack<TracksAtVertex.size();iTrack++){

                larlite::track thisTrack = TracksAtVertex.at(iTrack);
                larlite::track thisNewTrack;

                double averagedEdx      = 0;
                double averagedEdx_5cm  = 0;
                double averagedEdx_10cm = 0;

                double averageCorrdQdx      = 0;
                double averageCorrdQdx_5cm  = 0;
                double averageCorrdQdx_10cm = 0;

                double protonChi2 = 0;
                double muonChi2   = 0;

                double InvProtonChi2 = 0;
                double InvMuonChi2   = 0;

                int Npts      = 0;
                int Npts_5cm  = 0;
                int Npts_10cm = 0;
                int NDF = 0;
                int invNDF = 0;

                double thisLength = thisTrack.Length();

                for(int iNode = 0;iNode<thisTrack.NumberTrajectoryPoints();iNode++){
                    // read locus and direction from old track
                    TVector3 thisPoint = thisTrack.LocationAtPoint(iNode);
                    TVector3 thisDir   = thisTrack.DirectionAtPoint(iNode);

                    // write locus and direction as is to new track
                    thisNewTrack.add_vertex(thisPoint);
                    thisNewTrack.add_direction(thisDir);
                }

                thisNewTrack.set_track_id(thisTrack.ID());

                for(int iPlane = 0;iPlane<3;iPlane++){
                    std::vector<double> dQdx_one_plane;
                    for(int iNode = 0;iNode<thisTrack.NumberTrajectoryPoints();iNode++){
                        // read locus from old track
                        TVector3 thisPoint = thisTrack.LocationAtPoint(iNode);

                        // read dQdx for each plane from old track
                        double ion=0;
                        double loc_avg_ion=0;
                        double loc_ion_rms=0;
                        if(!useTruncateddQdx) ion = thisTrack.DQdxAtPoint(iNode,views[iPlane]);
                        else{
                            int ipoints=0;
                            loc_avg_ion=0;
                            for(int jNode = 0;jNode<thisTrack.NumberTrajectoryPoints();jNode++){
                                if(thisTrack.DQdxAtPoint(jNode,views[iPlane]) > 80) continue;
                                if((thisTrack.LocationAtPoint(iNode)-thisTrack.LocationAtPoint(jNode)).Mag()<5){
                                    loc_avg_ion+= thisTrack.DQdxAtPoint(jNode,views[iPlane]);
                                    ipoints++;
                                }
                            }
                            if(ipoints!=0)loc_avg_ion/=ipoints;
                            ipoints=0;
                            loc_ion_rms=0;
                            for(int jNode = 0;jNode<thisTrack.NumberTrajectoryPoints();jNode++){
                                if(thisTrack.DQdxAtPoint(jNode,views[iPlane]) > 80) continue;
                                if((thisTrack.LocationAtPoint(iNode)-thisTrack.LocationAtPoint(jNode)).Mag()<5){
                                    loc_ion_rms+= pow((thisTrack.DQdxAtPoint(jNode,views[iPlane])-loc_avg_ion),2);
                                    ipoints++;
                                }
                            }
                            if(ipoints!=0)loc_ion_rms/=ipoints;
                            ipoints=0;
                            ion=0;
                            for(int jNode = 0;jNode<thisTrack.NumberTrajectoryPoints();jNode++){
                                if(thisTrack.DQdxAtPoint(jNode,views[iPlane]) > 80) continue;
                                if((thisTrack.LocationAtPoint(iNode)-thisTrack.LocationAtPoint(jNode)).Mag()<5){
                                    if( std::abs(thisTrack.DQdxAtPoint(jNode,views[iPlane])-loc_avg_ion) < loc_ion_rms){
                                        ion+= thisTrack.DQdxAtPoint(jNode,views[iPlane]);
                                        ipoints++;
                                    }
                                }
                            }
                            if(ipoints!=0)ion/=ipoints;
                        }
                        //std::cout << loc_avg_ion << "\t" << loc_ion_rms << "\t" << ion << std::endl;

                        // Get correction factors for new track
                        GetSpatialCorrectionFactor(thisPoint, views[iPlane]);
                        // Correct ion value
                        double corr_ion = ion*SpatialCalib;
                        double dEdx = (exp(beta_box*Wion*C_dqdx_calib*corr_ion/Efield)-alpha_box)/(beta_box/Efield);
                        double resLength = thisTrack.Length(iNode);

                        // Filter for obviously wrong values
                        if(ion == 0)dEdx=0;
                        if(dEdx > 200)dEdx = 200;

                        // Compute Averaged values
                        if(iPlane==2 && dEdx < 10 && dEdx > 0){
                            Npts++;
                            averagedEdx+=dEdx;
                            averageCorrdQdx+=corr_ion;
                            if(resLength < 30 && resLength > 10){
                                protonChi2 += pow(dEdx-sProtonRange2dEdx.Eval(resLength),2);
                                muonChi2   += pow(dEdx-sMuonRange2dEdx.Eval(resLength)  ,2);
                                NDF++;
                            }

                            if(thisLength-resLength < 30 && thisLength-resLength > 10){
                                InvProtonChi2 += pow(dEdx-sProtonRange2dEdx.Eval(thisLength-resLength),2);
                                InvMuonChi2   += pow(dEdx-sMuonRange2dEdx.Eval(thisLength-resLength)  ,2);
                                invNDF++;
                            }

                            if(thisTrack.Length()-resLength < 5){
                                Npts_5cm++;
                                averagedEdx_5cm+=dEdx;
                                averageCorrdQdx_5cm+=corr_ion;
                            }
                            if(thisTrack.Length()-resLength < 10){
                                Npts_10cm++;
                                averagedEdx_10cm+=dEdx;
                                averageCorrdQdx_10cm+=corr_ion;
                            }
                        }


                        dQdx_one_plane.push_back(dEdx);

                        // fill the dEdx vs residual length histogram
                        /*if(iPlane==2
                           &&TracksAtVertex.size() == 2
                           && TracksAtVertex[0].Length() > 30
                           && TracksAtVertex[1].Length() > 30
                           && TracksAtVertex[0].Length()+TracksAtVertex[1].Length() < 200
                           && ion!=0
                           && ion != 111
                          // && dEdx < 100
                           //&& thisTrack.Length(iNode) > 5
                           && thisTrack.Length()-thisTrack.Length(iNode) > 5)hdEdxVresLength->Fill(resLength,dEdx);*/
                    }//iNode
                    thisNewTrack.add_dqdx(dQdx_one_plane);
                } //iPlane

                if(Npts!=0){averagedEdx/=Npts;}
                if(Npts_5cm!=0 )averagedEdx_5cm /= Npts_5cm;
                if(Npts_10cm!=0)averagedEdx_10cm/= Npts_10cm;
                if(NDF > 0){protonChi2/=NDF;muonChi2/=NDF;}else{protonChi2=-1;muonChi2=-1;}
                if(invNDF > 0){InvProtonChi2/=invNDF;InvMuonChi2/=invNDF;}else{InvProtonChi2=-1;InvMuonChi2=-1;}
                Avg_dEdx.push_back(averagedEdx);
                Avg_dEdx_5cm.push_back(averagedEdx_5cm);
                Avg_dEdx_10cm.push_back(averagedEdx_10cm);

                if(Npts!=0     )averageCorrdQdx     /= Npts;
                if(Npts_5cm!=0 )averageCorrdQdx_5cm /= Npts_5cm;
                if(Npts_10cm!=0)averageCorrdQdx_10cm/= Npts_10cm;
                Avg_corrdQdx.push_back(     averageCorrdQdx     );
                Avg_corrdQdx_5cm.push_back( averageCorrdQdx_5cm );
                Avg_corrdQdx_10cm.push_back(averageCorrdQdx_10cm);
                chi2_p_hypothesis.push_back(protonChi2);
                chi2_m_hypothesis.push_back(muonChi2);
                chi2_invert_p_hypothesis.push_back(InvProtonChi2);
                chi2_invert_m_hypothesis.push_back(InvMuonChi2);

                if(thisNewTrack.NumberTrajectoryPoints()!= thisTrack.NumberTrajectoryPoints()){
                    std::cout << "ERROR! not the same ammount of points in corrected track" << std::endl;
                }
                corr_track_v->push_back(thisNewTrack);
            } //iTrack


            dEdxTree->Fill();
        }// vtx_id
        return true;





        if(corr_track_v->size() != track_v->size()){
            std::cout << "ERROR! not the same ammount of tracks" << std::endl;
        }
        return true;

    }
    //=====================================================
    bool ConvertdEdx::finalize() {

//        TCanvas *cdEdxVresLength = new TCanvas("cdEdxVresLength","cdEdxVresLength",800,600);
//        std::cout << hdEdxVresLength->GetEntries() << std::endl;
//        hdEdxVresLength->Draw("colz");
//        sMuonRange2dEdx.Draw("same");
//        sProtonRange2dEdx.Draw("same");

//        cdEdxVresLength->SaveAs("cdEdxVresLength.png");
//        cdEdxVresLength->SaveAs("cdEdxVresLength.root");

        std::cout << "Bye World" << std::endl;
        return true;
    }
    //=====================================================
    bool ConvertdEdx::LoadCalibrationFile(){
        fCalib = TFile::Open(Form("%s/%s.root",CalibrationFilePath.c_str(),CalibrationFileName.c_str()),"READ");
        if(!(fCalib->IsOpen())){
            std::cout << "ERROR : could not open calibration file!" << std::endl;
        }
        dirName = "Cal_1_1_1_9970";
        ChoseCalibration();

        hSpatialCorr     = (TH3D*)fCalib->Get(Form("%s/SpecialCalibration",dirName.c_str()));
        RooDouble *alpha = (RooDouble*)fCalib->Get(Form("%s/alpha",dirName.c_str()));
        RooDouble *beta  = (RooDouble*)fCalib->Get(Form("%s/beta",dirName.c_str()));
        RooDouble *C     = (RooDouble*)fCalib->Get(Form("%s/C",dirName.c_str()));

        alpha_box    = *alpha;
        beta_box     = *beta;
        C_dqdx_calib = *C;

        return true;
    }
    //=====================================================
    void ConvertdEdx::GetSpatialCorrectionFactor(TVector3 point,geo::View_t view){
        int iBin = hSpatialCorr->FindBin(point.X(),point.Y(),point.Z());
        SpatialCalib = hSpatialCorr->GetBinContent(iBin);
    }
    //=====================================================
    void ConvertdEdx::ChoseCalibration(){

    }
    //=====================================================
    void ConvertdEdx::DefineTreeBranches(){
        dEdxTree = new TTree("dEdxTree","dEdxTree");

        // ID info
        dEdxTree->Branch("run",&run);
        dEdxTree->Branch("subrun",&subrun);
        dEdxTree->Branch("event",&event);
        dEdxTree->Branch("vtx_id",&vtx_id);

        // dEdx variables
        dEdxTree->Branch("Avg_dEdx",&Avg_dEdx);
        dEdxTree->Branch("Avg_dEdx_5cm",&Avg_dEdx_5cm);
        dEdxTree->Branch("Avg_dEdx_10cm",&Avg_dEdx_10cm);

        // corrected dQdx variables
        dEdxTree->Branch("Avg_corrdQdx",&Avg_corrdQdx);
        dEdxTree->Branch("Avg_corrdQdx_5cm",&Avg_corrdQdx_5cm);
        dEdxTree->Branch("Avg_corrdQdx_10cm",&Avg_corrdQdx_10cm);

        //PID variables
        dEdxTree->Branch("chi2_m_hypothesis",&chi2_m_hypothesis);
        dEdxTree->Branch("chi2_p_hypothesis",&chi2_p_hypothesis);
        dEdxTree->Branch("chi2_invert_p_hypothesis",&chi2_invert_p_hypothesis);
        dEdxTree->Branch("chi2_invert_m_hypothesis" ,&chi2_invert_m_hypothesis);


    }
    //=====================================================
    void ConvertdEdx::Get_dEdx_Splines(){
        TFile *fSplines = TFile::Open(Form("%s",SplineFileName.c_str()),"READ");
        sMuonRange2dEdx   = (*(TSpline3*)fSplines->Get("sMuonRange2dEdx"));
        sProtonRange2dEdx = (*(TSpline3*)fSplines->Get("sProtonRange2dEdx"));
    }
    //=====================================================
    
}
#endif
