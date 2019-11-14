#include "cc_nasch_pbc.h"

namespace CA
{

 // ----------------------------------------------------------------
 // Constructor -- do nothing
 // ----------------------------------------------------------------
 NaSchPBC::NaSchPBC() 
 {
  
 }
 
 // ----------------------------------------------------------------
 // Constructor
 // ----------------------------------------------------------------
 NaSchPBC::NaSchPBC(unsigned long lane_size, unsigned maximum_velocity, double break_probability)
 {
  // Set lane configuration
  initialise(lane_size, maximum_velocity, break_probability); 
 }
 
 // ----------------------------------------------------------------
 // Destructor - do nothing
 // ----------------------------------------------------------------
 NaSchPBC::~NaSchPBC()
 {
  clear();
 }
 
 // ----------------------------------------------------------------
 // Initialise lane configuration
 // ----------------------------------------------------------------
 void NaSchPBC::initialise(unsigned long lane_size, unsigned maximum_velocity, double break_probability)
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
 void NaSchPBC::clear()
 {
  // Initialise data structures representing the lane
  Lane.clear();  
  Lane.resize(Lane_size, 0);
 
  // Free vehicles in lane memory
  for (unsigned long i = 0; i < Lane_size; i++)
   {
    if (Lane[i] != 0)
     {
      delete Lane[i];
      Lane[i] = 0;
     }
   }
 
  Vehicles_pt.resize(Lane_size, 0);
 
  // Reset lane configuration
  Number_of_vehicles = 0;
  Density = 0;
 }

 // ----------------------------------------------------------------
 // Fill in vehicles
 // ----------------------------------------------------------------
 void NaSchPBC::fill_in_vehicles(double density)
 {
  // Initialise data structures
  clear();

  // Used to get a seed for the random number engine
  std::random_device rd;
  // Standard mersenne_twister_engine seeded with rd()
  std::mt19937 gen(rd());

  // Use dist to generate a random number into a double in the range
  // [0,1)
  std::uniform_real_distribution<> dis(0.0, 1.0);
 
  // Set the density
  Density = density;
 
  // Compute the number of vehicles to be added to the lane
  Number_of_vehicles = Density * Lane_size;
 
  unsigned long i = 0;
  // Add vehicles in the lane randomly
  while(i < Number_of_vehicles)
   { 
    // Random position to add a vehicle
    const double r = dis(gen);
    unsigned long k = r * Lane_size;
    // Check whether there is a vehicle in the k lane position
    if (Lane[k] == 0)
     {
      // Create a new vehicle
      unsigned initial_velocity = 0;
      unsigned long initial_position = k;
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
 unsigned long NaSchPBC::update_vehicles_list()
 {
  // Get the new vehicles order
  unsigned long i = 0;
  for (unsigned long k = 0; k < Lane_size; k++)
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
 // Update lane based on NaSchPBC rules
 // ----------------------------------------------------------------
 unsigned NaSchPBC::apply_nasch()
 {
 
  // Accumulated velocity
  unsigned long sum_velocity = 0;
 
  // Used to get a seed for the random number engine
  std::random_device rd;
  // Standard mersenne_twister_engine seeded with rd()
  std::mt19937 gen(rd());
 
  // Use dist to generate a random number into a double in the range
  // [0,1)
  std::uniform_real_distribution<> dis(0.0, 1.0);
 
  for (unsigned long i = 0; i < Number_of_vehicles; i++)
   {
    // Get a pointer to the current vehicle
    Vehicle *current_vehicle_pt = Vehicles_pt[i];
    const unsigned long current_position = current_vehicle_pt->position();
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
    // NaSchPBC rules
    // -----------------------------------------------------------------
   
    // First rule (acceleration)
    unsigned new_velocity = std::min(current_velocity + 1, Maximum_velocity);
   
    // Second rule (deceleration)
    new_velocity = std::min(new_velocity, spatial_headway);
   
    // Third rule (randomization)
    const double r = dis(gen); 
    if (r <= Break_probability)
     {
      new_velocity = std::max(int(new_velocity - 1), 0);
      //std::cerr << "NV: " << new_velocity << std::endl;
     }
   
    // Fourth rule (movement)
    unsigned long new_position = current_position + new_velocity;
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
 void NaSchPBC::update()
 {
  for (unsigned long i = 0; i < Number_of_vehicles; i++)
   {
    // Get a pointer to the current vehicle
    Vehicle *vehicle_pt = Vehicles_pt[i];
    
    const unsigned long old_position = vehicle_pt->position(0);
    const unsigned long new_position = vehicle_pt->position(1);
    
    // Update the pointer on the lane
    Lane[old_position] = 0; // Delete the pointer from the old position
    Lane[new_position] = vehicle_pt;
    
    // Update vehicle
    vehicle_pt->update();
    
   }
  
 }
 
 // ----------------------------------------------------------------
 // Prints the lane status
 // ---------------------------------------------------------------- 
 void NaSchPBC::print(bool print_velocities)
 {
  for (unsigned long i = 0; i < Lane_size; i++)
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

 // ----------------------------------------------------------------
 // Output lane status
 // ---------------------------------------------------------------- 
 void NaSchPBC::output_time_space(std::ofstream &output_file)
 {
  //std::cout << Current_number_of_vehicles << std::endl;
  // Loop over the lane and output its state
  for (unsigned long k = 0; k < Lane_size - 1; k++)
   {
    if (Lane[k] != 0)
     {
      output_file << "1 ";
     }
    else
     {
      output_file << "0 ";
     }
   }

  // The last element in the vector
  if (Lane[Lane_size - 1] != 0)
   {
    output_file << "1";
   }
  else
   {
    output_file << "0";
   }
 
  output_file << std::endl;
 
 }

} // namespace CA
