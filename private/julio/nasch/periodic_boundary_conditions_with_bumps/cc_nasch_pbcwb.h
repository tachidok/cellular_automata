#include "../../../../src/general/general.h"
#include "../../../../src/general/cc_vehicle.h"
#include "../../../../src/general/cc_bump.h"
#include "../../../../src/nasch/cc_nasch_pbc.h"

#ifndef CC_NASCH_PBCWB_H
#define CC_NASCH_PBCWB_H

// Implements Nasch model with periodic boundary conditions and bumps
class NaSchPBCwb : public NaSchPBC
{
 
 public:
 
 // ----------------------------------------------------------------
 // Constructor -- do nothing
 // ----------------------------------------------------------------
 NaSchPBCwb();
 
 // ----------------------------------------------------------------
 // Constructor
 // ----------------------------------------------------------------
 NaSchPBCwb(unsigned long lane_size, unsigned maximum_velocity, double break_probability);
 
 // ----------------------------------------------------------------
 // Destructor
 // ----------------------------------------------------------------
 ~NaSchPBCwb();
 
 // ----------------------------------------------------------------
 // Clear data structures
 // ----------------------------------------------------------------
 void clear();
 
 // ----------------------------------------------------------------
 // Set bumps
 // ----------------------------------------------------------------
 void set_bumps(std::vector<unsigned long> &bumps_positions);
 
 // ----------------------------------------------------------------
 // Update lane based on NaSchPBCwb rules
 // ----------------------------------------------------------------
 unsigned long apply_nasch();
  
 // ----------------------------------------------------------------
 // Check whether there is a bump close to the current position
 // ----------------------------------------------------------------
 unsigned long distance_to_nearest_bump(unsigned long position);

 inline const unsigned nbumps() {return Bumps_pt.size();}
 
 protected:
 
 std::vector<CCBump*> Bumps_pt;
 
};

#endif // #ifndef CC_NASCH_PBCWB_H

