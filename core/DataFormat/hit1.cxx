#ifndef LARLITE_HIT1_CXX
#define LARLITE_HIT1_CXX

#include "hit1.h"

namespace larlite {

  //#################################################
  hit1::hit1()
  //#################################################
  {
    clear_data();
  }

  //##########################################################################
  void hit1::clear_data()
  //##########################################################################
  {
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

  raw::TDCtick_t   hit1::StartTick()      const { return fStartTick;     }
  raw::TDCtick_t   hit1::EndTick()        const { return fEndTick;       }
  float            hit1::PeakTime()       const { return fPeakTime;      }
  float            hit1::SigmaPeakTime()  const { return fSigmaPeakTime; }
  float            hit1::RMS()            const { return fRMS;           }
  float            hit1::PeakAmplitude()  const { return fPeakAmplitude; }
  float            hit1::SigmaPeakAmplitude() const { return fSigmaPeakAmplitude; }
  float            hit1::SummedADC()      const { return fSummedADC;     }
  float            hit1::Integral()       const { return fIntegral;      }
  float            hit1::SigmaIntegral()  const { return fSigmaIntegral; }
  short int        hit1::Multiplicity()   const { return fMultiplicity;  }
  short int        hit1::LocalIndex()     const { return fLocalIndex;    }
  float            hit1::GoodnessOfFit()  const { return fGoodnessOfFit; }
  int              hit1::DegreesOfFreedom() const { return fNDF;         }
  raw::ChannelID_t hit1::Channel()        const { return fChannel;       }
  geo::SigType_t   hit1::SignalType()     const { return fSignalType;    }
  geo::View_t      hit1::View()           const { return fView;          }
  geo::WireID      hit1::WireID()         const { return fWireID;        }


  float hit1::PeakTimePlusRMS(float sigmas /* = +1. */) const
  { return PeakTime() + sigmas * RMS(); }

  float hit1::PeakTimeMinusRMS(float sigmas /* = +1. */) const
  { return PeakTimePlusRMS(-sigmas); }

  float hit1::TimeDistanceAsRMS(float time) const
  { return (time - PeakTime()) / RMS(); }

  bool operator < (const hit1 & a, const hit1 & b)
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
