#ifndef LARLITE_FLASHMATCH2D_CXX
#define LARLITE_FLASHMATCH2D_CXX

#include "FlashMatch2D.h"
#include "DataFormat/wire.h"
#include "DataFormat/opflash.h"
#include "OpT0Finder/Base/OpT0FinderTypes.h"
#include "FhiclLite/ConfigManager.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/LArProperties.h"

namespace larlite {

  bool FlashMatch2D::initialize() {

    ::fcllite::ConfigManager cfg_mgr;
    cfg_mgr.AddCfgFile(_config_file);
    auto const& main_cfg = cfg_mgr.Config();
    auto const p = main_cfg.get_pset(_name);

    _opflash_producer_beam   = p.get<std::string>("BeamOpFlashProducer");
    _opflash_producer_cosmic = p.get<std::string>("CosmicOpFlashProducer");
    _flash_trange_start      = p.get<double>("FlashVetoTimeStart");
    _flash_trange_end        = p.get<double>("FlashVetoTimeEnd");
    _num_ticks               = p.get<size_t>("SubVolumeTicks");
    _num_wires               = p.get<size_t>("SubVolumeWires");
    _wire_producer           = p.get<std::string>("WireProducer");
    _gain_correction         = p.get<std::vector<double> >("GainCorrection");
    
    auto const geo = ::larutil::Geometry::GetME();
    if(geo->NOpDets() != _gain_correction.size()) {
      std::cout << "GainCorrection array size is " << _gain_correction.size() << " != # OpDet " << geo->NOpDets() << std::endl;
      return false;
    }

    _mgr.Configure(main_cfg);

    return true;
  }
  
  bool FlashMatch2D::analyze(storage_manager* storage) {

    _mgr.Reset();
    _result.clear();
    // _mgr.PrintConfig();

    const ::larutil::Geometry* geo = ::larutil::Geometry::GetME();

    auto ev_flash_beam   = storage->get_data<event_opflash> ( _opflash_producer_beam   );
    auto ev_flash_cosmic = storage->get_data<event_opflash> ( _opflash_producer_cosmic );
    if((!ev_flash_cosmic || ev_flash_cosmic->empty()) || (!ev_flash_beam || ev_flash_beam->empty())) {
      std::cout << "No opflash found. Skipping event: " << storage->event_id() << std::endl;
      return false;
    }
    
    size_t flash_id=0;
    if(ev_flash_beam) {
      for (size_t n = 0; n < ev_flash_beam->size(); n++) {
	
	auto const& flash = ev_flash_beam->at(n);
	if(flash.Time() < _flash_trange_start || _flash_trange_end < flash.Time())
	  continue;
	
	::flashana::Flash_t f;
	f.x = f.x_err = 0;
	f.y = flash.YCenter();
	f.z = flash.ZCenter();
	f.y_err = flash.YWidth();
	f.z_err = flash.ZWidth();
	f.pe_v.resize(geo->NOpDets());
	f.pe_err_v.resize(geo->NOpDets());
	for (unsigned int i = 0; i < f.pe_v.size(); i++) {
	  unsigned int opdet = geo->OpDetFromOpChannel(i);
	  f.pe_v[opdet] = flash.PE(i) / _gain_correction[i];
	  f.pe_err_v[opdet] = sqrt(flash.PE(i) / _gain_correction[i]);
	}
	f.time = flash.Time();
	f.idx = flash_id;
	++flash_id;
	_mgr.Emplace(std::move(f));
      }
    }

    if(ev_flash_cosmic) {
      for (size_t n = 0; n < ev_flash_cosmic->size(); n++) {
	
	auto const& flash = ev_flash_cosmic->at(n);
	if(flash.Time() < _flash_trange_start || _flash_trange_end < flash.Time())
	  continue;
	
	::flashana::Flash_t f;
	f.x = f.x_err = 0;
	f.y = flash.YCenter();
	f.z = flash.ZCenter();
	f.y_err = flash.YWidth();
	f.z_err = flash.ZWidth();
	f.pe_v.resize(geo->NOpDets());
	f.pe_err_v.resize(geo->NOpDets());
	for (unsigned int i = 0; i < f.pe_v.size(); i++) {
	  unsigned int opdet = geo->OpDetFromOpChannel(i);
	  if(flash.PE(i) == 0.) {
	    f.pe_v[opdet]=-1.;
	    f.pe_err_v[opdet]=-1.;
	  }else{
	    f.pe_v[opdet] = flash.PE(i) / _gain_correction[i] / 0.424;
	    f.pe_err_v[opdet] = sqrt(flash.PE(i) / _gain_correction[i]) / 0.424;
	  }
	}
	f.time = flash.Time();
	f.idx = flash_id;
	++flash_id;
	_mgr.Emplace(std::move(f));
      }
    }
    if(!flash_id) {
      std::cerr << "No relevant flash found... Event: " << storage->event_id() << std::endl;
      return false;
    }
    
    auto ev_wire = storage->get_data<event_wire>(_wire_producer);
    if (!ev_wire || ev_wire->empty()) {
      std::cerr << "Cosmic tag exists but not track! Cannot happen..." << std::endl;
      throw std::exception();
    }

    const double drift_velocity = ::larutil::LArProperties::GetME()->DriftVelocity();
    
    const size_t tot_num_wires = ::larutil::Geometry::GetME()->Nwires(2);
    const size_t tot_num_ticks = ::larutil::Geometry::GetME()->DetHalfWidth() * 2 / drift_velocity / 0.5;
    std::cout <<"wires: " << tot_num_wires << " ... ticks: " << tot_num_ticks << std::endl;
    size_t tot_vbox = tot_num_ticks / _num_ticks;
    size_t tot_hbox = tot_num_wires / _num_wires;
    if( (tot_num_ticks % _num_ticks) ) tot_vbox += 1;
    if( (tot_num_wires % _num_wires) ) tot_hbox += 1;
    std::cout << "total: " << tot_vbox << " : " << tot_hbox << std::endl;
    std::vector<flashana::QCluster_t> qclus_v((tot_vbox * tot_hbox));
    
    //static std::vector<std::vector<double> > qmap(size_t(tot_num_wires/6.)+1,std::vector<double>(size_t(tot_num_ticks/6.)+1,0.));
    //for(auto& qv : qmap) { for(auto& q : qv) q = 0.; }
      
    for(auto const& wire_roi : *ev_wire) {

      auto const& wire_id = ::larutil::Geometry::GetME()->ChannelToWireID(wire_roi.Channel());
      
      if ((int)(wire_id.Plane) != 2) continue;

      for (auto const& range : wire_roi.SignalROI().get_ranges()) {

	auto const& adcs = range.data();

	int start_index = range.begin_index();

	for (size_t index = 0; index < adcs.size(); ++index) {
	  size_t tick = start_index + index;
	  if( adcs[index] <= 15. ) continue;
	  if( tick < 800 ) continue;
	  if( tick >= 5400 ) continue;
	  tick -= 800;
	  //qmap[size_t(wire_id.Wire/6.)][size_t(tick/6.)] += adcs[index] / 30. * 29000. * 0.3;
	  size_t vbox = tick / _num_ticks;
	  size_t hbox = wire_id.Wire / _num_wires;
	  size_t qclus_idx = hbox + vbox * tot_hbox;
	  if(qclus_idx >= qclus_v.size())
	    std::cout << "wire: " << wire_id.Wire << " tick: " << tick
		      << "hbox: " << hbox<< " vbox: " << vbox << " qclus_idx: " << qclus_idx << std::endl;
	  auto& qclus = qclus_v.at(qclus_idx);
	  flashana::QPoint_t pt;
	  pt.y = 0;
	  pt.x = tick * 0.5 * drift_velocity;
	  pt.z = wire_id.Wire * 0.3;
	  pt.q = adcs[index] / 30. * 8000. * 0.3;
	  qclus.emplace_back(std::move(pt));
	}
      }
    }
    /*
    for(size_t w=0; w<qmap.size(); ++w) {
      size_t wire = 3*w + 3;
      for(size_t t=0; t<qmap[w].size(); ++t) {
	size_t tick = 3*t + 3;

	size_t vbox = tick / _num_ticks;
	size_t hbox = wire / _num_wires;
	
	size_t qclus_idx = hbox + vbox * tot_hbox;
	auto& qclus = qclus_v[qclus_idx];
	flashana::QPoint_t pt;
	pt.y = 0;
	pt.x = tick * 0.5 * drift_velocity;
	pt.z = wire * 0.3;
	pt.q = qmap[w][t];
	//if(pt.q<100) continue;
	qclus.emplace_back(std::move(pt));	
      }
    }
    */
    for(size_t i=0; i<qclus_v.size(); ++i) {
      qclus_v[i].idx = i;
      _mgr.Add(qclus_v[i]);
    }

    _result = _mgr.Match();
    
    return true;
  }

  bool FlashMatch2D::finalize() {

    return true;
  }

}
#endif
