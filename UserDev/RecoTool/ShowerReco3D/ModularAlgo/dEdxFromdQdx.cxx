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
  _tree->Branch("_dEdx", &_dEdx, "dEdx/D");
  _tree->Branch("_pl", &_pl, "pl/I");
  _tree->Branch("_pl_best", &_pl_best, "pl_best/I");
  return;
}

void dEdxFromdQdx::do_reconstruction(const ::protoshower::ProtoShower & proto_shower,
                                     Shower_t& resultShower) {

  auto & clusters = proto_shower.params();


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

    _dEdx = dedx;
    _pl = pl;
    _pl_best = pl_best;
    _tree->Fill();
    resultShower.fdEdx_v[pl] = dedx;

  }

  return;
}


} //showerreco

#endif
