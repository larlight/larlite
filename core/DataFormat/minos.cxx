#include "minos.h"

namespace larlite{
  minos::minos() : data_base(data::kMinos)
		 , ftrkstpX(1,-999.0)
		 , ftrkstpY(1,-999.0)
		 , ftrkstpZ(1,-999.0)
		 , ftrkstpU(1,-999.0)
		 , ftrkstpV(1,-999.0)
		 , fmatched(-1)
  {}
  
  minos::~minos()
  {}
}
