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
 // Constructor (the occupancy considers the number of cells the
 // vehicle occupies in the Cellular Automata, the length is the car
 // size in meters)
 // ----------------------------------------------------------------
 Vehicle::Vehicle(unsigned velocity, unsigned long position,
                  unsigned occupancy, Real length, Vehicle_type type)
 {
  // Set initial velocity, position and length
  initialise(velocity, position, occupancy, length, type);
 }
 
 // ----------------------------------------------------------------
 // Destructor
 // ----------------------------------------------------------------
 Vehicle::~Vehicle()
 {
  
 }
 
 // ----------------------------------------------------------------
 // Set the current velocity and position (the occupancy considers
 // the number of cells the vehicle occupies in the Cellular
 // Automata, the length is the car size in meters)
 // ----------------------------------------------------------------
 void Vehicle::initialise(unsigned velocity, unsigned long position,
                          unsigned occupancy, Real length, Vehicle_type type)
 {
  Velocity[1] = Velocity[0] = velocity;
  Position[1] = Position[0] = position;
  
  // Occupancy
  Occupancy = occupancy;
  // Length
  Length = length;
  
  // Length/occupancy ratio
  Eta = length/static_cast<Real>(occupancy);

  // Vehicle's type
  Type = type;
  
  // Initialise delays and travel time
  Delay = Travel_time = 0;
  
  // Initialise emission table
  initialise_emission_tables();
  
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
  Emission_table_CO2[0][0] = 0.0;
  Emission_table_CO2[0][1] = 0.553;
  Emission_table_CO2[0][2] = 0.161;
  Emission_table_CO2[0][3] = -0.00289;
  Emission_table_CO2[0][4] = 0.266;
  Emission_table_CO2[0][5] = 0.511;
  Emission_table_CO2[0][6] = 0.183;
  
  // ---------------------
  // Diesel
  // ---------------------
  Emission_table_CO2[1][0] = 0.0;
  Emission_table_CO2[1][1] = 0.324;
  Emission_table_CO2[1][2] = 0.0859;
  Emission_table_CO2[1][3] = 0.00496;
  Emission_table_CO2[1][4] = -0.0586;
  Emission_table_CO2[1][5] = 0.448;
  Emission_table_CO2[1][6] = 0.230;

  // ---------------------
  // LPG car
  // ---------------------
  Emission_table_CO2[2][0] = 0.0;
  Emission_table_CO2[2][1] = 0.600;
  Emission_table_CO2[2][2] = 0.219;
  Emission_table_CO2[2][3] = -0.00774;
  Emission_table_CO2[2][4] = 0.357;
  Emission_table_CO2[2][5] = 0.514;
  Emission_table_CO2[2][6] = 0.17;
  
  // ---------------------
  // HDV
  // ---------------------
  Emission_table_CO2[3][0] = 0.0;
  Emission_table_CO2[3][1] = 1.52;
  Emission_table_CO2[3][2] = 1.88;
  Emission_table_CO2[3][3] = -0.0695;
  Emission_table_CO2[3][4] = 4.71;
  Emission_table_CO2[3][5] = 5.88;
  Emission_table_CO2[3][6] = 2.09;

  // ---------------------
  // Bus
  // ---------------------
  Emission_table_CO2[4][0] = 0.0;
  Emission_table_CO2[4][1] = 0.904;
  Emission_table_CO2[4][2] = 1.13;
  Emission_table_CO2[4][3] = -0.0427;
  Emission_table_CO2[4][4] = 2.81;
  Emission_table_CO2[4][5] = 3.45;
  Emission_table_CO2[4][6] = 1.22;
  
  // ====================================================
  // NOx
  // ====================================================
  // ---------------------
  // Petrol a >= -0.5m/s^2
  // ---------------------
  Emission_table_NOx[0][0] = 0.0;
  Emission_table_NOx[0][1] = 0.000619;
  Emission_table_NOx[0][2] = 0.00008;
  Emission_table_NOx[0][3] = -0.00000403;
  Emission_table_NOx[0][4] = -0.000413;
  Emission_table_NOx[0][5] = 0.000380;
  Emission_table_NOx[0][6] = 0.000177;

  // ---------------------
  // Petrol a < -0.5m/s^2
  // ---------------------
  Emission_table_NOx[1][0] = 0.0;
  Emission_table_NOx[1][1] = 0.000217;
  Emission_table_NOx[1][2] = 0.0;
  Emission_table_NOx[1][3] = 0.0;
  Emission_table_NOx[1][4] = 0.0;
  Emission_table_NOx[1][5] = 0.0;
  Emission_table_NOx[1][6] = 0.0;
  
  // ---------------------
  // Diesel a >= -0.5m/s^2
  // ---------------------
  Emission_table_NOx[2][0] = 0.0;
  Emission_table_NOx[2][1] = 0.00241;
  Emission_table_NOx[2][2] = -0.000411;
  Emission_table_NOx[2][3] = 0.0000673;
  Emission_table_NOx[2][4] = -0.00307;
  Emission_table_NOx[2][5] = 0.00214;
  Emission_table_NOx[2][6] = 0.00150;
  
  // ---------------------
  // Diesel a < -0.5m/s^2
  // ---------------------
  Emission_table_NOx[3][0] = 0.0;
  Emission_table_NOx[3][1] = 0.00168;
  Emission_table_NOx[3][2] = -0.0000662;
  Emission_table_NOx[3][3] = 0.000009;
  Emission_table_NOx[3][4] = 0.000250;
  Emission_table_NOx[3][5] = 0.000291;
  Emission_table_NOx[3][6] = 0.000120;
  
  // ---------------------
  // LPG car a >= -0.5m/s^2
  // ---------------------
  Emission_table_NOx[4][0] = 0.0;
  Emission_table_NOx[4][1] = 0.000892;
  Emission_table_NOx[4][2] = 0.0000161;
  Emission_table_NOx[4][3] = -0.000000806;
  Emission_table_NOx[4][4] = -0.0000823;
  Emission_table_NOx[4][5] = 0.0000760;
  Emission_table_NOx[4][6] = 0.0000354;
  
  // ---------------------
  // LPG car a < -0.5m/s^2
  // ---------------------
  Emission_table_NOx[5][0] = 0.0;
  Emission_table_NOx[5][1] = 0.000343;
  Emission_table_NOx[5][2] = 0.0;
  Emission_table_NOx[5][3] = 0.0;
  Emission_table_NOx[5][4] = 0.0;
  Emission_table_NOx[5][5] = 0.0;
  Emission_table_NOx[5][6] = 0.0;
  
  // ---------------------
  // HDV
  // ---------------------
  Emission_table_NOx[6][0] = 0.0;
  Emission_table_NOx[6][1] = 0.0356;
  Emission_table_NOx[6][2] = 0.00971;
  Emission_table_NOx[6][3] = -0.000240;
  Emission_table_NOx[6][4] = 0.0326;
  Emission_table_NOx[6][5] = 0.0133;
  Emission_table_NOx[6][6] = 0.0115;

  // ---------------------
  // Bus
  // ---------------------
  Emission_table_NOx[7][0] = 0.0;
  Emission_table_NOx[7][1] = 0.0236;
  Emission_table_NOx[7][2] = 0.00651;
  Emission_table_NOx[7][3] = -0.000170;
  Emission_table_NOx[7][4] = 0.0217;
  Emission_table_NOx[7][5] = 0.00894;
  Emission_table_NOx[7][6] = 0.00757;
  
  // ====================================================
  // VOC
  // ====================================================
  // ---------------------
  // Petrol a >= -0.5m/s^2
  // ---------------------
  Emission_table_VOC[0][0] = 0.0;
  Emission_table_VOC[0][1] = 0.00447;
  Emission_table_VOC[0][2] = 0.000000732;
  Emission_table_VOC[0][3] = -0.0000000287;
  Emission_table_VOC[0][4] = -0.00000341;
  Emission_table_VOC[0][5] = 0.00000494;
  Emission_table_VOC[0][6] = 0.00000166;

  // ---------------------
  // Petrol a < -0.5m/s^2
  // ---------------------
  Emission_table_VOC[1][0] = 0.0;
  Emission_table_VOC[1][1] = 0.00263;
  Emission_table_VOC[1][2] = 0.0;
  Emission_table_VOC[1][3] = 0.0;
  Emission_table_VOC[1][4] = 0.0;
  Emission_table_VOC[1][5] = 0.0;
  Emission_table_VOC[1][6] = 0.0;
  
  // ---------------------
  // Diesel a >= -0.5m/s^2
  // ---------------------
  Emission_table_VOC[2][0] = 0.0;
  Emission_table_VOC[2][1] = 0.0000922;
  Emission_table_VOC[2][2] = 0.00000909;
  Emission_table_VOC[2][3] = -0.000000229;
  Emission_table_VOC[2][4] = -0.0000220;
  Emission_table_VOC[2][5] = 0.0000169;
  Emission_table_VOC[2][6] = 0.00000375;
  
  // ---------------------
  // Diesel a < -0.5m/s^2
  // ---------------------
  Emission_table_VOC[3][0] = 0.0;
  Emission_table_VOC[3][1] = 0.0000525;
  Emission_table_VOC[3][2] = 0.00000722;
  Emission_table_VOC[3][3] = -0.000000187;
  Emission_table_VOC[3][4] = 0.0;
  Emission_table_VOC[3][5] = -0.0000102;
  Emission_table_VOC[3][6] = -0.00000422;
  
  // ---------------------
  // LPG car a >= -0.5m/s^2
  // ---------------------
  Emission_table_VOC[4][0] = 0.0;
  Emission_table_VOC[4][1] = 0.0144;
  Emission_table_VOC[4][2] = 0.000000174;
  Emission_table_VOC[4][3] = -0.00000000682;
  Emission_table_VOC[4][4] = -0.000000811;
  Emission_table_VOC[4][5] = 0.00000118;
  Emission_table_VOC[4][6] = 0.000000396;
  
  // ---------------------
  // LPG car a < -0.5m/s^2
  // ---------------------
  Emission_table_VOC[5][0] = 0.0;
  Emission_table_VOC[5][1] = 0.00842;;
  Emission_table_VOC[5][2] = 0.0;
  Emission_table_VOC[5][3] = 0.0;
  Emission_table_VOC[5][4] = 0.0;
  Emission_table_VOC[5][5] = 0.0;
  Emission_table_VOC[5][6] = 0.0;
  
  // ---------------------
  // HDV
  // ---------------------
  Emission_table_VOC[6][0] = 0.0;
  Emission_table_VOC[6][1] = 0.00104;
  Emission_table_VOC[6][2] = 0.000487;
  Emission_table_VOC[6][3] = -0.0000149;
  Emission_table_VOC[6][4] = 0.00127;
  Emission_table_VOC[6][5] = 0.000210;
  Emission_table_VOC[6][6] = 0.000100;
  
  // ---------------------
  // Bus
  // ---------------------
  Emission_table_VOC[7][0] = 0.0;
  Emission_table_VOC[7][1] = 0.00155;
  Emission_table_VOC[7][2] = 0.000820;
  Emission_table_VOC[7][3] = -0.0000242;
  Emission_table_VOC[7][4] = 0.00186;
  Emission_table_VOC[7][5] = 0.000321;
  Emission_table_VOC[7][6] = 0.000136;
    
  // ====================================================
  // PM
  // ====================================================
  // ---------------------
  // Petrol
  // ---------------------
  Emission_table_PM[0][0] = 0.0;
  Emission_table_PM[0][1] = 0.0;
  Emission_table_PM[0][2] = 0.0000157;
  Emission_table_PM[0][3] = -0.000000921;
  Emission_table_PM[0][4] = 0.0;
  Emission_table_PM[0][5] = 0.0000375;
  Emission_table_PM[0][6] = 0.0000189;

  // ---------------------
  // Diesel
  // ---------------------
  Emission_table_PM[1][0] = 0.0;
  Emission_table_PM[1][1] = 0.0;
  Emission_table_PM[1][2] = 0.000313;
  Emission_table_PM[1][3] = -0.0000184;
  Emission_table_PM[1][4] = 0.0;
  Emission_table_PM[1][5] = 0.000750;
  Emission_table_PM[1][6] = 0.000378;
  
  // ---------------------
  // LPG car
  // ---------------------
  Emission_table_PM[2][0] = 0.0;
  Emission_table_PM[2][1] = 0.0;
  Emission_table_PM[2][2] = 0.0000157;
  Emission_table_PM[2][3] = -0.000000921;
  Emission_table_PM[2][4] = 0.0;
  Emission_table_PM[2][5] = 0.0000375;
  Emission_table_PM[2][6] = 0.0000189;
  
  // ---------------------
  // HDV
  // ---------------------
  Emission_table_PM[3][0] = 0.0;
  Emission_table_PM[3][1] = 0.000214;
  Emission_table_PM[3][2] = 0.000335;
  Emission_table_PM[3][3] = -0.0000222;
  Emission_table_PM[3][4] = 0.00207;
  Emission_table_PM[3][5] = 0.00180;
  Emission_table_PM[3][6] = 0.000227;

  // ---------------------
  // Bus
  // ---------------------
  Emission_table_PM[4][0] = 0.0;
  Emission_table_PM[4][1] = 0.000223;
  Emission_table_PM[4][2] = 0.000347;
  Emission_table_PM[4][3] = -0.0000238;
  Emission_table_PM[4][4] = 0.00208;
  Emission_table_PM[4][5] = 0.00176;
  Emission_table_PM[4][6] = 0.000223;
  
 }

 // ----------------------------------------------------------------
 // In charge of computing all emissions and transform
 // ----------------------------------------------------------------
 Real Vehicle::compute_emissions(Real &CO2, Real &NOx, Real &VOC, Real &PM)
 {
  // Trasformed velocity and acceleration, considering occupancy ratio
  const Real v = Velocity[0] * Eta;
  const Real a = (Velocity[0] - Velocity[1]) * Eta;
  
  // Based on vehicle's type
  unsigned TypeNew = 0;
  
  if (Type == Vehicle_type::PETROL && a >= -0.5)
   {
    TypeNew = 0;
   }
  else if (Type == Vehicle_type::PETROL && a < -0.5)
   {
    TypeNew = 1;
   }
  else if (Type == Vehicle_type::DIESEL && a >= -0.5)
   {
    TypeNew = 2;
   }
  else if (Type == Vehicle_type::DIESEL && a < -0.5)
   {
    TypeNew = 3;
   }
  else if (Type == Vehicle_type::LPG && a >= -0.5)
   {
    TypeNew = 4;
   }
  else if (Type == Vehicle_type::LPG && a < -0.5)
   {
    TypeNew = 5;
   }
  else if (Type == Vehicle_type::HDV)
   {
    TypeNew = 6;
   }
  else if (Type == Vehicle_type::BUS)
   {
    TypeNew = 7;
   }
  
  // --------------------------------------
  // CO2
  // --------------------------------------
  CO2 = std::max(Emission_table_CO2[Type][0],
                 Emission_table_CO2[Type][1] + Emission_table_CO2[Type][2]*v +
                 Emission_table_CO2[Type][3]*v*v + Emission_table_CO2[Type][4]*a +
                 Emission_table_CO2[Type][5]*a*a + Emission_table_CO2[Type][6]*v*a);
  
  // --------------------------------------
  // NOx (using TypeNew)
  // --------------------------------------
  NOx = std::max(Emission_table_NOx[TypeNew][0],
                 Emission_table_NOx[TypeNew][1] + Emission_table_NOx[TypeNew][2]*v +
                 Emission_table_NOx[TypeNew][3]*v*v + Emission_table_NOx[TypeNew][4]*a +
                 Emission_table_NOx[TypeNew][5]*a*a + Emission_table_NOx[TypeNew][6]*v*a);
  
  // --------------------------------------
  // VOC (using TypeNew)
  // --------------------------------------
  VOC = std::max(Emission_table_VOC[TypeNew][0],
                 Emission_table_VOC[TypeNew][1] + Emission_table_VOC[TypeNew][2]*v +
                 Emission_table_VOC[TypeNew][3]*v*v + Emission_table_VOC[TypeNew][4]*a +
                 Emission_table_VOC[TypeNew][5]*a*a + Emission_table_VOC[TypeNew][6]*v*a);
  
  // --------------------------------------
  // PM
  // --------------------------------------
  PM = std::max(Emission_table_PM[Type][0],
                Emission_table_PM[Type][1] + Emission_table_PM[Type][2]*v +
                Emission_table_PM[Type][3]*v*v + Emission_table_PM[Type][4]*a +
                Emission_table_PM[Type][5]*a*a + Emission_table_PM[Type][6]*v*a);
  
 }
 
} // namespace CA
