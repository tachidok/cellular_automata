#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <random>
#include <set>

namespace CA
{
#define DEB(x)  std::cout << "[" << #x << "]: "  << x << std::endl
 
#ifdef TYPEDEF_REAL_IS_DOUBLE
 typedef double Real;
#else
 typedef float Real;
#endif // #ifdef TYPEDEF_REAL_IS_DOUBLE
 
} // namespace CA
