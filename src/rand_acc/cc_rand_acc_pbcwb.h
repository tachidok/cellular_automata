#include "../../src/general/general.h"
#include "../../src/general/cc_vehicle.h"
#include "../../src/general/cc_bump.h"
#include "cc_rand_acc_pbc.h"

#ifndef CC_RAND_ACC_PBCWB_H
#define CC_RAND_ACC_PBCWB_H

// Implements random acceleration algorithm with periodic boundary
// conditions and bumps
class RandAccPBCwb : public RandAccPBC
{
 
 public:
 
 // ----------------------------------------------------------------
 // Constructor -- do nothing
 // ----------------------------------------------------------------
 RandAccPBCwb();
 
 // ----------------------------------------------------------------
 // Constructor
 // ----------------------------------------------------------------
 RandAccPBCwb(unsigned long lane_size, unsigned maximum_velocity, double p_0, double p_1);
 
 // ----------------------------------------------------------------
 // Destructor
 // ----------------------------------------------------------------
 ~RandAccPBCwb();
 
 // ----------------------------------------------------------------
 // Clear data structures
 // ----------------------------------------------------------------
 void clear();
 
 // ----------------------------------------------------------------
 // Set bumps
 // ----------------------------------------------------------------
 void set_bumps(std::vector<unsigned long> &bumps_positions);
 
 // ----------------------------------------------------------------
 // Update lane based on RandAccPBCwb rules
 // ----------------------------------------------------------------
 unsigned long apply_rand_acc();
  
 // ----------------------------------------------------------------
 // Check whether there is a bump close to the current position
 // ----------------------------------------------------------------
 unsigned long distance_to_nearest_bump(unsigned long position);
 
 protected:
 
 std::vector<CCBump*> Bumps_pt;
 
};

#endif // #ifndef CC_RAND_ACC_PBCWB_H

