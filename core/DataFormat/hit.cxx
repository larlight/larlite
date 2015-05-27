#ifndef LARLITE_HIT_CXX
#define LARLITE_HIT_CXX

#include "hit.h"

namespace larlite {

  //#################################################
  hit::hit() : data_base(data::kHit)
  //#################################################
  {
    clear_data();
  }

  //##########################################################################
  void hit::clear_data()
  //##########################################################################
  {
    data_base::clear_data();
    fChannel = raw::InvalidChannelID;
    fStartTick = 0;
    fEndTick = 0;
    fPeakTime = 0;
    fSigmaPeakTime = -1.;
    fRMS = 0.;
    fPeakAmplitude = 0.;
    fSigmaPeakAmplitude = -1.;
    fSummedADC = 0.;
    fIntegral = 0.;
    fSigmaIntegral = -1;
    fMultiplicity = 0;
    fLocalIndex = -1;
    fGoodnessOfFit = 0.;
    fNDF = -1;
    fView = geo::kUnknown;
    fSignalType = geo::kMysteryType;
    fWireID = geo::WireID(); // invalid
  }

  raw::TDCtick_t   hit::StartTick()      const { return fStartTick;     }
  raw::TDCtick_t   hit::EndTick()        const { return fEndTick;       }
  float            hit::PeakTime()       const { return fPeakTime;      }
  float            hit::SigmaPeakTime()  const { return fSigmaPeakTime; }
  float            hit::RMS()            const { return fRMS;           }
  float            hit::PeakAmplitude()  const { return fPeakAmplitude; }
  float            hit::SigmaPeakAmplitude() const { return fSigmaPeakAmplitude; }
  float            hit::SummedADC()      const { return fSummedADC;     }
  float            hit::Integral()       const { return fIntegral;      }
  float            hit::SigmaIntegral()  const { return fSigmaIntegral; }
  short int        hit::Multiplicity()   const { return fMultiplicity;  }
  short int        hit::LocalIndex()     const { return fLocalIndex;    }
  float            hit::GoodnessOfFit()  const { return fGoodnessOfFit; }
  int              hit::DegreesOfFreedom() const { return fNDF;         }
  raw::ChannelID_t hit::Channel()        const { return fChannel;       }
  geo::SigType_t   hit::SignalType()     const { return fSignalType;    }
  geo::View_t      hit::View()           const { return fView;          }
  geo::WireID      hit::WireID()         const { return fWireID;        }
  
  
  float hit::PeakTimePlusRMS(float sigmas /* = +1. */) const
  { return PeakTime() + sigmas * RMS(); }
  
  float hit::PeakTimeMinusRMS(float sigmas /* = +1. */) const
  { return PeakTimePlusRMS(-sigmas); }
  
  float hit::TimeDistanceAsRMS(float time) const
  { return (time - PeakTime()) / RMS(); }

  bool operator < (const hit & a, const hit & b)
  {
    if (a.Channel() != b.Channel())
      return a.Channel() < b.Channel();
    if (a.View() != b.View())
      return a.View() < b.View();
    if (a.StartTick() != b.StartTick())
      return a.StartTick() < b.StartTick();

    return false; //They are equal
  } // operator< (Hit, Hit)

}
#endif
  
