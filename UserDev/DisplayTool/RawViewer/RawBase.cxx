#ifndef RAWBASE_CXX
#define RAWBASE_CXX

#include "RawBase.h"

namespace evd {

  RawBase::RawBase(){ 
    wiredata = new std::vector<std::vector<std::vector<float> > > ;
    geoService = larutil::Geometry::GetME();
    std::cout << "Constructing." << std::endl;
  }

  RawBase::~RawBase(){ 
    if (wiredata)
      delete wiredata;
  }


  const std::vector<std::vector<float>> & RawBase::getDataByPlane(unsigned int p) const{
    static std::vector<std::vector<float>> returnNull;
    if (p >= geoService->Nviews()){
      std::cerr << "ERROR: Request for nonexistant plane " << p << std::endl;
      return returnNull;
    }
    else{
      if (wiredata !=0){
        return wiredata->at(p);
      }
      else{
        return returnNull;
      }
    }
    
  }

  const std::vector<float> & RawBase::getWireData(unsigned int plane, unsigned int wire) const{
    static std::vector<float> returnNull;
    if (plane >= geoService->Nviews()){
      std::cerr << "ERROR: Request for nonexistant plane " << plane << std::endl;
      return returnNull;
    }
    if (wire >= geoService->Nwires(plane)){
        std::cerr << "ERROR: Request for nonexistant wire " << wire << std::endl;
        return returnNull;
    }
    else{
      if (wiredata !=0){
        return wiredata->at(plane).at(wire);
      }
      else{
        return returnNull;
      }
    }
    
  }
} // evd


#endif
