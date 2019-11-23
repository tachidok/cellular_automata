#ifndef CC_NASCH_PBCWB_H
#define CC_NASCH_PBCWB_H

#include "../../../../src/general/general.h"
#include "../../../../src/general/cc_vehicle.h"
#include "../../../../src/general/cc_bump.h"
#include "../../../../src/nasch/cc_nasch_pbc.h"

namespace CA
{

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
  NaSchPBCwb(unsigned lane_size, unsigned maximum_velocity, Real break_probability);
 
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
  void set_bumps(std::vector<unsigned> &bumps_positions);
 
  // ----------------------------------------------------------------
  // Update lane based on NaSchPBCwb rules
  // ----------------------------------------------------------------
  void apply_nasch(Real &mean_velocity, Real &mean_current, Real &mean_delay,
                   unsigned &sum_travel_time, Real &mean_travel_time,
                   Real &mean_queue_length,
                   Real &mean_co2, Real &mean_nox, Real &mean_voc, Real &mean_pm);
  
  // ----------------------------------------------------------------
  // Check whether there is a bump close to the current position
  // ----------------------------------------------------------------
  unsigned distance_to_nearest_bump(unsigned position);

  inline const unsigned nbumps() {return Bumps_pt.size();}
 
 protected:
 
  std::vector<CCBump*> Bumps_pt;
 
 };
 
} // namespace CA
 
#endif // #ifndef CC_NASCH_PBCWB_H

