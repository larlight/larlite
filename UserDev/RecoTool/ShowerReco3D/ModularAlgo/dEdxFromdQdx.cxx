#ifndef DEDXFROMDQDX_CXX
#define DEDXFROMDQDX_CXX

#include "dEdxFromdQdx.h"
#include "LArUtil/Geometry.h"
#include "LArUtil/GeometryHelper.h"
#include "LArUtil/LArProperties.h"

namespace showerreco {

dEdxFromdQdx::dEdxFromdQdx()
{
  _name = "dEdxFromdQdx";
  _use_pitch = true;
}

void dEdxFromdQdx::initialize()
{
  if (_tree) delete _tree;
  _tree = new TTree(_name.c_str(), "dQdx Info Tree");
  _tree->Branch("_dedx_u", &_dedx_u, "dedx_u/D");
  _tree->Branch("_dedx_v", &_dedx_v, "dedx_v/D");
  _tree->Branch("_dedx_y", &_dedx_y, "dedx_y/D");
  _tree->Branch("_event");
  return;
}

void dEdxFromdQdx::do_reconstruction(const ::protoshower::ProtoShower & proto_shower,
                                     Shower_t& resultShower) {

  auto & clusters = proto_shower.params();

  _dedx_u = _dedx_v = _dedx_y = 0;

  // auto geomHelper = larutil::GeometryHelper::GetME();

  // // get the 3D direction reconstructed hopefully in a previous step
  // auto const& dir3D = resultShower.fDCosStart;

  // get the vector of dQdx filled by someone else
  auto const& dQdx_v = resultShower.fdQdx_v;

  // loop over all input cluster -> calculate a dQdx per plane
  for (size_t n = 0; n < clusters.size(); n++) {

    // get the plane associated with this cluster
    auto const& pl = clusters.at(n).plane_id.Plane;

    // get the best plane
    auto const& pl_best = resultShower.fBestdQdxPlane;

    //*** we need to calculate a pitch for this plane
    //double pitch = geomHelper->GetPitch(dir3D,(int)pl);

    double dedx = 0.;

    double dqdx = dQdx_v[pl];

    if (_verbose) { std::cout << "dQdx on plane : " << pl << " -> " << dqdx << std::endl; }

    dedx = larutil::LArProperties::GetME()->ModBoxCorrection(dqdx);

    if (_verbose) { std::cout << "dEdx on plane : " << pl << " -> " << dedx << std::endl; }

    // take the dQdx measured on each plane and convert
    // to dEdx using a recombination model

    if (pl == 0)
      _dedx_u = dedx;
    if (pl == 1)
      _dedx_v = dedx;
    if (pl == 2)
      _dedx_y = dedx;

    resultShower.fdEdx_v[pl] = dedx;

  } // for all input clusters

  _tree->Fill();

  return;
}


} //showerreco

#endif
