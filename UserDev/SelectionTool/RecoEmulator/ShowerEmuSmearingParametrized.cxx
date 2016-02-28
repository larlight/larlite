#ifndef __SHOWEREMUSMEARINGPARAMETRIZED_CXX__
#define __SHOWEREMUSMEARINGPARAMETRIZED_CXX__

#include "ShowerEmuSmearingParametrized.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

namespace recoemu {

  ShowerEmuSmearingParametrized::ShowerEmuSmearingParametrized()
    : ShowerEmulatorBase("ShowerEmuSmearingParametrized")
  {
    srand( time(NULL) );
  }

  void ShowerEmuSmearingParametrized::Configure(const ::fcllite::PSet &pset)
  {

    // efficiency function
    // this function is used to determine whether a shower should
    // be reconstructed in the first place
    // the return is expected to be from 0 and 1, denoting the
    // reconstruction efficiency
    // the x-axis is taken as the shower energy, in MeV
    auto EffFunc   = pset.get< std::string >("EffFunc");
    auto EffParams = pset.get< std::vector<double> >("EffParams");
    auto EffMin    = pset.get< double >("EffMin");
    auto EffMax    = pset.get< double >("EffMax");
    _fEff = new TF1("eff", EffFunc.c_str(), EffMin, EffMax);
    for (size_t i = 0; i < EffParams.size(); i++)
      _fEff->SetParameter(i, EffParams[i]);

    // energy resolution function to be applied
    // this function should represent the fractional energy
    // resolution @ 1 GeV of shower energy
    // the energy-dependence is introduced according to
    // Eres (E) = Eres (1 GeV) / sqrt( E [GeV] / 1 GeV )
    auto EResFunc   = pset.get< std::string >("EResFunc");
    auto EResParams = pset.get< std::vector<double> >("EResParams");
    auto EResMin    = pset.get< double >("EResMin");
    auto EResMax    = pset.get< double >("EResMax");
    _fEres = new TF1("eres", EResFunc.c_str(), EResMin, EResMax);
    for (size_t i = 0; i < EResParams.size(); i++)
      _fEres->SetParameter(i, EResParams[i]);

    // angular resolution function
    // this function represents how the angle will be smeared
    // the values sampled are assumed to be in degrees
    auto AngleResFunc   = pset.get< std::string >("AngleResFunc");
    auto AngleResParams = pset.get< std::vector<double> >("AngleResParams");
    auto AngleResMin    = pset.get< double >("AngleResMin");
    auto AngleResMax    = pset.get< double >("AngleResMax");
    _fAngleres = new TF1("angleres", AngleResFunc.c_str(), AngleResMin, AngleResMax);
    for (size_t i = 0; i < AngleResParams.size(); i++)
      _fAngleres->SetParameter(i, AngleResParams[i]);

    // dEdx smearing function
    // this is an additional resolution function to be applied
    // to the input dEdx
    // this is a fractional resolution
    auto dEdxResFunc   = pset.get< std::string >("dEdxResFunc");
    auto dEdxResParams = pset.get< std::vector<double> >("dEdxResParams");
    auto dEdxResMin    = pset.get< double >("dEdxResMin");
    auto dEdxResMax    = pset.get< double >("dEdxResMax");
    _fdEdxres = new TF1("dedxres", dEdxResFunc.c_str(), dEdxResMin, dEdxResMax);
    for (size_t i = 0; i < dEdxResParams.size(); i++)
      _fdEdxres->SetParameter(i, dEdxResParams[i]);

    // position resolution function
    // x,y,z start positions are all smeared according to this function
    // the smearing is absolute (in cm)
    auto PosResFunc   = pset.get< std::string >("PosResFunc");
    auto PosResParams = pset.get< std::vector<double> >("PosResParams");
    auto PosResMin    = pset.get< double >("PosResMin");
    auto PosResMax    = pset.get< double >("PosResMax");
    _fPosres = new TF1("posres", PosResFunc.c_str(), PosResMin, PosResMax);
    for (size_t i = 0; i < PosResParams.size(); i++)
      _fPosres->SetParameter(i, PosResParams[i]);

    // direction flip
    // fraction of time a shwower's direction should be flipped
    // this function is applied uniformly for all showers
    _DirectionFlipFrac = pset.get< double >("DirectionFlipFrac");

  }

  recoemu::Shower_t ShowerEmuSmearingParametrized::Emulate(const recoemu::Shower_t& mc)
  {

    Shower_t result;


    // output dedx = input dedx (for now)
    result.dedx = mc.dedx;

    // smear energy
    // resolution is energy rependent:
    // Eres = frac / sqrt(E [1 GeV] )
    double fracres = _fEres->GetRandom() / sqrt( mc.energy / 1000. );
    result.energy = mc.energy * ( 1 + fracres );
    if (result.energy < 0)
      result.energy = 0;

    // Now, use the reconstruction efficiency function to determine if the shower
    // should be reconstructed (based on the SMEARED energy)
    // convert from energy (MeV) to length (cm) because this functional form
    // takes as input cm length. To convert use 2.3 MeV/cm assumption
    // which is reasonable for the single muon tracks the function was fitted to
    double eff = _fEff->Eval( result.energy / 2.3 );

    // random number
    double randeff = ( (double) rand() ) / RAND_MAX;

    // if we sample a number above the efficiency value -> mark for deletion
    if (randeff > eff) {
      result.mark_for_deletion = true;
      return result;
    }

    // smear start point
    ::geoalgo::Point_t start( mc.cone.Start()[0] + _fPosres->GetRandom(),
                              mc.cone.Start()[1] + _fPosres->GetRandom(),
                              mc.cone.Start()[2] + _fPosres->GetRandom() );

    // smear direction
    // result is in degrees
    double anglesmear = _fAngleres->GetRandom();

    // current momentum components
    double px = mc.cone.Dir()[0];
    double py = mc.cone.Dir()[1];
    double pz = mc.cone.Dir()[2];

    // find rotation angles to bring momentum vector to z-direction
    double phi   = 0.;
    if ( px != 0)
      phi = acos( px / sqrt(px * px + py * py) );
    double theta = 0.;
    if ( (px != 0) and (py != 0) )
      theta = acos( pz / sqrt(px * px + py * py + pz * pz) );

    // generate a 3D vector with an angle = anglesmear
    // w.r.t. the z-direction, and a random phase
    // in the phi direction
    double random_phase = 2 * 3.1415 * ( (double) rand() ) / RAND_MAX;

    double px_new = cos(random_phase);
    double py_new = sin(random_phase);

    // the dot product of the pz_new and the z-direction needs to be the cosine
    // of the smearing angle
    double pz_new = cos( anglesmear * 3.1415 / 180. );
    // normalize px_new and py_new so that p_new is a unit vector
    double p_perp_mag = sqrt( 1 - pz_new * pz_new);
    px_new *= p_perp_mag;
    py_new *= p_perp_mag;

    ::geoalgo::Vector_t dir(px_new, py_new, pz_new);

    // now rotatte by the inverse angles
    dir.RotateY(theta);
    dir.RotateZ(phi);

    // flip direction if necessary
    double randnum = ( (double) rand() ) / RAND_MAX;
    if (randnum < _DirectionFlipFrac) {

      // if the direction change we should account for other shifts
      // start point will also be flipped
      // and dE/dx will most likely be ~ 2 MeV/cm regardless
      // of whether we have a photon or electron shower.
      // 1) flip start point by getting the length from the energy
      double len = _EMprofile.Length(mc.energy);
      // move the start point to a point at the end of the shower, given the length and direction
      start += dir * len;

      // 2) change the dedx
      if (mc.pdg == 22)
        result.dedx /= 2;

      // 3) finally, flip the direction
      dir *= -1.;

    }// if the direction should be flipped

    ::geoalgo::Cone_t cone(start, dir, mc.cone.Length(), mc.cone.Radius());

    result.cone = cone;

    // smear dEdx
    // draw a random value from the dEdx smearing distribution
    double dedx_smeared = _fdEdxres->GetRandom();
    result.dedx = mc.dedx + ( mc.dedx * dedx_smeared );
    if (result.dedx < 0)
      result.dedx = 0;

    result.pdg  = mc.pdg;
    result.time = mc.time;

    return result;
  }

}

#endif
