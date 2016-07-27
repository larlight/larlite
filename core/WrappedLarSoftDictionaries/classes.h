#include "DataFormat/wrapper.h"
#include "lardataobj/RecoBase/Hit.h"
#include "lardataobj/RecoBase/Cluster.h"

namespace DataFormats_Common {
  struct dictionary {
    larlite::wrapper<std::vector<recob::Hit> > dummywh;
    larlite::wrapper<std::vector<recob::Cluster> > dummywc;
  };
}

