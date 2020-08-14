#ifndef INITIALISE_H
#define INITIALISE_H

#include "common_includes.h"
#include "utilities.h"

namespace CA
{
 /// Initialisation function, everything required before using CA
 /// should be called here
 bool initialise_ca();
 
 /// Finalise function, call all finalisation methods, free memory
 /// allocated by CA
 bool finalise_ca();
 
}

#endif // #ifndef INITIALISE_H
