#ifndef DQDXMODULE_CXX
#define DQDXMODULE_CXX

#include "dQdxModule.h"
#include "LArUtil/GeometryHelper.h"
#include "LArUtil/LArProperties.h"
#include "LArUtil/DetectorProperties.h"
#include "ShowerReco3D/Base/ShowerRecoException.h"
#include "math.h"
#include <algorithm>
#include <functional>

bool larger( std::pair<int, double> a, std::pair<int, double> b) {
  return a.second < b.second;
}

namespace showerreco {

dQdxModule::dQdxModule()
{
  _caloAlg = ::calo::CalorimetryAlg();
  _caloAlg.setUseModBox(true);
  _name = "dQdxModule";
  _tree = nullptr;
}

void dQdxModule::initialize()
{

  // Correction factr
  _tau = larutil::LArProperties::GetME()->ElectronLifetime();             //electron lifetime in usec
  _timetick = larutil::DetectorProperties::GetME()->SamplingRate() * 1.e-3; //time sample in usec, 0.5us

  // degbugging tree
  if (_tree) delete _tree;
  _tree = new TTree(_name.c_str(),"dQdx tree");
  _tree->Branch("_pitch",&_pitch,"pitch/D");
  _tree->Branch("_pl_best",&_pl_best,"pl_best/I");
  _tree->Branch("_px",&_px,"px/D");
  _tree->Branch("_py",&_py,"py/D");
  _tree->Branch("_pz",&_pz,"pz/D");
  _tree->Branch("_dqdx",&_dqdx,"dqdx/D");
  _tree->Branch("_dqdx_u",&_dqdx_u,"dqdx_u/D");
  _tree->Branch("_dqdx_v",&_dqdx_v,"dqdx_v/D");
  _tree->Branch("_dqdx_y",&_dqdx_y,"dqdx_y/D");
  _tree->Branch("_n_hits_u",&_n_hits_u,"n_hits_u/I");
  _tree->Branch("_n_hits_v",&_n_hits_v,"n_hits_v/I");
  _tree->Branch("_n_hits_y",&_n_hits_y,"n_hits_y/I");
  _tree->Branch("_trunk_l_u",&_trunk_l_u,"trunk_l_u/D");
  _tree->Branch("_trunk_l_v",&_trunk_l_v,"trunk_l_v/D");
  _tree->Branch("_trunk_l_y",&_trunk_l_y,"trunk_l_y/D");

  return;
}

void dQdxModule::do_reconstruction(const ::protoshower::ProtoShower & proto_shower, Shower_t & resultShower) {

  //if the module does not have 2D cluster info -> fail the reconstruction
  if (!proto_shower.hasCluster2D()){
    std::stringstream ss;
    ss << "Fail @ algo " << this->name() << " due to missing 2D cluster";
    throw ShowerRecoException(ss.str());
    }
  
  auto & clusters = proto_shower.params();
  
  /***
  dQdx = hit charge / pitch .
  For details, see Doc-db 4909 .
  dQdx of a shower is the median of dQdx_v of hits along trunk .
  Trunk connects cluster and shower start points .
  Calculation starts line 84 .
  dQdx is calculated for 3 planes. All variables are initialized
  correspondingly as a 3-dimensional vector.
  Note within this Algorithm, the best plane is defined as the
  plane w/ longest trunk length. Therefore here best plane does
  NOT refer to best dQdx.
  User should define their own best plane OUTSIDE this algorithm.
  --------------------------------------------------------------
  In OCT 2015, developers tried sigma or rms to better calculate
  dQdx but results showed no improvements. Now this part is removed.
  ***/

  //std::cout << "Reconstructing shower!" << std::endl;

  auto geom = larutil::Geometry::GetME();
  auto geomHelper = larutil::GeometryHelper::GetME();
  const size_t nplanes = geom->Nplanes();
  auto t2cm = geomHelper->TimeToCm();  //time to cm conversion for e lifetime correction
  auto const& dir3D = resultShower.fDCosStart;

  _px = dir3D[0];
  _py = dir3D[1];
  _pz = dir3D[2];

  std::vector<double> trunk_length(nplanes, 0);
  std::vector<int>    n_hits(nplanes, 0);
  std::vector<std::vector<double> > dQdx_v(nplanes, std::vector<double>());
  std::vector<double> median(nplanes, 0);

  //// loop over all input cluster -> calculate a dQdx per plane
  for (size_t n = 0; n < clusters.size(); n++) {

    // get the hits associated with this cluster
    auto const& hits = clusters.at(n).hit_vector;
    // get the plane associated with this cluster
    auto const& pl = clusters.at(n).plane_id.Plane;
    // get the start point for this cluster
    auto const& clu_start = clusters.at(n).start_point;
    // get the showering point for this cluster
    auto const& shr_start = clusters.at(n).showering_point;
    // cluster open angle
    auto const& cluster_open_angle = clusters.at(n).opening_angle;
    // start direction
    //auto const& start_dir = clusters.at(n).start_dir;

    if (pl >= nplanes) throw ShowerRecoException("Invalid plane found!");

    /***
    Find list of hits that are along the trunk of
    the shower. Grab all hits within 2.4cm w.r.t
    to cluster strat points and hits within trunk
    length compared to cluster start point.
    ---------------------------------------------
    2.4cm is just another magic number in uBooNE.
    If you found a better cut, should just modify it.
    ---------------------------------------------
    For all these hits, save dQdx into dQdx if tan
    value at cluster start point is no larger than
    tan value of shower open angle.
    ***/
    _pitch = geomHelper->GetPitch(dir3D, (int)pl);

    //std::cout << "Plane : " << pl << std::endl;

    double dist_hit_clu;
    double dist_hit_shr;
    trunk_length[pl] = sqrt ( (shr_start.w - clu_start.w) * (shr_start.w - clu_start.w) +
                              (shr_start.t - clu_start.t) * (shr_start.t - clu_start.t) );
    
    n_hits[pl] = hits.size();

    for (int i = 0; i < n_hits[pl]; i++) {

      auto const hit = hits[i];

      // distance of hit to the shower start point
      dist_hit_clu = sqrt((hit.w - clu_start.w) * (hit.w - clu_start.w) +
                          (hit.t - clu_start.t) * (hit.t - clu_start.t));
      // distance to clustering start point
      dist_hit_shr = sqrt((hit.w - shr_start.w) * (hit.w - shr_start.w) +
                          (hit.t - shr_start.t) * (hit.t - shr_start.t));

      // Correction for electron life time,
      // Now removed since T0 is unknown.
      //  ---------------------------------
      double hit_tick =hit.t/t2cm;
      double lifetimeCorr = exp( hit_tick * _timetick / _tau );

      double Q = _caloAlg.ElectronsFromADCArea( hit.charge, pl);
      Q *= lifetimeCorr;

      if (dist_hit_clu <= 2.4 || dist_hit_clu < trunk_length[pl]) {

        if (trunk_length[pl] == 0 || dist_hit_clu * dist_hit_shr == 0) {
          dQdx_v[pl].push_back(Q / _pitch);
        }

        else if (dist_hit_clu * dist_hit_shr > 0) {

          double cos_hit_angle = (dist_hit_clu * dist_hit_clu +
                                  trunk_length[pl] * trunk_length[pl] -
                                  dist_hit_shr * dist_hit_shr) / (2 * dist_hit_clu * trunk_length[pl]);

          if (tan(acos(cos_hit_angle)) <= tan(cluster_open_angle)) {
            dQdx_v[pl].push_back(Q / _pitch);
	    if (_verbose) { std::cout << "\t dQ/dx : " << Q/_pitch << std::endl; }
          }
        }
      }
    }

    
    // Get median of dQdx
    std::nth_element(dQdx_v[pl].begin(), dQdx_v[pl].begin() + dQdx_v[pl].size() / 2, dQdx_v[pl].end());
    if (dQdx_v[pl].size() > 0){
      median[pl] = dQdx_v[pl].at((dQdx_v[pl].size() / 2));
      if (_verbose) { std::cout << "dQ/dx for plane " << pl << " = " << median[pl] << std::endl << std::endl << std::endl; }
    }
    _pl_best = nplanes - 1;
    resultShower.fBestdQdxPlane = _pl_best;
    resultShower.fdQdx_v[pl] = median[pl];

  }// for all clusters (planes)

  // figure out best plane based on the one with the most number of points used
  // for dQdx calculation
  if ( (dQdx_v[2].size() > dQdx_v[0].size()) and (dQdx_v[2].size() > dQdx_v[1].size()) )
    _pl_best = 2;
  else if (dQdx_v[1].size() > dQdx_v[0].size())
    _pl_best = 1;
  else
    _pl_best = 0;

  resultShower.fBestdQdxPlane = _pl_best;
  resultShower.fdQdx = resultShower.fdQdx_v[_pl_best];

  _dqdx   = resultShower.fdQdx;
  _dqdx_u = resultShower.fdQdx_v[0];
  _dqdx_v = resultShower.fdQdx_v[1];
  _dqdx_y = resultShower.fdQdx_v[2];

  _n_hits_u = dQdx_v[0].size();
  _n_hits_v = dQdx_v[1].size();
  _n_hits_y = dQdx_v[2].size();

  _trunk_l_u = trunk_length[0];
  _trunk_l_v = trunk_length[1];
  _trunk_l_y = trunk_length[2];

  _pitch = geomHelper->GetPitch(dir3D, 2);

  _tree->Fill();
  
  return;
}
} //showerreco

#endif
