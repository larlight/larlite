#ifndef LARLITE_COSMICRATEANA_CXX
#define LARLITE_COSMICRATEANA_CXX

#include "CosmicRateAna.h"
#include "DataFormat/ophit.h"
#include "DataFormat/opflash.h"
#include <TGraphErrors.h>
namespace larlite {

  bool CosmicRateAna::initialize() {
    _hBeamWindowWidth = new TH1D("hBeamWindowWidth",
				 "Width of Beam Gate Window; Width [#mus]; Events",
				 100,0,50);
    return true;
  }
  
  bool CosmicRateAna::analyze(storage_manager* storage) {

    auto ev_flash = storage->get_data<event_opflash>("opflash");
    auto ev_hit   = storage->get_data<event_ophit>("opflash");

    double t_min=1e9;
    double t_max=-1e9;
    for(auto const& h : *ev_hit) {

      if(h.PeakTime() < t_min) t_min = h.PeakTime();
      if(h.PeakTime() > t_max) t_max = h.PeakTime();
      //std::cout<<h.PeakTime()<<std::endl;
    }
    //std::cout<<t_min<<" : "<<t_max<<" ... "<<ev_flash->size()<<std::endl;

    _hBeamWindowWidth->Fill(t_max - t_min);

    if( t_max==-1e9 || (t_max - t_min) < 20 ) return false;

    t_min = t_min + 2.5;
    t_max = t_max - 2.5;


    //if( (t_max - t_min) < 10 ) return false;

    int n_window = ((int)(t_max - t_min))/1.7;

    for(auto const& flash : *ev_flash) {

      if(flash.Time() < t_min || flash.Time() > (t_min+n_window * 1.7)) continue;

      double flash_time = flash.Time() - t_min;

      unsigned int window_id = ((int)(flash_time))/1.7;

      if(window_id >= n_window) continue;
      
      double local_time = flash_time - 1.7 * window_id;
      if(local_time < 0 || local_time > 1.7) continue;

      for(size_t i=0; i<_flash_ctr_v.size(); ++i) {

	if(flash.TotalPE() < _npe_cut_v[i]) break;
	_flash_ctr_v[i] += 1;
	
      }
    }
    
    _total_window_ctr += n_window;
    
    return true;
  }

  bool CosmicRateAna::finalize() {

    if(_fout) {

      TGraphErrors* g = new TGraphErrors(_flash_ctr_v.size());
      g->SetName("gMisID");
      g->SetTitle("Cosmic Mis ID Rate; Num. Photo-Electron Cut; Mis-ID Rate");
      for(size_t i=0; i<_flash_ctr_v.size(); ++i) {

	g->SetPoint(i, _npe_cut_v[i], ((double)(_flash_ctr_v[i])) / (double)(_total_window_ctr) );

	g->SetPointError(i, 0, 0);

      }
      
      _fout->cd();
      g->Write();
      _hBeamWindowWidth->Write();

    }
    
    return true;
  }

}
#endif
