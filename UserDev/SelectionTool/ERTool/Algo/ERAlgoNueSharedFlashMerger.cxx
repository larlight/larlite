#ifndef ERTOOL_ERALGONUESHAREDFLASHMERGER_CXX
#define ERTOOL_ERALGONUESHAREDFLASHMERGER_CXX

#include "ERAlgoNueSharedFlashMerger.h"

namespace ertool {

  ERAlgoNueSharedFlashMerger::ERAlgoNueSharedFlashMerger(const std::string& name) : AlgoBase(name)
  {
    _debug = false;
  }

  void ERAlgoNueSharedFlashMerger::Reset()
  {}

  void ERAlgoNueSharedFlashMerger::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAlgoNueSharedFlashMerger::ProcessBegin()
  {}

  bool ERAlgoNueSharedFlashMerger::Reconstruct(const EventData &data, ParticleGraph& graph)
  {

    FlashID_t default_flash_id = 9999999;
    FlashID_t nue_flashid = default_flash_id;
    FlashID_t other_flashid = default_flash_id + 1;

    // Hard coded 5cm distance cut. This will come from a config file!
    double dist2_cut = 25.;
    double common_dist_cut = 10.;

    /// Loop over the reconstructed particle graph and try to find a nue
    for ( auto const &nue : graph.GetParticleArray() ) {

      if ( abs(nue.PdgCode()) != 12 ) continue;

      /// Now we have a nue
      /// Get the flash for that nue, if there is one:
      try {
        nue_flashid = data.Flash(graph.GetParticle(nue.Ancestor())).FlashID();
      }
      catch ( ERException &e ) {
        //No flash found for the nue
      }

      /// If we didn't find a flash for the nue, skip this nue
      if ( nue_flashid == default_flash_id ) continue;

      /// Now we have a nue and its associated flash.
      /// Loop through every other particle in the reco particle graph that
      /// is NOT a descendant of the nue already, and see if it has a matching flash.
      for ( auto const &part : graph.GetParticleArray() ) {

        /// If this particle is already set as the child of anything else (including the nue), skip it
        if ( part.Descendant() ) continue;

        /// Get the flash for this particle
        try {
          other_flashid = data.Flash(graph.GetParticle(part.Ancestor())).FlashID();
        }
        catch ( ERException &e ) {
          //No flash found for this particle
          other_flashid = default_flash_id + 1;
        }

        /// If this particle doesn't share a flash with the nue, skip it
        if ( other_flashid != nue_flashid ) continue;


        /// Check if particle is close to the nue, and potentially add it as a child of the nue

        /// (Two cases: the particle is a track or a shower)
        if ( part.RecoType() == kTrack ) {

          if (_debug)
            std::cout << "Found a track in the event, square distance to neutrino vtx is " << data.Track(part).front().SqDist(nue.Vertex()) << std::endl;

          /// If start point of the track is close to the neutrino vertex
          if ( data.Track(part).front().SqDist(nue.Vertex()) < dist2_cut ) {

            if (_debug)
              std::cout << "w00t! found a track (pdg = " << part.PdgCode() << ") a squared distance "
                        << data.Track(part).front().SqDist(nue.Vertex())
                        << " from the neutrino vtx! adding as child... " << std::endl;

            /// Set parentage: this track is a child of the nue
            graph.SetParentage(nue.ID(), part.ID());

          }
        } // End if potential particle to merge into interaction is a track

        else if ( part.RecoType() == kShower ) {
          auto shr = data.Shower(part);

          if (_debug)
            std::cout << "found a shower in the event... pdg = " << part.PdgCode() << " and ID = " << part.ID() << std::endl;
          if (_debug)
            std::cout << "and distance to neutrino vertex is " << std::sqrt(data.Shower(part).Start().SqDist(nue.Vertex())) << std::endl;

          /// Let's see if the nue has an electron as a daughter
          /// (right now i'm imagining it's a gamma that was MIDd in a NC pi0 event)
          /// and let's project the electron backwards and see if it comes from
          /// a common origin with this gamma

          for ( auto const &kid : nue.Children() ) {
            auto mykid = graph.GetParticle(kid);

            /// This is the electron daughter of the nue
            if ( abs(mykid.PdgCode()) == 11 ) {

              auto electron = data.Shower(mykid);
              double common_dist = std::sqrt( _geoalgo.commonOrigin(shr, electron, true) );

              if (_debug) {
                std::cout << "nue has a mykid that is an electron, with ID " << mykid.ID() << std::endl;
                std::cout << "Common origin says electron to shower bkwd dist is " << common_dist << std::endl;
                std::cout << " (common_dist = " << common_dist << ")" << std::endl;
              }

              /// If the backward projection of the electron and the flash-matched other shower
              /// are close together, set the nue as process=kPiZeroMID
              /// In the future, perhaps the nue should be deleted here, but right now
              /// particle deletion is not implemented in particlegraph
              if (common_dist < common_dist_cut) {
                if (_debug) {
                  std::cout << "Woah! Found a shower that nearly aligns with backward projected nue electron."
                            << std::endl;
                  std::cout << "Setting the neutrino process type to kPiZeroMID!" << std::endl;
                }

                graph.GetParticle(nue.ID()).SetProcess(kPiZeroMID);

              }
            } // End if nue's child is an electron

          } // End loop over nue's children

        } // End if potential particle to merge into interaction is a shower

      } // End inner loop over reconstructed particle graph to find additional particles to merge

    } // End outer loop over reconstructed particle graph

    return true;
  }

  void ERAlgoNueSharedFlashMerger::ProcessEnd(TFile* fout)
  {}

}

#endif
