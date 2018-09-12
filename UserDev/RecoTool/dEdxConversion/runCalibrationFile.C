#include "Calibration_File.h"
#include <iostream>

int runCalibrationFile(double _Nevt = 1000000){
    using namespace larlite;
    Calibration_File my_Calib;
    std::string version = "test17e";
    std::string sample = "cocktail";
    std::string InDir  = "/Users/hourlier/Documents/PostDocMIT/Research/MicroBooNE/FinalFiles/"+version+"/"+sample;
    std::string OutDir = ".";
    larlite::storage_manager mgr;

    mgr.set_io_mode(mgr.kREAD); // Option: kREAD, kWRITE, kBOTH
    mgr.add_in_filename(InDir+"/tracker_reco.root");

    if(!mgr.open()) {
        std::cerr << "Failed to open ROOT file. Aborting." << std::endl;
        return 1;
    }

    my_Calib.SetdEdxSplineFile("../../../../LArCV/app/Reco3D/Proton_Muon_Range_dEdx_LAr_TSplines.root");
    TFile *f2 = TFile::Open(Form("Calib_dEdx_%s_%s.root",version.c_str(),sample.c_str()),"RECREATE");

    my_Calib.SetSelectionFile("/Users/hourlier/Documents/PostDocMIT/Research/MicroBooNE/FinalFiles/"+version+"/"+sample+"/FinalVertexVariables.root");
    my_Calib.set_output_file(f2);
    my_Calib.initialize();

    int Nevt = 0;
    if(_Nevt<=1)Nevt= (int)(_Nevt*mgr.get_entries());
    else{Nevt = (int)_Nevt;}

    if(Nevt > mgr.get_entries()){std::cout << "Requested " << Nevt << " but only " << mgr.get_entries() << " availables" << std::endl; Nevt = mgr.get_entries();}
    else{std::cout << "Requested " << Nevt << " out of " << mgr.get_entries() << " availables" << std::endl;}
    int ievt = 0;
    while(mgr.next_event() && ievt < Nevt){
        if(((int)(ievt*100/(Nevt))%10==0) && ((int)((ievt-1)*100/(Nevt))%10!=0))std::cout << (int)(ievt*100./Nevt) << " %" << std::endl;
        //std::cout << "about to  : " << mgr.run_id() << "\t" << mgr.subrun_id() << "\t" << mgr.event_id() << std::endl;
        ievt++;
        my_Calib.analyze(&mgr);
        //std::cout << "done : " << mgr.run_id() << "\t" << mgr.subrun_id() << "\t" << mgr.event_id() << std::endl;

    }

    //TTree *dEdxTree = my_Calib.GetAnaTree();

    //f2->cd();
    //dEdxTree->Write();
    my_Calib.finalize();
    mgr.close();
    return 0;
}
