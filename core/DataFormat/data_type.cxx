#include "data_type.h"

namespace larlite {

  template<> data::DataType_t data_type<trigger> ()
  { return data::kTrigger; }

  template<> data::DataType_t data_type<event_gtruth> ()
  { return data::kGTruth; }

  template<> data::DataType_t data_type<event_mctruth> ()
  { return data::kMCTruth; }

  template<> data::DataType_t data_type<event_mcpart> ()
  { return data::kMCParticle; }

  template<> data::DataType_t data_type<event_mcflux> ()
  { return data::kMCFlux; }

  template<> data::DataType_t data_type<event_simch> ()
  { return data::kSimChannel; }

  template<> data::DataType_t data_type<event_mcshower> ()
  { return data::kMCShower; }

  template<> data::DataType_t data_type<event_rawdigit> ()
  { return data::kRawDigit;}

  template<> data::DataType_t data_type<event_opdetwaveform> ()
  { return data::kOpDetWaveform; }

  template<> data::DataType_t data_type<event_wire> ()
  { return data::kWire; }

  template<> data::DataType_t data_type<event_hit> ()
  { return data::kHit; }

  template<> data::DataType_t data_type<event_ophit> ()
  { return data::kOpHit; }

  template<> data::DataType_t data_type<event_opflash> ()
  { return data::kOpFlash; }

  template<> data::DataType_t data_type<event_cluster> ()
  { return data::kCluster; }

  template<> data::DataType_t data_type<event_seed> ()
  { return data::kSeed; }

  template<> data::DataType_t data_type<event_spacepoint> ()
  { return data::kSpacePoint; }

  template<> data::DataType_t data_type<event_track> ()
  { return data::kTrack; }

  template<> data::DataType_t data_type<event_trackmom> ()
  { return data::kTrackMomentum; }

  template<> data::DataType_t data_type<event_shower> ()
  { return data::kShower; }

  template<> data::DataType_t data_type<event_vertex> ()
  { return data::kVertex; }

  template<> data::DataType_t data_type<event_endpoint2d> ()
  { return data::kEndPoint2D; }

  template<> data::DataType_t data_type<event_calorimetry> ()
  { return data::kCalorimetry; }

  template<> data::DataType_t data_type<event_partid> ()
  { return data::kParticleID; }

  template<> data::DataType_t data_type<event_pfpart> ()
  { return data::kPFParticle; }

  template<> data::DataType_t data_type<event_user> ()
  { return data::kUserInfo; }

  template<> data::DataType_t data_type<event_mctrack> ()
  { return data::kMCTrack;  }

  template<> data::DataType_t data_type<event_mctree> ()
  { return data::kMCTree;  }

  template<> data::DataType_t data_type<event_cosmictag> ()
  { return data::kCosmicTag; }

  template<> data::DataType_t data_type<event_minos> ()
  { return data::kMinos; }

  template<> data::DataType_t data_type<event_ass> ()
  { return data::kAssociation; }

  template<> data::DataType_t data_type<event_fifo> ()
  { return data::kFIFO; }

  template<> data::DataType_t data_type<event_pcaxis> ()
  { return data::kPCAxis; }

  template<> data::DataType_t data_type<event_flashmatch> ()
  { return data::kFlashMatch; }

  template<> data::DataType_t data_type<event_mucsdata> ()
  { return data::kMuCSData; }

  template<> data::DataType_t data_type<event_mucsreco> ()
  { return data::kMuCSReco; }

  template<> data::DataType_t data_type<event_pizeroroi> ()
  { return data::kPiZeroROI; }

  template<> data::DataType_t data_type<event_auxsimch> ()
  { return data::kAuxDetSimChannel; }

  template<> data::DataType_t data_type<event_chstatus> ()
  { return data::kChStatus; }

  template<> data::DataType_t data_type<wrapper<std::vector<recob::Hit> > > ()
  { return data::kLarSoftHit; }

  template<> data::DataType_t data_type<wrapper<std::vector<recob::Cluster> > > ()
  { return data::kLarSoftCluster; }

  template<> data::DataType_t data_type<wrapper<std::vector<recob::OpHit> > > ()
  { return data::kLarSoftOpHit; }

  template<> data::DataType_t data_type<wrapper<std::vector<recob::OpFlash> > > ()
  { return data::kLarSoftOpFlash; }

  template<> data::DataType_t data_type<wrapper<std::vector<recob::PFParticle> > > ()
  { return data::kLarSoftPFParticle; }

  template<> data::DataType_t data_type<wrapper<std::vector<recob::Track> > > ()
  { return data::kLarSoftTrack; }

  template<> data::DataType_t data_type<wrapper<std::vector<recob::Shower> > > ()
  { return data::kLarSoftShower; }

  template<> data::DataType_t data_type<wrapper<std::vector<recob::EndPoint2D> > > ()
  { return data::kLarSoftEndPoint2D; }

  template<> data::DataType_t data_type<wrapper<std::vector<recob::Vertex> > > ()
  { return data::kLarSoftVertex; }

  template<> data::DataType_t data_type<wrapper<std::vector<recob::SpacePoint> > > ()
  { return data::kLarSoftSpacePoint; }

  template<> data::DataType_t data_type<wrapper<std::vector<sim::MCShower> > > ()
  { return data::kLarSoftMCShower; }

  template<> data::DataType_t data_type<wrapper<std::vector<sim::SimPhotons> > > ()
  { return data::kLarSoftSimPhotons; }

  template<> data::DataType_t data_type<wrapper<std::vector<sim::MCTrack> > > ()
  { return data::kLarSoftMCTrack; }

  template<> data::DataType_t data_type<wrapper<std::vector<sim::SimChannel> > > ()
  { return data::kLarSoftSimChannel; }

  template<> data::DataType_t data_type<wrapper<std::vector<raw::OpDetWaveform> > > ()
  { return data::kLarSoftOpDetWaveform; }

  template<> data::DataType_t data_type<wrapper<std::vector<raw::Trigger> > > ()
  { return data::kLarSoftTrigger; }

  template<> data::DataType_t data_type<wrapper<std::vector<::simb::MCTruth> > > ()
  { return data::kLarSoftMCTruth; }

  template<> data::DataType_t data_type<wrapper<std::vector<::simb::MCParticle> > > ()
  { return data::kLarSoftMCParticle; }

  template<> data::SubRunDataType_t subrundata_type<potsummary>()
  { return data::kPOTSummary; }
}
