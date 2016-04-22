#ifndef __SHOWEREMUSMEARINGRECOBASED_CXX__
#define __SHOWEREMUSMEARINGRECOBASED_CXX__

#include "ShowerEmuSmearingRecoBased.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

namespace recoemu {
  
  ShowerEmuSmearingRecoBased::ShowerEmuSmearingRecoBased()
    : ShowerEmulatorBase("ShowerEmuSmearingRecoBased")
  { 
    srand( time(NULL) ); 
  }

  void ShowerEmuSmearingRecoBased::Configure(const ::fcllite::PSet &pset)
  {
    
    auto EnergyResolutionValues = pset.get< std::vector<double> >("EnergyResolutionValues");
    auto EnergyResolutionMin    = pset.get<double>("EnergyResolutionMin");
    auto EnergyResolutionMax    = pset.get<double>("EnergyResolutionMax");

    _energySmear.SetDistribution(EnergyResolutionValues,EnergyResolutionMin,EnergyResolutionMax);

    auto XResolutionValues = pset.get< std::vector<double> >("XResolutionValues");
    auto XResolutionMin    = pset.get<double>("XResolutionMin");
    auto XResolutionMax    = pset.get<double>("XResolutionMax");

    _xSmear.SetDistribution(XResolutionValues,XResolutionMin,XResolutionMax);

    auto YResolutionValues = pset.get< std::vector<double> >("YResolutionValues");
    auto YResolutionMin    = pset.get<double>("YResolutionMin");
    auto YResolutionMax    = pset.get<double>("YResolutionMax");

    _ySmear.SetDistribution(YResolutionValues,YResolutionMin,YResolutionMax);

    auto ZResolutionValues = pset.get< std::vector<double> >("ZResolutionValues");
    auto ZResolutionMin    = pset.get<double>("ZResolutionMin");
    auto ZResolutionMax    = pset.get<double>("ZResolutionMax");

    _zSmear.SetDistribution(ZResolutionValues,ZResolutionMin,ZResolutionMax);

    auto AngleResolutionValues = pset.get< std::vector<double> >("AngleResolutionValues");
    auto AngleResolutionMin    = pset.get<double>("AngleResolutionMin");
    auto AngleResolutionMax    = pset.get<double>("AngleResolutionMax");

    _angleSmear.SetDistribution(AngleResolutionValues,AngleResolutionMin,AngleResolutionMax);

    auto dEdxResolutionValues = pset.get< std::vector<double> >("dEdxResolutionValues");
    auto dEdxResolutionMin    = pset.get<double>("dEdxResolutionMin");
    auto dEdxResolutionMax    = pset.get<double>("dEdxResolutionMax");
    _dEdxResolutionOffset     = (2 - pset.get<double>("dEdxResolutionMean")) / 2.;
    // std::cout << "dedx offset is " << _dEdxResolutionOffset << std::endl;

    _dedxSmear.SetDistribution(dEdxResolutionValues,dEdxResolutionMin, dEdxResolutionMax);
    
  }
  
  recoemu::Shower_t ShowerEmuSmearingRecoBased::Emulate(const recoemu::Shower_t& mc)
  {



    Shower_t result;

    // smear energy
    result.energy = mc.energy + mc.energy * _energySmear.Draw();
    //std::cout << "energy : " << mc.energy << " -> " << result.energy << std::endl;

    // smear start point
    ::geoalgo::Point_t start( mc.cone.Start()[0] + _xSmear.Draw(), mc.cone.Start()[1] + _ySmear.Draw(), mc.cone.Start()[2] + _zSmear.Draw() );

    // smear direction
    double anglesmear = _angleSmear.Draw();
    //std::cout << "smearing angle : " << anglesmear << std::endl;

    // current momentum components
    double px = mc.cone.Dir()[0];
    double py = mc.cone.Dir()[1];
    double pz = mc.cone.Dir()[2];

    // find rotation angles to bring momentum vector to z-direction
    double phi   = 0.;
    if ( px != 0)
      phi = acos( px / sqrt(px*px+py*py) );
    double theta = 0.;
    if ( (px !=0) and (py != 0) )
      theta = acos( pz / sqrt(px*px+py*py+pz*pz) );

    // generate a 3D vector with an angle = anglesmear
    // w.r.t. the z-direction, and a random phase
    // in the phi direction
    double random_phase = 2 * 3.1415 * ( (double) rand() ) / RAND_MAX;

    //std::cout << "random angle phase is : " << random_phase << std::endl;
    
    double px_new = cos(random_phase);
    double py_new = sin(random_phase);
    
    // the dot product of the pz_new and the z-direction needs to be the cosine
    // of the smearing angle
    double pz_new = cos( anglesmear * 3.1415 / 180. );
    // normalize px_new and py_new so that p_new is a unit vector
    double p_perp_mag = sqrt( 1 - pz_new*pz_new);
    px_new *= p_perp_mag;
    py_new *= p_perp_mag;

    ::geoalgo::Vector_t dir(px_new,py_new,pz_new);

    // now rotatte by the inverse angles
    dir.RotateY(theta);
    dir.RotateZ(phi);

    ::geoalgo::Cone_t cone(start,dir,mc.cone.Length(),mc.cone.Radius());
    
    result.cone = cone;
    
    // smear dEdx
    // draw a random value from the dEdx smearing distribution
    double dedx_smeared = _dedxSmear.Draw();
    // calculate offset w.r.t. central value (2 for e-, 4 for gamma)
    if (mc.pdg == 11)
      dedx_smeared *= ( 2 * (1 - _dEdxResolutionOffset) );
    if (mc.pdg == 22)
      dedx_smeared *= ( 4 * (1 - _dEdxResolutionOffset) );
    
    result.dedx = dedx_smeared;
    result.pdg  = mc.pdg; 
    result.time = mc.time;

    return result;
  }

}

#endif
