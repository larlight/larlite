#ifndef LARLITE_T0_CXX
#define LARLITE_T0_CXX

#include "t0.h"

namespace larlite {

  t0::t0()
    : data_base(data::kT0)
  {
    clear_data();
  }

  //----------------------------------------------------------------------
  t0::t0(double Time,
	 unsigned int TriggerType,
	 int TriggerBits,
	 int ID,
	 double TriggerConfidence)
    : fTime(Time)
    , fTriggerType(TriggerType)
    , fTriggerBits(TriggerBits)
    , fID(ID)
    , fTriggerConfidence(TriggerConfidence)
  {
    
  }

  //----------------------------------------------------------------------
  // copy constructor
  t0::t0(const t0& origin)
  {
    clear_data();
    fTime              = origin.Time();
    fTriggerType       = origin.TriggerType();
    fTriggerBits       = origin.TriggerBits();
    fID                = origin.ID();
    fTriggerConfidence = origin.TriggerConfidence();
  }


  //----------------------------------------------------------------------
  // ostream operator.
  //
  std::ostream& operator<< (std::ostream & o, t0 const& a)
  {
    o  << "T0 with Time: "           << a.fTime
       << "\n from Trigger type: "   << a.fTriggerType
       << "\n with bits: "           << a.fTriggerBits
       << "\n with ID: "             << a.fID
       << "\n with confidence: "     << a.fTriggerConfidence
       <<std::endl;
    
    return o;
  }
  
  
  //**********************
  void  t0::clear_data()
  //**********************
  {
    
    fTime              = data::kINVALID_DOUBLE;
    fTriggerType       = data::kINVALID_UINT;
    fTriggerBits       = data::kINVALID_INT;
    fID                = data::kINVALID_INT;
    fTriggerConfidence = data::kINVALID_DOUBLE;
  }
  
  
  double          t0::Time()              const { return fTime;              }
  unsigned int    t0::TriggerType()       const { return fTriggerType;       }
  int             t0::TriggerBits()       const { return fTriggerBits;       }
  int             t0::ID()                const { return fID;                }
  double          t0::TriggerConfidence() const { return fTriggerConfidence; }
  
}


  
#endif
