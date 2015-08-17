#ifndef DEFAULTPARAMSALG_CXX
#define DEFAULTPARAMSALG_CXX

#include "DefaultParamsAlg.h"




namespace cluster {

  DefaultParamsAlg::DefaultParamsAlg(){

    // In the constructor, create, initialize and attach a set of algs
    GetAverages        * averages        = new GetAverages();
    FillPolygon        * polygon         = new FillPolygon();
    GetRoughStartPoint * roughStartPoint = new GetRoughStartPoint();
    // RefineDirection    * direction       = new RefineDirection();
    // RefineStartPoints  * startPoint      = new RefineStartPoints();
    // FindShoweringPoint * showeringpoint  = new FindShoweringPoint();

    attachAlg(averages);
    attachAlg(polygon);
    attachAlg(roughStartPoint);
    // attachAlg(direction);
    // attachAlg(startPoint);
    // attachAlg(showeringpoint);
    SetDebug(true);
    SetVerbose(false);
  }

} // cluster

#endif
