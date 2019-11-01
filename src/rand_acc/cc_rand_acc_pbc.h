#include "../../src/general/general.h"
#include "../../src/general/cc_vehicle.h"

#ifndef CC_RAND_ACC_PBC_H
#define CC_RAND_ACC_PBC_H

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
 RandAccPBC(unsigned long lane_size, unsigned maximum_velocity, double p_0, double p_1);
 
 // ----------------------------------------------------------------
 // Destructor
 // ----------------------------------------------------------------
 ~RandAccPBC();
 
 // ----------------------------------------------------------------
 // Initialise lane configuration
 // ----------------------------------------------------------------
 void initialise(unsigned long lane_size, unsigned maximum_velocity, double p_0, double p_1);
 
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
 // Update lane based on RandAccPBC rules
 // ----------------------------------------------------------------
 unsigned long apply_rand_acc();
 
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
 inline unsigned long &current_number_of_vehicles() {return Current_number_of_vehicles;}
 inline unsigned long current_number_of_vehicles() const {return Current_number_of_vehicles;}
 
 protected:
 
 unsigned long Lane_size;
 unsigned Maximum_velocity;
 double P_0;
 double P_1;
 
 // Density associated with the number of vehicles in the lane
 double Density;
 
 // Current number of vehicles (less or equal than the Lane size)
 unsigned long Current_number_of_vehicles;
 
 std::vector<Vehicle*> Lane;
 std::vector<Vehicle*> Vehicles_pt;
 
};

#endif // #ifndef CC_RAND_ACC_PBC_H

