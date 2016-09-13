#ifndef EVD_DRAWSHOWER_CXX
#define EVD_DRAWSHOWER_CXX

#include "DrawShower.h"
#include "DataFormat/shower.h"
#include "DataFormat/cluster.h"
#include "DataFormat/hit.h"
#include "DataFormat/event_ass.h"
#include "LArUtil/DetectorProperties.h"

namespace evd {


DrawShower::DrawShower()
  : RecoBase<Shower2D>()
{
  _name = "DrawShower";
  _fout = 0;
  // showerVectorByPlane = new std::vector<std::vector<Shower2D> >;
}

bool DrawShower::initialize() {

  // // Resize data holder to accommodate planes and wires:
  if (_dataByPlane.size() != geoService -> Nviews()) {
    _dataByPlane.resize(geoService -> Nviews());
  }
  return true;

}

bool DrawShower::analyze(larlite::storage_manager* storage) {

  // get a handle to the showers
  auto showerHandle = storage->get_data<larlite::event_shower>(_producer);

  // are there associated clusters / hits?
  larlite::event_cluster* clusterHandle = nullptr;
  larlite::event_hit*     hitHandle     = nullptr;

  larlite::AssSet_t ass_cluster_v;
  larlite::AssSet_t ass_hit_v;

  bool hits = false;

  ass_cluster_v = storage->find_one_ass( showerHandle->id(), clusterHandle, showerHandle->name());

  if (clusterHandle)
    ass_hit_v = storage->find_one_ass( clusterHandle->id(), hitHandle, clusterHandle->name() );

  if (hitHandle and (hitHandle->size() > 0) )
    hits = true;
  
  // Clear out the hit data but reserve some space for the showers
  for (unsigned int p = 0; p < geoService -> Nviews(); p ++) {
    _dataByPlane.at(p).clear();
    _dataByPlane.at(p).reserve(showerHandle -> size());
    _wireRange.at(p).first  = 99999;
    _timeRange.at(p).first  = 99999;
    _timeRange.at(p).second = -1.0;
    _wireRange.at(p).second = -1.0;
  }


  // Populate the shower vector:
  for (size_t s = 0; s < showerHandle->size(); s++){
    
    auto const& shower = showerHandle->at(s);

    std::vector<evd::Cluster2D> clusters;
    clusters.resize(3);
    // find hits associated to this shower on this plane, if any, and add
    if (hits){
      auto const& clus_idx_v = ass_cluster_v[s];
      for (auto const& clus_idx : clus_idx_v){
	auto const& hit_idx_v = ass_hit_v[ clus_idx ];
	if (hit_idx_v.size() == 0) continue;
	auto const& plane = hitHandle->at( hit_idx_v[0] ).WireID().Plane;
	for (auto const& hit_idx : hit_idx_v){
	  auto const& hit = hitHandle->at(hit_idx);
	  Hit2D hit2d(hit.WireID().Wire, hit.PeakTime(), hit.Integral(), hit.RMS(),
		      hit.StartTick(),   hit.PeakTime(), hit.EndTick(), hit.PeakAmplitude() );
	  clusters[plane].push_back( hit2d );
	}// for all hits
      }// for all clusters
    }// if there are associated hits
    
    for (unsigned int view = 0; view < geoService -> Nviews(); view++) {
      // get the reconstructed shower for this plane
      auto shr2D = getShower2d(shower,view);
      if (clusters[view].size() > 0){
	shr2D._hits = clusters[view];
      }
      _dataByPlane.at(view).push_back( shr2D );
    }
  }


  return true;
}

bool DrawShower::finalize() {

  return true;
}



Shower2D DrawShower::getShower2d(larlite::shower shower, unsigned int plane) {

  auto detProp = larutil::DetectorProperties::GetME();


  Shower2D result;
  result._is_good = false;
  result._plane = plane;
  // Fill out the parameters of the 2d shower
  result._startPoint
    = geoHelper -> Point_3Dto2D(shower.ShowerStart(), plane);

  // Next get the direction:
  result._angleInPlane = geoHelper->Slope_3Dto2D(shower.Direction(), plane);

  // Get the opening Angle:
  result._openingAngle = shower.OpeningAngle();


  auto secondPoint = shower.ShowerStart() + shower.Length() * shower.Direction();


  result._endPoint
    = geoHelper -> Point_3Dto2D(secondPoint, plane);

  result._length = sqrt(pow(result.startPoint().w - result.endPoint().w, 2) +
                        pow(result.startPoint().t - result.endPoint().t, 2));

  result._dedx = shower.dEdx();

  result._energy = shower.Energy();

  result._is_good = true;
  return result;
}



} // larlite

#endif
