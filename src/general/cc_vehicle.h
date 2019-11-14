#ifndef CC_VEHICLE_H
#define CC_VEHICLE_H

#include "general.h"

namespace CA
{
 
 //enum Pollutant_type {CO2=0, NOx=1, VOC=2, PM=3};
 enum Pollutant_type {CO2=0, PM=1};
 
 // Based on classification of cars proposed by Luc Int Panis
 // et. al. in Luc Int Panis, Steven Broekx and Ronghui Liu, "Modelling
 // instantaneous traffic emission and the influence of traffic speed
 // limits", Science of The Total Environment, 371 (1), 270-285, 2006
 enum Vehicle_type {PETROL=0, DIESEL=1, LPG=2, HDV=3, BUS=4};
 
#define N_POLLUTANTS_TYPES 2
#define N_VEHICLES_TYPES 5
#define N_FUNCTIONS 7 // Functions of the model by Panis et. al. We
                      // set seven functions such that the first value
                      // is set to E_0
 
 class Vehicle
 {
 
 public:
 
  // ----------------------------------------------------------------
  // Constructor
  // ----------------------------------------------------------------
  Vehicle();
 
  // ----------------------------------------------------------------
  // Constructor
  // ----------------------------------------------------------------
  Vehicle(unsigned velocity, unsigned long position, unsigned length = 1, Vehicle_type Type = PETROL);
 
  // ----------------------------------------------------------------
  // Destructor
  // ----------------------------------------------------------------
  virtual ~Vehicle();
 
  // ----------------------------------------------------------------
  // Set the current velocity and position
  // ----------------------------------------------------------------
  void initialise(unsigned velocity, unsigned long position, unsigned length = 1, Vehicle_type Type = PETROL);
 
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
  // Set position
  inline unsigned &length() {return Length;}
  // Get position
  inline unsigned length() const {return Length;}
 
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
  // Length of vehicle
  unsigned Length;
  // Travel time, increases per each update call
  unsigned Travel_time;
  // Increases only when velocity = 0
  unsigned Delay;
  
  // Vehicle's type
  Vehicle_type Type;
  
  Real Emission_table_CO2[N_VEHICLES_TYPES][N_FUNCTIONS];
  Real Emission_table_PM[N_VEHICLES_TYPES][N_FUNCTIONS];
  
 };
 
} // namespace CA
 
#endif // #ifndef CC_VEHICLE_H

