#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <random>
#include <set>

#ifndef GENERAL_H
#define GENERAL_H

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

} // namespace CA

#endif // #ifndef GENERAL_H
