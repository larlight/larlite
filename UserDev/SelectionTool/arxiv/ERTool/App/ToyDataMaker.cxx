#ifndef TOYDATAMAKER_CXX
#define TOYDATAMAKER_CXX

#include "ToyDataMaker.h"

#endif
namespace ertool {

Track ToyDataMaker::MakeTrack(double x, double y, double z, double px,
		double py, double pz, int pdg_code) {
	Track t;

	// Add start point to trajectory
	t.push_back(::geoalgo::Vector(x, y, z));

	auto mass = ParticleMass(pdg_code);
	auto p = ::geoalgo::Vector(px, py, pz);

	t._energy = sqrt(pow(mass, 2) + pow(px, 2) + pow(py, 2) + pow(pz, 2));
	double current_energy = t._energy;

	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "Particle Info: " << std::endl;
	std::cout << " - Mass: " << mass << std::endl;
	std::cout << " - Momentum: " << p << std::endl;
	std::cout << std::endl;

	// get the direction vector for the first step from the momentum vector
	::geoalgo::Vector dx = smearVector(p);
	if (_debug) {
		std::cout << "start position: " << t.back() << " current energy: "
				<< current_energy << std::endl;
	}

	// loop until we are out of energy, add each position value to the trajectory
	while (current_energy > 0.0) {
		t.push_back(t.back() + dx);

		current_energy = current_energy - (_track_step_dedx * dx.Length());

		dx = smearVector(t.end()[-1] - t.end()[-2]);
		if (_debug) {
			std::cout << "current position: " << t.back() << " current energy: "
					<< current_energy << std::endl;
		}

	}

	return t;
}

geoalgo::Vector ToyDataMaker::smearVector(::geoalgo::Vector v) {
	// get the direction of the vector
	double phi = v.Phi();
	double theta = v.Theta();
	double length = _track_step_size;

	// generate individual smearing for each component
	TRandom3 random;
	random.SetSeed(821986); // always use the same seed, set to 0 if you want some random seed
	double theta_smeared = random.Gaus(theta, _track_step_angle_sigma);
	double phi_smeared = random.Gaus(phi, _track_step_angle_sigma);
	double length_smeared = random.Gaus(length, _track_step_sigma);

	// generate the smeared vector
	// nasty trick because we have no setTheta or setPhi in geoVector
	TVector3 v_smeared;
	v_smeared.SetMagThetaPhi(length_smeared, theta_smeared, phi_smeared);

	// construct geoVector out of an  TVector3
	::geoalgo::Vector out = ::geoalgo::Vector(v_smeared);

	return out;
}

Shower ToyDataMaker::MakeShower(double x, double y, double z, double px,
		double py, double pz, int pdg_code) {
	Shower s(geoalgo::Vector(x, y, z), geoalgo::Vector(px, py, pz), 10, 2);
	return s;
}
}
