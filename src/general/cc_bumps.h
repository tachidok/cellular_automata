#include "general.h"

#ifndef CC_BUMPS_H
#define CC_BUMPS_H

class CCBumps
{
 
 public:
 
 // ----------------------------------------------------------------
 // Constructor (empty)
 // ----------------------------------------------------------------
 CCBumps();
 
 // ----------------------------------------------------------------
 // Constructor (randomly generate bumps)
 // ----------------------------------------------------------------
 CCBumps(const unsigned n_bumps, unsigned lane_size);
 
 // ----------------------------------------------------------------
 // Destructor
 // ----------------------------------------------------------------
 virtual ~CCBumps();
 
 // ----------------------------------------------------------------
 // Sets the bumps (receives a set with the position of the bumps)
 // ----------------------------------------------------------------
 void set_bumps(std::set<unsigned> &bumps);
 
 // ----------------------------------------------------------------
 // Check whether there is a bump at the given position
 // ----------------------------------------------------------------
 bool is_there_a_bump(unsigned i);
 // Get the number of bumps
 inline unsigned n_bumps() {return N_bumps;}
 
 protected:
 
 // The number of bumps
 unsigned N_bumps;
 // Position of bumps
 std::set<unsigned> Position;
 
};

#endif // #ifndef CC_BUMPS_H

