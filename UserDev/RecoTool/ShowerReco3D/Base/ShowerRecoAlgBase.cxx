#ifndef RECOTOOL_SHOWERRECOALGBASE_CXX
#define RECOTOOL_SHOWERRECOALGBASE_CXX

#include "ShowerRecoAlgBase.h"

namespace showerreco {

ShowerRecoAlgBase::ShowerRecoAlgBase()
{
  _verbosity = false;
}

void ShowerRecoAlgBase::Reset()
{}

void ShowerRecoAlgBase::Verbose(bool on)
{ _verbosity = on; }

}

#endif
