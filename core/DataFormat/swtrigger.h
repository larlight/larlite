#ifndef _LARLITE_DAQSWTRIGGERDATA_H
#define _LARLITE_DAQSWTRIGGERDATA_H
//#include <sys/types.h>
//#include <inttypes.h>
//#include "evttypes.h"
#include <string>
#include <vector>
#include <iostream>
#include "data_base.h"
//#include "uboone/TriggerSim/UBTriggerTypes.h"

namespace larlite {

  //class ubdaqSoftwareTriggerData {
  class swtrigger : public event_base {

 public:
    swtrigger(std::string name="noname"); // standard constructor

    /// Clear method
    void clear_data();

  void addAlgorithm(std::string name, bool pass, bool pass_prescale,
		    uint32_t phmax, uint32_t multiplicity, uint32_t triggerTick, double triggerTime, float prescale);

  int getNumberOfAlgorithms(void) const;
  std::vector<std::string> getListOfAlgorithms(void) const;
  
  // pass/veto by name
  bool passedAlgo(std::string algo) const; // passed this algorithm
  bool passedPrescaleAlgo(std::string algo) const; // passed this algorithms prescale
  bool vetoAlgo(std::string algo) const; // NOT passAlgo
  bool passedAlgos(std::vector<std::string> algos) const; // passed any on a list of these algorithms
  bool vetoAlgos(std::vector<std::string> algos) const; // NOT passedAlgos
  bool passedPrescaleAlgos(std::vector<std::string> algos) const; //passed any on a list of these prescales

  //getters by entry index
  bool getPass(int i) const;
  bool getPassPrescale(int i) const;
  uint32_t getPhmax(int i) const;
  uint32_t getMultiplicity(int i) const;
  uint32_t getTriggerTick(int i) const;
  double getTimeSinceTrigger(int i) const;
  std::string getTriggerAlgorithm(int i) const;
  float getPrescale(int i) const;

  //getters by name
  uint32_t getPhmax(std::string algo) const;  // max adc sum at (software) trigger firing time
  uint32_t getMultiplicity(std::string algo) const;  // multiplicity at (software) trigger firing time
  uint32_t getTriggerTick(std::string algo) const;   // tick since the beam-gate opened
  double getTimeSinceTrigger(std::string algo) const;  // time since the event (hardware) trigger, in us
  int getID(std::string algo) const; // get the index of a given algorithm
  float getPrescale(std::string algo) const; // returns prescale_weight (see below)

 private:

  std::vector<std::pair<std::string,bool> > passAlgo; // list of algorithms and corresponding bools for pass/fail
  std::vector<bool> passPrescale; // list of algorithms and corresponding bools for pass/fail
  std::vector<uint32_t> PHMAX; // max adc sum at (software) trigger firing time
  std::vector<uint32_t> multiplicity; // multiplicity at (software) trigger firing time
  std::vector<uint32_t> triggerTick; // tick since the beam-gate opened
  std::vector<double> triggerTime; // time since the event (hardware) trigger, in us
  std::vector<float> prescale_weight; // 1/prescale_weight gives the fraction of events that are let through

};


}  // end of namespace raw

#endif 



