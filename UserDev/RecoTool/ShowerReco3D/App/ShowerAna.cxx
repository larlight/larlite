#ifndef LARLITE_SHOWERANA_CXX
#define LARLITE_SHOWERANA_CXX

#define _USE_MATH_DEFINES
 
#include <cmath>
#include <iostream>
#include "ShowerAna.h"
#include "DataFormat/mctruth.h"
#include "DataFormat/mcshower.h"
#include "DataFormat/shower.h"
#include "LArUtil/Geometry.h"
//use function NearestWire()
//takes 3d point and plane as an argument and returns wire nearest to it
//get time by scaling x
#include "LArUtil/GeometryUtilities.h"

namespace larlite {

  bool ShowerAna::initialize() {

    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you have a histogram to fill in the event loop, for example,
    // here is a good place to create one on the heap (i.e. "new TH1D"). 
    //

    // Histograms to compare vertex information, dE/dx, direction:
    vertex_X = new TH1F("vertex_x","X vertex True - reco",100,-25,25);
    vertex_Y = new TH1F("vertex_y","Y vertex True - reco",100,-25,25);
    vertex_Z = new TH1F("vertex_z","Z vertex True - reco",100,-25,25);
    vertex_abs = new TH1F("vertex_abs","Abs vertex True - reco",100,-25,25);

    direction_X = new TH1F("direction_x","X direction True - reco",100,-25,25);
    direction_Y = new TH1F("direction_y","Y direction True - reco",100,-25,25);
    direction_Z = new TH1F("direction_z","Z direction True - reco",100,-25,25);
    direction_angle = new TH1F("direction_angle","Angle Between True and reco",100,-3.14,3.14);

    dEdx = new TH1F("dEdx","Reconstructed dEdx",50,-2,20);
    dEdx_fid = new TH1F("dEdx_fid","Reconstructed dEdx",50,-2,20);

    return true;
  }
        
  
  
  bool ShowerAna::analyze(storage_manager* storage) {
    auto geom = larutil::Geometry::GetME();
    auto geomUtil = larutil::GeometryUtilities::GetME();
  
    //
    // Do your event-by-event analysis here. This function is called for 
    // each event in the loop. You have "storage" pointer which contains 
    // event-wise data. To see what is available, check the "Manual.pdf":
    //
    // http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
    // 
    // Or you can refer to Base/DataFormatConstants.hh for available data type
    // enum values. Here is one example of getting PMT waveform collection.
    //
    // event_fifo* my_pmtfifo_v = (event_fifo*)(storage->get_data(DATA::PMFIFO));
    //
    // if( event_fifo )
    //
    //   std::cout << "Event ID: " << my_pmtfifo_v->event_id() << std::endl;
    //


    event_mctruth * event_truth = storage -> get_data<event_mctruth>("generator");
    auto truth = event_truth -> at(0);
    if ( truth.GetNeutrino().CCNC() == 1){
      return false;
    }


    auto neutrino = truth.GetNeutrino().Nu();
    auto lep = truth.GetNeutrino().Lepton();
  
    TLorentzVector trueVertex = neutrino.Trajectory().front().Position();
    TLorentzVector trueDirection = lep.Trajectory().front().Momentum();

    // Only look at electron neutrino events?

    // get the mc showers:
    event_mcshower * ev_shower = storage -> get_data<event_mcshower>("mcreco");
    event_shower * reco_shower = storage -> get_data<event_shower>("showerreco");
    
    if (reco_shower -> size() == 0) return false;

    // Normalize the direction:
    TVector3 trueDirNorm = trueDirection.Vect();
    trueDirNorm *= 1.0/trueDirNorm.Mag(); 

    int shower_number = 1;

    // std::cout << "\n\n\nThis event ("<< storage -> event_id() << ") has " << ev_shower->size() << " mc showers.\n";
    for (auto & shower : * ev_shower){
      //if (shower.DetProfile().E() < 10) continue;
        if (abs(shower.PdgCode()) == 11)
        // {
        //   std::cout << "shower number: " << shower_number << '\n';
        //   std::cout << "\tPDG: ........ " << shower.PdgCode() << "\n"
        //             << "\tTrack ID: ... " << shower.TrackID() << "\n"
        //             << "\tMother ID: .. " << shower.MotherTrackID() << "\n"
        //             << "\tTotal E: .... " << shower.MotherStart().E() << "\n"
        //             << "\tDep E: ...... " << shower.DetProfile().E() << "\n";
        //   std::cout << "\tvertex: (" << trueVertex.X() << ", " << trueVertex.Y() << ", " << trueVertex.Z() << ")\n"
        //               << "\tdirection: (" << trueDirNorm.X() << ", " << trueDirNorm.Y() << ", " << trueDirNorm.Z() << ")" << '\n';
        //   std::cout << "\tWire (Vertex) @ Plane 0: " << geom->NearestWire(trueVertex.Vect(), 0) * 0.4 << '\n';
        //   std::cout << "\tWire (Vertex) @ Plane 1: " << geom->NearestWire(trueVertex.Vect(), 1) * 0.4 << '\n';
        //   std::cout << "\tTime (Vertex): " << trueVertex.X() << '\n';
        //   std::cout << "\t2D Angle @ plane 0: " << geomUtil -> Get2DangleFrom3D(0, trueDirNorm) * 180/M_PI << '\n';
        //   std::cout << "\t2D Angle @ plane 1: " << geomUtil -> Get2DangleFrom3D(1, trueDirNorm) * 180/M_PI << '\n';
        //   std::cout << std::endl;
        // }
        // else {
        //   std::cout << "shower number " << shower_number << " is not an electron or positron shower." << '\n';
        // }
        shower_number++ ;
        // std::cout << std:: endl;
    }

    // std::cout << "This event ("<< storage -> event_id() << ") has " << reco_shower->size() << " reco showers.\n";
    //std::cout << "PDG of the outgoing lepton is " << lep.PdgCode() << std::endl;

    for (auto & shower : * reco_shower){
      TVector3 start = shower.ShowerStart();
      TVector3 dir   = shower.Direction();
      if (dir.Mag() == 0) continue;
      std::vector<double> dedx = shower.dEdx();
      std::vector<double> energy = shower.Energy();
      // std::cout << "Start point of the shower: ("
      //           << start.X() << ", " << start.Y() << ", " << start.Z() << ")\n"
      //           << "\tDirection: "
      //           << dir.X() << ", " << dir.Y() << ", " << dir.Z() << ")\n"
      //           << "\tdEdx: " << dedx[1] << "\n"
      //           << "\tenergy: " << energy[1] << "\n";
      // std::cout << "\tWire (Vertex) @ Plane 0: " << geom->NearestWire(shower.ShowerStart(), 0) * 0.4 << '\n';
      // std::cout << "\tWire (Vertex) @ Plane 1: " << geom->NearestWire(shower.ShowerStart(), 1) * 0.4 << '\n';
      // std::cout << "\tTime (Vertex): " << shower.ShowerStart().X() << '\n';
      // std::cout << "\t2D Angle @ plane 0: " << geomUtil -> Get2DangleFrom3D(0, shower.Direction()) * 180/M_PI << '\n';
      // std::cout << "\t2D Angle @ plane 1: " << geomUtil -> Get2DangleFrom3D(1, shower.Direction()) * 180/M_PI << '\n';

     
      // Fill in the histograms:
      vertex_X      -> Fill(trueVertex.X() - start.X());
      vertex_Y      -> Fill(trueVertex.Y() - start.Y());
      vertex_Z      -> Fill(trueVertex.Z() - start.Z());
      // vertex_abs    -> Fill();
      direction_X   -> Fill(trueDirNorm.X() - dir.X());
      direction_Y   -> Fill(trueDirNorm.Y() - dir.Y());
      direction_Z   -> Fill(trueDirNorm.Z() - dir.Z());
      float theta = dir.Dot(trueDirection.Vect()) / (dir.Mag() * trueDirection.Vect().Mag());
      theta = acos(theta);
      // std::cout << "Theta is " << theta << std::endl;
      direction_angle -> Fill(theta);
      dEdx          -> Fill(dedx[1]);
      if (isFiducial(start)){
        dEdx_fid -> Fill(dedx[1]);
        if (dedx[1] > 1.5 && dedx[1] < 5.0) _good_event_list.push_back(storage->event_id());
      }
    }

    return true;
  }

  bool ShowerAna::finalize() {

    // This function is called at the end of event loop.
    // Do all variable finalization you wish to do here.
    // If you need, you can store your ROOT class instance in the output
    // file. You have an access to the output file through "_fout" pointer.
    //
    // Say you made a histogram pointer h1 to store. You can do this:
    //
    // if(_fout) { _fout->cd(); h1->Write(); }
    //
    // else 
    //   print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
    //
  
    if (_fout){
      _fout -> cd();
      vertex_X      -> Write();
      vertex_Y      -> Write();
      vertex_Z      -> Write();
      vertex_abs    -> Write();
      direction_X   -> Write();
      direction_Y   -> Write();
      direction_Z   -> Write();
      direction_angle -> Write();
      dEdx          -> Write() ;
      dEdx_fid      -> Write() ;
    }

    std::cout << "Printing good events: \n";
    for (auto event : _good_event_list){
      std::cout << event << '\n';
    }


    return true;
  }

  bool ShowerAna::isFiducial(const TVector3 & vertex){
    // Argoneut specific
    if (vertex.X() > 23.5 || vertex.X() < -23.5) return false;
    if (vertex.Y() > 20 || vertex.Y() < -20) return false;
    if (vertex.Z() > 90 || vertex.Z() < 0) return false;
  
    return true;
  }

}
#endif
