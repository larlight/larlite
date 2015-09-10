#ifndef DEFAULTPARAMSALG_CXX
#define DEFAULTPARAMSALG_CXX

#include "DefaultParamsAlg.h"




namespace cluster {

  DefaultParamsAlg::DefaultParamsAlg(){

    // In the constructor, create, initialize and attach a set of algs
    GetAverages        * averages        = new GetAverages();
    averages                             -> SetVerbose(false);
    FillPolygon        * polygon         = new FillPolygon();
    polygon                              -> SetVerbose(false);
    GetRoughStartPoint * roughStartPoint = new GetRoughStartPoint();
    roughStartPoint                      -> SetVerbose(false);
    RefineDirection    * direction       = new RefineDirection();
    direction                            -> SetVerbose(false);
    FindShoweringPoint * showeringpoint  = new FindShoweringPoint();
    showeringpoint                       -> SetVerbose(false);
    RefineStartPoints  * startPoint      = new RefineStartPoints();
    startPoint                           -> SetVerbose(false);

    attachAlg(averages);
    attachAlg(polygon);
    attachAlg(roughStartPoint);
    // attachAlg(direction);
    attachAlg(showeringpoint);
    attachAlg(startPoint);

    
    SetDebug(false);
    SetVerbose(false);

  }

} // cluster

#endif
