/**
 * \file hit1.h
 *
 * \ingroup DataFormat
 *
 * \brief Class def header for hit1 data container
 *
 * @author Kazu - Nevis 2013
 */

/** \addtogroup DataFormat

    @{*/

#ifndef LARLITE_HIT1_H
#define LARLITE_HIT1_H

#include "Base/GeoConstants.h"
#include "Base/GeoTypes.h"
#include "Base/RawConstants.h"
#include <vector>
namespace larlite{
  /**
     \class hit1
     LArSoft Hit class equivalent data container
  */
  class hit1 {

  public:
    /// Default constructor
    hit1();

    /// Default destructor
    virtual ~hit1(){}

    void clear_data();

    // Set methods
    void set_rms (float rms)
    {fRMS = rms; }
    void set_time_range (raw::TDCtick_t start, raw::TDCtick_t end)
    {fStartTick = start; fEndTick = end;}
    void set_time_peak (float peak, float err)
    {fPeakTime = peak; fSigmaPeakTime = err; }
    void set_time_rms (float rms)
    {fRMS = rms;}
    void set_amplitude (float peak, float err)
    {fPeakAmplitude = peak; fSigmaPeakAmplitude = err; }
    void set_integral (float integral, float err)
    {fIntegral = integral; fSigmaIntegral = err; }
    void set_sumq (float sumadc)
    {fSummedADC = sumadc; }
    void set_multiplicity(short mult)
    { fMultiplicity = mult; }
    void set_local_index(short id)
    { fLocalIndex = id; }
    void set_goodness(float quality)
    { fGoodnessOfFit = quality; }
    void set_ndf(int ndf)
    { fNDF = ndf; }
    void set_channel(raw::ChannelID_t ch)
    { fChannel = ch; }
    void set_view(geo::View_t v)
    { fView = v; }
    void set_signal_type(geo::SigType_t sid)
    { fSignalType = sid; }
    void set_wire(const geo::WireID& wid)
    { fWireID = wid; }

    /// @{
    /// @name Accessors

    /// Initial tdc tick for hit1
    raw::TDCtick_t          StartTick()                 const;

    /// Final tdc tick for hit1
    raw::TDCtick_t          EndTick()                   const;

    /// Time of the signal peak, in tick units
    float                   PeakTime()                  const;

    /// Uncertainty for the signal peak, in tick units
    float                   SigmaPeakTime()             const;

    /// RMS of the hit1 shape, in tick units
    float                   RMS()                       const;

    /// The estimated amplitude of the hit1 at its peak, in ADC units
    float                   PeakAmplitude()             const;

    /// Uncertainty on estimated amplitude of the hit1 at its peak, in ADC units
    float                   SigmaPeakAmplitude()        const;

    /// The sum of calibrated ADC counts of the hit1 (0. by default)
    float                   SummedADC()                 const;

    /// Integral under the calibrated signal waveform of the hit1, in tick x ADC units
    float                   Integral()                  const;

    ///< Uncertainty of integral under the calibrated signal waveform of the hit1, in ADC units
    float                   SigmaIntegral()             const;

    /// How many hit1s could this one be shared with
    short int               Multiplicity()              const;

    ///< Index of this hit1 among the Multiplicity() hit1s in the signal window (-1 by default)
    short int               LocalIndex()                const;

    ///< How well do we believe we know this hit1?
    float                   GoodnessOfFit()             const;

    ///< Degrees of freedom in the determination of the hit1 signal shape (-1 by default)
    int                     DegreesOfFreedom()          const;

    /// ID of the readout channel the hit1 was extracted from
    raw::ChannelID_t        Channel()                   const;

    /// View for the plane of the hit1
    geo::View_t             View()                      const;

    /// Signal type for the plane of the hit1
    geo::SigType_t          SignalType()                const;

    ///< ID of the wire the hit1 is on (Cryostat, TPC, Plane, Wire)
    geo::WireID             WireID()                    const;

    /// @}

    //@{
    /**
     * @brief Returns a time sigmas RMS away from the peak time
     * @param sigmas the number of RMS units to move away
     * @return the shifted time in TDC ticks
     *
     * PeakTimePlusRMS() returns PeakTime() + sigmas x RMS();
     * PeakTimeMinusRMS() returns PeakTime() - sigmas x RMS().
     *
     * @note StartTime() of recob::Hit1 version <=13 was defined by
     *   GausHit1Finder to be PeakTimePlusRMS(-1.), and EndTime() was
     *   PeakTimePlusRMS(+1.).
     */
    float                   PeakTimePlusRMS(float sigmas = +1.) const;
    float                   PeakTimeMinusRMS(float sigmas = +1.) const;
    //@}

    /**
     * @brief Returns the distance of the specified time from peak, in RMS units
     * @param ticks the time, in TDC tick units
     * @return the distance of the specified time from peak, in RMS units
     *
     * This returns (ticks - PeakTime()) / RMS().
     * There is no protection in case RMS is 0!
     */
    float                   TimeDistanceAsRMS(float time) const;

    friend bool           operator <  (const hit1 & a, const hit1 & b);

  protected:
    raw::ChannelID_t        fChannel;        ///< ID of the readout channel the hit1 was extracted from
    raw::TDCtick_t          fStartTick;      ///< initial tdc tick for hit1
    raw::TDCtick_t          fEndTick;        ///< final tdc tick for hit1
    float                   fPeakTime;       ///< time of the signal peak, in tick units
    float                   fSigmaPeakTime;  ///< uncertainty for the signal peak, in tick units
    float                   fRMS;            ///< RMS of the hit1 shape, in tick units
    float                   fPeakAmplitude;  ///< the estimated amplitude of the hit1 at its peak, in ADC units
    float                   fSigmaPeakAmplitude; ///< uncertainty on estimated amplitude of the hit1 at its peak, in ADC units
    float                   fSummedADC;      ///< the sum of calibrated ADC counts of the hit1
    float                   fIntegral;       ///< the integral under the calibrated signal waveform of the hit1, in tick x ADC units
    float                   fSigmaIntegral;  ///< the uncertainty of integral under the calibrated signal waveform of the hit1, in ADC units
    short int               fMultiplicity;   ///< how many hit1s could this one be shared with
    short int               fLocalIndex;     ///< index of this hit1 among the Multiplicity() hit1s in the signal window
    float                   fGoodnessOfFit;  ///< how well do we believe we know this hit1?
    int                     fNDF;            ///< degrees of freedom in the determination of the hit1 shape
    geo::View_t             fView;           ///< view for the plane of the hit1
    geo::SigType_t          fSignalType;     ///< signal type for the plane of the hit1
    geo::WireID             fWireID;         ///< WireID for the hit1 (Cryostat, TPC, Plane, Wire)

  private:

  };
}
#endif

/** @} */ // end of doxygen group
