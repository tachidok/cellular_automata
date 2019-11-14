#ifndef CC_NASCH_PBC_H
#define CC_NASCH_PBC_H

#include "../../src/general/general.h"
#include "../../src/general/cc_vehicle.h"

namespace CA
{

 // Implements NaSch algorithm with periodic boundary conditions
 class NaSchPBC
 {
 
 public:
 
  // ----------------------------------------------------------------
  // Constructor -- do nothing
  // ----------------------------------------------------------------
  NaSchPBC();
 
  // ----------------------------------------------------------------
  // Constructor
  // ----------------------------------------------------------------
  NaSchPBC(unsigned long lane_size, unsigned maximum_velocity, double break_probability);
 
  // ----------------------------------------------------------------
  // Destructor
  // ----------------------------------------------------------------
  ~NaSchPBC();
 
  // ----------------------------------------------------------------
  // Initialise lane configuration
  // ----------------------------------------------------------------
  void initialise(unsigned long lane_size, unsigned maximum_velocity, double break_probability);
 
  // ----------------------------------------------------------------
  // Clear data structures
  // ----------------------------------------------------------------
  void clear();
 
  // ----------------------------------------------------------------
  // Fill in vehicles
  // ----------------------------------------------------------------
  void fill_in_vehicles(double density);
 
  // ----------------------------------------------------------------
  // Update vehicles list
  // ----------------------------------------------------------------
  unsigned long update_vehicles_list();
 
  // ----------------------------------------------------------------
  // Update lane based on NaSchPBC rules
  // ----------------------------------------------------------------
  unsigned apply_nasch();
 
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
 
  inline unsigned long lane_size() {return Lane_size;}
  inline unsigned maximum_velocity() {return Maximum_velocity;}
  inline double &density() {return Density;}
  inline double density() const {return Density;}
  inline unsigned long &number_of_vehicles() {return Number_of_vehicles;}
  inline unsigned long number_of_vehicles() const {return Number_of_vehicles;}
 
 protected:
 
  unsigned long Lane_size;
  unsigned Maximum_velocity;
  double Break_probability;
 
  // Density associated with the number of vehicles in the lane
  double Density;
  unsigned long Number_of_vehicles;
 
  std::vector<Vehicle*> Lane;
  std::vector<Vehicle*> Vehicles_pt;
 
 };
 
} // namespace CA

#endif // #ifndef CC_NASCH_PBC_H

