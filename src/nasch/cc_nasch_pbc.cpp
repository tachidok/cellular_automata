#include "cc_nasch_pbc.h"

namespace CA
{

 // ----------------------------------------------------------------
 /// Constructor -- do nothing
 // ----------------------------------------------------------------
 CCNaSchPBC::CCNaSchPBC() 
 {
  
 }
 
 // ----------------------------------------------------------------
 /// Constructor
 // ----------------------------------------------------------------
 CCNaSchPBC::CCNaSchPBC(unsigned lane_size, unsigned maximum_velocity, Real break_probability)
 {
  // Set lane configuration
  initialise(lane_size, maximum_velocity, break_probability); 
 }
 
 // ----------------------------------------------------------------
 /// Destructor - do nothing
 // ----------------------------------------------------------------
 CCNaSchPBC::~CCNaSchPBC()
 {
  clear();
 }
 
 // ----------------------------------------------------------------
 /// Initialise lane configuration
 // ----------------------------------------------------------------
 void CCNaSchPBC::initialise(unsigned lane_size, unsigned maximum_velocity, Real braking_probability)
 {
  // Set lane configuration
  Lane_size = lane_size;
  Maximum_velocity = maximum_velocity;
  Braking_probability = braking_probability;
  
  // Initialise data structures
  clear();
 }
 
 // ----------------------------------------------------------------
 /// Clear data structures
 // ----------------------------------------------------------------
 void CCNaSchPBC::clear()
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
  
  // Get the number of bumps
  const unsigned n_bumps = Bumps_pt.size();
  // Clear bumps
  for (unsigned i = 0; i < n_bumps; i++)
   {
    if (Bumps_pt[i] != 0)
     {
      delete Bumps_pt[i];
      Bumps_pt[i] = 0;
     }
   }
  
  // Reset lane configuration
  Current_number_of_vehicles = 0;
  Density = 0;
 }
 
 // ----------------------------------------------------------------
 /// Set bumps
 // ----------------------------------------------------------------
 void CCNaSchPBC::set_bumps(std::vector<unsigned> &bumps_positions)
 {
  const unsigned n_bumps = bumps_positions.size();
  for (unsigned i = 0; i < n_bumps; i++)
   {
    // Create a bump
    CCBump* bump_pt = new CCBump(bumps_positions[i]);
    Bumps_pt.push_back(bump_pt);
   }
 
 }
 
 // ----------------------------------------------------------------
 /// Fill in vehicles
 // ----------------------------------------------------------------
 void CCNaSchPBC::fill_in_vehicles(Real density)
 {
  // Initialise data structures
  clear();

  // Used to get a seed for the random number engine
  std::random_device rd;
  // Standard mersenne_twister_engine seeded with rd()
  std::mt19937 gen(rd());

  // Use dist to generate a random number into a Real in the range
  // [0,1)
  std::uniform_real_distribution<> dis(0.0, 1.0);
 
  // Set the density
  Density = density;
  
  // Compute the number of vehicles to be added to the lane
  Current_number_of_vehicles = Density * Lane_size;
  
  unsigned i = 0;
  // If the number of vehicles is equal or greater than the lane size
  // then fill it with vehicles
  if (Current_number_of_vehicles >= Lane_size)
   {
    // Add vehicles in the lane randomly
    while(i < Current_number_of_vehicles)
     {
      // Create a new vehicle
      unsigned initial_velocity = 0;
      unsigned initial_position = i;
      CCVehicle *new_vehicle_pt = new CCVehicle(initial_velocity, initial_position); 
      // Add a vehicle
      Lane[i] = new_vehicle_pt;
      // Increase the number of added vehicles to the lane
      i++;
      
     } // while(i < Current_number_of_vehicles)
   }
  else
   {
    // Add vehicles in the lane randomly
    while(i < Current_number_of_vehicles)
     {
      // Random position to add a vehicle
      const Real r = dis(gen);
      unsigned k = r * Lane_size;
      // Check whether there is a vehicle in the k lane position
      if (Lane[k] == 0)
       {
        // Create a new vehicle
        unsigned initial_velocity = 0;
        unsigned initial_position = k;
        CCVehicle *new_vehicle_pt = new CCVehicle(initial_velocity, initial_position); 
        // Add a vehicle
        Lane[k] = new_vehicle_pt;
        // Increase the number of added vehicles to the lane
        i++;
       }
      
     } // while(i < Current_number_of_vehicles)
    
   }
  
 }
 
 // ----------------------------------------------------------------
 /// Update vehicles list
 // ----------------------------------------------------------------
 unsigned CCNaSchPBC::update_vehicles_list()
 {
  // Get the new vehicles order
  unsigned i = 0;
  for (unsigned k = 0; k < Lane_size; k++)
   {
    if (Lane[k] != 0)
     {
      // Get a pointer to the vehicle
      CCVehicle *vehicle_pt = Lane[k];
     
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
 /// Set all statistics values to zero
 // ----------------------------------------------------------------
 void CCNaSchPBC::reset_statistics()
 {
  Mean_velocity = 0;
  Mean_current = 0;
  Mean_co2 = 0;
  Mean_nox = 0;
  Mean_voc = 0;
  Mean_pm = 0;
  Std_velocity = 0;
  Std_co2 = 0;
  Std_nox = 0;
  Std_voc = 0;
  Std_pm = 0;
 }
 
 // ----------------------------------------------------------------
 /// Update lane based on CCNaSchPBC rules
 // ----------------------------------------------------------------
 void CCNaSchPBC::apply_nasch()
 {
  // Set all statistics values to zero
  reset_statistics();
  
  // Temporarly store the new values for each vehicle so we can
  // compute standard deviations
  std::vector<unsigned> tmp_vector_velocity(Current_number_of_vehicles, 0);
  std::vector<unsigned> tmp_vector_co2(Current_number_of_vehicles, 0);
  std::vector<unsigned> tmp_vector_nox(Current_number_of_vehicles, 0);
  std::vector<unsigned> tmp_vector_voc(Current_number_of_vehicles, 0);
  std::vector<unsigned> tmp_vector_pm(Current_number_of_vehicles, 0);
  
  // Accumulated velocity
  unsigned sum_velocity = 0;
    
  // Emissions
  Real sum_co2 = 0.0;
  Real sum_nox = 0.0;
  Real sum_voc = 0.0;
  Real sum_pm = 0.0;
    
  // Used to get a seed for the random number engine
  std::random_device rd;
  // Standard mersenne_twister_engine seeded with rd()
  std::mt19937 gen(rd());
  
  // Use dist to generate a random number into a Real in the range
  // [0,1)
  std::uniform_real_distribution<> dis(0.0, 1.0);
  
  for (unsigned i = 0; i < Current_number_of_vehicles; i++)
   {
    // Get a pointer to the current vehicle
    CCVehicle *current_vehicle_pt = Vehicles_pt[i];
    const unsigned current_position = current_vehicle_pt->position();
    const unsigned current_velocity = current_vehicle_pt->velocity();
    
    // -----------------------------------------------------------------
    // Compute the spatial headway (the empty spaces between vehicles)
    // -----------------------------------------------------------------
    unsigned spatial_headway = 0;
    // Is this the last vehicle
    if (i + 1 == Current_number_of_vehicles)
     {
      CCVehicle *next_vehicle_pt = Vehicles_pt[0];
      spatial_headway = (next_vehicle_pt->position() + Lane_size) - current_position - 1;
     }
    else
     {
      CCVehicle *next_vehicle_pt = Vehicles_pt[i+1];
      spatial_headway = next_vehicle_pt->position() - current_position - 1;
     }
    
    //std::cerr << i + 1 << ":" << current_position << "-" << spatial_headway << std::endl;
    
    // -----------------------------------------------------------------
    // CCNaSchPBC rules
    // -----------------------------------------------------------------
   
    // First rule (acceleration)
    unsigned new_velocity = std::min(current_velocity + 1, Maximum_velocity);
   
    // Second rule (deceleration)
    // Second rule (deceleration)
    // Are there bumps?
    const unsigned n_bumps = nbumps();
    if (n_bumps > 0)
     {
      // Get the distance to the closest bump
      const unsigned distance_to_closest_bump = distance_to_nearest_bump(current_position);
      // If the car is already at the bump then reduce the velocity to that allowed by the bump
      if (distance_to_closest_bump == 0)
       {
        // This may be a function of the type of bump
        const unsigned max_velocity_to_pass_bump = 1;
        new_velocity = std::min(max_velocity_to_pass_bump, spatial_headway);
       }
      // If the bump is close then reduce velocity accordingly
      else if (distance_to_closest_bump < new_velocity)
       {
        new_velocity = std::min(distance_to_closest_bump, spatial_headway);
       }
      // If bump is no close enough then use new velocity
      else
       {
        new_velocity = std::min(new_velocity, spatial_headway);
       }
     }
    // No bumps
    else
     {
      new_velocity = std::min(new_velocity, spatial_headway);
     }
    
    // Third rule (randomization)
    const Real r = dis(gen); 
    if (r <= Braking_probability)
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
        
    // Emissions
    Real tmp_co2 = 0.0;
    Real tmp_nox = 0.0;
    Real tmp_voc = 0.0;
    Real tmp_pm = 0.0;
    current_vehicle_pt->compute_emissions(tmp_co2, tmp_nox, tmp_voc, tmp_pm);
    
    // Add up emissions of current vehicle
    sum_co2+=tmp_co2;
    sum_nox+=tmp_nox;
    sum_voc+=tmp_voc;
    sum_pm+=tmp_pm;
    
    // Keep track of each vehicles associated values
    tmp_vector_velocity[i] = new_velocity;
    tmp_vector_co2[i] = tmp_co2;
    tmp_vector_nox[i] = tmp_nox;
    tmp_vector_voc[i] = tmp_voc;
    tmp_vector_pm[i] = tmp_pm;
    
   } // for (i < Current_number_of_vehicles)
  
  if (Current_number_of_vehicles > 0)
   {
    Mean_velocity=static_cast<Real>(sum_velocity)/static_cast<Real>(Current_number_of_vehicles);
   }
  else
   {
    Mean_velocity=0;
   }
  
  Mean_current=static_cast<Real>(sum_velocity)/static_cast<Real>(Lane_size);
  
  // Emssions
  if (Current_number_of_vehicles > 0)
   {
    Mean_co2=static_cast<Real>(sum_co2)/static_cast<Real>(Current_number_of_vehicles);
    Mean_nox=static_cast<Real>(sum_nox)/static_cast<Real>(Current_number_of_vehicles);
    Mean_voc=static_cast<Real>(sum_voc)/static_cast<Real>(Current_number_of_vehicles);
    Mean_pm=static_cast<Real>(sum_pm)/static_cast<Real>(Current_number_of_vehicles);
   }
  else
   {
    Mean_co2=0;
    Mean_nox=0;
    Mean_voc=0;
    Mean_pm=0;
   } 
  
  // Compute standard deviations
  Std_velocity = 0;
  Std_co2 = 0;
  Std_nox = 0;
  Std_voc = 0;
  Std_pm = 0;
  
  for (unsigned i = 0; i < Current_number_of_vehicles; i++)
   {
    Std_velocity+=(tmp_vector_velocity[i] - Mean_velocity)*(tmp_vector_velocity[i] - Mean_velocity);
    Std_co2+=(tmp_vector_co2[i] - Mean_co2)*(tmp_vector_co2[i] - Mean_co2);
    Std_nox+=(tmp_vector_nox[i] - Mean_nox)*(tmp_vector_nox[i] - Mean_nox);
    Std_voc+=(tmp_vector_voc[i] - Mean_voc)*(tmp_vector_voc[i] - Mean_voc);
    Std_pm+=(tmp_vector_pm[i] - Mean_pm)*(tmp_vector_pm[i] - Mean_pm);
   }
  
  Std_velocity=sqrt(Std_velocity/static_cast<Real>(Current_number_of_vehicles));
  Std_co2=sqrt(Std_co2/static_cast<Real>(Current_number_of_vehicles));
  Std_nox=sqrt(Std_nox/static_cast<Real>(Current_number_of_vehicles));
  Std_voc=sqrt(Std_voc/static_cast<Real>(Current_number_of_vehicles));
  Std_pm=(Std_pm/static_cast<Real>(Current_number_of_vehicles));
  
 }
 
 // ----------------------------------------------------------------
 /// Update the lane status
 // ---------------------------------------------------------------- 
 void CCNaSchPBC::update()
 {
  for (unsigned i = 0; i < Current_number_of_vehicles; i++)
   {
    // Get a pointer to the current vehicle
    CCVehicle *vehicle_pt = Vehicles_pt[i];
    
    const unsigned old_position = vehicle_pt->position(0);
    const unsigned new_position = vehicle_pt->position(1);
    
    // Update the pointer on the lane
    Lane[old_position] = 0; // Delete the pointer from the old position
    Lane[new_position] = vehicle_pt;
    
    // Update vehicle
    vehicle_pt->update();
    
   }
  
 }

 // ----------------------------------------------------------------
 /// Compute the distance to the nearest bump
 // ----------------------------------------------------------------
 unsigned CCNaSchPBC::distance_to_nearest_bump(unsigned position)
 {
  // Get the number of bumps in the lane
  const unsigned n_bumps = Bumps_pt.size();
  unsigned gap_to_nearest_bump = this->lane_size();
  // Loop over the bumps and compute the distance to the nearest bump
  for (unsigned i = 0; i < n_bumps; i++)
   {
    const unsigned i_bump_position = Bumps_pt[i]->position();
    if (i_bump_position >= position)
     {
      const int long distance = i_bump_position - position;
      if (distance < static_cast<int long>(gap_to_nearest_bump))
       {
        gap_to_nearest_bump = distance;
       }
     }
    
   }
 
  return gap_to_nearest_bump;
 
 }
 
 // ----------------------------------------------------------------
 /// Prints the lane status
 // ---------------------------------------------------------------- 
 void CCNaSchPBC::print(bool print_velocities)
 {
  for (unsigned i = 0; i < Lane_size; i++)
   {
    CCVehicle *vehicle_pt = Lane[i];
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
 /// Output lane status
 // ---------------------------------------------------------------- 
 void CCNaSchPBC::output_time_space(std::ofstream &output_file)
 {
  //std::cout << Current_number_of_vehicles << std::endl;
  // Loop over the lane and output its state
  for (unsigned k = 0; k < Lane_size - 1; k++)
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
