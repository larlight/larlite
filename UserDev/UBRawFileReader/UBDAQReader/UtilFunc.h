#ifndef UTILFUNC_H
#define UTILFUNC_H

#include "UBDaqID.h"
#include <vector>
namespace ubdaq {

  unsigned int RollOver(unsigned int ref,
			unsigned int subject,
			unsigned int nbits);

  std::vector<short> decodeChannelTrailer(unsigned short last_adc, unsigned short data);

  void LoadUBChannelMap(UBChannelMap_t& fChannelMap, UBChannelReverseMap_t& fChannelReverseMap );
  
}
#endif
