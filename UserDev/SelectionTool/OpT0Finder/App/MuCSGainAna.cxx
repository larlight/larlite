#ifndef LARLITE_MUCSGAINANA_CXX
#define LARLITE_MUCSGAINANA_CXX

#include "MuCSGainAna.h"
#include "DataFormat/ophit.h"
namespace larlite {

  bool MuCSGainAna::initialize() {

    _tree = new TTree("tree","");
    _tree->Branch("area",&_area,"area/D");
    _tree->Branch("amp",&_amp,"amp/D");
    _tree->Branch("pe",&_pe,"pe/D");
    _tree->Branch("ch",&_ch,"ch/I");
    _tree->Branch("twidth",&_twidth,"twidth/D");
    _tree->Branch("tpeak",&_tpeak,"tpeak/D");

    return true;
  }
  
  bool MuCSGainAna::analyze(storage_manager* storage) {
  
    auto ev_ophit = storage->get_data<event_ophit>(_ophit_producer);

    for(auto const& oph : *ev_ophit) {
      _ch = oph.OpChannel();
      if(_ch > 31)  continue;
      _tpeak = oph.PeakTime();
      if(_tpeak < 4.0 || _tpeak > 23) continue;
      _amp = oph.Amplitude();
      if(_amp > 90) continue;
      _pe = oph.PE();
      _area = oph.Area();
      _twidth = oph.Width();
      _tree->Fill();
    }

    return true;
  }

  bool MuCSGainAna::finalize() {

    if(_fout) {
      _fout->cd();
      _tree->Write();
    }
    return true;
  }

}
#endif
