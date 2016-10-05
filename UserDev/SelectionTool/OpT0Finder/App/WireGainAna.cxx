#ifndef LARLITE_WIREGAINANA_CXX
#define LARLITE_WIREGAINANA_CXX

#include "WireGainAna.h"
#include "DataFormat/wire.h"
#include "DataFormat/opflash.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/LArProperties.h"
#include "OpT0Finder/PhotonLibrary/PhotonVisibilityService.h"

namespace larlite {

  bool WireGainAna::initialize() {

    _tpc.resize(32);
    _pmt.resize(32);
    
    _tree = new TTree("tree","");
    _tree->Branch("tpc","std::vector<double>",&_tpc);
    _tree->Branch("pmt","std::vector<double>",&_pmt);
    _tree->Branch("sumpe_tpc",&_sumpe_tpc,"sumpe_tpc/D");
    _tree->Branch("sumpe_pmt",&_sumpe_pmt,"sumpe_pmt/D");

    return true;
  }
  
  bool WireGainAna::analyze(storage_manager* storage) {

    auto ev_flash = storage->get_data<event_opflash>("simpleFlashBeam");
    bool found=false;
    _sumpe_pmt = 0;
    for(auto const& f : *ev_flash) {
      if(f.Time() < 6 || f.Time() > 8) continue;
      found=true;
      for(size_t opch=0; opch<32; ++opch) {
	unsigned int opdet = larutil::Geometry::GetME()->OpDetFromOpChannel(opch);
	_pmt[opdet] = f.PE(opch);
	_sumpe_pmt += f.PE(opch);
      }
    }
    if(!found) return false;

    auto ev_wire = storage->get_data<event_wire>("caldata");
    if (!ev_wire || ev_wire->empty()) {
      std::cerr << "Cosmic tag exists but not track! Cannot happen..." << std::endl;
      throw std::exception();
    }

    const double drift_velocity = ::larutil::LArProperties::GetME()->DriftVelocity();
    for(auto& v : _tpc) v = 0;
    for(auto const& wire_roi : *ev_wire) {

      auto const& wire_id = ::larutil::Geometry::GetME()->ChannelToWireID(wire_roi.Channel());

      if ((int)(wire_id.Plane) != 2) continue;

      for (auto const& range : wire_roi.SignalROI().get_ranges()) {

	auto const& adcs = range.data();

	int start_index = range.begin_index();

	for (size_t index = 0; index < adcs.size(); ++index) {
	  size_t tick = start_index + index;
	  if( adcs[index] <= 15 ) continue;
	  if( tick < 3200 ) continue;
	  if( tick > 8000 ) continue;
	  tick -= 3200;
	  double x = tick * 0.5 * drift_velocity;
	  double y = 0.;
	  double z = wire_id.Wire * 0.3;
	  double q = adcs[index] / 30. * 4000. * 0.3;
	  for(size_t pmt=0; pmt<32; ++pmt)
	    _tpc[pmt] += q * (::phot::PhotonVisibilityService::GetME().GetVisibility( x, y, z, pmt)) * 0.0093;
	}
      }
    }

    _sumpe_tpc = 0;
    for(auto const& pe : _tpc) _sumpe_tpc += pe;

    _tree->Fill();
	  
    return true;
  }

  bool WireGainAna::finalize() {

    if(_fout) {

      _fout->cd();
      _tree->Write();

    }
  
    return true;
  }

}
#endif
