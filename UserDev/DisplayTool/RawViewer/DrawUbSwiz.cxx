#ifndef LARLITE_DRAWUBSWIZ_CXX
#define LARLITE_DRAWUBSWIZ_CXX

#include "LArUtil/DetectorProperties.h"
#include "DataFetcher.h"
#include "DrawUbSwiz.h"
#include "DataFormat/rawdigit.h"
#include <fstream>

#include "TStopwatch.h"

namespace evd {

DrawUbSwiz::DrawUbSwiz() {
  // _name = "DrawUbSwiz";
  // producer = "daq";

  // Initialize whether or not to save the data:
  _save_data = true;

  // And whether or not to correct the data:
  _correct_data = true;

  stepSize.clear();
  stepSize.push_back(16);
  stepSize.push_back(16);
  stepSize.push_back(2 * 16);

}

bool DrawUbSwiz::initialize() {

  //
  // This function is called in the beggining of event loop
  // Do all variable initialization you wish to do here.
  // If you have a histogram to fill in the event loop, for example,
  // here is a good place to create one on the heap (i.e. "new TH1D").
  //
  //

  _event_no = 0;
  _run = 0;
  _spill = 0;

  for (unsigned int p = 0; p < geoService -> Nviews(); p ++) {
    setXDimension(geoService->Nwires(p), p);
    setYDimension(9595, p);
    // setYDimension(detProp -> ReadOutWindowSize(), p);
  }
  initDataHolder();

  _noise_filter.set_n_time_ticks(9595);


  return true;

}

void DrawUbSwiz::setInput(std::string s) {
  // if the file isn't new, do nothing:
  if (s == _input_file) return;
  // check to see if this file exists.
  std::cout << "Attempting to open file " << s << std::endl;
  std::ifstream ifile(s);
  if (!ifile.is_open()) {
    std::cerr << "ERROR: Input file failed to open.\n";
    return;
  }
  else {
    // The file exists, try to read it.
    _input_file = s;
    _current_event = 0;
    df.set_file_path(_input_file);
    _n_events  = df.entries();
    if (_n_events == 0) {
      _run = 0;
      _event_no = 0;
      return;
    }
    // readData();
  }
}


// This is the function that actually reads in an event
void DrawUbSwiz::nextEvent() {

  if (_current_event >= _n_events) {
    std::cout << "On Event " << _current_event << std::endl;
    std::cout << "Warning, end of file reached, select a new file.\n";
    return;
  }
  else {
    _current_event ++;
    readData();
  }

  return;
}

void DrawUbSwiz::prevEvent() {

  if (_current_event <= 0) {
    std::cout << "On event " << _current_event << std::endl;
    std::cout << "Warning, at beginning of file, can not go backwards.\n";
    return;
  }
  else {
    _current_event --;
    readData();
  }

  return;

}

void DrawUbSwiz::goToEvent(size_t e) {
  if (e < 0) {
    std::cout << "Selected event is too low.\n";
    return;
  }
  if (e >= _n_events) {
    std::cout << "Selected event is too high.\n";
    return;
  }
  _current_event = e;
  readData();

}

void DrawUbSwiz::readData() {

  TStopwatch watch;
  watch.Start();

  df.get_entry(_current_event);

  // std::cout << "Reading data\n";

  // get the channel data:
  std::vector<short>  channel =  df.channel();
  std::vector<short>  adc = df.adc();



  _run =  df.run();
  _subrun = df.subrun();
  _event_no = df.event();

  // std::cout << "Run " << _run << ", subrun " << _subrun << ", event " << _event_no << std::endl;
  //
  float init_time = watch.RealTime();
  watch.Start();

  // Set the size of the digits in the file
  unsigned int digitSize = df.adc_cols();

  for (size_t i_channel = 0; i_channel < channel.size(); i_channel ++) {
    unsigned int ch = channel.at(i_channel);

    if (ch >= 8254) {
      continue;
    }

    unsigned int wire = geoService->ChannelToWire(ch);
    unsigned int plane = geoService->ChannelToPlane(ch);



    if (wire > geoService -> Nwires(plane))
      continue;

    int offset = wire * digitSize;

    // convert short ADCs to float
    int i = 0;
    for (size_t index = i_channel * digitSize; index < (i_channel + 1)*digitSize; index ++) {
      _planeData.at(plane).at(offset + i) = adc.at(index);
      i++;
    }



  }

  float first_pass = watch.RealTime();
  watch.Start();
  if (_correct_data || _save_data){
    _noise_filter.set_data(&_planeData);
    _noise_filter.clean_data();
  }

  float second_pass = watch.RealTime();


  std::cout << "Time to read data:    " << init_time  << std::endl;
  std::cout << "Time to arrange data: " << first_pass << std::endl;
  std::cout << "Time to clean data:   " << second_pass << std::endl;

  // std::cout << "_planeData.size() " << _planeData.size() << std::endl;
  // std::cout << "_planeData.at(0).at(0) " << _planeData.at(0).at(0) << std::endl;
  // std::cout << "_planeData.at(0).at(1) " << _planeData.at(0).at(1) << std::endl;
  // std::cout << "_planeData.at(0).at(2) " << _planeData.at(0).at(2) << std::endl;

  std::cout << "done reading data.\n";

  return;
}



bool DrawUbSwiz::finalize() {
  return true;
}


void DrawUbSwiz::SetStepSizeByPlane(int step, int plane) {
  if (plane < 0 || plane >= geoService -> Nviews()) {
    std::cerr << "ERROR: can't set step size for non existent plane " << plane << std::endl;
  }
  else {
    if (step < 5) return;
    stepSize.at(plane) = step;
  }
}

// void DrawUbSwiz::correctData() {

//   // Loops over the data, figures out the coherent noise, and removes it.
//   // If requested, it saves out the pedestal, rms, and badChannelFlag for each wire
//   // If requested, it saves the subtracted wave form for each block as well as the
//   //  size of the blocks in each plane (so that the blocks can be determined later)
//   // If _correct_data is true, it saves out the corrected rms as well as
//   //  the uncorrected rms


//   // Clear the stuff that gets saved every event:
//   //

//   // Contains the wave form to subtract [plane][block][tick]
//   std::vector<std::vector<std::vector<float> > > _subtractionWaveForm;

//   // Declare all the stuff necessary to save, but don't initialize unless used


//   // Save one file per event ...
//   TFile * _out;
//   TTree * _tree;



//   // Determine how many steps are needed, and make space for all of the waveforms
//   if (_correct_data) {
//     _subtractionWaveForm.resize(geoService -> Nplanes());

//     for (unsigned int p = 0; p < geoService -> Nplanes(); p++) {
//       int nSteps = geoService -> Nwires(p) / stepSize.at(p);
//       _subtractionWaveForm.at(p).resize(nSteps);
//       for (auto & vec : _subtractionWaveForm.at(p))
//         vec.resize(detProp->ReadOutWindowSize());
//     }
//   }

//   char nameFile[100];
//   // Initialize all of the variables and branches possible
//   if (_save_data) {

//     // Setup the file and the ttree
//     sprintf(nameFile, "RawDigitAna_%zu_%zu_%zu.root", _run, _subrun, _event_no);

//     _out = new TFile(nameFile, "RECREATE");
//     _out -> cd();
//     _tree = new TTree("waveformsub", "waveformsub");

//     // Save the run, subrun, and event number for redundancy
//     _tree -> Branch("run", &_run);
//     _tree -> Branch("subrun", &_subrun);
//     _tree -> Branch("event", &_event_no);

//     // Save the stepSize too
//     _tree -> Branch("stepSize", &stepSize);

//     // Save the pedestals, rms, and corrected rms by plane
//     for (unsigned int p = 0; p < geoService -> Nviews(); p ++) {
//       _tree -> Branch(Form("pedestal_%u", p), &(pedestalByPlane.at(p)));
//       _tree -> Branch(Form("rms_%u", p),      &(rmsByPlane.at(p)));
//       if (_correct_data)
//         _tree -> Branch(Form("rmsCorrected_%u", p),  &(rmsByPlaneCorrected.at(p)));
//     }

//   }



//   // Only perform the median subtraction if we're correcting the data:

//   if (_correct_data) {

//     // We want to save the corrected RMS if we're doing the correction
//     // Make sure it's zeroed out before the loops over steps

//     // Do a median subtraction to remove the correlated noise
//     for (unsigned int plane = 0; plane < geoService -> Nplanes(); plane ++) {

//       rmsByPlaneCorrected.at(plane).clear();
//       rmsByPlaneCorrected.at(plane).resize(geoService->Nwires(plane));


//       // Figure out how many steps to take across this wire plane
//       unsigned int nSteps = geoService -> Nwires(plane) / stepSize.at(plane);

//       for (unsigned int step = 0; step < nSteps; step ++) {


//         // Within each step, loop over the ticks.
//         // For each tick, loop over each wire and find the median.
//         // Then, subtract the median from each tick.

//         for (unsigned int tick = 0; tick < detProp -> ReadOutWindowSize(); tick ++) {

//           // Get the median across this step's set of wires.
//           unsigned int wireStart = step * stepSize.at(plane);

//           // Use this vector to store the horizontal slice of values
//           std::vector<float> vals;
//           // Since we're skipping bad wires, don't want spurious zeros
//           // so only push back values and don't intialize the whole space.  Do, however,
//           // allocate it
//           vals.reserve(stepSize.at(plane));

//           // Loop over this block of wires and get the median value for that tick
//           for (unsigned int wire = wireStart; wire < wireStart + stepSize.at(plane); wire ++) {
//             // skip bad wires
//             int offset = wire * detProp->ReadOutWindowSize();
//             if ( wireStatusByPlane.at(plane)[wire] != kNormal ) {
//               // For now, set the tick to zero:
//               _planeData.at(plane).at(offset + tick) = 0.0;
//               continue;
//             }
//             // if (plane != 2)
//             vals.push_back(_planeData.at(plane).at(offset + tick));

//             // Keep this commented out, for now, but it's pretty wrong.
//             // // do a rolling average for collection plane:

//             // else{
//             //   unsigned int averageSize = 0;
//             //   if (tick < averageSize || tick >= detProp->ReadOutWindowSize() - averageSize)
//             //     vals.at(wire-wireStart) = _planeData.at(plane).at(offset + tick);
//             //   else{
//             //     for (unsigned int workingTick = tick - averageSize; workingTick <= tick + averageSize; workingTick ++ )
//             //       vals.at(wire-wireStart) += _planeData.at(plane).at(offset + workingTick);
//             //     vals.at(wire-wireStart) /= (2*averageSize + 1);
//             //   }
//             // }

//           }


//           float median = 0;

//           if (vals.size() > stepSize.at(plane) / 4) {
//             median = getMedian(vals);
//           }

//           // Now subtract the median from the values:
//           for (unsigned int wire = wireStart; wire < wireStart + stepSize.at(plane); wire ++) {
//             // Skip bad wires
//             int offset = wire * detProp->ReadOutWindowSize();

//             if (wireStatusByPlane.at(plane)[wire] != kNormal) {
//               rmsByPlaneCorrected.at(plane).at(wire)
//               += pow((_planeData.at(plane).at(offset + tick)), 2);
//               continue;
//             }
//             else {
//               _planeData.at(plane).at(offset + tick) -= median;
//               rmsByPlaneCorrected.at(plane).at(wire)
//               += pow((_planeData.at(plane).at(offset + tick)), 2);
//             }
//             // While we're here, do stuff to get the corrected RMS

//           }


//           // Save the subtraction waveform:
//           _subtractionWaveForm.at(plane).at(step).at(tick) = median;


//         } // loop over ticks

//         // Now that the tick loop is finished, the wave form is finalized
//         //

//         // Copy the data to the tgraph so that it can be stored, and set up the branches
//         if (_save_data) {
//           char name[100];
//           sprintf(name, "subwaveform_%u_%u", plane, step);
//           _tree -> Branch(name, &(_subtractionWaveForm.at(plane).at(step)) );
//         }
//       } // loop over steps

//       // Divide the RMS by the number of ticks (which is actually 9595, not 9600)
//       for (auto & wire : rmsByPlaneCorrected.at(plane)) {
//         wire /= 9595.0;
//         wire = sqrt(wire);
//       }

//     } // loop over planes

//   }

//   int totalWaveforms = 0;
//   for (auto & vec : _subtractionWaveForm) {
//     totalWaveforms += vec.size();
//   }


//   std::vector<std::vector<float> > correlationMatrix(totalWaveforms, std::vector<float>(totalWaveforms, 0.0));


//   if (_save_data) {
//     int count = 0;
//     // Loop over the waveforms and get the correlations:
//     for (unsigned int i = 0; i < _subtractionWaveForm.size(); i ++) {
//       for (int j = 0; j < _subtractionWaveForm.at(i).size(); j ++ )
//       {

//         // For each wave form, correlate it to all the other waveforms.
//         // Increment the count here, and don't correlate if the correlation
//         // has already been calculated
//         int count2 = 0;
//         for (unsigned int i2 = 0; i2 < _subtractionWaveForm.size(); i2 ++) {
//           for (int j2 = 0; j2 < _subtractionWaveForm.at(i2).size(); j2 ++ ) {
//             if (correlationMatrix[count][count2] == 0) {
//               // This entry unfilled.  See if the cross diagonal is filled:
//               if (correlationMatrix[count2][count] != 0) {
//                 correlationMatrix[count][count2] = correlationMatrix[count2][count];
//                 count2 ++;
//                 continue;
//               }
//               else {
//                 correlationMatrix[count][count2] = getCorrelation(_subtractionWaveForm[i][j], _subtractionWaveForm[i2][j2]);
//                 correlationMatrix[count2][count] = correlationMatrix[count][count2];
//                 count2 ++;
//                 continue;
//               }
//             }
//             count2++;
//           } // j2
//         } // i2
//         // Increment the number of counts
//         count ++;
//       }
//     }

//     // Save the correlation matrix
//     _tree -> Branch("correlationMatrix", &(correlationMatrix));
//   }

//   if (_save_data) {
//     _tree -> Fill();
//     _tree -> Write();
//     _out -> Close();
//     std::cout << "Output successfully written to " << nameFile << std::endl;
//   }

//   return;

// }

// float DrawUbSwiz::getCorrelation(const std::vector<float> & vec1, const std::vector<float> & vec2)
// {

//   if (vec1.size() != vec2.size()) {
//     std::cerr << "ERROR: vector sizes do not match" << std::endl;
//     return 0.0;
//   }

//   float rms_1  = 0.0;
//   float rms_2  = 0.0;
//   float mean_1 = 0.0;
//   float mean_2 = 0.0;

//   for (unsigned int i = 0; i < vec1.size(); i++) {
//     mean_1 += vec1.at(i);
//     mean_2 += vec2.at(i);
//   }

//   mean_1 /= vec1.size();
//   mean_2 /= vec2.size();

//   float corr = 0.0;

//   for (unsigned int i = 0; i < vec1.size(); i++) {
//     rms_1 += pow(mean_1 - vec1.at(i), 2);
//     rms_2 += pow(mean_2 - vec2.at(i), 2);
//     corr += (mean_1 - vec1.at(i)) * (mean_2 - vec2.at(i));
//   }

//   rms_1 = sqrt(rms_1 / vec1.size());
//   rms_2 = sqrt(rms_2 / vec2.size());

//   corr /= (rms_1 * rms_2 * vec1.size());
//   return corr;

// }

// float DrawUbSwiz::getMedian(std::vector<float> & vals) {
//   // std::map<float, int> mode_finder;
//   // for (auto & val : vals ) {
//   //   mode_finder[val] ++;
//   // }
//   // auto max = std::max_element(mode_finder.begin(), mode_finder.end());
//   // return max->first;
//   ////Old style:
//   // Calculate the median:
//   // sort(vals.begin(), vals.end());
//   // float median = 0;
//   // if (vals.size() % 2 == 0) {
//   //   median =  0.5 * vals.at(vals.size() / 2)
//   //             + 0.5 * vals.at(vals.size() / 2 - 1);
//   // }
//   // else {
//   //   median = vals.at((int)vals.size() / 2);
//   // }
//   // return median;

//   // This way gives a 25% decrease in execution time:
//   std::nth_element(vals.begin(), vals.begin() + vals.size()/2, vals.end());
//   return vals[vals.size()/2];


// }
}
#endif



// // TEMPORARY: fix collection mapping.
// if (plane == 2){
//   // std::cout << "Wire from " << wire;
//   int wireAnchor = wire - (wire % 32);
//   wire = wireAnchor + 32 - (wire % 32) - 1;
//   // std::cout << " to " << wire <<std::endl;
// }
// if (plane == 1){
//   if (wire < 672){
//      int wireTemp = 0;
//      /*// 0
//      if (wire > -1 && wire <= 31)
//         {wireTemp = wire + 64; }
//      // 1
//      if (wire > 31 && wire <=63)
//         { wireTemp = wire - 32; }
//      // 2
//      if (wire > 63 && wire  <= 95)
//         { wireTemp = wire - 32; }

//      // 3
//      if (wire > 95 && wire <= 127)
//         { wireTemp = wire + 256; }
//      // 4
//      if (wire > 127 && wire <= 159)
//         { wireTemp = wire + 320; }
//      // 5
//      if (wire > 159 && wire <= 191)
//         { wireTemp = wire + 256;}

//      // 6
//      if (wire > 191 && wire <= 223)
//         { wireTemp = wire +320; }
//      // 7
//      if (wire > 223 && wire <= 255)
//         { wireTemp = wire  +256; }
//      // 8
//      if (wire > 255 && wire <= 287)
//         { wireTemp = wire +320; }

//      // 9
//      if (wire > 287 && wire <= 319)
//         { wireTemp = wire +256; }
//      // 10
//      if (wire > 319 && wire <= 351)
//         { wireTemp = wire  +320; }
//      // 11
//      if (wire > 351 && wire <= 383)
//         { wireTemp = wire +256; }

//      // 12
//      if (wire > 383 && wire <= 415)
//         { wireTemp = wire -256; }

//      // 13
//      if (wire > 415 && wire <= 447)
//         { wireTemp = wire -320; }
//      // 14
//      if (wire > 447 && wire <= 479)
//         { wireTemp = wire -256; }

//      // 15
//      if (wire > 479 && wire <= 511)
//         { wireTemp = wire  -320; }
//      // 16
//      if (wire > 511 && wire <= 543)
//         { wireTemp = wire -256; }
//      // 17
//      if (wire > 543 && wire <= 575)
//         { wireTemp = wire  -320; }

//      // 18
//      if (wire > 575 && wire <= 607)
//         { wireTemp = wire  -256; }
//      // 19
//      if (wire > 607 && wire <= 639)
//         { wireTemp = wire  -320; }
//      // 20
//      if (wire > 639 && wire <= 671)
//         { wireTemp = wire  -256; }
//      */


//      // 0
//      if (wire > -1 && wire <= 31)
//         {wireTemp = wire + 32; }
//      // 1
//      if (wire > 31 && wire <=63)
//         { wireTemp = wire + 32; }
//      // 2
//      if (wire > 63 && wire  <= 95)
//         { wireTemp = wire - 64; }

//      // 3
//      if (wire > 95 && wire <= 127)
//         { wireTemp = wire + 320; }
//      // 4
//      if (wire > 127 && wire <= 159)
//         { wireTemp = wire + 256; }
//      // 5
//      if (wire > 159 && wire <= 191)
//         { wireTemp = wire + 320;}

//      // 6
//      if (wire > 191 && wire <= 223)
//         { wireTemp = wire +256; }
//      // 7
//      if (wire > 223 && wire <= 255)
//         { wireTemp = wire  +320; }
//      // 8
//      if (wire > 255 && wire <= 287)
//         { wireTemp = wire +256; }

//      // 9
//      if (wire > 287 && wire <= 319)
//         { wireTemp = wire +320; }
//      // 10
//      if (wire > 319 && wire <= 351)
//         { wireTemp = wire  +256; }
//      // 11
//      if (wire > 351 && wire <= 383)
//         { wireTemp = wire -256; }

//      // 12
//      if (wire > 383 && wire <= 415)
//         { wireTemp = wire +256; }

//      // 13
//      if (wire > 415 && wire <= 447)
//         { wireTemp = wire -256; }
//      // 14
//      if (wire > 447 && wire <= 479)
//         { wireTemp = wire -320; }

//      // 15
//      if (wire > 479 && wire <= 511)
//         { wireTemp = wire  -256; }
//      // 16
//      if (wire > 511 && wire <= 543)
//         { wireTemp = wire -320; }
//      // 17
//      if (wire > 543 && wire <= 575)
//         { wireTemp = wire  -256; }

//      // 18
//      if (wire > 575 && wire <= 607)
//         { wireTemp = wire  -320; }
//      // 19
//      if (wire > 607 && wire <= 639)
//         { wireTemp = wire  -256; }
//      // 20
//      if (wire > 639 && wire <= 671)
//         { wireTemp = wire  -320; }

//       // int wireTemp = wire - 32;
//      int wireAnchor = wireTemp - (wireTemp % 32);
//      wireTemp = wireAnchor + 32 - (wire % 32) - 1;

//      if(wireTemp > 63 && wireTemp <=95)
//         {wireTemp += 320;}
//      if(wireTemp >383 && wireTemp <=447)
//         {wireTemp -= 320;}

//       wire = wireTemp;
//     // }
//     // int wireAnchor = wire - (wire % 256);
//     // wire = wireAnchor + 256 - (wire % 256) - 1 ;
//     //int wireAnchor = wire - (wire % 64) + 16;
//     //wire = wireAnchor + 64 - (wire % 64) - 1;
//     //wire = wireTemp;
//   }

// }

// if (plane == 0){
//   if (wire > 1727){
//   int wireAnchor = wire - (wire % 32);
//   wire = wireAnchor + 32 - (wire % 32) - 1;

//   }
// }

// // Catch the situation where the hanging wires are getting set:
// if (plane == 2){
//   if (wire > 1727 && wire < 1824){
//     continue;
//   }
// }

// // Now map the induction blocks that should be collection to collection:
// if (plane == 1){
//   if (wire > 1535 && wire < 1584){
//     plane = 2;
//     wire = 1728 + (wire - 1535)*2 - 2;
//     // std::cout << "wire set to " << wire << std::endl;
//   }
// }

// if (plane == 0){
//   if (wire > 863 && wire < 912){
//     plane = 2;
//     wire = 1728 + (wire - 863)*2 - 1 ;
//   }
// }

