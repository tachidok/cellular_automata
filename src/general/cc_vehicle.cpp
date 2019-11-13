#include "cc_vehicle.h"
 
// ----------------------------------------------------------------
// Constructor
// ----------------------------------------------------------------
Vehicle::Vehicle()
 : Clear_statistics_at_next_update(false)
{
 // Set initial velocity, position and length
 initialise(0, 0, 1);
}

// ----------------------------------------------------------------
// Constructor
// ----------------------------------------------------------------
Vehicle::Vehicle(unsigned velocity, unsigned long position, unsigned length)
 : Clear_statistics_at_next_update(false)
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
 clear_statistics();
 Velocity[1] = Velocity[0] = velocity;
 Position[1] = Position[0] = position;
 Length = length;
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
 
 // Check whether to clear statistics
 if (Clear_statistics_at_next_update)
  {
   clear_statistics();
   disable_clear_statistics_at_next_update();
  }
 
}

// ----------------------------------------------------------------
// Get average delay value
// ----------------------------------------------------------------
Real Vehicle::average_delay()
{
 // Get the number of entries in the histogram
 const unsigned n_entries = Delay_histogram.size();
 // Check whether there are entries or not
 if (n_entries == 0)
  {
   return 0;
  }
 
 unsigned sum_delay = 0;
 // Loop over the entries to sum them up
 for (unsigned i = 0; i < n_entries; i++)
  {
   sum_delay+=Delay_histogram[i];
  }
 
 return static_cast<Real>(sum_delay)/static_cast<Real>(n_entries);
 
}

// ----------------------------------------------------------------
// In charge of clearing all statistics
// ----------------------------------------------------------------
void Vehicle::clear_statistics()
{
 Travel_time = 0;
 Delay = 0;
 reset_delay_histogram();
}

// Update statistics
void Vehicle::update_statistics()
{
 // Increase travel time
 Travel_time++;
 // Compute delay
 const unsigned previous_velocity = Velocity[0];
 const unsigned current_velocity = Velocity[1];
 // Increase delay
 if (previous_velocity == 0)
  {
   Delay++;
  }
 
 // Add delay to histogram
 if (previous_velocity == 0 && current_velocity > 0)
  {
   add_delay_to_histrogram(Delay);
   // Reset delay
   Delay=0;
  }
 
}
