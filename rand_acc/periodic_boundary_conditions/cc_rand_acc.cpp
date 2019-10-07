#include "cc_rand_acc.h"

// ----------------------------------------------------------------
// Constructor -- do nothing
// ----------------------------------------------------------------
RandAcc::RandAcc() 
{
  
}
 
// ----------------------------------------------------------------
// Constructor
// ----------------------------------------------------------------
RandAcc::RandAcc(unsigned lane_size, unsigned maximum_velocity,
                 double p0, double p1)
{
 // Set lane configuration
 initialise(lane_size, maximum_velocity, p0, p1);
}

// ----------------------------------------------------------------
// Destructor - do nothing
// ----------------------------------------------------------------
RandAcc::~RandAcc()
{
 clear();
}
 
// ----------------------------------------------------------------
// Initialise lane configuration
// ----------------------------------------------------------------
void RandAcc::initialise(unsigned lane_size, unsigned maximum_velocity, double p0, double p1)
{
 // Set lane configuration
 Lane_size = lane_size;
 Maximum_velocity = maximum_velocity;
 P_0 = p0;
 P_1 = p1;
 
 // Initialise data structures
 clear();
}

// ----------------------------------------------------------------
// Clear data structures
// ----------------------------------------------------------------
void RandAcc::clear()
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
 
 Vehicles_pt.resize(Lane_size, 0);
 
 // Reset lane configuration
 Current_number_of_vehicles = 0;
 Density = 0;
 
}

// ----------------------------------------------------------------
// Fill in vehicles
// ----------------------------------------------------------------
void RandAcc::fill_in_vehicles(double density)
{
 // Set the density
 Density = density;
 
 // Compute the number of vehicles to be added to the lane
 Current_number_of_vehicles = Density * Lane_size;
 
 unsigned i = 0;
 // Add vehicles in the lane randomly
 while(i < Current_number_of_vehicles)
  {
   // Random position to add a vehicle
   const double r = rand();
   unsigned k = (r / RAND_MAX) * Lane_size;
   // Check whether there is a vehicle in the k lane position
   if (Lane[k] == 0)
    {
     // Create a new vehicle
     unsigned initial_velocity = 0;
     unsigned initial_position = k;
     Vehicle *new_vehicle_pt = new Vehicle(initial_velocity, initial_position); 
     // Add a vehicle
     Lane[k] = new_vehicle_pt;
     // Increase the number of added vehicles to the lane
     i++;
    }
    
  }
  
}

// ----------------------------------------------------------------
// Update vehicles list
// ----------------------------------------------------------------
unsigned RandAcc::update_vehicles_list()
{
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
     Vehicles_pt[i] = vehicle_pt;
      
     i++;
    }
  }
  
 // Return the number of vehicles in the lane
 return i;
 
}

// ----------------------------------------------------------------
// Update lane based on RandAcc rules
// ----------------------------------------------------------------
unsigned RandAcc::apply_rand_acc()
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
   // Is this the last vehicle (periodic boundary conditions)
   if (i + 1 == Current_number_of_vehicles)
    {
     Vehicle *next_vehicle_pt = Vehicles_pt[0];
     spatial_headway = (next_vehicle_pt->position() + Lane_size) - current_position - 1;
    }
   else
    {
     Vehicle *next_vehicle_pt = Vehicles_pt[i+1];
     spatial_headway = next_vehicle_pt->position() - current_position - 1;
    }
   
   //std::cerr << i + 1 << ":" << current_position << "-" << spatial_headway << std::endl;
   
   // -----------------------------------------------------------------
   // Random acceleration rules
   // -----------------------------------------------------------------
   
   // Compute the randomisation parameter for the acceleration
   const double r_acc_temp = std::rand();
   const double r_acc_normal = r_acc_temp / RAND_MAX; // Map it to [0,1]
   // Compute acceleration based on random number and as a function of
   // the spatial headway and the maximum velocity
   const unsigned r_acc = r_acc_normal * std::min(Maximum_velocity, spatial_headway);
   //DEB(r_acc);
   
   //const unsigned r_acc = 1;
   
   // First rule (acceleration)
   unsigned new_velocity = std::min(current_velocity + r_acc, Maximum_velocity);
   
   // Second rule (deceleration)
   new_velocity = std::min(new_velocity, spatial_headway);
   
   // Delay probability
   const double p_0 = P_0;
   const double p_1 = P_1;
   
   // Third rule (randomization) if new velocity is equal to zero
   if (new_velocity == 0)
    {
     const double r = std::rand();
     if ((r / RAND_MAX) <= p_0)
      {
       new_velocity = std::max(int(new_velocity - 1), 0);
       //std::cerr << "NV: " << new_velocity << std::endl;
      }
    }
   else // (new_velocity > 0)
    {
     const double r = std::rand();
     if ((r / RAND_MAX) <= p_1)
      {
       new_velocity = std::max(int(new_velocity - 1), 0);
       //std::cerr << "NV: " << new_velocity << std::endl;
      }
    }

   // Fourth rule (movement)
   unsigned new_position = current_position + new_velocity;
   if (new_position >= Lane_size)
    {
     new_position = new_position - Lane_size;
    }

   // Update velocity and positon of vehicle
   current_vehicle_pt->velocity(1) = new_velocity;
   current_vehicle_pt->position(1) = new_position;
   
   sum_velocity+=new_velocity;
   
  } // for (i < Current_number_of_vehicles)
 
 return sum_velocity;
 
}

// ----------------------------------------------------------------
// Update the lane status
// ---------------------------------------------------------------- 
void RandAcc::update()
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
void RandAcc::print(bool print_velocities)
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

