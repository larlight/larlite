#ifndef __LARLITE_DETECTORSTATUSTYPES_H__
#define __LARLITE_DETECTORSTATUSTYPES_H__

namespace larlite {
  namespace larch {
    static const short kNOTPRESENT = -1;       ///< flag channels that do not exist
    static const short kNEGATIVEPEDESTAL = -2; ///< flag channels w/ negative pedestal
    enum ChannelStatus_t { kDISCONNECTED=0,    ///< flag channels not connected (from DB)
			   kDEAD=1,            ///< flag channels that are dead (from DB)
			   kLOWNOISE=2,        ///< flag channels w/ abnormaly low noise (from DB)
			   kNOISY=3,           ///< flag channels that are very noisy (from DB)
			   kGOOD=4,            ///< flag GOOD channels wtf (from DB)
			   kUNKNOWN=5          ///< default status of a channel when no info (from DB)
    };
  }
}

#endif
