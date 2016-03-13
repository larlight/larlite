#ifndef DRAWPFPARTICLE3D_CXX
#define DRAWPFPARTICLE3D_CXX

#include "DrawPFParticle3D.h"

namespace evd {


DrawPFParticle3D::DrawPFParticle3D() {
  _name = "DrawPFParticle3D";
  _fout = 0;
}

bool DrawPFParticle3D::initialize() {

  return true;
}

bool DrawPFParticle3D::analyze(larlite::storage_manager* storage) {

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


  // get a handle to the particles
  auto pfpartHandle = storage->get_data<larlite::event_pfpart>(_producer);

  // Get the associated spacepoints for this particle:
  larlite::event_spacepoint * ev_sps = nullptr;
  auto const& sps_index_v
    = storage->find_one_ass(pfpartHandle->id(), ev_sps, _producer);


  if (!ev_sps) {
    std::cout << "Did not find spacepoint data product"
              << "!" << std::endl;
    return false;
  }


  if (!sps_index_v.size())
    return false;

  // Clear out the data but reserve some space
  _data.clear();
  _data.reserve(pfpartHandle -> size());


  // Populate the particles:
  for (auto & spt_v : sps_index_v) {
    if (spt_v.size() == 0)
      continue;
    PFPart3D temp;

    // use cru3d helper to generate poin3D:
    _cru3d_helper.GeneratePoint3D(spt_v,ev_sps,temp._params.point_vector);

    // // reserve space for all the spacepoints:
    // temp.reserve(spt_v.size());
    // for (auto & index : spt_v){
    //   const double * xyz = ev_sps->at(index).XYZ();
    //   temp.emplace_back(cluster3D::Point3D(xyz[0],xyz[1],xyz[2]));
    // }

    // Here is where the params are filled and stored:
    _params_alg.FillParams(temp._params);
    temp._params.length = 100;
    
    // Save the output:
    _data.emplace_back(temp);

  }


  return true;
}

bool DrawPFParticle3D::finalize() {

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

DrawPFParticle3D::~DrawPFParticle3D() {}


} // evd

#endif
