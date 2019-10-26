#include "cc_nasch.h"

// ----------------------------------------------------------------
// Constructor -- do nothing
// ----------------------------------------------------------------
NaSch::NaSch() 
{
  
}
 
// ----------------------------------------------------------------
// Constructor
// ----------------------------------------------------------------
NaSch::NaSch(unsigned lane_size, unsigned maximum_velocity, double break_probability,
             double alpha, double beta)
{
 // Set lane configuration
 initialise(lane_size, maximum_velocity, break_probability, alpha, beta);
}
 
// ----------------------------------------------------------------
// Destructor - do nothing
// ----------------------------------------------------------------
NaSch::~NaSch()
{
 clear();
}
 
// ----------------------------------------------------------------
// Initialise lane configuration
// ----------------------------------------------------------------
void NaSch::initialise(unsigned lane_size, unsigned maximum_velocity, double break_probability, double alpha, double beta)
{
 // Set lane configuration
 Allowed_number_of_vehicles = Lane_size = lane_size;
 Maximum_velocity = maximum_velocity;
 Break_probability = break_probability;
 
 // Initialise entry and exit probability
 Alpha = alpha; // entry probability
 Beta = beta; // exit probability
 
 // Initialise data structures
 clear();
}
 
// ----------------------------------------------------------------
// Clear data structures
// ----------------------------------------------------------------
void NaSch::clear()
{
 // Initialise data structures representing the lane
 Lane.clear();  
 Lane.resize(Lane_size, 0);

 // Free vehicles in lane memory
 for (unsigned i = 0; i < Lane_size; i++)
  {
   if (Lane[i] != 0)
    {
     delete Lane[i];
     Lane[i] = 0;
    }
  }
 
 Vehicles_pt.clear();
 
 // Reset lane configuration
 Current_number_of_vehicles = 0;
}

// ----------------------------------------------------------------
// Computes the density
// ----------------------------------------------------------------
double NaSch::density()
{ 
 return double(Current_number_of_vehicles)/double(Lane_size);
}

// ----------------------------------------------------------------
// Update vehicles list
// ----------------------------------------------------------------
unsigned NaSch::update_vehicles_list()
{
 // Clear current vehicles vector status
 Vehicles_pt.clear();
 
 // Reserve space for storing data in vehicles vector. If reallocation
 // is required (because more data of those expected are required)
 // then allocation time is linear on the current number of data
 Vehicles_pt.reserve(Allowed_number_of_vehicles + 1);
 
 // Before adding the first vehicle check whether we can add a new
 // vehicle at the very first position of the lane
 if (Current_number_of_vehicles < Allowed_number_of_vehicles &&
     Lane[0] == 0 &&
     Alpha > 0.0)
  {
   // Compute a random number
   const double r = std::rand();
   // Is the computed probability is less or equal than the Alpha
   // probability
   if ((r / RAND_MAX) <= Alpha)
    {
     // Create a new vehicle
     unsigned initial_velocity = 0;
     unsigned initial_position = 0;
     Vehicle *new_vehicle_pt = new Vehicle(initial_velocity, initial_position); 
     // Add a vehicle
     Lane[0] = new_vehicle_pt;
     // Increase the current number of vehicles
     Current_number_of_vehicles++;
    }
   
  }
 
 // Get the new vehicles order
 unsigned i = 0;
 for (unsigned k = 0; k < Lane_size; k++)
  {
   if (Lane[k] != 0)
    {
     // Get a pointer to the vehicle
     Vehicle *vehicle_pt = Lane[k];
     
     // Copy the vehicle at positon k in the lane as the i-th vehicle
     // in the vehicles vector
     //Vehicles_pt[i] = vehicle_pt;
     Vehicles_pt.push_back(vehicle_pt);
     
     i++;
    }
  }
 
 // Return the number of 'found' vehicles in the lane
 return i;
 
}

// ----------------------------------------------------------------
// Update lane based on NaSch rules
// ----------------------------------------------------------------
unsigned NaSch::apply_nasch()
{
 
 // Accumulated velocity
 unsigned sum_velocity = 0;
 
 for (unsigned i = 0; i < Current_number_of_vehicles; i++)
  {
   // Get a pointer to the current vehicle
   Vehicle *current_vehicle_pt = Vehicles_pt[i];
   const unsigned current_position = current_vehicle_pt->position();
   const unsigned current_velocity = current_vehicle_pt->velocity();
   
   // -----------------------------------------------------------------
   // Compute the spatial headway (the empty spaces between vehicles)
   // -----------------------------------------------------------------
   unsigned spatial_headway = 0;
   // Is this the last vehicle (non-periodic boundary conditions)
   if (i + 1 == Current_number_of_vehicles)
    {
     spatial_headway = Maximum_velocity; // Infinity velocity so it
                                         // can leave the lane
    }
   else
    {
     Vehicle *next_vehicle_pt = Vehicles_pt[i+1];
     spatial_headway = next_vehicle_pt->position() - current_position - 1;
    }
    
   //std::cerr << i + 1 << ":" << current_position << "-" << spatial_headway << std::endl;
   
   // -----------------------------------------------------------------
   // NaSch rules
   // -----------------------------------------------------------------
   
   // First rule (acceleration)
   unsigned new_velocity = std::min(current_velocity + 1, Maximum_velocity);
   
   // Second rule (deceleration)
   new_velocity = std::min(new_velocity, spatial_headway);
   
   // Third rule (randomization) 
   const double r = std::rand();
   if ((r / RAND_MAX) <= Break_probability)
    {
     new_velocity = std::max(int(new_velocity - 1), 0);
     //std::cerr << "NV: " << new_velocity << std::endl;
    }
   
   // Fourth rule (movement)
   unsigned new_position = current_position + new_velocity;
   
   // This only applies to the last vehicle on the lane. Check whether
   // we allow it to leave the lane or not. We decide this based on
   // the Beta parameter
   if (i + 1 == Current_number_of_vehicles && new_position >= Lane_size)
    {
     // Check whether we allow it to leave
     if (Beta > 0.0)
      {
       // Compute a random number
       const double r = std::rand();
       // Is the computed probability is less or equal than the Beta
       // probability
       if ((r / RAND_MAX) <= Beta)
        {
         // Delete vehicle
         delete current_vehicle_pt;
         current_vehicle_pt = 0;
         Vehicles_pt[i] = 0;
         Lane[current_position] = 0;
         // Decrease the current number of vehicles
         Current_number_of_vehicles--;
        }
       else
        {
         // Update velocity and positon of vehicle
         current_vehicle_pt->velocity(1) = new_velocity;
         current_vehicle_pt->position(1) = Lane_size-1;
        }
      }
     else
      {
       // Update velocity and positon of vehicle
       current_vehicle_pt->velocity(1) = new_velocity;
       current_vehicle_pt->position(1) = Lane_size-1;
      }
    }
   else
    {
     // Update velocity and positon of vehicle
     current_vehicle_pt->velocity(1) = new_velocity;
     current_vehicle_pt->position(1) = new_position;
    }
   
   sum_velocity+=new_velocity; 
   
  } // for (i < Current_number_of_vehicles)
 
 return sum_velocity;
 
}

// ----------------------------------------------------------------
// Update the lane status
// ---------------------------------------------------------------- 
void NaSch::update()
{
 for (unsigned i = 0; i < Current_number_of_vehicles; i++)
  {
   // Get a pointer to the current vehicle
   Vehicle *vehicle_pt = Vehicles_pt[i];
    
   const unsigned old_position = vehicle_pt->position(0);
   const unsigned new_position = vehicle_pt->position(1);
    
   // Update the pointer on the lane
   Lane[old_position] = 0; // Delete the pointer from the old position
   Lane[new_position] = vehicle_pt;
    
   // Update vehicle
   vehicle_pt->update();
    
  } // (for i < Current_number_of_vehicles)
 
}
 
// ----------------------------------------------------------------
// Prints the lane status
// ---------------------------------------------------------------- 
void NaSch::print(bool print_velocities)
{
 for (unsigned i = 0; i < Lane_size; i++)
  {
   Vehicle *vehicle_pt = Lane[i];
   if (vehicle_pt != 0)
    {
     if (print_velocities)
      {
       const unsigned velocity = vehicle_pt->velocity();
       std::cerr << velocity;
      }
     else
      {
       std::cerr << "#";
      }
    }
   else
    {
     std::cerr << ".";
    }
    
  }

 std::cerr << std::endl;
  
}

