#ifndef CHIRPFILTER_CXX
#define CHIRPFILTER_CXX

#include "ChirpFilter.h"


namespace ub_noise_filter {



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

      if (runningAmpRMS - runningAmpMean * runningAmpMean < 0){
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

    if (firstLowRMSBin < 100)
      firstLowRMSBin = 0;

    if (numTicks - lastLowRMSBin < 100)
      lastLowRMSBin = numTicks;

    // if ((numTicks - lastLowRMSBin) < windowSize)
    // {
    //   lastLowRMSBin = numTicks;
    // }
    // chirpFrac = 1.0 * (lastLowRMSBin - firstLowRMSBin) / numTicks;

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

void ChirpFilter::ZigzagFilterAlg(float * wf, int numTicks)
{
  float meanVal = 0.0;
  float rmsVal = 0.0;
  int counter = 0;
  float ADCval, ADCval2;
  for (int i = 0; i < numTicks; i++)
  {
    ADCval = wf[i];

    if (ADCval < 4096.0)
    {
      meanVal += ADCval;
      rmsVal += pow(ADCval, 2.0);
      counter++;
    }
  }

  if (counter == 0)
  {
    meanVal = 0.0;
    rmsVal = 0.0;
  }
  else
  {
    meanVal /= (float)counter;
    rmsVal /= (float)counter;
    rmsVal = pow(rmsVal - pow(meanVal, 2.0), 0.5);
  }

  if (rmsVal < 0.5) return;

  for (int i = 0; i < numTicks - 1; i++)
  {
    ADCval = wf[i];
    ADCval2 = wf[i + 1];
    if ((ADCval != 10000.0) && (ADCval2 != 10000.0))
    {
      wf[i] = ((ADCval + ADCval2) / 2.0) - meanVal;
    }
    else if (ADCval != 10000.0)
    {
      wf[i] = ADCval - meanVal;
    }
  }
  wf[numTicks - 1] -= meanVal;

  return;
}

void ChirpFilter::RawAdaptiveBaselineAlg(float * wf, int numTicks)
{
  const int windowSize = 20;

  int minWindowBins = windowSize / 2;

  float baselineVec[numTicks];
  bool isFilledVec[numTicks];

  int numFlaggedBins = 0;
  for (int j = 0; j < numTicks; j++)
  {
    if (wf[j] == 10000.0)
    {
      numFlaggedBins++;
    }
  }
  if (numFlaggedBins == numTicks) return;
  if (numFlaggedBins == 0) return;

  float baselineVal = 0.0;
  int windowBins = 0;
  float ADCval;
  for (int j = 0; j <= windowSize / 2; j++)
  {
    ADCval = wf[j];
    if (ADCval < 4096.0)
    {
      baselineVal += ADCval;
      windowBins++;
    }
  }

  if (windowBins == 0)
    baselineVec[0] = 0.0;
  else
    baselineVec[0] = baselineVal / ((float) windowBins);

  if (windowBins < minWindowBins)
    isFilledVec[0] = false;
  else
    isFilledVec[0] = true;

  int oldIndex;
  int newIndex;
  for (int j = 1; j < numTicks; j++)
  {
    oldIndex = j - windowSize / 2 - 1;
    newIndex = j + windowSize / 2;

    if (oldIndex >= 0)
    {
      ADCval = wf[oldIndex];
      if (ADCval < 4096.0)
      {
        baselineVal -= wf[oldIndex];
        windowBins--;
      }
    }

    if (newIndex < numTicks)
    {
      ADCval = wf[newIndex];
      if (ADCval < 4096)
      {
        baselineVal += wf[newIndex];
        windowBins++;
      }
    }

    if (windowBins == 0)
      baselineVec[j] = 0.0;
    else
      baselineVec[j] = baselineVal / windowBins;

    if (windowBins < minWindowBins)
      isFilledVec[j] = false;
    else
      isFilledVec[j] = true;
  }

  int downIndex;
  int upIndex;
  bool downFlag;
  bool upFlag;
  for (int j = 0; j < numTicks; j++)
  {
    downFlag = false;
    upFlag = false;

    ADCval = wf[j];
    if (ADCval != 10000.0)
    {
      if (isFilledVec[j] == false)
      {
        downIndex = j;
        while ((isFilledVec[downIndex] == false) && (downIndex > 0) && (wf[downIndex] != 10000.0))
        {
          downIndex--;
        }

        if (isFilledVec[downIndex] == false)
          downFlag = true;

        upIndex = j;
        while ((isFilledVec[upIndex] == false) && (upIndex < numTicks - 1) && (wf[upIndex] != 10000.0))
        {
          upIndex++;
        }

        if (isFilledVec[upIndex] == false)
          upFlag = true;

        if ((downFlag == false) && (upFlag == false))
          baselineVec[j] = ((j - downIndex) * baselineVec[downIndex] + (upIndex - j) * baselineVec[upIndex]) / ((float) upIndex - downIndex);
        else if ((downFlag == true) && (upFlag == false))
          baselineVec[j] = baselineVec[upIndex];
        else if ((downFlag == false) && (upFlag == true))
          baselineVec[j] = baselineVec[downIndex];
        else
          baselineVec[j] = 0.0;
      }

      wf[j] = ADCval - baselineVec[j];
    }
  }
}

void ChirpFilter::RemoveChannelFlags(float * wf, int numTicks)
{
  float ADCval;

  /////////////////////////////////////
  // Do stuff here to set chirp_info //
  /////////////////////////////////////

  for (int i = 0; i < numTicks; i++)
  {
    ADCval = wf[i];

    if (ADCval == 10000.0)
    {
      wf[i] = 0.0;
    }
  }

  return;
}



}

#endif
