#ifndef RECOTOOL_SHOWERCALO_CXX
#define RECOTOOL_SHOWERCALO_CXX

#include "ShowerCalo.h"

namespace showerreco {

std::pair<double, double> ShowerCalo::ShowerEnergy(double energy,
    ::showerreco::energy::RecoType_t type,
    bool mc)
{
  double result = -1;
  double result_err = -1;

  switch (type) {

  case ::showerreco::energy::kDEFAULT:

    if (!mc) throw ShowerRecoException(Form("DATA correction not yet supported!"));

    result = ::showerreco::energy::DEFAULT_ECorr * energy + 10;
    result_err = ::showerreco::energy::DEFAULT_ECorrError * result;
    break;

  default:

    throw ShowerRecoException(Form("RecoType_t: %d not supported!", type));
  }

  return std::make_pair(result, result_err);
}

}
#endif
