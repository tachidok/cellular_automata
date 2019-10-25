#include "cc_vehicle.h"
 
// ----------------------------------------------------------------
// Constructor
// ----------------------------------------------------------------
Vehicle::Vehicle()
{
 // Set initial velocity and position
 initialise(0, 0);
}

// ----------------------------------------------------------------
// Constructor
// ----------------------------------------------------------------
Vehicle::Vehicle(unsigned velocity, unsigned position)
{
 // Set initial velocity and position
 initialise(velocity, position);
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
void Vehicle::initialise(unsigned velocity, unsigned position)
{
 Velocity[1] = Velocity[0] = velocity;
 Position[1] = Position[0] = position;
}
 
// ----------------------------------------------------------------
// Update vehicles status
// ----------------------------------------------------------------
void Vehicle::update()
{
 Velocity[0] = Velocity[1];
 Position[0] = Position[1];
}
 
