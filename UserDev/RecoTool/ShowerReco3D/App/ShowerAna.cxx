#ifndef LARLITE_SHOWERANA_CXX
#define LARLITE_SHOWERANA_CXX

#define _USE_MATH_DEFINES
 
#include <cmath>
#include <iostream>
#include "ShowerAna.h"
#include "DataFormat/shower.h"

namespace larlite {

  ShowerAna::ShowerAna() 
    : _shower_tree(nullptr)
  {
    _producer = "showerreco";
    _name     = "ShowerAna";
    _fout     = 0;
  }

  bool ShowerAna::initialize() {

    if (_shower_tree) { delete _shower_tree; }

    _shower_tree = new TTree("_shower_tree","Reco Shower Info Tree");

    _shower_tree->Branch("_energy_Y",&_energy_Y,"energy_Y/D");
    _shower_tree->Branch("_energy_V",&_energy_V,"energy_V/D");
    _shower_tree->Branch("_energy_U",&_energy_U,"energy_U/D");
    _shower_tree->Branch("_dedx_Y",&_dedx_Y,"dedx_Y/D");
    _shower_tree->Branch("_dedx_V",&_dedx_V,"dedx_V/D");
    _shower_tree->Branch("_dedx_U",&_dedx_U,"dedx_U/D");
    _shower_tree->Branch("_dqdx_Y",&_dqdx_Y,"dqdx_Y/D");
    _shower_tree->Branch("_dqdx_V",&_dqdx_V,"dqdx_V/D");
    _shower_tree->Branch("_dqdx_U",&_dqdx_U,"dqdx_U/D");
    _shower_tree->Branch("_x",&_x,"x/D");
    _shower_tree->Branch("_y",&_y,"y/D");
    _shower_tree->Branch("_z",&_z,"z/D");
    _shower_tree->Branch("_px",&_px,"px/D");
    _shower_tree->Branch("_py",&_py,"py/D");
    _shower_tree->Branch("_pz",&_pz,"pz/D");
    // theta is the angle w.r.t. the Z direction
    _shower_tree->Branch("_theta",&_theta,"theta/D");
    // phi is the angle w.r.t. the XY plane (0 if aligned w/ positive X axis)
    _shower_tree->Branch("_phi",&_phi,"phi/D");
    _shower_tree->Branch("_opening_angle",&_opening_angle,"opening_angle/D");
    _shower_tree->Branch("_length",&_length,"length/D");

    return true;
  }
        
  
  
  bool ShowerAna::analyze(storage_manager* storage) {

    event_shower * ev_shower = storage -> get_data<event_shower>(_producer);

    if (!ev_shower or (ev_shower->size() == 0) )
      return true;
    
    for (auto & shower : * ev_shower){

      _energy_U = shower.Energy_v()[0];
      _energy_V = shower.Energy_v()[1];
      _energy_Y = shower.Energy_v()[2];
      _dqdx_U   = shower.dQdx_v()[0];
      _dqdx_V   = shower.dQdx_v()[1];
      _dqdx_Y   = shower.dQdx_v()[2];
      _dedx_U   = shower.dEdx_v()[0];
      _dedx_V   = shower.dEdx_v()[1];
      _dedx_Y   = shower.dEdx_v()[2];
      _x        = shower.ShowerStart()[0];
      _y        = shower.ShowerStart()[1];
      _z        = shower.ShowerStart()[2];
      _px       = shower.Direction()[0];
      _py       = shower.Direction()[1];
      _pz       = shower.Direction()[2];
      _length   = shower.Length();
      _phi      = 0.;
      _theta    = 0.;
      _opening_angle = shower.OpeningAngle();

      _shower_tree->Fill();

    }// for all showers

    return true;
  }

  bool ShowerAna::finalize() {

    if (_fout){
      _fout -> cd();
      _shower_tree->Write();
    }

    return true;
  }

}
#endif
