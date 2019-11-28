#ifndef CC_KKW_PBC_H
#define CC_KKW_PBC_H

#include "../general/common_includes.h"
#include "../general/cc_vehicle.h"
#include "../general/cc_bump.h"

namespace CA
{
 
 enum KKW_D_Function {LINEAR=0, QUADRATIC=1};
 
 // Implements KKW model with periodic boundary conditions (bumps are
 // optional)
 class KKWPBC
 {
  
 public:
  
  // ----------------------------------------------------------------
  // Constructor -- do nothing
  // ----------------------------------------------------------------
  KKWPBC();
  
  // ----------------------------------------------------------------
  // Constructor (with its corresponding KKW parameters, default
  // values based on section 8.3.2, page 301 of Stochastic Transport
  // in Complex Systems: From Molecules to Vehicles, 2011) book and
  // Cellular Automata approach to three-phase traffic theory, Boris S
  // Kerner, Sergey L Klenov and Dietrich E Wolf, Journal of Physics
  // A: Mathematical and General, 35, 9971-10013, (2002)
  // ----------------------------------------------------------------
  KKWPBC(unsigned lane_size, unsigned maximum_velocity = 3,
         unsigned a = 1, unsigned b = 1, Real k = 2.55, Real p = 0.04,
         Real p_0 = 0.425, Real p_a1 = 0.2, Real p_a2 = 0.052, Real v_p = 28);
  
  // ----------------------------------------------------------------
  // Destructor
  // ----------------------------------------------------------------
  ~KKWPBC();
  
  // ----------------------------------------------------------------
  // Initialise lane configuration, default values based on section
  // 8.3.2, page 301 of Stochastic Transport in Complex Systems: From
  // Molecules to Vehicles, 2011) book and Cellular Automata approach
  // to three-phase traffic theory, Boris S Kerner, Sergey L Klenov
  // and Dietrich E Wolf, Journal of Physics A: Mathematical and
  // General, 35, 9971-10013, (2002)
  // ----------------------------------------------------------------
  void initialise(unsigned lane_size, unsigned maximum_velocity = 3,
                  unsigned a = 1, unsigned b = 1, Real k = 2.55, Real p = 0.04,
                  Real p_0 = 0.425, Real p_a1 = 0.2, Real p_a2 = 0.052, Real v_p = 28);

  // ----------------------------------------------------------------
  // Set bumps
  // ----------------------------------------------------------------
  void set_bumps(std::vector<unsigned> &bumps_positions);
  
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
  // Update lane based on KKWPBC rules
  // ----------------------------------------------------------------
  void apply_kkw(Real &mean_velocity, Real &mean_current, Real &mean_delay,
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
  inline Real &density() {return Density;}
  inline Real density() const {return Density;}
  inline const unsigned nbumps() {return Bumps_pt.size();} 
  inline unsigned current_number_of_vehicles() const {return Current_number_of_vehicles;}
  inline unsigned nvehicles_complete_travel() const {return N_vehicles_complete_travel;}
  
  inline void reset_n_vehicles_complete_travel() {N_vehicles_complete_travel = 0;}
  
  // Change the syncronisation function
  inline void set_linear_syncronisation_function() {kkw_d_function = KKW_D_Function::LINEAR;}
  inline void set_quadratic_syncronisation_function() {kkw_d_function = KKW_D_Function::QUADRATIC;}
  
 protected:
  
  // Check whether there is a bump close to the current position 
  unsigned distance_to_nearest_bump(unsigned position);
  
  // Synchronization distance, it may implement a linear or a
  // quatratic function. Requires current velocity and vehicles
  // length
  Real D(const unsigned v, const unsigned l);
  
  unsigned Lane_size;
  unsigned Maximum_velocity;
  
  // KKW parameters
  Real A;
  Real B;
  Real K;
  Real P;
  Real P_0;
  Real P_a1;
  Real P_a2;
  Real V_p;
  
  // Density associated with the number of vehicles in the lane
  Real Density;
  
  // Current number of vehicles (less or equal than the Lane size)
  unsigned Current_number_of_vehicles; 
 
  std::vector<Vehicle*> Lane;
  std::vector<Vehicle*> Vehicles_pt;
  
  // The type of syncronisation D function
  KKW_D_Function kkw_d_function;
  
  // Keep track of the number of vehicles that have leave the lane
  // (used to compute travel time)
  unsigned N_vehicles_complete_travel;
  
  // Store the bumps
  std::vector<CCBump*> Bumps_pt;
  
 };
 
} // namespace CA
 
#endif // #ifndef CC_KKW_PBC_H
