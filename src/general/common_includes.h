#ifndef COMMON_INCLUDES_H
#define COMMON_INCLUDES_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <random>
#include <set>

// Include ArgParse
#include "../../external_src/libargparse/src/argparse.hpp"

namespace CA
{
#define DEB(x)  std::cout << "[" << #x << "]: "  << x << std::endl

#define KMH_TO_MS 0.2778
#define MS_TO_KMH 3.6
 
#ifdef TYPEDEF_REAL_IS_DOUBLE
 typedef double Real;
#else
 typedef float Real;
#endif // #ifdef TYPEDEF_REAL_IS_DOUBLE

 //Pre-processor magic for error reporting
 //Macro that converts argument to string
#define CA_MAKE_STRING(x) #x

 //Macro wrapper to CA_MAKE_STRING, required because calling
 //CA_MAKE_STRING(__LINE__) directly returns __LINE__
 //i.e. the conversion of __LINE__ into a number must be performed before
 //its conversion into a string
#define CA_TO_STRING(x) CA_MAKE_STRING(x)

 //Combine the FILE and LINE built-in macros into a string that can
 //be used in error messages.
#define CA_EXCEPTION_LOCATION __FILE__ ":" CA_TO_STRING(__LINE__)

 // Get the current function name. All the mess is due to different
 // compilers naming the macro we need differently.
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
# define CA_CURRENT_FUNCTION __PRETTY_FUNCTION__

#elif defined(__DMC__) && (__DMC__ >= 0x810)
# define CA_CURRENT_FUNCTION __PRETTY_FUNCTION__

#elif defined(__FUNCSIG__)
# define CA_CURRENT_FUNCTION __FUNCSIG__

#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
# define CA_CURRENT_FUNCTION __FUNCTION__

#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
# define CA_CURRENT_FUNCTION __FUNC__

#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
# define CA_CURRENT_FUNCTION __func__

#else
# define CA_CURRENT_FUNCTION "[Unknown function -- unrecognised compiler]"

#endif
 
} // namespace CA

#endif // #ifndef COMMON_INCLUDES_H
