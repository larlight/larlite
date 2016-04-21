#ifndef PROTOSHOWERHELPER_CXX
#define PROTOSHOWERHELPER_CXX

#include "ProtoShowerHelper.h"

namespace protoshower {

ProtoShowerHelper::ProtoShowerHelper()
  : _proto_shower_alg(nullptr)
{}

// From the storage manager, and with the pfpart producer name, generate
// a vector of all the protoshowers in the event.
void ProtoShowerHelper::GenerateProtoShowers(::larlite::storage_manager* storage,
    const std::string &pfpart_producer_name,
    std::vector<ProtoShower> & proto_showers,
    std::vector<unsigned int> showerLikePFParts) {


  // Clear the current proto-shower container.
  proto_showers.clear();

  // First, get the pfparticles and verify they are there.
  auto * ev_pfpart = storage -> get_data<larlite::event_pfpart>(pfpart_producer_name);

  // Check if there are pfparticles:
  if (!ev_pfpart) {
    return;
  }
  if (ev_pfpart -> size() == 0) {
    return;
  }

  // Now we have pfparticles, so start to build protoshowers

  // In general, only build the ones with index specified by showerLikePFParts.
  // If it's not specified, build them all.


  if (showerLikePFParts.size() == 1) {

    if (showerLikePFParts.at(0) == 999999) {


      showerLikePFParts.resize(ev_pfpart -> size());
      for (unsigned int i = 0; i < showerLikePFParts.size(); ++i)
      {
        showerLikePFParts.at(i) = i;
      }
    }

  }

  if (showerLikePFParts.size() == 0) {
    return;
  }


  proto_showers.resize(showerLikePFParts.size());

  // use the indexes stored in showerLikePFParts to loop through the clusters
  for (size_t i = 0; i < showerLikePFParts.size(); i ++ ) {

    // this is the index of the PFParticle for the protoshower we are trying to build
    size_t proto_shower_pfpart = showerLikePFParts.at(i);

    _proto_shower_alg->GenerateProtoShower( storage,
                                            ev_pfpart,
                                            proto_shower_pfpart,
                                            proto_showers.at(i) );

  }// for all PFParticles

  // Do the seeds:
  // \TODO
  /*   _____ ___  ____   ___
      |_   _/ _ \|  _ \ / _ \
        | || | | | | | | | | |
        | || |_| | |_| | |_| |
        |_| \___/|____/ \___/
  */

  return;
}

} // protoshower

#endif
