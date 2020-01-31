#ifndef CC_NASCH_NPBC_H
#define CC_NASCH_NPBC_H

#include "../general/common_includes.h"
#include "../general/cc_vehicle.h"

namespace CA
{

 /// Implements NaSch algorithm with non periodic boundary conditions
 class CCNaSchNPBC
 {
 
 public:
 
  // ----------------------------------------------------------------
  /// Constructor -- do nothing
  // ----------------------------------------------------------------
  CCNaSchNPBC();
 
  // ----------------------------------------------------------------
  /// Constructor
  // ----------------------------------------------------------------
  CCNaSchNPBC(unsigned lane_size, unsigned maximum_velocity, double break_probability,
              double alpha = 0.0, double beta = 0.0);
 
  // ----------------------------------------------------------------
  /// Destructor
  // ----------------------------------------------------------------
  ~CCNaSchNPBC();
 
  // ----------------------------------------------------------------
  /// Initialise lane configuration
  // ----------------------------------------------------------------
  void initialise(unsigned lane_size, unsigned maximum_velocity, double break_probability,
                  double alpha = 0.0, double beta = 0.0);
 
  // ----------------------------------------------------------------
  /// Clear data structures
  // ----------------------------------------------------------------
  void clear();
 
  // ----------------------------------------------------------------
  /// Fill in vehicles
  // ----------------------------------------------------------------
  void fill_in_vehicles(double density);
 
  // ----------------------------------------------------------------
  /// Update vehicles list
  // ----------------------------------------------------------------
  unsigned update_vehicles_list();
  
  // ----------------------------------------------------------------
  /// Set all statistics values to zero
  // ----------------------------------------------------------------
  void reset_statistics();
  
  // ----------------------------------------------------------------
  /// Update lane based on CCNaSchNPBC rules
  // ----------------------------------------------------------------
  unsigned apply_nasch();
 
  // ----------------------------------------------------------------
  /// Update the lane status
  // ---------------------------------------------------------------- 
  void update();
 
  // ----------------------------------------------------------------
  /// Prints the lane status
  // ---------------------------------------------------------------- 
  void print(bool print_velocities = false);
 
  /// Computes the density
  double density();

  /// Gets the size of the lane (in number of cells)
  inline unsigned lane_size() {return Lane_size;}
  
  /// Sets the maximum velocity (in number of cells per unit time)
  inline unsigned &maximum_velocity() {return Maximum_velocity;}
  
  /// Gets the maximum velocity (in number of cells per unit time)
  inline unsigned maximum_velocity() const {return Maximum_velocity;}
  
  /// The maximum number of vehicles
  inline unsigned &allowed_number_of_vehicles() {return Allowed_number_of_vehicles;}
  inline unsigned allowed_number_of_vehicles() const {return Allowed_number_of_vehicles;}
  inline unsigned &current_number_of_vehicles() {return Current_number_of_vehicles;}
  inline unsigned current_number_of_vehicles() const {return Current_number_of_vehicles;}
  inline double &alpha() {return Alpha;}
  inline double alpha() const {return Alpha;}
  inline double &beta() {return Beta;}
  inline double beta() const {return Beta;}
  
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

  /// The size of the lane
  unsigned Lane_size;
  /// The maximum velocity
  unsigned Maximum_velocity;
  /// Braking probability
  double Break_probability;
  
  /// Using non-periodic boundary conditions requires setting Alpha and
  /// Beta values, they state the entry and exit probability for the
  /// first and last vehicle in the lane, respectively.
  double Alpha; // entry probability
  double Beta; // exit probability
 
  /// Allowed number of vehicles
  unsigned Allowed_number_of_vehicles;
  /// Current number of vehicles (less or equal than
  /// "Allowed_number_of_vehicles")
  unsigned Current_number_of_vehicles;

  /// The lane
  std::vector<CCVehicle*> Lane;
  /// A vector with pointers to the vehicles on the lane
  std::vector<CCVehicle*> Vehicles_pt;
  
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
 
#endif // #ifndef CC_NASCH_NPBC_H

