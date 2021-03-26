#ifndef CC_NASCH_NPBC_H
#define CC_NASCH_NPBC_H

#include "../general/common_includes.h"
#include "../general/cc_vehicle.h"
#include "../general/cc_bump.h"
#include "../general/cc_pedestrian.h"
#include "../crosswalk/cc_crosswalk.h"

namespace CA
{

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
  NaSchNPBC(unsigned lane_size, unsigned maximum_velocity, Real break_probability,
            Real alpha = 0.0, Real beta = 0.0);

  // ----------------------------------------------------------------
  // Destructor
  // ----------------------------------------------------------------
  ~NaSchNPBC();

  // ----------------------------------------------------------------
  // Initialise lane configuration
  // ----------------------------------------------------------------
  void initialise(unsigned lane_size, unsigned maximum_velocity, Real break_probability,
                  Real alpha = 0.0, Real beta = 0.0);

  // ----------------------------------------------------------------
  // Clear data structures
  // ----------------------------------------------------------------
  void clear();

  // ----------------------------------------------------------------
  // Set bumps
  // ----------------------------------------------------------------
  void set_bumps(std::vector<unsigned> &bumps_positions);

  // ----------------------------------------------------------------
  // Set crosswalks
  // ----------------------------------------------------------------
  //void add_crosswalk(unsigned position, unsigned crosswalk_length, unsigned crosswalk_width, unsigned maximum_speed, Real alpha = 0.0, Real go_back_prob = 0.0);
  //void add_crosswalk(Crosswalk* crosswalk_pt);
  void add_crosswalk(unsigned position, unsigned crosswalk_length, unsigned crosswalk_width, unsigned maximum_speed, Real alpha, float safety_factor, unsigned N_c, Real go_back_prob = 0.0);

  // ----------------------------------------------------------------
  // Fill in vehicles
  // ----------------------------------------------------------------
  void fill_in_vehicles(Real density);

  // ----------------------------------------------------------------
  // Update vehicles list
  // ----------------------------------------------------------------
  unsigned update_vehicles_list();

  // ----------------------------------------------------------------
  // Update lane based on NaSchNPBC rules
  // ----------------------------------------------------------------
  void apply_nasch();

  // ----------------------------------------------------------------
  // Update the lane status
  // ----------------------------------------------------------------
  void update();

  // ----------------------------------------------------------------
  // Check whether there is a bump close to the current position
  // ----------------------------------------------------------------
  unsigned distance_to_nearest_bump(unsigned position);

  // ----------------------------------------------------------------
  // Get the nearest bump relative to a given position
  // ----------------------------------------------------------------
  Crosswalk* get_the_nearest_crosswalk(unsigned position);

  // ----------------------------------------------------------------
  // Get the number of bumps
  // ----------------------------------------------------------------
  inline const unsigned nbumps() {return Bumps_pt.size();}


  // ----------------------------------------------------------------
  // Get the number of crosswalks
  // ----------------------------------------------------------------
  inline const unsigned ncrosswalks() {return Cwalks_pt.size();}

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
  inline unsigned &allowed_number_of_vehicles() {return Allowed_number_of_vehicles;}
  inline unsigned allowed_number_of_vehicles() const {return Allowed_number_of_vehicles;}
  inline unsigned &current_number_of_vehicles() {return Current_number_of_vehicles;}
  inline unsigned current_number_of_vehicles() const {return Current_number_of_vehicles;}
  inline void allow_travel_time_computation() {travel_time_computation = true;}
  inline Real &alpha() {return Alpha;}
  inline Real alpha() const {return Alpha;}
  inline Real &beta() {return Beta;}
  inline Real beta() const {return Beta;}

  inline Real density() {return Density;}
  inline Real mean_velocity() {return Mean_velocity;}
  inline Real mean_delay() {return Mean_delay;}
  inline Real mean_travel_time() {return Mean_travel_time;}
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
  Real Break_probability;

  // Using non-periodic boundary conditions requires setting Alpha and
  // Beta values, they state the entry and exit probability for the
  // first and last vehicle in the lane, respectively.
  Real Alpha; // entry probability
  Real Beta; // exit probability

  // Allowed number of vehicles
  unsigned Allowed_number_of_vehicles;
  // Current number of vehicles (less or equal than
  // "Allowed_number_of_vehicles")
  unsigned Current_number_of_vehicles;

  std::vector<Vehicle*> Lane;
  std::vector<Vehicle*> Vehicles_pt;

  // Keep track of the bumps, if any
  std::vector<CCBump*> Bumps_pt;

  std::vector<Crosswalk*> Cwalks_pt;
  unsigned Number_of_crosswalks;

  // Keep track of the number of vehicles that have leave the lane
  // (used to compute travel time)
  unsigned N_vehicles_complete_travel;
  unsigned sum_travel_time;
  bool travel_time_computation;

  /// Statistics
  Real Density;
  Real Mean_velocity;
  Real Mean_delay;
  Real Mean_travel_time;
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
