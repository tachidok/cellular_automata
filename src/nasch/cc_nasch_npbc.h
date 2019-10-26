#include "../../src/general/general.h"
#include "../../src/general/cc_vehicle.h"

#ifndef CC_NASCH_NPBC_H
#define CC_NASCH_NPBC_H

// Implements NaSch algorithm with non periodic boundary conditions
class NaSchNPBC
{
 
 public:
 
 // ----------------------------------------------------------------
 // Constructor -- do nothing
 // ----------------------------------------------------------------
 NaSchNPBC();
 
 // ----------------------------------------------------------------
 // Constructor
 // ----------------------------------------------------------------
 NaSchNPBC(unsigned lane_size, unsigned maximum_velocity, double break_probability,
           double alpha = 0.0, double beta = 0.0);
 
 // ----------------------------------------------------------------
 // Destructor
 // ----------------------------------------------------------------
 ~NaSchNPBC();
 
 // ----------------------------------------------------------------
 // Initialise lane configuration
 // ----------------------------------------------------------------
 void initialise(unsigned lane_size, unsigned maximum_velocity, double break_probability,
                 double alpha = 0.0, double beta = 0.0);
 
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
 // Update lane based on NaSchNPBC rules
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
 
 // Computes the density
 double density();
 
 inline unsigned lane_size() {return Lane_size;}
 inline unsigned maximum_velocity() {return Maximum_velocity;}
 inline unsigned &allowed_number_of_vehicles() {return Allowed_number_of_vehicles;}
 inline unsigned allowed_number_of_vehicles() const {return Allowed_number_of_vehicles;}
 inline unsigned &current_number_of_vehicles() {return Current_number_of_vehicles;}
 inline unsigned current_number_of_vehicles() const {return Current_number_of_vehicles;}
 inline double &alpha() {return Alpha;}
 inline double alpha() const {return Alpha;}
 inline double &beta() {return Beta;}
 inline double beta() const {return Beta;}
 
 protected:
 
 unsigned Lane_size;
 unsigned Maximum_velocity;
 double Break_probability;

 // Using non-periodic boundary conditions requires setting Alpha and
 // Beta values, they state the entry and exit probability for the
 // first and last vehicle in the lane, respectively.
 double Alpha; // entry probability
 double Beta; // exit probability
 
 // Allowed number of vehicles
 unsigned Allowed_number_of_vehicles;
 // Current number of vehicles (less or equal than
 // "Allowed_number_of_vehicles")
 unsigned Current_number_of_vehicles;
 
 std::vector<Vehicle*> Lane;
 std::vector<Vehicle*> Vehicles_pt;
 
};

#endif // #ifndef CC_NASCH_NPBC_H

