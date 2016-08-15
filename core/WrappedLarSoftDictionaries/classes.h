#include "DataFormat/wrapper.h"
#include "lardataobj/RecoBase/Hit.h"
#include "lardataobj/RecoBase/Cluster.h"
#include "lardataobj/RecoBase/OpHit.h"
#include "lardataobj/RecoBase/OpFlash.h"
#include "lardataobj/RecoBase/PFParticle.h"
#include "lardataobj/RecoBase/Track.h"
#include "lardataobj/RawData/TriggerData.h"
#include "lardataobj/RawData/OpDetWaveform.h"
#include "lardataobj/MCBase/MCShower.h"
#include "lardataobj/MCBase/MCTrack.h"
#include "larsimobj/Simulation/SimPhotons.h"
#include "larsimobj/Simulation/SimChannel.h"
#include "lardataobj/RecoBase/Shower.h"
#include "lardataobj/RecoBase/EndPoint2D.h"
#include "nusimdata/SimulationBase/MCTruth.h"
#include "nusimdata/SimulationBase/MCTruth.h"
#include "lardataobj/RecoBase/Vertex.h"
#include "lardataobj/RecoBase/SpacePoint.h"

namespace DataFormats_Common {
  struct dictionary {
    larlite::wrapper<std::vector<recob::Hit> > dummywh;
    larlite::wrapper<std::vector<recob::Cluster> > dummywc;
    larlite::wrapper<std::vector<recob::OpHit> > dummyoph;
    larlite::wrapper<std::vector<recob::OpFlash> > dummyopf;
    larlite::wrapper<std::vector<recob::PFParticle> > dummypfp;
    larlite::wrapper<std::vector<recob::Track> > dummytr;
    larlite::wrapper<std::vector<recob::Shower> > dummysho;
    larlite::wrapper<std::vector<recob::EndPoint2D> > dummyep2;
    larlite::wrapper<std::vector<recob::Vertex> > dummyvtx;
    larlite::wrapper<std::vector<recob::SpacePoint> > dummyspo;
    larlite::wrapper<std::vector<sim::SimPhotons> > dummyspt;
    larlite::wrapper<std::vector<sim::MCShower> > dummyws;
    larlite::wrapper<std::vector<sim::MCTrack> > dummywt;
    larlite::wrapper<std::vector<sim::SimChannel> > dummyschn;
    larlite::wrapper<std::vector<raw::Trigger> > dummytrg;
    larlite::wrapper<std::vector<raw::OpDetWaveform> > dummyodw;
    larlite::wrapper<std::vector<::simb::MCTruth> > dummymcth;
    larlite::wrapper<std::vector<::simb::MCParticle> > dummymcpa;
  };
}

