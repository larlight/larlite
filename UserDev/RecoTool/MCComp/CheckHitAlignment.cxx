#ifndef LARLITE_CHECKHITALIGNMENT_CXX
#define LARLITE_CHECKHITALIGNMENT_CXX

#include "CheckHitAlignment.h"

namespace larlite {

  CheckHitAlignment::CheckHitAlignment()
    : _tree(nullptr)
  {
    _fout = 0;
    _name = "CheckHitAlignment";
    _nsigma = 3;
    _offset = 0;

  }

  bool CheckHitAlignment::initialize() {

    if (_tree) delete _tree;
    _tree = new TTree("_tree","tree");
    _tree->Branch("_ch",&_ch,"ch/I");
    _tree->Branch("_pl",&_pl,"pl/I");
    _tree->Branch("_nhits",&_nhits,"nhits/I");
    _tree->Branch("_tick",&_tick,"tick/D");
    _tree->Branch("_integral",&_integral,"integral/D");
    _tree->Branch("_ampl",&_ampl,"ampl/D");
    _tree->Branch("_rms",&_rms,"rms/D");
    _tree->Branch("_chi",&_chi,"chi/D");
    _tree->Branch("_qhit",&_qhit,"qhit/D");
    _tree->Branch("_edep",&_edep,"edep/D");
    _tree->Branch("_xpos",&_xpos,"xpos/D");

    // warning offset uninitialized!
    if (_offset == 0){
      std::cout << "Warning! Offset uninitialized!" << std::endl;
      return false;
    }
    
    return true;
  }
  
  bool CheckHitAlignment::analyze(storage_manager* storage) {

    // clear maps
    _chan_to_hit_map.clear();
    _chan_to_ide_map.clear();
    _chan_to_edep_map.clear();
    _chan_to_xpos_map.clear();

    // load hits
    auto ev_hits = storage->get_data<event_hit>(_hit_producer);

    // load simchannel info
    auto ev_simch = storage->get_data<event_simch>("largeant");

    // loop through hits and fill a map (channel number -> hit)
    for (size_t i=0; i < ev_hits->size(); i++){
      auto const& hit = ev_hits->at(i);
      // has this channel been added yet?
      if (_chan_to_hit_map.find( hit.Channel() ) == _chan_to_hit_map.end()){
	std::vector<larlite::hit> hit_v = {hit};
	_chan_to_hit_map[ hit.Channel() ] = hit_v;
      }
      else
	_chan_to_hit_map[ ev_hits->at(i).Channel() ].push_back( hit );
    }// for all hits

    // loop through simchannels and get energy deposited at various TDCs
    for (size_t i=0; i < ev_simch->size(); i++){
      auto const& simch = ev_simch->at(i);
      unsigned short ch = simch.Channel();
      // get map connecting TDC -> ide
      auto const TDC_IDE_map = simch.TDCIDEMap();
      // map connecting TDC -> charge
      std::map< unsigned short, double > tdcQ;
      std::map< unsigned short, double > tdcE;
      std::map< unsigned short, double > tdcX;
      typedef std::map<unsigned short, std::vector<larlite::ide> >::const_iterator ittype;
      for (ittype it = TDC_IDE_map.begin(); it != TDC_IDE_map.end(); it++){
	// get tick
	unsigned short tick = it->first;
	// get charge
	double Q = 0;
	// and energy
	double E = 0;
	// and X position
	double X = 0;
	auto const& ides = it->second;
	// loop throgh IDEs
	for (auto const& ide : ides){
	  Q += ide.numElectrons;
	  E += ide.energy;
	  X += ide.x;
	}
	tdcQ[ tick ] = Q;
	tdcE[ tick ] = E;
	tdcX[ tick ] = X / ((float)(ides.size()));
      }// for all ides for this tick
      // add TDC -> Q map to a simchanel map
      _chan_to_ide_map [ ch ] = tdcQ;
      _chan_to_edep_map[ ch ] = tdcE;
      _chan_to_xpos_map[ ch ] = tdcX;
    }// for all simchannels
      

    // now fill hit -> simchannel ttree

    for (_ch=0; _ch < 8256; _ch++){

      if (this->hasHits(_ch) == false) continue;

      auto const& hits = this->getHits(_ch);
      auto const& ides = this->getIDEs(_ch);
      auto const& edep = this->getEDEP(_ch);
      auto const& xpos = this->getXPOS(_ch);

      for (auto const& hit : hits){

	_pl = hit.WireID().Plane;
	_tick = hit.PeakTime();
	_integral = hit.Integral();
	_ampl = hit.PeakAmplitude();
	_chi = hit.GoodnessOfFit();
	_nhits = hit.Multiplicity();
	_rms = hit.RMS();
	_qhit = 0;
	_edep = 0;
	_xpos = 0;

	for (auto const& ide : ides){

	  if ( ( (ide.first - _offset) > hit.PeakTime() - _nsigma * hit.RMS()) and
	       ( (ide.first - _offset) < hit.PeakTime() + _nsigma * hit.RMS()) )
	    _qhit += ide.second;
	  
	}// for all ides


	for (auto const& e : edep){

	  if ( ( (e.first - _offset) > hit.PeakTime() - _nsigma * hit.RMS()) and
	       ( (e.first - _offset) < hit.PeakTime() + _nsigma * hit.RMS()) )
	    _edep += e.second;
	  
	}// for all edeps

	int nxpos = 0;
	for (auto const& x : xpos){
		  
	  if ( ( (x.first - _offset) > hit.PeakTime() - _nsigma * hit.RMS()) and
	       ( (x.first - _offset) < hit.PeakTime() + _nsigma * hit.RMS()) ){
	    nxpos += 1;
	    _xpos += x.second;
	  }
	  
	}// for all ides

	_xpos /= ((float)nxpos);

	_tree->Fill();

      }// for all hits

    }// for all channels
    
    return true;
  }

  bool CheckHitAlignment::finalize() {

    if (_fout) _fout->cd();
    _tree->Write();

    return true;
  }

}
#endif
