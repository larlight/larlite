#ifndef CHIRPFILTER_CXX
#define CHIRPFILTER_CXX

#include "ChirpFilter.h"


namespace ub_noise_filter {

ChirpFilter::ChirpFilter() {
  // Initialize the exponential here, out until it has
  // fallen to below 1% amplitude
  // At 835 tick time constant, that's about 4000 ticks
  chirp_exponential.resize(4000);
  for (size_t tick = 0; tick < chirp_exponential.size(); tick ++) {
    chirp_exponential[tick] = exp(-1.0 * tick / chirp_rc_const);
  }
}

bool ChirpFilter::ChirpFilterAlg(float * wf, int numTicks)
{

  _current_chirp_info = chirp_info();

  int counter = 0;
  double ADCval;
  double runningAmpMean = 0.0;
  double runningAmpRMS = 0.0;
  int numLowRMS = 0;
  int firstLowRMSBin = -1;
  int lastLowRMSBin = -1;
  bool lowRMSFlag = false;
  double RMSfirst = 0.0;
  double RMSsecond = 0.0;
  double RMSthird = 0.0;
  int numNormalNeighbors = 0;


  // Loop over the whole waveform.
  for (int i = 0; i < numTicks; i++)
  {
    // Get the current tick value
    ADCval = wf[i];

    // The running mean and RMS are used to compare mean and rms between windows
    runningAmpMean += ADCval;
    runningAmpRMS += ADCval * ADCval;

    // Counter keeps track of where within the window this tick is
    counter++;
    if (counter == windowSize)
    {
      // counter == windowSize means we've hit the tick at the edge of the window

      // Finish Calculating the mean and RMS
      runningAmpMean /= (double)windowSize;
      runningAmpRMS /= (double)windowSize;
      // std::cout << "runningAmpRMS - runningAmpMean * runningAmpMean  " << runningAmpRMS - runningAmpMean * runningAmpMean << std::endl;

      if (runningAmpRMS - runningAmpMean * runningAmpMean < 0) {
        std::cout << runningAmpRMS - runningAmpMean * runningAmpMean << std::endl;
        exit(-1);
      }

      runningAmpRMS = sqrt(runningAmpRMS - runningAmpMean * runningAmpMean);

      RMSfirst = RMSsecond;
      RMSsecond = RMSthird;
      RMSthird = runningAmpRMS;

      // A very low RMS indicates chirping (or otherwise dead wire) in this region
      // std::cout << "runningAmpRMS 2 " << runningAmpRMS << std::endl;
      if (runningAmpRMS < chirpMinRMS)
      {
        numLowRMS++;
      }

      // This asks if we are in a window that is at least 3 windows in from the edge
      if (i >= 3 * windowSize )
        // if (i >= 3 * windowSize - 1)
      {
        // This asks if the middle RMS is below chirp threshold while either of it's
        // neighbors are above threshold.
        // This indicates a chirping transition I think
        if ( (RMSsecond < chirpMinRMS) &&
             ((RMSfirst > chirpMinRMS) ||
              (RMSthird > chirpMinRMS)
             )
           )
        {
          numNormalNeighbors++;
        }

        if (lowRMSFlag == false)
        {
          if (((RMSsecond < chirpMinRMS) &&
               (RMSthird < chirpMinRMS2)) ||
              ((RMSsecond < chirpMinRMS2) &&
               (RMSthird < chirpMinRMS))
             )
          {
            lowRMSFlag = true;
            firstLowRMSBin = i - 2 * windowSize + 1;
            lastLowRMSBin = i - windowSize + 1;
          }

          if ((i == 3 * windowSize) &&
              (((RMSfirst < chirpMinRMS) &&
                (RMSsecond < chirpMinRMS2)) ||
               ((RMSfirst < chirpMinRMS2) &&
                (RMSsecond < chirpMinRMS))))
          {
            lowRMSFlag = true;
            firstLowRMSBin = i - 3 * windowSize + 1;
            lastLowRMSBin = i - 2 * windowSize + 1;
          }
        }
        else
        {
          if (((RMSsecond < chirpMinRMS) &&
               (RMSthird < chirpMinRMS2)) ||
              ((RMSsecond < chirpMinRMS2) &&
               (RMSthird < chirpMinRMS)))
          {
            lastLowRMSBin = i - windowSize + 1;
          }
        }
      }

      counter = 0;
      runningAmpMean = 0.0;
      runningAmpRMS = 0.0;
    }
  }


  float chirpFrac = ((float) numLowRMS) / (((float) numTicks) / ((float) windowSize));
  float normalNeighborFrac = ((float) numNormalNeighbors) / ((float) numLowRMS);

  // std::cout << "numTicks " << numTicks << std::endl;
  // std::cout << "windowSize " << windowSize << std::endl;
  // std::cout << "numNormalNeighbors " << numNormalNeighbors << std::endl;
  // std::cout << "numLowRMS " << numLowRMS << std::endl;

  // std::cout << "chirpFrac " << chirpFrac << std::endl;
  // std::cout << "normalNeighborFrac " << normalNeighborFrac << std::endl;
  // std::cout << "lastLowRMSBin " << lastLowRMSBin << std::endl;
  // std::cout << "firstLowRMSBin " << firstLowRMSBin << std::endl;


  if (((normalNeighborFrac < maxNormalNeighborFrac) ||
       ((numLowRMS < 2.0 / maxNormalNeighborFrac) &&
        (lastLowRMSBin - firstLowRMSBin == numLowRMS * windowSize))
      ) &&
      (numLowRMS > 4))
  {
    firstLowRMSBin = std::max(0, firstLowRMSBin - windowSize);
    lastLowRMSBin = std::min(numTicks, lastLowRMSBin + 2 * windowSize);

    if (firstLowRMSBin < 100) {
      firstLowRMSBin = 0;
      // Be conservative and avoid the chirping region
      lastLowRMSBin = std::min(numTicks, lastLowRMSBin + 100);
    }

    if (numTicks - lastLowRMSBin < 100) {
      lastLowRMSBin = numTicks;
      // Be conservative and avoid the chirping region
      firstLowRMSBin = std::max(0, firstLowRMSBin - 100);
    }



    // if ((numTicks - lastLowRMSBin) < windowSize)
    // {
    //   lastLowRMSBin = numTicks;
    // }
    // chirpFrac = 1.0 * (lastLowRMSBin - firstLowRMSBin) / numTicks;

    if (lastLowRMSBin - firstLowRMSBin > 0.9 * numTicks) {
      chirpFrac = 1.0;
    }

    if (chirpFrac > 0.990)
    {
      firstLowRMSBin = 0;
      lastLowRMSBin = numTicks;

      //////////////////////////////////////////////////
      // Set channel status to "DEAD" (LOW-RMS?) here //
      //////////////////////////////////////////////////
      _current_chirp_info.chirp_start = 0;
      _current_chirp_info.chirp_stop = numTicks;
      _current_chirp_info.chirp_frac = 1.0;
    }
    else
    {
      ///////////////////////////////////////////////
      // Set channel status to "MID-CHIRPING" here //
      ///////////////////////////////////////////////

      _current_chirp_info.chirp_start = firstLowRMSBin;
      _current_chirp_info.chirp_stop = lastLowRMSBin;
      _current_chirp_info.chirp_frac = chirpFrac;

    }

    for (int i = 0; i < numTicks; i++)
    {
      if ((i >= firstLowRMSBin) && (i < lastLowRMSBin))
      {
        wf[i] = 0.0;
      }
    }
  } else {
    return false;
  }

  return true;
}

void ChirpFilter::remove_baseline_deviation(float * wf, int numTicks) const {

  // Remove the baseline deviation in this wire caused by chirping.
  //
  // If the wire starts ok, and then starts chriping, that's

  // If the wire is chirping at the start, fix it.  Otherwise, return
  if ( _current_chirp_info.chirp_start == 0 &&
       _current_chirp_info.chirp_stop != 9595 ) {

    // take the amplitude at the start of the chirping
    // and subtract the exponential from the start.

    // Get the start amplitude:
    double integral = 0.0;
    double n_integral = 0;
    double baseline = 0.0;
    double n_baseline = 0;
    // Find the amplitude by integrating the first time-constant range
    // of the waveform.
    for (int tick = 0; tick < chirp_rc_const; tick ++ ) {
      if (_current_chirp_info.chirp_stop + tick < numTicks) {
        integral += wf[_current_chirp_info.chirp_stop + tick];
        n_integral ++;
      }
    }

    for (int tick = numTicks;
         tick > _current_chirp_info.chirp_stop + chirp_rc_const &&
         tick > numTicks - chirp_rc_const;
         tick --) {
      baseline += wf[tick];
      n_baseline ++;
    }

    // If the amplitude of the exponential is A, then the integral is A*tau*(1 - e^-(n/tau))
    // In this case, there is a baseline correction as well.
    // So, the integral is baseline*N + A*tau*(1-e^(-n/tau))

    baseline /= n_baseline;

    double amplitude = integral - n_integral * baseline;

    amplitude /= (chirp_rc_const * (1 - exp(-n_integral / chirp_rc_const))  );  // Now == A


    // Now subtract the exponential from the waveform:
    for (int tick = _current_chirp_info.chirp_stop; tick < numTicks; tick ++ ) {
      size_t exp_tick = tick - _current_chirp_info.chirp_stop;
      if (exp_tick >= 4000)
        break;
      wf[tick] -= (amplitude) * chirp_exponential[exp_tick];
    }


  }

  return;

}



}

#endif
