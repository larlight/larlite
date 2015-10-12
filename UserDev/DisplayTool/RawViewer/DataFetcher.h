#ifndef DATAFETCHER_H_
#define DATAFETCHER_H_

// #include "stdint.h"

#include <string>
#include <vector>

#include "TTree.h"
#include "TBranchElement.h"

namespace evd {

class DataFetcher {
public:

  DataFetcher();
  ~DataFetcher();

  //int * event_number();

  void get_entry(unsigned int entry);

  unsigned int entries() {
    return entries_;
  }

  // TODO: Rename to adc_number_wires()?
  unsigned int adc_rows() {
    return adc_rows_;
  }

  // TODO: Rename to adc_number_time_ticks() or adc_number_samples()?
  unsigned int adc_cols() {
    return adc_cols_;
  }

  std::vector<short> & adc() {
    // short * adc_array = &adc_[0];
    // return adc_array;
    return adc_;
  }

  std::vector<short> & channel() {
    // short * channel_array = &channel_[0];
    // return channel_array;
    return channel_;
  }

  double * pedestal() {
    double * pedestal_array = &pedestal_[0];
    return pedestal_array;
  }

  unsigned int run() {return run_;}
  unsigned int subrun() {return subrun_;}
  unsigned int event() {return event_;}

  void set_file_path(std::string s);


private:

  std::string file_path_;
  TTree * tree_;

  std::vector<std::string> FindLeavesOfType(std::string pattern);

  UShort_t run_;
  UShort_t subrun_;
  UShort_t event_;

  // TBranchElement        *b_raw__DAQHeader_daq__Swizzler_obj_fRun;   //!
  // TBranchElement        *b_raw__DAQHeader_daq__Swizzler_obj_fSubRun;   //!
  // TBranchElement        *b_raw__DAQHeader_daq__Swizzler_obj_fEvent;   //!
  // TBranch        *b_raw__DAQHeader_daq__Swizzler_obj_fTime;

  unsigned int entries_;
  unsigned int adc_rows_;
  unsigned int adc_cols_;

  std::vector<short> adc_;
  std::vector<short> channel_;
  std::vector<double> pedestal_;

};

}
#endif  // DATAFETCHER_H_