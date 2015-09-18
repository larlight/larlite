#ifndef DEFAULTPARAMSALG_CXX
#define DEFAULTPARAMSALG_CXX

#include "DefaultParamsAlg.h"




namespace cluster {

DefaultParamsAlg::DefaultParamsAlg() {

    // In the constructor, create, initialize and attach a set of algs
    GetAverages        * averages              = new GetAverages();
    averages                                   -> SetVerbose(false);
    attachAlg(averages);

    FillPolygon        * polygon               = new FillPolygon();
    polygon                                    -> SetVerbose(false);
    attachAlg(polygon);

    GetRoughStartPoint * roughStartPoint       = new GetRoughStartPoint();
    roughStartPoint                            -> SetVerbose(false);
    attachAlg(roughStartPoint);

    FindShoweringPoint * showeringpoint        = new FindShoweringPoint();
    showeringpoint                             -> SetVerbose(false);
    attachAlg(showeringpoint);

    SelectStartPoint   * selectStartPoint      = new SelectStartPoint();
    selectStartPoint                           -> SetVerbose(false);
    attachAlg(selectStartPoint);

    RefineStartPoints  * refineStartPoint      = new RefineStartPoints();
    refineStartPoint                           -> SetVerbose(false);
    attachAlg(refineStartPoint);

    FillGeomParams     * fillGeomParams        = new FillGeomParams();
    fillGeomParams                             -> SetVerbose(false);
    attachAlg(fillGeomParams);

    SetDebug(false);
    SetVerbose(false);

}

} // cluster

#endif
