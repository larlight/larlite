#ifndef SELECTIONTOOL_SPTBASE_TYPEDEF_H
#define SELECTIONTOOL_SPTBASE_TYPEDEF_H

// Header files included here
#include "SPAManager.h"
#include "SPTBase.h"
#include "SPAData.h"
#include "SPAHelper.h"
#include "ShowerPdfFactory.h"
//ADD_NEW_HEADER ... do not change this comment line

// Class forward declaration here
namespace sptool {
  class ShowerPdfFactory;
  class SPTBase;
  class SPAShower;
  class SPAData;
  class SPAManager;
  class SPAHelper;
}
//ADD_EMPTY_CLASS ... do not change this comment line
#endif

/** @defgroup ShowerPdfTool ShowerPdfTool
    @brief Package for single/multi-shower selection using probability distribution function (PDF).
    ShowerPdfTool (SPT) is a simple framework to perform single/multi-shower interaction using
    probability distribution functions (PDF). The framework consists of multiple C++ classes that
    live under sptool namespace. In particular the following classes are the building blocks:
    
    0) sptool::SPTBase      \n
    1) sptool::SPAlgoBase   \n
    2) sptool::SPFilterBase \n
    3) sptool::SPAManager   \n
    4) sptool::ShowerPdfFactory \n

    See doxygen documentation for each class for details.

    If you have a question about framework basics, contact kazu.
    If you have a question about specific SPAlgoX or PDF in ShowerPdfFactory, contact Jeremy Hewes and David Caratelli.

 */








