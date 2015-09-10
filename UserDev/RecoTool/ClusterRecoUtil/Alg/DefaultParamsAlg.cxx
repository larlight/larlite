#ifndef DEFAULTPARAMSALG_CXX
#define DEFAULTPARAMSALG_CXX

#include "DefaultParamsAlg.h"




namespace cluster {

DefaultParamsAlg::DefaultParamsAlg() {

    // In the constructor, create, initialize and attach a set of algs
    GetAverages        * averages              = new GetAverages();
    averages                                   -> SetVerbose(false);
    FillPolygon        * polygon               = new FillPolygon();
    polygon                                    -> SetVerbose(false);
    GetRoughStartPoint * roughStartPoint       = new GetRoughStartPoint();
    roughStartPoint                            -> SetVerbose(false);

    FindShoweringPoint * showeringpoint        = new FindShoweringPoint();
    showeringpoint                             -> SetVerbose(false);
    SelectStartPoint   * selectStartPoint      = new SelectStartPoint();
    selectStartPoint                           -> SetVerbose(false);
    RefineStartPoints  * refineStartPoint      = new RefineStartPoints();
    refineStartPoint                           -> SetVerbose(false);

    attachAlg(averages);
    attachAlg(polygon);
    attachAlg(roughStartPoint);
    attachAlg(showeringpoint);
    attachAlg(selectStartPoint);
    // attachAlg(refineStartPoint);


    SetDebug(false);
    SetVerbose(false);

}

} // cluster

#endif
