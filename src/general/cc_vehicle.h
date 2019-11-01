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
 Vehicle(unsigned velocity, unsigned long position, unsigned length = 1);
 
 // ----------------------------------------------------------------
 // Destructor
 // ----------------------------------------------------------------
 virtual ~Vehicle();
 
 // ----------------------------------------------------------------
 // Set the current velocity and position
 // ----------------------------------------------------------------
 void initialise(unsigned velocity, unsigned long position, unsigned length = 1);
 
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
 
 protected:
 
 // Velocity (index 0 is current time, other index is at time i-th)
 unsigned Velocity[2];
 // Position (index 0 is current time, other index is at time i-th)
 unsigned long Position[2];
 // Length of vehicle
 unsigned Length;
 
};

#endif // #ifndef CC_VEHICLE_H

