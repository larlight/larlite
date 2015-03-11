#ifndef LARLITE_CLUSTER_CXX
#define LARLITE_CLUSTER_CXX

#include "cluster.h"

namespace larlite{

  void cluster::set_start_wire(float w,float err)
  { fEndWires[clStart] = w; fSigmaEndWires[clStart] = err; }
  void cluster::set_start_tick(float t,float err)
  { fEndTicks[clStart] = t; fSigmaEndTicks[clStart] = err; }
  void cluster::set_start_charge(float q)
  { fEndCharges[clStart] = q; }
  void cluster::set_start_angle(float a)
  { fAngles[clStart] = a; }
  void cluster::set_start_opening(float o)
  { fOpeningAngles[clStart] = o; }
  void cluster::set_end_wire(float w,float err)
  { fEndWires[clEnd] = w; fSigmaEndWires[clEnd] = err; }
  void cluster::set_end_tick(float t,float err)
  { fEndTicks[clEnd] = t; fSigmaEndTicks[clEnd] = err; }
  void cluster::set_end_charge(float q)
  { fEndCharges[clEnd] = q; }
  void cluster::set_end_angle(float a)
  { fAngles[clEnd] = a; }
  void cluster::set_end_opening(float o)
  { fOpeningAngles[clEnd] = o; }
  void cluster::set_integral(float m, float err, float average)
  { fChargeSum[cmFit]=m; fChargeStdDev[cmFit]=err; fChargeAverage[cmFit]=average;}
  void cluster::set_summedADC(float m, float err, float average)
  { fChargeSum[cmADC]=m; fChargeStdDev[cmADC]=err; fChargeAverage[cmADC]=average;}
  void cluster::set_n_hits(unsigned int n)
  { fNHits = n; }
  void cluster::set_multiple_hit_density(float v)
  { fMultipleHitDensity = v; }
  void cluster::set_width(float v)
  { fWidth = v; }
  void cluster::set_id(ID_t id)
  { fID = id; }
  void cluster::set_view(geo::View_t v)
  { fView = v; }
  void cluster::set_planeID(const geo::PlaneID& id)
  { fPlaneID = id; }

  bool operator < (const cluster& a, const cluster& b) {
    
    if (a.hasPlane() && b.hasPlane() && a.Plane() != b.Plane())
      return a.Plane() < b.Plane();
    if (a.View() != b.View())
      return a.View() < b.View();
    if (a.ID() != b. ID())
      return a.ID() < b.ID();
    if (a.StartTick() != b.StartTick())
      return a.StartTick() < b.StartTick();
    if (a.EndTick() != b.EndTick())
      return a.EndTick() < b.EndTick();
    
    return false; // they are equal enough
  } // operator < (Cluster, Cluster)
  
}

#endif
  
