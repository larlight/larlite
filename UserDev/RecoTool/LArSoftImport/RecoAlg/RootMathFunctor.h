// wrap the root Math/Functor.h package so it does not complain about
// the deprecated auto_ptr

#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 9)) && !defined(__INTEL_COMPILER) 
# pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
#include "Math/Functor.h"
