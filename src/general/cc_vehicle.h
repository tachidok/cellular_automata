#ifndef CC_VEHICLE_H
#define CC_VEHICLE_H

#include "ac_agent.h"

namespace CA
{
 
 //enum Pollutant_type {CO2=0, NOx=1, VOC=2, PM=3};
 enum Pollutant_type {CO2=0, NOx=1, VOC=2, PM=3};
 
 /// Based on classification of cars proposed by Luc Int Panis
 /// et. al. in Luc Int Panis, Steven Broekx and Ronghui Liu, "Modelling
 /// instantaneous traffic emission and the influence of traffic speed
 /// limits", Science of The Total Environment, 371 (1), 270-285, 2006
 enum Vehicle_type {PETROL=0, DIESEL=1, LPG=2, HDV=3, BUS=4};
 
#define N_POLLUTANTS_TYPES 4
#define N_VEHICLES_TYPES 5
#define N_FUNCTIONS 6 // Functions of the model by Panis et. al.

#define TYPICAL_VEHICLES_OCCUPANCY 1
#define TYPICAL_VEHICLES_LENGTH 7.5
 
 /// Implements the vehicle type
 class CCVehicle : virtual public ACAgent
 {
 
 public:
 
  // ----------------------------------------------------------------
  /// Constructor
  // ----------------------------------------------------------------
  CCVehicle();
  
  // ----------------------------------------------------------------
  /// Constructor (the occupancy considers the number of cells the
  /// vehicle occupies in the Cellular Automata, the length is the car
  /// size in meters)
  // ----------------------------------------------------------------
  CCVehicle(unsigned velocity, unsigned position, unsigned occupancy = TYPICAL_VEHICLES_OCCUPANCY,
          Real length = TYPICAL_VEHICLES_LENGTH, Vehicle_type Type = PETROL);
  
  // ----------------------------------------------------------------
  /// Destructor
  // ----------------------------------------------------------------
  virtual ~CCVehicle();
  
  // ----------------------------------------------------------------
  /// Set the current velocity (the occupancy considers the number of
  /// cells the vehicle occupies in the Cellular Automata, the length
  /// is the car size in meters)
  //----------------------------------------------------------------
  void initialise(unsigned velocity, unsigned occupancy = TYPICAL_VEHICLES_OCCUPANCY,
                  Real length = TYPICAL_VEHICLES_LENGTH, Vehicle_type Type = PETROL);
  
  // ----------------------------------------------------------------
  /// Update vehicles status
  // ----------------------------------------------------------------
  void update();
 
  /// Set velocity
  inline unsigned &velocity(unsigned i = 0) {return Velocity[i];}
  /// Get velocity
  inline unsigned velocity(unsigned i = 0) const {return Velocity[i];}
  /// Compute velocity in meters per second
  inline Real velocity_in_ms_per_second(unsigned i = 0) const {return Velocity[i] * Eta;}
  /// Compute velocity kilometers per hour
  inline Real velocity_in_km_per_hour(unsigned i = 0) const {return Velocity[i] * Eta * MS_TO_KMH;}
  /// Set occupancy (in number of cells on the lane)
  inline unsigned &occupancy() {return Occupancy;}
  /// Get occupancy (in number of cells on the lane)
  inline unsigned occupancy() const {return Occupancy;}
  /// Set length
  inline Real &length() {return Length;}
  /// Get length
  inline Real length() const {return Length;}
  /// Get length/occupancy ratio
  inline Real eta() const {return Eta;}
  /// Get vehicle type
  inline Vehicle_type &type() {return Type;}
  /// Set vehicle type
  inline Vehicle_type type() const {return Type;}
  
  /// In charge of computing all emissions and transform 
  void compute_emissions(Real &CO2, Real &NOx, Real &VOC, Real &PM);
  
 protected:
 
  /// Update statistics
  void update_statistics();
  
  /// In charge of setting the initial and constant values to the
  /// emission table
  void initialise_emission_tables();
  
  /// Velocity (index 0 is current time, other index is at time i-th)
  unsigned Velocity[2];
  
  /// Occupancy, number of cells the vehicle uses in the CA
  unsigned Occupancy;
  /// Length of vehicle, in meters
  Real Length;
  
  /// Vehicle's type
  Vehicle_type Type;
  
  /// Length/occupancy ratio
  Real Eta;
  
  /// Based on classification of cars proposed by Luc Int Panis
  /// et. al. in Luc Int Panis, Steven Broekx and Ronghui Liu,
  /// "Modelling instantaneous traffic emission and the influence of
  /// traffic speed limits", Science of The Total Environment, 371 (1),
  /// 270-285, 2006
  Real Emission_table_CO2[N_VEHICLES_TYPES][N_FUNCTIONS+1];
  /// The plus three is to store the values for different
  /// accelerations, and the plus one is to store the value of E_0
  Real Emission_table_NOx[N_VEHICLES_TYPES+3][N_FUNCTIONS+1];
  Real Emission_table_VOC[N_VEHICLES_TYPES+3][N_FUNCTIONS+1];
  Real Emission_table_PM[N_VEHICLES_TYPES][N_FUNCTIONS+1];
  
  /// Acceleration and velocity convertions factors, these values
  /// depend on the number of meters represented by each cell
  Real Velocity_factor;
  Real Acceleration_factor;
  
 };
 
} // namespace CA

#endif // #ifndef CC_VEHICLE_H

