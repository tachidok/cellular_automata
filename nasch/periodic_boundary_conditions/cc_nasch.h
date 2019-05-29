#include "general.h"
#include "cc_vehicle.h"

#ifndef CC_NASCH_H
#define CC_NASCH_H
 
class NaSch
{
 
 public:
 
 // ----------------------------------------------------------------
 // Constructor -- do nothing
 // ----------------------------------------------------------------
 NaSch();
 
 // ----------------------------------------------------------------
 // Constructor
 // ----------------------------------------------------------------
 NaSch(unsigned lane_size, unsigned maximum_velocity, double break_probability);
 
 // ----------------------------------------------------------------
 // Destructor
 // ----------------------------------------------------------------
 ~NaSch();
 
 // ----------------------------------------------------------------
 // Initialise lane configuration
 // ----------------------------------------------------------------
 void initialise(unsigned lane_size, unsigned maximum_velocity, double break_probability);
 
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
 unsigned update_vehicles_list();
 
 // ----------------------------------------------------------------
 // Update lane based on NaSch rules
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
 
 inline unsigned lane_size() {return Lane_size;}
 inline unsigned maximum_velocity() {return Maximum_velocity;}
 inline double &density() {return Density;}
 inline double density() const {return Density;}
 inline unsigned &number_of_vehicles() {return Number_of_vehicles;}
 inline unsigned number_of_vehicles() const {return Number_of_vehicles;}
 
 protected:
 
 unsigned Lane_size;
 unsigned Maximum_velocity;
 double Break_probability;
 
 // Density associated with the number of vehicles in the lane
 double Density;
 unsigned Number_of_vehicles;
 
 std::vector<Vehicle*> Lane;
 std::vector<Vehicle*> Vehicles_pt;
 
};

#endif // #ifndef CC_NASCH_H

