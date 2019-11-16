#ifndef CC_VEHICLE_H
#define CC_VEHICLE_H

#include "general.h"

namespace CA
{
 
 //enum Pollutant_type {CO2=0, NOx=1, VOC=2, PM=3};
 enum Pollutant_type {CO2=0, NOx=1, VOC=2, PM=3};
 
 // Based on classification of cars proposed by Luc Int Panis
 // et. al. in Luc Int Panis, Steven Broekx and Ronghui Liu, "Modelling
 // instantaneous traffic emission and the influence of traffic speed
 // limits", Science of The Total Environment, 371 (1), 270-285, 2006
 enum Vehicle_type {PETROL=0, DIESEL=1, LPG=2, HDV=3, BUS=4};
 
#define N_POLLUTANTS_TYPES 4
#define N_VEHICLES_TYPES 5
#define N_FUNCTIONS 6 // Functions of the model by Panis et. al.

#define TYPICAL_VEHICLES_OCCUPANCY 1
#define TYPICAL_VEHICLES_LENGTH 5.0
 
 class Vehicle
 {
 
 public:
 
  // ----------------------------------------------------------------
  // Constructor
  // ----------------------------------------------------------------
  Vehicle();
  
  // ----------------------------------------------------------------
  // Constructor (the occupancy considers the number of cells the
  // vehicle occupies in the Cellular Automata, the length is the car
  // size in meters)
  // ----------------------------------------------------------------
  Vehicle(unsigned velocity, unsigned long position, unsigned occupancy = TYPICAL_VEHICLES_OCCUPANCY,
          Real length = TYPICAL_VEHICLES_LENGTH, Vehicle_type Type = PETROL);
  
  // ----------------------------------------------------------------
  // Destructor
  // ----------------------------------------------------------------
  virtual ~Vehicle();
 
  // ----------------------------------------------------------------
  // Set the current velocity and position (the occupancy considers
  // the number of cells the vehicle occupies in the Cellular
  // Automata, the length is the car size in meters)
  // ----------------------------------------------------------------
  void initialise(unsigned velocity, unsigned long position, unsigned occupancy = TYPICAL_VEHICLES_OCCUPANCY,
                  Real length = TYPICAL_VEHICLES_LENGTH, Vehicle_type Type = PETROL);
  
  // ----------------------------------------------------------------
  // Update vehicles status
  // ----------------------------------------------------------------
  void update();
 
  // Set velocity
  inline unsigned &velocity(unsigned i = 0) {return Velocity[i];}
  // Get velocity
  inline unsigned velocity(unsigned i = 0) const {return Velocity[i];}
  // Set position
  inline unsigned long &position(unsigned long i = 0) {return Position[i];}
  // Get position
  inline unsigned long position(unsigned long i = 0) const {return Position[i];}
  // Set occupancy
  inline unsigned &occupancy() {return Occupancy;}
  // Get occupancy
  inline unsigned occupancy() const {return Occupancy;}
  // Set length
  inline Real &length() {return Length;}
  // Get length
  inline Real length() const {return Length;}
  
  // Get length/occupancy ratio
  inline Real eta() const {return Eta;}
  
  // Set travel_time
  inline unsigned &travel_time() {return Travel_time;}
  // Get position
  inline unsigned travel_time() const {return Travel_time;}
  // Set delay
  inline unsigned &delay() {return Delay;}
  // Get delay
  inline unsigned delay() const {return Delay;}
 
  // Set delay
  inline Vehicle_type &type() {return Type;}
  // Get delay
  inline Vehicle_type type() const {return Type;}
  
  // In charge of computing all emissions and transform 
  Real compute_emissions(Real &CO2, Real &NOx, Real &VOC, Real &PM);
  
 protected:
 
  // Update statistics
  void update_statistics();
  
  // In charge of setting the initial and constant values to the
  // emission table
  void initialise_emission_tables();
  
  // Velocity (index 0 is current time, other index is at time i-th)
  unsigned Velocity[2];
  // Position (index 0 is current time, other index is at time i-th)
  unsigned long Position[2];
  // Occupancy, number of cells the vehicle uses in the CA
  unsigned Occupancy;
  // Length of vehicle, in meters
  Real Length;
  // Travel time, increases per each update call
  unsigned Travel_time;
  // Increases only when velocity = 0
  unsigned Delay;
  
  // Vehicle's type
  Vehicle_type Type;
  
  // Length/occupancy ratio
  Real Eta;
  
  // Based on classification of cars proposed by Luc Int Panis
  // et. al. in Luc Int Panis, Steven Broekx and Ronghui Liu,
  // "Modelling instantaneous traffic emission and the influence of
  // traffic speed limits", Science of The Total Environment, 371 (1),
  // 270-285, 2006
  Real Emission_table_CO2[N_VEHICLES_TYPES][N_FUNCTIONS+1];
  // The plus three is to store the values for different
  // accelerations, and the plus one is to store the value of E_0
  Real Emission_table_NOx[N_VEHICLES_TYPES+3][N_FUNCTIONS+1];
  Real Emission_table_VOC[N_VEHICLES_TYPES+3][N_FUNCTIONS+1];
  Real Emission_table_PM[N_VEHICLES_TYPES][N_FUNCTIONS+1];
  
  // Acceleration and velocity convertions factors, these values
  // depend on the number of meters represented by each cell
  Real Velocity_factor;
  Real Acceleration_factor;
  
 };
 
} // namespace CA

#endif // #ifndef CC_VEHICLE_H

