#ifndef LARLITE_TOYSHOWERGENERATOR_CXX
#define LARLITE_TOYSHOWERGENERATOR_CXX

#include "ToyShowerGenerator.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryHelper.h"
#include "TRandom3.h"

#include "DataFormat/hit.h"
#include "DataFormat/cluster.h"
#include "DataFormat/shower.h"

namespace larlite {

  ToyShowerGenerator::ToyShowerGenerator(){ 
    _name="ToyShowerGenerator"; 
    _fout=0;
    auto geom = larutil::Geometry::GetME();

    setEnergyRange(0.2, 3.0);
    setXRange(- geom -> DetHalfWidth(), geom -> DetHalfWidth());
    setYRange(- geom -> DetHalfHeight(), geom -> DetHalfHeight());
    setZRange(0.0, geom -> DetLength());
    setThetaRange(0.0, 3.14159);
    setPhiRange(0.0, 2*3.14159);
    setLengthSmearing(50); // cm
    setWidthSmearing(50); // cm    
  }

  bool ToyShowerGenerator::initialize() {

    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you have a histogram to fill in the event loop, for example,
    // here is a good place to create one on the heap (i.e. "new TH1D"). 
    //


    return true;
  }

  void ToyShowerGenerator::printParams() const{
    std::cout << "Print parameters of generation: \n"
              << "Energy: ....... " << _e_min << " to " << _e_max << "\n"
              << "X Position: ... " << _x_min << " to " << _x_max << "\n"
              << "Y Position: ... " << _y_min << " to " << _y_max << "\n"
              << "Z Position: ... " << _z_min << " to " << _z_max << "\n"
              << "Theta Range: .. " << _theta_min << " to " << _theta_max << "\n"
              << "Phi Range: .... " << _phi_min << " to " << _phi_max << "\n";
  }
  
  bool ToyShowerGenerator::analyze(storage_manager* storage) {
  
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
    
    // Going to make a shower, to start.  Need a number of parameters to fill in

    auto rand = TRandom3(0);

    // First, get the position and direction of this shower:
    TVector3 position;
    position.SetX(rand.Uniform(_x_min, _x_max));
    position.SetY(rand.Uniform(_y_min, _y_max));
    position.SetZ(rand.Uniform(_z_min, _z_max));

    TVector3 direction;
    // Doing this in a really stupid, shitty, clunky way ....
    bool gotDir = false;
    while (!gotDir){
      direction.SetX(rand.Uniform());
      direction.SetY(rand.Uniform());
      direction.SetZ(rand.Uniform());

      // Enforce it into a sphere to get isotropic
      if (direction.Mag() > 1){
        continue;
      }

      // Force it into the phi range
      if (direction.Phi() > _phi_max || direction.Phi() < _phi_min){
        continue;
      }

      // Force it into the theta range
      if (direction.Theta() > _theta_max || direction.Theta() < _theta_min){
        continue;
      }

      gotDir = true;
    }

    // Normalize the direction 
    direction *= 1.0/direction.Mag();

    // Now have a start point and direction, get the energy:
    float energy = rand.Uniform(_e_min, _e_max);

    // Next get the length, width:
    float length = rand.Gaus(150*log(energy),_length_smearing);
    float width = rand.Gaus(0.3*length,_width_smearing);

    // Now we start to make energy depositions
    // Keep track of how much energy has been allocated so far
    float _total_energy_used = 0;
    float mean_hit_energy = 0.002;

    std::vector<TVector3> _hit_locations_3d;
    std::vector<float> _hit_charge;

    // Put some depositions right at the start of the shower
    // Step size of half a centimeter?  I don't really care
    float _start_length = rand.Uniform(1.0,3.0);
    float pos = 0;
    while (pos < _start_length){
      // Generate a hit:
      _hit_locations_3d.push_back(position + pos*direction);
      _hit_charge.push_back(mean_hit_energy* rand.Poisson(4));
      _total_energy_used += _hit_charge.back();
      pos += 0.5;
    }

    // Generate hits along the axis
    float step = length /(0.5*energy/mean_hit_energy);
    while (_total_energy_used < 0.5*energy){
      _hit_locations_3d.push_back(position + pos*direction);
      _hit_charge.push_back(mean_hit_energy* rand.Poisson(4));
      _total_energy_used += _hit_charge.back();
      pos += step;
    }

    // Generate other random hits until all the energy is use up:

    while (_total_energy_used < energy){
      // Select a random position along the shower
      pos = rand.Gaus(length/2.0,(length/4.0));      
      // Generate a random displacement that is perpendicular to the axis, Gaus in shape
      TVector3 perp = direction.Orthogonal();
      // Normalize the perp, and rotate it a random amount:
      perp *= 1.0/perp.Mag();
      perp.Rotate(rand.Uniform(0,2.0*3.14159), direction);
      // Get a random displacement:
      float dist = rand.Gaus(0.0, width);
      _hit_locations_3d.push_back(position + pos*direction + perp*dist);
      _hit_charge.push_back(mean_hit_energy* rand.Poisson(4));
      _total_energy_used += _hit_charge.back();
    }

    // Ok, now there is a list of points in 3D with charge associated with them
    // Project them all into each 2D plane and make hits and clusters to go with them
    std::cout << "Shower is created, attempting to make hits and clusters." << std::endl;
    std::cout << "Shower Parameters: \n"
              << "Start Point: ....." << " (" << position.X()  << ", " << position.Y() << ", " << position.Z() << ")\n"
              << "Direction: ......." << " (" << direction.X()  << ", " << direction.Y() << ", " << direction.Z() << ")\n"
              << "Length: .........." << length << "\n"
              << "Width: ..........." << width << "\n"
              << "Energy: .........." << energy << "\n"
              << "Energy Used: ....." << _total_energy_used << "\n"
              << "Number of Hits: .." << _hit_charge.size() << "\n";



    auto geom = larutil::Geometry::GetME();
    auto geomHelper = larutil::GeometryHelper::GetME();  

    auto out_cluster_v = storage->get_data<event_cluster>(_out_producer_name);
    auto out_hit_v = storage->get_data<event_hit>(_out_producer_name);
    int clust_id = 0;

    // Make new associations
    AssSet_t hit_ass;
    // for (unsigned int i = 0; i < ass_info.size(); i ++){
    //   hit_ass.push_back(ass_info.at(i));
    //   out_cluster_v -> push_back(ev_clus -> at(i));
    // }


    for (unsigned int plane = 0; plane < geom -> Nplanes(); plane ++ ){
      
      // Project each point into the plane and create a hit
      std::vector <larlite::hit> hitList;
      AssUnit_t new_association;
      for (unsigned int hit_id = 0; hit_id < _hit_locations_3d.size(); hit_id ++ ){
        // Get the point in 2D for this plane:
        larutil::PxPoint loc;
        try {
          loc = geomHelper -> Point_3Dto2D(_hit_locations_3d.at(hit_id),plane);
        }
        catch( ... ) {
          continue;
        }
        larlite::hit thisHit;
        auto channel = geom -> PlaneWireToChannel(plane, (int) loc.w / geomHelper -> WireToCm() );
  
        thisHit.set_rms(sqrt(_hit_charge.at(hit_id)));
        thisHit.set_time_range(loc.t/ geomHelper -> TimeToCm() - 15, loc.t/ geomHelper -> TimeToCm() + 15);
        thisHit.set_time_peak(loc.t/ geomHelper -> TimeToCm(), 0.0);
        thisHit.set_time_rms(15);
        thisHit.set_amplitude(_hit_charge.at(hit_id), 0.0);
        thisHit.set_integral(_hit_charge.at(hit_id), 0.0);
        thisHit.set_sumq(_hit_charge.at(hit_id));
        thisHit.set_multiplicity(1);
        thisHit.set_local_index(0);
        thisHit.set_goodness(0);
        thisHit.set_ndf(0);
        thisHit.set_channel(channel);
        thisHit.set_view(geom -> View(channel));
        thisHit.set_signal_type(geom->PlaneToSignalType(loc.plane));
        thisHit.set_wire(geom -> ChannelToWireID(channel));

        hitList.push_back(thisHit);
        out_hit_v -> push_back(thisHit);
        new_association.push_back(out_hit_v->size() -1 );
      }
      if (hitList.size() == 0 ) continue;
      hit_ass.push_back(new_association);
      out_cluster_v -> push_back(larlite::cluster());
      out_cluster_v -> back().set_integral(hitList.front().Integral(),0,0);
      out_cluster_v -> back().set_id(clust_id ++);
      out_cluster_v -> back().set_view(hitList.front().View());
      // clust.set_start_wire(float w,float err);
      // clust.set_start_tick(float t,float err);
      // clust.set_start_charge(float q);
      // clust.set_start_angle(float a);
      // clust.set_start_opening(float o);
      // clust.set_end_wire(float w,float err);
      // clust.set_end_tick(float t,float err);
      // clust.set_end_charge(float q);
      // clust.set_end_angle(float a);
      // clust.set_end_opening(float o);
      // clust.set_integral(float m, float err, float average);
      // clust.set_summedADC(float m, float err, float average);
      // clust.set_n_hits(unsigned int n);
      // clust.set_multiple_hit_density(float v);
      // clust.set_width(float v);
      // clust.set_id(ID_t id);
      // clust.set_view(geo::View_t v);
      // clust.set_planeID(const geo::PlaneID& id);

    }

    auto out_ass = storage->get_data<event_ass>(out_cluster_v->name());
    out_ass->set_association(out_cluster_v->id(), out_hit_v->id(), hit_ass);

    storage -> set_id(1,0,out_cluster_v->event_id());


    return true;
  }

  bool ToyShowerGenerator::finalize() {

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
  
    return true;
  }

}
#endif
