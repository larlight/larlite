#ifndef STARTPOINT3DMODULE_CXX
#define STARTPOINT3DMODULE_CXX

#include "StartPoint3DModule.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryHelper.h"

namespace showerreco {

void StartPoint3DModule::do_reconstruction(const ShowerClusterSet_t & inputShowers, Shower_t & resultShower) {

	// This function takes the shower cluster set and computes the best fit 3D start point
	// and then assigns it to the shower

	auto geom = larutil::Geometry::GetME();
	auto geomHelper = larutil::GeometryHelper::GetME();

	//Kaleko to-do:
	//Make sure to run axis3d before this, so I have the 3d axis
	//Take in the ShowerClusterSet (list of clusterparams (2 or 3)), somehow compute a 3d start point, save it to the Shower_t refernce.

	// Temporary: Make sure I can store the result appropriately
	resultShower.fXYZStart = TVector3(1., 2., 3.);

}


} //showerreco

#endif