#ifndef DEFINITIONS_H
#define DEFINITIONS_H

namespace CA
{

#ifdef TYPEDEF_REAL_IS_DOUBLE
 typedef double Real;
#else
 typedef float Real;
#endif // #ifdef TYPEDEF_REAL_IS_DOUBLE
 
#ifdef CELLULAR_AUTOMATON_PANIC_MODE
 // =======================================================================
 /// Macros for debugging
 // =======================================================================
#define DEB(x) std::cerr<<"DEB ["<<#x<<"] = "<<x<<std::endl<<std::flush;
#define DEB2(x,y) std::cerr<<"DEB ["<<x<<" "<<y<<"]"<<std::endl<<std::flush;
#define DEB3(x,y,z) std::cerr<<"DEB ["<<x<<" "<<y<<" "<<z<<"]"<<std::endl<<std::flush;
#define DEB4(t,x,y,z) std::cerr<<"DEB ["<<t<<" "<<x<<" "<<y<<" "<<z<<"]"<<std::endl<<std::flush;
#define DEB_TO_FILE(F,x) F<<"["<<#x<<"] = "<<x<<std::endl<<std::flush;
#define DEB_TO_FILE1(F,x) F<<x<<std::endl<<std::flush;
#define DEB_TO_FILE2(F,x,y) F<<x<<" "<<y<<std::endl<<std::flush;
#define DEB_TO_FILE3(F,x,y,z) F<<x<<" "<<y<<" "<<z<<std::endl<<std::flush;
#define DEB_TO_FILE4(F,t,x,y,z) F<<t<<" "<<x<<" "<<y<<" "<<z<<std::endl<<std::flush;
#endif // #ifdef CELLULAR_AUTOMATON_PANIC_MODE

 // =======================================================================
 /// Macros to transform from degrees to radians and vice versa
 // =======================================================================
#define TO_RADIANS (M_PI/180.0)
#define TO_DEGREES (180.0/M_PI)
 
 // =======================================================================
 /// Macros to transform from Km/h to m/s and vice versa
 // =======================================================================
#define KMH_TO_MS 0.2778
#define MS_TO_KMH 3.6
 
 // =======================================================================
 /// The macros to deal with strings for error messages are taken from
 /// oomph-lib
 // =======================================================================

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
 
#endif // #ifdef DEFINITIONS_H

