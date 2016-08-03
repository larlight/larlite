#include "DataFormat/wrapper.h"
#include "lardataobj/RecoBase/Hit.h"
#include "lardataobj/RecoBase/Cluster.h"
#include "lardataobj/RecoBase/OpHit.h"
#include "lardataobj/RecoBase/OpFlash.h"
#include "lardataobj/RecoBase/PFParticle.h"
#include "lardataobj/RecoBase/Track.h"
#include "lardataobj/MCBase/MCShower.h"
#include "larsimobj/Simulation/SimPhotons.h"
#include "lardataobj/RawData/TriggerData.h"
#include "lardataobj/RawData/OpDetWaveform.h"


namespace DataFormats_Common {
  struct dictionary {
    larlite::wrapper<std::vector<recob::Hit> > dummywh;
    larlite::wrapper<std::vector<recob::Cluster> > dummywc;
    larlite::wrapper<std::vector<recob::OpHit> > dummyoph;
    larlite::wrapper<std::vector<recob::OpFlash> > dummyopf;
    larlite::wrapper<std::vector<recob::PFParticle> > dummypfp;
    larlite::wrapper<std::vector<recob::Track> > dummytr;
    larlite::wrapper<std::vector<sim::SimPhotons> > dummyspt;
    larlite::wrapper<std::vector<sim::MCShower> > dummyws;
    larlite::wrapper<std::vector<raw::Trigger> > dummytrg;
    larlite::wrapper<std::vector<raw::OpDetWaveform> > dummyodw;
  };
}

