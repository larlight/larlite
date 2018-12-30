#ifndef LARLITE_TIMESERVICE_CXX
#define LARLITE_TIMESERVICE_CXX

#include "TimeService.h"

namespace larutil {

  TimeService* TimeService::_me = 0;

  TimeService::TimeService(bool default_load) : LArUtilBase()
  {
    ClearData();
    _name = "TimeService";
    if(default_load) {
      _file_name = Form("%s/LArUtil/dat/%s",
			getenv("LARLITE_COREDIR"),
			kUTIL_DATA_FILENAME[LArUtilConfig::Detector()].c_str());
      _tree_name = kTREENAME_DETECTORPROPERTIES;
      LoadData();
    }
  }

  void TimeService::ClearData()
  {
    fG4RefTime        = -4.05e3;
    fFramePeriod      =  1.6e3;
    fTriggerOffsetTPC = kDEFAULT_TRIG_OFFSET_TPC;//-0.4e3; //hack for MCC8
    fTriggerTime      =  4.05e3;
    fBeamGateTime     =  4.05e3;

    fTPCClock     = ElecClock(fTriggerTime, fFramePeriod, 2);
    fOpticalClock = ElecClock(fTriggerTime, fFramePeriod, 64);
    fTriggerClock = ElecClock(fTriggerTime, fFramePeriod, 16);
    /*
    fG4RefTime   = kDEFAULT_MC_CLOCK_T0;
    fFramePeriod = kDEFAULT_FRAME_PERIOD;
    fTriggerOffsetTPC = kDEFAULT_TRIG_OFFSET_TPC;
    fTriggerTime = 0;
    fBeamGateTime = 0;
    fTPCClock = ElecClock(0,kDEFAULT_FRAME_PERIOD,kDEFAULT_FREQUENCY_TPC);
    fOpticalClock = ElecClock(0,kDEFAULT_FRAME_PERIOD,kDEFAULT_FREQUENCY_OPTICAL);
    fTriggerClock = ElecClock(0,kDEFAULT_FRAME_PERIOD,kDEFAULT_FREQUENCY_TRIGGER);
    */
  }

  bool TimeService::ReadTree()
  {

    ClearData();
    TChain *ch = new TChain(_tree_name.c_str());
    ch->AddFile(_file_name.c_str());

    std::string error_msg("");

    if(!(ch->GetBranch("fG4RefTime")))        error_msg += "     fG4RefTime\n";
    if(!(ch->GetBranch("fFramePeriod")))      error_msg += "     fFramePeriod\n";
    if(!(ch->GetBranch("fTriggerOffsetTPC"))) error_msg += "     fTriggerOffsetTPC\n";
    if(!(ch->GetBranch("fTriggerTime")))      error_msg += "     fTriggerTime\n";
    if(!(ch->GetBranch("fBeamGateTime")))     error_msg += "     fBeamGateTime\n";
    if(!(ch->GetBranch("fFrequencyOptical"))) error_msg += "     fFrequencyOptical\n";
    if(!(ch->GetBranch("fFrequencyTPC")))     error_msg += "     fFrequencyTPC\n";
    if(!(ch->GetBranch("fFrequencyTrigger"))) error_msg += "     fFrequencyTrigger\n";
    
    if(!error_msg.empty()) {

      throw LArUtilException(Form("Missing following TBranches...\n%s",error_msg.c_str()));

      return false;
    }

    ch->SetBranchAddress("fG4RefTime",&fG4RefTime);
    ch->SetBranchAddress("fFramePeriod",&fFramePeriod);
    ch->SetBranchAddress("fTriggerOffsetTPC",&fTriggerOffsetTPC);
    ch->SetBranchAddress("fTriggerTime",&fTriggerTime);
    ch->SetBranchAddress("fBeamGateTime",&fBeamGateTime);

    double freq_tpc, freq_opt, freq_trg;

    ch->SetBranchAddress("fFrequencyOptical",&freq_opt);
    ch->SetBranchAddress("fFrequencyTPC",&freq_tpc);
    ch->SetBranchAddress("fFrequencyTrigger",&freq_trg);

    ch->GetEntry(0);

    fOpticalClock = ElecClock(fTriggerTime, fFramePeriod, freq_opt);
    fTPCClock     = ElecClock(fTriggerTime, fFramePeriod, freq_tpc);
    fTriggerClock = ElecClock(fTriggerTime, fFramePeriod, freq_trg);

    delete ch;
    return true;
  }

}
#endif
