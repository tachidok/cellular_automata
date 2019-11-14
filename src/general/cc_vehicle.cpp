#include "cc_vehicle.h"

namespace CA
{

 // ----------------------------------------------------------------
 // Constructor
 // ----------------------------------------------------------------
 Vehicle::Vehicle()
 {
  // Set initial velocity, position and length
  initialise(0, 0, 1);
 }

 // ----------------------------------------------------------------
 // Constructor
 // ----------------------------------------------------------------
 Vehicle::Vehicle(unsigned velocity, unsigned long position, unsigned length)
 {
  // Set initial velocity, position and length
  initialise(velocity, position, length);
 }
 
 // ----------------------------------------------------------------
 // Destructor
 // ----------------------------------------------------------------
 Vehicle::~Vehicle()
 {
  
 }
 
 // ----------------------------------------------------------------
 // Set the current velocity and position
 // ----------------------------------------------------------------
 void Vehicle::initialise(unsigned velocity, unsigned long position, unsigned length)
 {
  Velocity[1] = Velocity[0] = velocity;
  Position[1] = Position[0] = position;
  Length = length;
  Delay = Travel_time = 0;
  
  // Initialise emission table
  initialise_emission_table();
  
 }
 
 // ----------------------------------------------------------------
 // Update vehicles status
 // ----------------------------------------------------------------
 void Vehicle::update()
 {
  //Update statistics
  update_statistics();
 
  Velocity[0] = Velocity[1];
  Position[0] = Position[1];
 
 }

 // ----------------------------------------------------------------
 // Update statistics
 // ----------------------------------------------------------------
 void Vehicle::update_statistics()
 {
  // Increase travel time
  Travel_time++;
  // Compute delay
  const unsigned next_velocity = Velocity[1];
  // Increase delay
  if (next_velocity == 0)
   {
    Delay++;
   }
  else
   {
    Delay = 0;
   }
 
 }

 // ----------------------------------------------------------------
 // In charge of setting the initial and constant values to the
 // emission table
 // ----------------------------------------------------------------
 void Vehicle::initialise_emission_tables()
 {
  // ====================================================
  // CO2
  // ====================================================
  // ---------------------
  // Petrol
  // ---------------------
  Emission_table_CO2[0][0];
  Emission_table_CO2[0][1];
  Emission_table_CO2[0][2];
  Emission_table_CO2[0][3];
  Emission_table_CO2[0][4];
  Emission_table_CO2[0][5];
  Emission_table_CO2[0][6];

  // ---------------------
  // Diesel
  // ---------------------
  Emission_table_CO2[1][0];
  Emission_table_CO2[1][1];
  Emission_table_CO2[1][2];
  Emission_table_CO2[1][3];
  Emission_table_CO2[1][4];
  Emission_table_CO2[1][5];
  Emission_table_CO2[1][6];

  // ---------------------
  // LPG car
  // ---------------------
  Emission_table_CO2[2][0];
  Emission_table_CO2[2][1];
  Emission_table_CO2[2][2];
  Emission_table_CO2[2][3];
  Emission_table_CO2[2][4];
  Emission_table_CO2[2][5];
  Emission_table_CO2[2][6];
  
  // ---------------------
  // HDV
  // ---------------------
  Emission_table_CO2[3][0];
  Emission_table_CO2[3][1];
  Emission_table_CO2[3][2];
  Emission_table_CO2[3][3];
  Emission_table_CO2[3][4];
  Emission_table_CO2[3][5];
  Emission_table_CO2[3][6];

  // ---------------------
  // Bus
  // ---------------------
  Emission_table_CO2[4][0];
  Emission_table_CO2[4][1];
  Emission_table_CO2[4][2];
  Emission_table_CO2[4][3];
  Emission_table_CO2[4][4];
  Emission_table_CO2[4][5];
  Emission_table_CO2[4][6];
  
  // ====================================================
  // PM
  // ====================================================
  // ---------------------
  // Petrol
  // ---------------------
  Emission_table_PM[0][0];
  Emission_table_PM[0][1];
  Emission_table_PM[0][2];
  Emission_table_PM[0][3];
  Emission_table_PM[0][4];
  Emission_table_PM[0][5];
  Emission_table_PM[0][6];

  // ---------------------
  // Diesel
  // ---------------------
  Emission_table_PM[1][0];
  Emission_table_PM[1][1];
  Emission_table_PM[1][2];
  Emission_table_PM[1][3];
  Emission_table_PM[1][4];
  Emission_table_PM[1][5];
  Emission_table_PM[1][6];

  // ---------------------
  // LPG car
  // ---------------------
  Emission_table_PM[2][0];
  Emission_table_PM[2][1];
  Emission_table_PM[2][2];
  Emission_table_PM[2][3];
  Emission_table_PM[2][4];
  Emission_table_PM[2][5];
  Emission_table_PM[2][6];
  
  // ---------------------
  // HDV
  // ---------------------
  Emission_table_PM[3][0];
  Emission_table_PM[3][1];
  Emission_table_PM[3][2];
  Emission_table_PM[3][3];
  Emission_table_PM[3][4];
  Emission_table_PM[3][5];
  Emission_table_PM[3][6];

  // ---------------------
  // Bus
  // ---------------------
  Emission_table_PM[4][0];
  Emission_table_PM[4][1];
  Emission_table_PM[4][2];
  Emission_table_PM[4][3];
  Emission_table_PM[4][4];
  Emission_table_PM[4][5];
  Emission_table_PM[4][6];
  
 }
 
} // namespace CA
