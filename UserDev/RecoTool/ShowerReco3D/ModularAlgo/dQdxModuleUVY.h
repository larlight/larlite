/**
 * \file dQdxModuleUVY.h
 *
 * \ingroup ModularAlgo
 *
 * \brief Class def header for a class dQdx2DModule
 *
 * @authors David Caratelli, Nevis, dcaratelli@nevis.columbia.edu
 */

/** \addtogroup ModularAlgo

    @{*/
#ifndef DQDXMODULEUVY_H
#define DQDXMODULEUVY_H
#include <iostream>
#include "AnalysisAlg/CalorimetryAlg.h"
#include "ShowerRecoModuleBase.h"

//#include "AnalysisAlg/CalorimetryAlg.h"

/**
   \class dQdxModuleUVY : ShowerRecoModuleBase
   This is meant to compute the 2D dQdx along the start of the shower.
*/
namespace showerreco {
  
  class dQdxModuleUVY : ShowerRecoModuleBase {
    
  public:
    
    /// Default constructor
    dQdxModuleUVY();
    
    /// Default destructor
    ~dQdxModuleUVY() {}
    
    void do_reconstruction(const ::protoshower::ProtoShower &, Shower_t &);
    
    void initialize();
    
    // set distance along which to calculate dE/dx
    void setTrunkLength(double d) { _dtrunk = d; }

    // boolean set overall angle or trunk calculated angle
    void setUseTrunkAngle(bool b) { _use_trunk = b; }

    /// 3D charge correction function
    double ChargeCorrection(const double& q, const TVector3& vtx);

    // create a map for calorimetric corrections for position-dependent response variations
    void CreateResponseMap(const double& stepsize);

    // set response map value
    void SetResponse(const double& x, const double& y, const double& z, const double& q);

    /// find nearest cell response value (by averaging)
    double NearestCell(const size_t& i, const size_t& j, const size_t& k);
    
  protected:

    // sampling size in usec
    double _timetick; 
    
    // distance along which to calculate dEdx
    double _dtrunk;

    // flag to use trunk angle
    bool _use_trunk;
    
    // position-dependent response map
    std::vector< std::vector< std::vector< double > > >_responseMap;
    double _responseStep;
    
  };
  
} // showerreco

#endif
/** @} */ // end of doxygen group

