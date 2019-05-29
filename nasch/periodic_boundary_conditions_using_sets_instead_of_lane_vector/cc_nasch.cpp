#include "cc_nasch.h"

bool compare_vehicles_by_position(Vehicle *a, Vehicle *b)
{
 return (a->position() < b->position());
}

// ----------------------------------------------------------------
// Constructor -- do nothing
// ----------------------------------------------------------------
NaSch::NaSch() 
{
 
}

// ----------------------------------------------------------------
// Constructor
// ----------------------------------------------------------------
NaSch::NaSch(unsigned lane_size, unsigned maximum_velocity, double break_probability)
{
 // Set lane configuration
 initialise(lane_size, maximum_velocity, break_probability); 
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
void NaSch::initialise(unsigned lane_size, unsigned maximum_velocity, double break_probability)
{
 // Set lane configuration
  Lane_size = lane_size;
  Maximum_velocity = maximum_velocity;
  Break_probability = break_probability;
 // Initialise data structures
 clear();
}
 
// ----------------------------------------------------------------
// Clear data structures
// ----------------------------------------------------------------
void NaSch::clear()
{
 Vehicles_pt.clear();
 
 // Reset lane configuration
 Number_of_vehicles = 0;
 Density = 0;
}

// ----------------------------------------------------------------
// Fill in vehicles
// ----------------------------------------------------------------
void NaSch::fill_in_vehicles(double density)
{
 // Set the density
 Density = density;
 
 // Compute the number of vehicles that will be added to the lane
 Number_of_vehicles = Density * Lane_size;
 
 // Create a set to keep track of the occupied positions
 std::set<unsigned> occupied_positions;
 
 unsigned i = 0;
 // Add vehicles in the lane randomly
 while(i < Number_of_vehicles)
  {
   // Random position to add a vehicle
   const double r = rand();
   unsigned k = (r / RAND_MAX) * Lane_size; 
   // Check whether there is a vehicle in the k lane position
   std::set<unsigned>::iterator it = occupied_positions.find(k);
   if (it == occupied_positions.end())
    {
     // Create a new vehicle
     unsigned initial_velocity = 0;
     unsigned initial_position = k;
     Vehicle *new_vehicle_pt = new Vehicle(initial_velocity, initial_position); 
     // Add a vehicle to the vehicles vector
     Vehicles_pt.push_back(new_vehicle_pt);
     // Increase the number of added vehicles to the lane
     i++;
     // Add the position to the occupied positions set
     occupied_positions.insert(k);
    }
   
  }
  
}

// ----------------------------------------------------------------
// Update vehicles list
// ----------------------------------------------------------------
unsigned NaSch::sort_vehicles_list()
{
 std::sort(Vehicles_pt.begin(), Vehicles_pt.end(), compare_vehicles_by_position);
 
 // Return the number of 'found' vehicles in the lane
 return 0;
 
} 

// ----------------------------------------------------------------
// Update lane based on NaSch rules
// ----------------------------------------------------------------
unsigned NaSch::apply_nasch()
{
  
 // Accumulated velocity
 unsigned sum_velocity = 0;
  
 for (unsigned i = 0; i < Number_of_vehicles; i++)
  {
   // Get a pointer to the current vehicle
   Vehicle *current_vehicle_pt = Vehicles_pt[i];
   const unsigned current_position = current_vehicle_pt->position();
   const unsigned current_velocity = current_vehicle_pt->velocity();
   
   // -----------------------------------------------------------------
   // Compute the spatial headway (the empty spaces between vehicles)
   // -----------------------------------------------------------------
   unsigned spatial_headway = 0;
   // Is this the last vehicle
   if (i + 1 == Number_of_vehicles)
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
   if (new_position >= Lane_size)
    {
     new_position = new_position - Lane_size;
    }
    
   // Update velocity and positon of vehicle
   current_vehicle_pt->velocity(1) = new_velocity;
   current_vehicle_pt->position(1) = new_position;
      
   sum_velocity+=new_velocity; 
    
  } // for (i < Number_of_vehicles)
  
 return sum_velocity;
  
}

// ----------------------------------------------------------------
// Update the lane status
// ---------------------------------------------------------------- 
void NaSch::update()
{
 for (unsigned i = 0; i < Number_of_vehicles; i++)
  {
   // Get a pointer to the current vehicle
   Vehicle *vehicle_pt = Vehicles_pt[i];
   
   // Update vehicle
   vehicle_pt->update();
   
  }
 
}


// ----------------------------------------------------------------
// Prints the lane status
// ----------------------------------------------------------------
void NaSch::print(bool print_velocities)
{
 //std::sort(Vehicles_pt.begin(), Vehicles_pt.end());
 
 for (unsigned i = 0; i < Number_of_vehicles; i++)
  {
   Vehicle *vehicle_pt = Vehicles_pt[i];
   if (vehicle_pt != 0)
    {
     if (print_velocities)
      {
       //const unsigned velocity = vehicle_pt->velocity();
       //std::cerr << velocity;
       const unsigned position = vehicle_pt->position();
       std::cerr << position;
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

