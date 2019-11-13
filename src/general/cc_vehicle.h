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
 
 // Set travel_time
 inline unsigned &travel_time() {return Travel_time;}
 // Get position
 inline unsigned travel_time() const {return Travel_time;}
 // Set delay
 inline unsigned &delay() {return Delay;}
 // Get delay
 inline unsigned delay() const {return Delay;}
 // Add entry in delay histogram
 inline unsigned &add_delay_to_histrogram(unsigned delay) {Delay_histogram.push_back(delay);}
 // Get average delay value
 Real average_delay();
 
 // Indicate whether to clear statistics values at the next update calling method
 inline void enable_clear_statistics_at_next_update() {Clear_statistics_at_next_update=true;}
 inline void disable_clear_statistics_at_next_update() {Clear_statistics_at_next_update=false;}
 // In charge of clearing all statistics
 void clear_statistics();
 
 protected:
 
 // Reset delay histogram
 inline void reset_delay_histogram() {Delay_histogram.clear();}
 // Update statistics
 void update_statistics();
 
 // Velocity (index 0 is current time, other index is at time i-th)
 unsigned Velocity[2];
 // Position (index 0 is current time, other index is at time i-th)
 unsigned long Position[2];
 // Length of vehicle
 unsigned Length;
 // Travel time, increases per each update call
 unsigned Travel_time; 
 // Considers the number of unit times where velocity = 0
 unsigned Delay;
 // Delay histogram (stores the delay values)
 std::vector<unsigned> Delay_histogram;
 
 // A flag to indicate whether to clear statistical values or not at
 // next update
 bool Clear_statistics_at_next_update;
 
};

#endif // #ifndef CC_VEHICLE_H

