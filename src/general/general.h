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
 
#ifdef TYPEDEF_REAL_IS_DOUBLE
 typedef double Real;
#else
 typedef float Real;
#endif // #ifdef TYPEDEF_REAL_IS_DOUBLE

} // namespace CA

#endif // #ifndef GENERAL_H
