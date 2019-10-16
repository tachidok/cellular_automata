#include "general.h"

#ifndef CC_VEHICLE_H
#define CC_VEHICLE_H

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
 Vehicle(unsigned velocity, unsigned long position);
 
 // ----------------------------------------------------------------
 // Destructor
 // ----------------------------------------------------------------
 virtual ~Vehicle();
 
 // ----------------------------------------------------------------
 // Set the current velocity and position
 // ----------------------------------------------------------------
 void initialise(unsigned velocity, unsigned long position);
 
 // ----------------------------------------------------------------
 // Update vehicles status
 // ----------------------------------------------------------------
 void update();

 // Set velocity
 inline unsigned &velocity(unsigned i = 0) {return Velocity[i];}
 // Get velocity
 inline unsigned velocity(unsigned i = 0) const {return Velocity[i];}
 // Set position
 inline unsigned long &position(unsigned i = 0) {return Position[i];}
 // Get position
 inline unsigned long position(unsigned i = 0) const {return Position[i];}
 
 protected:
 
 // Velocity (index 0 is current time, other index is at time i-th)
 unsigned Velocity[2];
 // Position (index 0 is current time, other index is at time i-th)
 unsigned long Position[2];
 
};

#endif // #ifndef CC_VEHICLE_H

