
//#include <time.h>
//#include "ubdaqSoftwareTriggerData.h"
#include "swtrigger.h"

//-----------------------------------------------------------------------------------

larlite::swtrigger::swtrigger(std::string name)
  : event_base(data::kSWTrigger,name)
{}

void larlite::swtrigger::clear_data()
{
  event_base::clear_data();
  passAlgo.clear();
  passPrescale.clear();
  PHMAX.clear();
  multiplicity.clear();
  triggerTick.clear();
  triggerTime.clear();
  prescale_weight.clear();
}

void larlite::swtrigger::addAlgorithm(std::string name_, bool pass_, bool pass_prescale_, uint32_t phmax_, uint32_t multiplicity_, uint32_t triggerTick_, double triggerTime_, float prescale_){
  std::pair<std::string, bool> tmp_pair;
  std::pair<std::string, bool> tmp_pair_prescale;
  tmp_pair.first = name_;
  tmp_pair.second = pass_;
  
  passAlgo.push_back(tmp_pair);
  passPrescale.push_back(pass_prescale_);
  PHMAX.push_back(phmax_);
  multiplicity.push_back(multiplicity_);
  triggerTick.push_back(triggerTick_);
  triggerTime.push_back(triggerTime_);
  prescale_weight.push_back(prescale_);
}

//-----------------------------------------------------------------------------------

std::vector<std::string> larlite::swtrigger::getListOfAlgorithms(void) const{
  std::vector<std::string> list;
  for (auto const algoPair: passAlgo){
    list.push_back(algoPair.first);
  }
  return list;
}

//-----------------------------------------------------------------------------------

int larlite::swtrigger::getNumberOfAlgorithms(void) const{
  return passAlgo.size();
}


//-----------------------------------------------------------------------------------

bool larlite::swtrigger::passedAlgo(std::string algo) const{
  int id = getID(algo);
  return getPass(id);
}

//-----------------------------------------------------------------------------------

bool larlite::swtrigger::passedPrescaleAlgo(std::string algo) const{
  int id = getID(algo);
  return getPassPrescale(id);
}

//-----------------------------------------------------------------------------------

bool larlite::swtrigger::vetoAlgo(std::string algo) const{
  return not passedAlgo(algo);
}

//-----------------------------------------------------------------------------------

bool larlite::swtrigger::passedAlgos(std::vector<std::string> algos) const{
  for (auto const &algo: algos){
    if (passedAlgo(algo)){
      return true;
    }
  }
  return (algos.size() == 0);
}


//-----------------------------------------------------------------------------------

bool larlite::swtrigger::passedPrescaleAlgos(std::vector<std::string> algos) const{
  for (auto const &algo: algos){
    if (passedPrescaleAlgo(algo)){
      return true;
    }
  }
  return (algos.size() == 0);
}


//-----------------------------------------------------------------------------------

bool larlite::swtrigger::vetoAlgos(std::vector<std::string> algos) const{
  return not passedAlgos(algos);
}

//-----------------------------------------------------------------------------------

uint32_t larlite::swtrigger::getPhmax(std::string algo) const{  // max adc sum at (software) trigger firing time
  int id = getID(algo);
  return getPhmax(id);
}

//-----------------------------------------------------------------------------------

uint32_t larlite::swtrigger::getMultiplicity(std::string algo) const{  // multiplicity at (software) trigger firing time
  int id = getID(algo);
  return getMultiplicity(id);
}

//-----------------------------------------------------------------------------------

uint32_t larlite::swtrigger::getTriggerTick(std::string algo) const{   // tick since the beam-gate opened
  int id = getID(algo);
  return getTriggerTick(id);
}

//-----------------------------------------------------------------------------------

double larlite::swtrigger::getTimeSinceTrigger(std::string algo) const{  // time since the event (hardware) trigger, in us
  int id = getID(algo);
  return getTimeSinceTrigger(id);
}

//-----------------------------------------------------------------------------------

int larlite::swtrigger::getID(std::string algo) const{
  for (unsigned int i(0); i < passAlgo.size(); ++i){
    if (passAlgo.at(i).first == algo){ // correct algorithm
      return i;
    }
  }
  std::cout << "WARNING - asked for information on a trigger algorithm that isn't present!" << std::endl;
  return -999;
} 

//-----------------------------------------------------------------------------------

float larlite::swtrigger::getPrescale(std::string algo) const{
  int id = getID(algo);
  return getPrescale(id);
}

//-----------------------------------------------------------------------------------

bool larlite::swtrigger::getPass(int i) const { 
  if (i >= 0){
    if ((unsigned)i >= passAlgo.size()){
      std::cout << "WARNING - asked for information on a trigger algorithm that isn't present!" << std::endl;
      return 0;
    }
    return passAlgo.at(i).second;
  }
  std::cout << "WARNING - asked for information on a trigger algorithm that isn't present!" << std::endl; // negative index;
  return 0;
}
//-----------------------------------------------------------------------------------

bool larlite::swtrigger::getPassPrescale(int i) const { 
  if (i >= 0){
    if ((unsigned)i >= passAlgo.size()){
      std::cout << "WARNING - asked for information on a trigger algorithm that isn't present!" << std::endl;
      return 0;
    }
    return passPrescale.at(i);
  }
  std::cout << "WARNING - asked for information on a trigger algorithm that isn't present!" << std::endl; // negative index;
  return 0;
}

//-----------------------------------------------------------------------------------


uint32_t larlite::swtrigger::getPhmax(int i) const{
  if (i >= 0){
    if ((unsigned)i >= PHMAX.size()){
      std::cout << "WARNING - asked for information on a trigger algorithm that isn't present!" << std::endl;
      return 0;
    }
    return PHMAX.at(i);
  }
  std::cout << "WARNING - asked for information on a trigger algorithm that isn't present!" << std::endl; // negative index;
  return 0;

} 

//-----------------------------------------------------------------------------------

uint32_t larlite::swtrigger::getMultiplicity(int i) const{
  if (i >= 0){
    if ((unsigned)i >= multiplicity.size()){
      std::cout << "WARNING - asked for information on a trigger algorithm that isn't present!" << std::endl;
      return 0;
    }
    return multiplicity.at(i);
  }
  std::cout << "WARNING - asked for information on a trigger algorithm that isn't present!" << std::endl; // negative index
  return 0;
} 

//-----------------------------------------------------------------------------------

uint32_t larlite::swtrigger::getTriggerTick(int i) const{
  if (i >= 0){
    if ((unsigned)i >= triggerTick.size()){
      std::cout << "WARNING - asked for information on a trigger algorithm that isn't present!" << std::endl;
      return 0;
    }
    return triggerTick.at(i);
  }
  std::cout << "WARNING - asked for information on a trigger algorithm that isn't present!" << std::endl; // negative index
  return 0;
}  

//-----------------------------------------------------------------------------------

double larlite::swtrigger::getTimeSinceTrigger(int i) const{
  if (i >= 0){
    if ((unsigned)i >= triggerTime.size()){
      std::cout << "WARNING - asked for information on a trigger algorithm that isn't present!" << std::endl;
      return -999;
    }
    return triggerTime.at(i);
  }
  std::cout << "WARNING - asked for information on a trigger algorithm that isn't present!" << std::endl; // negative index
  return -999;
}

//-----------------------------------------------------------------------------------

std::string larlite::swtrigger::getTriggerAlgorithm(int i) const{
  if (i >= 0){
    if ((unsigned)i >= passAlgo.size()){
      std::cout << "WARNING - asked for information on a trigger algorithm that isn't present!" << std::endl;
      return "";
    }
    return passAlgo.at(i).first;
  }
  std::cout << "WARNING - asked for information on a trigger algorithm that isn't present!" << std::endl; // negative index
  return "";
}

//-----------------------------------------------------------------------------------

float larlite::swtrigger::getPrescale(int i) const{
  if (i >= 0){
    if ((unsigned)i >= prescale_weight.size()){
      std::cout << "WARNING - asked for information on a trigger algorithm that isn't present!" << std::endl;
      return 1;
    }
    return prescale_weight.at(i);
  }
  std::cout << "WARNING - asked for information on a trigger algorithm that isn't present!" << std::endl; // negative index
  return 1;
}

