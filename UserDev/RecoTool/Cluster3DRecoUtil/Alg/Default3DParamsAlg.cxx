#ifndef DEFAULT3DPARAMSALG_CXX
#define DEFAULT3DPARAMSALG_CXX

#include "Default3DParamsAlg.h"




namespace cluster3D {

Default3DParamsAlg::Default3DParamsAlg() {

    // In the constructor, create, initialize and attach a set of algs
    GetAverages3D      * averages              = new GetAverages3D();
    averages                                   -> SetVerbose(false);
    attachAlg(averages);

    SetDebug(false);
    SetVerbose(false);

}

} // cluster3D

#endif
