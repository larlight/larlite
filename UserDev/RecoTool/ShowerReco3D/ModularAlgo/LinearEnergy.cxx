#ifndef LINEARENERGY_CXX
#define LINEARENERGY_CXX

#include <iomanip>

#include "LinearEnergy.h"
#include "LArUtil/GeometryHelper.h"
#include "LArUtil/DetectorProperties.h"
#include "LArUtil/LArProperties.h"
#include "AnalysisAlg/AnalysisAlgConstants.h"

namespace showerreco {
  
  LinearEnergy::LinearEnergy()
  {
    
    _name = "LinearEnergy";
    _fill_tree   = false;
    _fC_to_e = 6250.; // e- / fC
    _e_to_eV = 23.6;  // eV / e-
    _eV_to_MeV = 1e-6;// eV / MeV 
    _ADC_to_mV = 0.5; // ADC -> mV conversion from gain measurements
    
    // ELECTRON LIFETIME
    _tau = larutil::LArProperties::GetME()->ElectronLifetime();
    
    // RECOMBINATION FACTOR
    _recomb_factor = 1.;
    return;
  }
  
  void LinearEnergy::initialize()
  {
    
    if (_fill_tree) {
      if (_tree) delete _tree;
      _tree = new TTree(_name.c_str(), "dQdx Info Tree");
      _tree->Branch("_dE", &_dE, "dE/D");
      _tree->Branch("_dQ", &_dQ, "dQ/D");
      _tree->Branch("_lifetime_corr", &_lifetime_corr, "lifetime_corr/D");
      _tree->Branch("_electrons",&_electrons,"electrons/D");
      _tree->Branch("_tick",&_tick,"tick/D");
    }
    
    std::cout << "Electron lifetime : " << _tau << " [ms]" << std::endl
	      << "Recomb factor     : " << _recomb_factor << std::endl
	      << "ADC -> fC         : " << _ADC_to_fC << std::endl
	      << "fC  -> e-         : " << _fC_to_e << std::endl
	      << "e-  -> eV         : " << _e_to_eV << std::endl
	      << "eV  -> MeV        : " << _eV_to_MeV << std::endl
	      << "ADC -> MeV        : " << _ADC_to_MeV << std::endl;
    
    _clocktick = larutil::DetectorProperties::GetME()->SamplingRate() * 1.e-3; //time sample in usec
    
    return;
  }
  
  void LinearEnergy::do_reconstruction(const ::protoshower::ProtoShower & proto_shower,
				       Shower_t& resultShower) {
    
    //if the module does not have 2D cluster info -> fail the reconstruction
    if (!proto_shower.hasCluster2D()){
      std::stringstream ss;
      ss << "Fail @ algo " << this->name() << " due to missing 2D cluster";
      throw ShowerRecoException(ss.str());
    }
    
    auto & clusters = proto_shower.params();
    
    // This function takes the shower cluster set and calculates an energy in MeV for each plane
    
    // 
    auto geomHelper = larutil::GeometryHelper::GetME();
    
    // check if plane 2 has been used.
    // if so, we will fill the global energy with that from plane 2
    // otherwise, average the other two planes
    bool hasPl2 = false;
    
    auto t2cm = geomHelper->TimeToCm();
    
    // we want an energy for each plane
    for (size_t n = 0; n < clusters.size(); n++) {
      
      // get the hits associated with this cluster
      auto const& hits = clusters.at(n).hit_vector;
      
      // get the plane associated with this cluster
      auto const& pl = clusters.at(n).plane_id.Plane;
      
      if (pl == 2)
	hasPl2 = true;
      
      // store calculated energy
      double E  = 0.;
      
      // loop over hits
      for (auto const &h : hits) {
	
	// lifetime correction
	_tick = h.t / t2cm;
	
	_lifetime_corr = exp( _tick * _clocktick / _tau );

	double qcorr = ChargeCorrection(h,resultShower.fDCosStart,resultShower.fXYZStart);
	
	_electrons = qcorr * _elec_gain;
	
	_dQ = _electrons * _lifetime_corr * _e_to_eV * _eV_to_MeV;

	
	_dE = _dQ / _recomb_factor;
	
	_tree->Fill();
	
	E += _dE;
	
      }// loop over all hits
      
      if (_verbose)
	std::cout << "energy on plane " << pl << " is : " << E << std::endl;
      
      // set the energy for this plane
      resultShower.fTotalEnergy_v[pl] = E;
      
    }// for all input clusters
    
    if (hasPl2)
      resultShower.fTotalEnergy = resultShower.fTotalEnergy_v[2];
    else
      resultShower.fTotalEnergy = ( resultShower.fTotalEnergy_v[0] + resultShower.fTotalEnergy_v[1] ) / 2.;
    
    return;
    
  }
  
  void LinearEnergy::CreateResponseMap(const double& stepsize) {
    
    _responseMap.clear();
    _responseStep = stepsize;

    auto geom = larutil::Geometry::GetME();
    
    int nx = int((geom->DetHalfWidth() * 2)  / _responseStep);
    int ny = int((geom->DetHalfHeight() * 2) / _responseStep);
    int nz = int((geom->DetLength())         / _responseStep);
    
    _responseMap = std::vector< std::vector< std::vector<double> > >(nx,std::vector< std::vector<double> >(ny, std::vector<double>(nz,0) ) );
    
    return;
  }

  double LinearEnergy::ChargeCorrection(const larutil::Hit2D& h,const TVector3& dir,const TVector3& vtx){

    auto geom = larutil::Geometry::GetME();

    // find 3D position of hit
    double z = h.w;
    double x = h.t;

    // get 2D distance of hit to vtx
    double r2D = sqrt( ( (z-vtx.Z()) * (z-vtx.Z()) ) + ( (x-vtx.X()) * (x-vtx.X()) ) );
    double r3D = r2D/dir[1];

    auto xyz = vtx + dir * r3D;

    double y = xyz[1];

    std::cout << "hit x = " << x << "\t z = " << z << std::endl;
    std::cout << "3D point x = "<< xyz[0] << ", y = " << xyz[1] << ", z = " << xyz[2] << std::endl;

    // find cells in map
    size_t i = ( (geom->DetHalfWidth()  * 2) - x) / _responseStep;
    size_t j = ( (2 * geom->DetHalfHeight()) - (geom->DetHalfHeight() + y) ) / _responseStep;
    size_t k = (geom->DetLength() - z) / _responseStep;

    // make sure not out of bounds:
    if ( (i >= 0) or (j > 0) or (k > 0) or
	 (i < _responseMap.size()) or
	 (j < _responseMap[0].size()) or
	 (k < _responseMap[0][0].size()) ) {
      return h.charge;
    }

    double corr = _responseMap[i][j][k];

    if (corr < 0) {
      std::cout << "No correction available..." << std::endl;
      // find nearest non-empty box
      corr = NearestCell(i,j,k);
    }

    return h.charge * corr;
    
  }
  
  
  void LinearEnergy::SetResponse(const double& x, const double& y, const double& z, const double& q)
    
  {

    auto geom = larutil::Geometry::GetME();
    
    if ( (x < 0) || (x > 2 * geom->DetHalfWidth()) ) {
      std::cout << "X value out of bounds." << std::endl;
      return;
    }

    if ( (y < -geom->DetHalfHeight()) || (y > geom->DetHalfHeight()) ) {
      std::cout << "y value out of bounds." << std::endl;
      return;
    }

    if ( (z < 0) || (z > geom->DetLength()) ) {
      std::cout << "X value out of bounds." << std::endl;
      return;
    }

    // find vector position
    size_t i = ( (geom->DetHalfWidth()  * 2) - x) / _responseStep;
    size_t j = ( (2 * geom->DetHalfHeight()) - (geom->DetHalfHeight() + y) ) / _responseStep;
    size_t k = (geom->DetLength() - z) / _responseStep;
    
    _responseMap[i][j][k] = q;

    return;
  }

  double LinearEnergy::NearestCell(const size_t& i, const size_t& j, const size_t& k) {

    double val;
    int    ctr;

    std::vector<int> cellspan;

    for (int cellsize = 1; cellsize < 4; cellsize++) {

      val = 0;
      ctr = 0;

      cellspan.clear();
      for (int n=1; n <= cellsize; n++){
	cellspan.push_back(-n);
	cellspan.push_back(n);
      }
      
      for (auto const& xd : cellspan) {
	for (auto const& yd : cellspan) {
	  for (auto const& zd : cellspan) {
	    
	    if ( (i+xd >= 0) and (j+yd > 0) and (k+zd > 0) and
		 (i+xd < _responseMap.size()) and
		 (j+yd < _responseMap[0].size()) and
		 (k+zd < _responseMap[0][0].size()) ) {
	      
	      if ( _responseMap[i+xd][j+yd][k+zd] > 0){
		val += _responseMap[i+xd][j+yd][k+zd];
		ctr += 1;
	      }
	      
	    }// if not out of bounds
	    
	  }// z loop
	}// y loop
      }// x loop

      if (val != 0) {
	val /= ctr;
	std::cout << "val is " << val << std::endl;
	return val;
      }
      
    }// for all cell-sizes

    // if we never found non-zero correction, return 1
    std::cout << "no neighboring correction found..return 1" << std::endl;
    return 1;
  }

} //showerreco

#endif
