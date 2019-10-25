#include "cc_bumps.h"

// ----------------------------------------------------------------
// Constructor (empty)
// ----------------------------------------------------------------
CCBumps::CCBumps()
{
 
}

// ----------------------------------------------------------------
// Constructor (randomly generate bumps)
// ----------------------------------------------------------------
CCBumps::CCBumps(const unsigned n_bumps, unsigned lane_size)
{
 // Iterate until creating the desired number of bumps
 unsigned i = 0;
 while (i < n_bumps)
  {
   // Random position of bump
   const double r = rand();
   unsigned k = (r / RAND_MAX) * lane_size;
   // Check whether there is a vehicle in the k lane position
   std::set<unsigned>::iterator it = Position.find(k);
   if (it == Position.end())
    {
     // Create a new bump
     Position.insert(i);
     // Increase the number of bumps added to the lane
     i++;
    }
  }
 
}

// ----------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------
CCBumps::~CCBumps()
{
  
}

// ----------------------------------------------------------------
// Sets the bumps (receives a set with the position of the bumps)
// ----------------------------------------------------------------
void CCBumps::set_bumps(std::set<unsigned> &bumps)
{
 Position = bumps;
}

// ----------------------------------------------------------------
// Check whether there is a bump at the given position
// ----------------------------------------------------------------
bool CCBumps::is_there_a_bump(unsigned i)
{
 std::set<unsigned>::iterator it;
 it = Position.find(i);
 if (it != Position.end())
  {
   return true;
  }
 return false;
}
 
