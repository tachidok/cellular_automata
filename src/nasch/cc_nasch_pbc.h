#ifndef CC_NASCH_PBC_H
#define CC_NASCH_PBC_H

#include "../general/common_includes.h"
#include "../general/cc_vehicle.h"
#include "../general/cc_bump.h"

namespace CA
{

 /// Implements NaSch algorithm with periodic boundary conditions
 class CCNaSchPBC
 {
 
 public:
 
  // ----------------------------------------------------------------
  /// Constructor -- do nothing
  // ----------------------------------------------------------------
  CCNaSchPBC();
 
  // ----------------------------------------------------------------
  /// Constructor
  // ----------------------------------------------------------------
  CCNaSchPBC(unsigned lane_size, unsigned maximum_velocity, Real break_probability);
 
  // ----------------------------------------------------------------
  /// Destructor
  // ----------------------------------------------------------------
  ~CCNaSchPBC();
 
  // ----------------------------------------------------------------
  /// Initialise lane configuration
  // ----------------------------------------------------------------
  void initialise(unsigned lane_size, unsigned maximum_velocity, Real break_probability);
 
  // ----------------------------------------------------------------
  /// Clear data structures
  // ----------------------------------------------------------------
  void clear();

  // ----------------------------------------------------------------
  /// Set bumps
  // ----------------------------------------------------------------
  void set_bumps(std::vector<unsigned> &bumps_positions);
  
  // ----------------------------------------------------------------
  /// Fill in vehicles
  // ----------------------------------------------------------------
  void fill_in_vehicles(Real density);
 
  // ----------------------------------------------------------------
  /// Update vehicles list
  // ----------------------------------------------------------------
  unsigned update_vehicles_list();
 
  // ----------------------------------------------------------------
  /// Update lane based on CCNaSchPBC rules
  // ----------------------------------------------------------------
  void apply_nasch();
  
  // ----------------------------------------------------------------
  /// Update the lane status
  // ---------------------------------------------------------------- 
  void update();
  
  // ----------------------------------------------------------------
  /// Check whether there is a bump close to the current position
  // ----------------------------------------------------------------
  unsigned distance_to_nearest_bump(unsigned position);
  
  // ----------------------------------------------------------------
  /// Get the number of bumps
  // ----------------------------------------------------------------
  inline const unsigned nbumps() {return Bumps_pt.size();}
  
  // ----------------------------------------------------------------
  /// Prints the lane status
  // ---------------------------------------------------------------- 
  void print(bool print_velocities = false);
 
  // ----------------------------------------------------------------
  /// Output lane status
  // ---------------------------------------------------------------- 
  void output_time_space(std::ofstream &output_file);

  /// Get the lane size
  inline unsigned lane_size() {return Lane_size;}

  /// Get the maximum velocity
  inline unsigned maximum_velocity() const {return Maximum_velocity;}
  /// Set the maximum velocity
  inline unsigned &maximum_velocity() {return Maximum_velocity;}

  /// Set the density
  inline Real &density() {return Density;}
  
  /// Get the density
  inline Real density() const {return Density;}

  /// Get the current number of vehicles on the lane
  inline unsigned current_number_of_vehicles() const {return Current_number_of_vehicles;}
  
  inline Real mean_velocity() {return Mean_velocity;}
  inline Real mean_current() {return Mean_current;}
  inline Real mean_co2() {return Mean_co2;}
  inline Real mean_nox() {return Mean_nox;}
  inline Real mean_voc() {return Mean_voc;}
  inline Real mean_pm() {return Mean_pm;}
  inline Real std_velocity() {return Std_velocity;}
  inline Real std_co2() {return Std_co2;}
  inline Real std_nox() {return Std_nox;}
  inline Real std_voc() {return Std_voc;}
  inline Real std_pm() {return Std_pm;}
  
 protected:
  
  /// Set all statistics values to zero
  void reset_statistics();
  
  /// The size of the lane (in number of cells)
  unsigned Lane_size;

  /// Maximum velocity (in number of cells per unit time)
  unsigned Maximum_velocity;
  
  /// Braking probability
  Real Braking_probability;
  
  /// Density associated with the number of vehicles in the lane
  Real Density;
  
  /// Current number of vehicles (less or equal than the Lane size)
  unsigned Current_number_of_vehicles; 
  
  /// Keep track of the lane status and vehicles
  std::vector<CCVehicle*> Lane;
  std::vector<CCVehicle*> Vehicles_pt;
  
  /// Keep track of the bumps, if any
  std::vector<CCBump*> Bumps_pt;
  
  /// Statistics
  Real Mean_velocity;
  Real Mean_current;
  Real Mean_co2;
  Real Mean_nox;
  Real Mean_voc;
  Real Mean_pm;
  Real Std_velocity;
  Real Std_co2;
  Real Std_nox;
  Real Std_voc;
  Real Std_pm;
  
 };
 
} // namespace CA

#endif // #ifndef CC_NASCH_PBC_H

