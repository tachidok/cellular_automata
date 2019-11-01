#include "cc_vehicle.h"
 
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
}

// ----------------------------------------------------------------
// Update vehicles status
// ----------------------------------------------------------------
void Vehicle::update()
{
 Velocity[0] = Velocity[1];
 Position[0] = Position[1];
}
 
