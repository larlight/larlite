#ifndef DATAFETCHER_H_
#define DATAFETCHER_H_

// #include "stdint.h"

#include <string>
#include <vector>

#include "TTree.h"

namespace evd {

class DataFetcher {
public:

  DataFetcher(std::string file_path);
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

  size_t run(){return run_;}
  size_t subrun(){return subrun_;}
  size_t event(){return event_;}



private:

  std::string file_path_;
  TTree * tree_;

  std::vector<std::string> FindLeavesOfType(std::string pattern);

  size_t run_;
  size_t subrun_;
  size_t event_;

  unsigned int entries_;
  unsigned int adc_rows_;
  unsigned int adc_cols_;
  unsigned int number_particles_;

  std::vector<short> adc_;
  std::vector<short> channel_;
  std::vector<double> pedestal_;
  std::vector<int> pdg_code_;
  std::vector<int> track_id_;
  std::vector<int> parent_id_;
  std::vector<std::string> process_;
  std::vector<double> start_momentum_;
  std::vector<double> trajectory_length_;
  std::vector< std::vector<double> > particle_x_;
  std::vector< std::vector<double> > particle_y_;
  std::vector< std::vector<double> > particle_z_;
  std::vector< std::vector<double> > particle_t_;
  std::vector< std::vector<double> > particle_px_;
  std::vector< std::vector<double> > particle_py_;
  std::vector< std::vector<double> > particle_pz_;
  std::vector< std::vector<double> > particle_energy_;

};

}
#endif  // DATAFETCHER_H_