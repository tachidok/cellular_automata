#ifndef CC_RAND_ACC_PBC_H
#define CC_RAND_ACC_PBC_H

#include "../general/common_includes.h"
#include "../general/cc_vehicle.h"

namespace CA
{

 // Implements random acceleration algorithm with periodic boundary conditions
 class RandAccPBC
 {
 
 public:
 
  // ----------------------------------------------------------------
  // Constructor -- do nothing
  // ----------------------------------------------------------------
  RandAccPBC();
 
  // ----------------------------------------------------------------
  // Constructor
  // ----------------------------------------------------------------
  RandAccPBC(unsigned lane_size, unsigned maximum_velocity, Real p_0, Real p_1);
 
  // ----------------------------------------------------------------
  // Destructor
  // ----------------------------------------------------------------
  ~RandAccPBC();
 
  // ----------------------------------------------------------------
  // Initialise lane configuration
  // ----------------------------------------------------------------
  void initialise(unsigned lane_size, unsigned maximum_velocity, Real p_0, Real p_1);
 
  // ----------------------------------------------------------------
  // Clear data structures
  // ----------------------------------------------------------------
  void clear();
 
  // ----------------------------------------------------------------
  // Fill in vehicles
  // ----------------------------------------------------------------
  void fill_in_vehicles(Real density);
 
  // ----------------------------------------------------------------
  // Update vehicles list
  // ----------------------------------------------------------------
  unsigned update_vehicles_list();
 
  // ----------------------------------------------------------------
  // Update lane based on RandAccPBC rules
  // ----------------------------------------------------------------
  void apply_rand_acc(Real &mean_velocity, Real &mean_current, Real &mean_delay,
                      unsigned &sum_travel_time, Real &mean_travel_time,
                      Real &mean_queue_length,
                      Real &mean_co2, Real &mean_nox, Real &mean_voc, Real &mean_pm);
  
  // ----------------------------------------------------------------
  // Update the lane status
  // ---------------------------------------------------------------- 
  void update();
 
  // ----------------------------------------------------------------
  // Prints the lane status
  // ---------------------------------------------------------------- 
  void print(bool print_velocities = false);
  
  // ----------------------------------------------------------------
  // Output lane status
  // ---------------------------------------------------------------- 
  void output_time_space(std::ofstream &output_file);
 
  inline unsigned lane_size() {return Lane_size;}
  inline unsigned maximum_velocity() {return Maximum_velocity;}
  inline Real density() const {return Density;}
  inline unsigned current_number_of_vehicles() const {return Current_number_of_vehicles;}
  inline unsigned nvehicles_complete_travel() const {return N_vehicles_complete_travel;}
  
  inline void reset_n_vehicles_complete_travel() {N_vehicles_complete_travel = 0;}
  
 protected:
  
  unsigned Lane_size;
  unsigned Maximum_velocity;
  Real P_0;
  Real P_1;
  
  // Density associated with the number of vehicles in the lane
  Real Density;
 
  // Current number of vehicles (less or equal than the Lane size)
  unsigned Current_number_of_vehicles;
 
  std::vector<Vehicle*> Lane;
  std::vector<Vehicle*> Vehicles_pt; 
 
  // Keep track of the number of vehicles that have leave the lane
  // (used to compute travel time)
  unsigned N_vehicles_complete_travel;
 
 };
 
} // namespace CA 
 
#endif // #ifndef CC_RAND_ACC_PBC_H

