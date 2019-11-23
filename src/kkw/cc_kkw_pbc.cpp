#include "cc_kkw_pbc.h"

namespace CA
{

 // ----------------------------------------------------------------
 // Constructor -- do nothing
 // ----------------------------------------------------------------
 KKWPBC::KKWPBC()
 {
  
 }
 
 // ----------------------------------------------------------------
 // Constructor (with its corresponding KKW parameters, default values
 // based on section 8.3.2, page 301 of Stochastic Transport in
 // Complex Systems: From Molecules to Vehicles, 2011) book and
 // Cellular Automata approach to three-phase traffic theory, Boris S
 // Kerner, Sergey L Klenov and Dietrich E Wolf, Journal of Physics A:
 // Mathematical and General, 35, 9971-10013, (2002)
 // ----------------------------------------------------------------
 KKWPBC::KKWPBC(unsigned lane_size, unsigned maximum_velocity,
                unsigned a, unsigned b, Real k,
                Real p, Real p_0, Real p_a1, Real p_a2, Real v_p)
 {
  // Set lane configuration
  initialise(lane_size, maximum_velocity, a, b, k, p, p_0, p_a1, p_a2, v_p);
 }
 
 // ----------------------------------------------------------------
 // Destructor - do nothing
 // ----------------------------------------------------------------
 KKWPBC::~KKWPBC()
 {
  clear();
 } 

 // ----------------------------------------------------------------
 // Initialise lane configuration, default values based on section
 // 8.3.2, page 301 of Stochastic Transport in Complex Systems: From
 // Molecules to Vehicles, 2011) book and Cellular Automata approach
 // to three-phase traffic theory, Boris S Kerner, Sergey L Klenov
 // and Dietrich E Wolf, Journal of Physics A: Mathematical and
 // General, 35, 9971-10013, (2002)
 // ----------------------------------------------------------------
 void KKWPBC::initialise(unsigned lane_size, unsigned maximum_velocity,
                         unsigned a, unsigned b, Real k, Real p,
                         Real p_0, Real p_a1, Real p_a2, Real v_p)
 {
  // Set lane configuration
  Lane_size = lane_size;
  Maximum_velocity = maximum_velocity;
  A = a;
  B = b;
  K = k;
  P = p;
  P_0 = p_0;
  P_a1 = p_a1;
  P_a2 = p_a2;
  V_p = v_p;
  
  // No vehicle has traverse the lane
  N_vehicles_complete_travel = 0;
  
  // Set syncronisation function
  set_linear_syncronisation_function();
  //set_quadratic_syncronisation_function();
  
  // Initialise data structures
  clear(); 
 }
 
 // ----------------------------------------------------------------
 // Set bumps
 // ----------------------------------------------------------------
 void KKWPBC::set_bumps(std::vector<unsigned> &bumps_positions)
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
 // Clear data structures
 // ----------------------------------------------------------------
 void KKWPBC::clear()
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
  
 }
 
 // ----------------------------------------------------------------
 // Fill in vehicles
 // ----------------------------------------------------------------
 void KKWPBC::fill_in_vehicles(Real density)
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
      Vehicle *new_vehicle_pt = new Vehicle(initial_velocity, initial_position); 
      // Add a vehicle
      Lane[k] = new_vehicle_pt;
      // Increase the number of added vehicles to the lane
      i++;
     }
    
   } // while(i < Current_number_of_vehicles)
  
 }
 
 // ----------------------------------------------------------------
 // Update vehicles list
 // ----------------------------------------------------------------
 unsigned KKWPBC::update_vehicles_list()
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
 // Update lane based on KKWPBC rules
 // ----------------------------------------------------------------
 void KKWPBC::apply_kkw(Real &mean_velocity, Real &mean_current, Real &mean_delay,
                        unsigned &sum_travel_time, Real &mean_travel_time,
                        Real &mean_queue_length,
                        Real &mean_co2, Real &mean_nox, Real &mean_voc, Real &mean_pm)
 {
  // Accumulated velocity
  unsigned sum_velocity = 0;
  
  // Accumulated delay
  unsigned sum_delay = 0;
  
  // Emissions
  Real sum_co2 = 0.0;
  Real sum_nox = 0.0;
  Real sum_voc = 0.0;
  Real sum_pm = 0.0;
  
  std::vector<unsigned> queues_length;
  unsigned current_queue_length = 0;
  
  // Used to get a seed for the random number engine
  std::random_device rd;
  // Standard mersenne_twister_engine seeded with rd()
  std::mt19937 gen(rd());
  
  // Use dist to generate a random number into a Real in the range
  // [0,1)
  std::uniform_real_distribution<> dis(0.0, 1.0);
  
  for (unsigned i = 0; i < current_number_of_vehicles(); i++)
   {
    // Get a pointer to the current vehicle
    Vehicle *current_vehicle_pt = this->Vehicles_pt[i];
    const unsigned current_position = current_vehicle_pt->position();
    const unsigned current_velocity = current_vehicle_pt->velocity();
    const Real current_velocity_in_km_per_hour = current_vehicle_pt->velocity_in_km_per_hour();
    
    // -----------------------------------------------------------------
    // Compute the spatial headway (the empty spaces between vehicles)
    // -----------------------------------------------------------------
    unsigned spatial_headway = 0;
    // Velocity difference
    int velocity_difference_with_front_vehicle = 0;
    // Is this the last vehicle
    if (i + 1 == current_number_of_vehicles())
     {
      Vehicle *next_vehicle_pt = this->Vehicles_pt[0];
      spatial_headway = (next_vehicle_pt->position() + lane_size()) - current_position - 1;
      velocity_difference_with_front_vehicle = next_vehicle_pt->velocity() - current_velocity;
     }
    else
     {
      Vehicle *next_vehicle_pt = this->Vehicles_pt[i+1];
      spatial_headway = next_vehicle_pt->position() - current_position - 1;
      velocity_difference_with_front_vehicle = next_vehicle_pt->velocity() - current_velocity;
     }
    
    //std::cerr << i + 1 << ":" << current_position << "-" << spatial_headway << std::endl;
    
    // -----------------------------------------------------------------
    // KKW-1 Deterministic rule
    // -----------------------------------------------------------------

    // ***************
    // Safe velocity
    int v_safe = spatial_headway;
    
    // *************** 
    // Desired velocity (considering velocity of front vehicle)
    int v_des = 0;
    int acc = 1;
    
    const unsigned length = current_vehicle_pt->length();
    // Conversion factor length/occupancy for velocity
    const Real v_eta = current_vehicle_pt->eta();
    const Real d_value = D(v_eta*current_velocity, length) - length; // in meters
    if (spatial_headway*v_eta > d_value) // in meters
     {
      v_des = current_velocity + acc; // NaSch condition
     }
    else // if (spatial_headway*v_eta <= d_value)
     {
      // Current vehicle's velocity is greater than front vehicle's
      // velocity
      if (velocity_difference_with_front_vehicle < 0)
       {
        acc = -B;
       }
      // Current vehicle's velocity is equal to front vehicle's
      // velocity
      else if (velocity_difference_with_front_vehicle == 0)
       {
        acc = 0;
       }
      // Current vehicle's velocity is smaller than front vehicle's
      // velocity
      else // if (velocity_difference_with_front_vehicle > 0)
       {
        acc = A;
       }
      
      // Update desired velocity
      v_des = current_velocity + acc; // KKW for desired velocity
      
     }
    
    // New velocity for deterministic stage
    unsigned v_tilde = std::max(0, std::min(static_cast<int>(Maximum_velocity), std::min(v_safe, v_des) ) );
    
    // -----------------------------------------------------------------
    // KKW-2 Probabilistic rule
    // -----------------------------------------------------------------

    // KKW probabilities
    Real pb = 0;
    if (current_velocity == 0)
     {
      pb = P_0;
     }
    else if (current_velocity > 0)
     {
      pb = P;
     }
    
    // KKW probabilities
    Real pa = 0;
    if (current_velocity_in_km_per_hour < V_p)
     {
      pa = P_a1;
     }
    else // if (current_velocity_in_km_per_hour >= V_p)
     {
      pa = P_a2;
     }
    
    // Random value
    const Real r = dis(gen);
    
    int eta = 0;
    if (r < pb)
     {
      eta = -1;
     }
    else if (pb <= r && r < (pb + pa))
     {
      eta = 1;
     }
    else
     {
      eta = 0;
     }
    
    const unsigned v_free = Maximum_velocity;
    unsigned new_velocity = std::max(0, std::min(static_cast<int>(v_tilde) + 1*eta, static_cast<int>(std::min(v_tilde + 1, std::min(v_free, spatial_headway) ) ) ) );
    
#if 0
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
        // Compute a random value to capture the behaviour of drivers
        // when encountering a bump
        const Real r_d = dis(gen);
        const unsigned r_velocity_due_to_distance_to_bump = std::max(static_cast<unsigned>(r_d * distance_to_closest_bump), static_cast<unsigned>(1));
        
        new_velocity = std::min(r_velocity_due_to_distance_to_bump, spatial_headway);
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
#endif // #if 0
    
    // **********************************
    // Compute new position
     unsigned new_position = current_position + new_velocity;
    if (new_position >= Lane_size)
     {
      new_position = new_position - Lane_size;
      
      // Increase the number of vehicles that have traverse the lane
      N_vehicles_complete_travel++;
      // Add up to the travel time sum
      sum_travel_time+=current_vehicle_pt->travel_time();
      // Reset travel time of vehicle
      current_vehicle_pt->travel_time() = 0;
     }
    
    // Update velocity and positon of vehicle
    current_vehicle_pt->velocity(1) = new_velocity;
    current_vehicle_pt->position(1) = new_position;
    
    sum_velocity+=new_velocity;
    
    // Get the current travel time of the vehicle and add it up to
    // the travel time of the lane
    sum_delay+=current_vehicle_pt->delay();
    
    // Check the length of the queue
    if (spatial_headway==0)
     {
      // Increase counter for the size of the queue if there are no
      // free space at the front
      current_queue_length++;
     }
    else 
     {
      // If there was a queue then store the length of the queue
      if (current_queue_length>0)
       {
        queues_length.push_back(current_queue_length);
        // Restart length of queue
        current_queue_length = 0;
       }
     }
    
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
    
   } // for (i < current_number_of_vehicles())
  
  if (Current_number_of_vehicles > 0)
   {
    mean_velocity=static_cast<Real>(sum_velocity)/static_cast<Real>(Current_number_of_vehicles);
    mean_delay=static_cast<Real>(sum_delay)/static_cast<Real>(Current_number_of_vehicles);
   }
  else
   {
    mean_velocity=0;
    mean_delay=0;
   }
 
  mean_current=static_cast<Real>(sum_velocity)/static_cast<Real>(Lane_size);
 
  if (N_vehicles_complete_travel > 0)
   {
    mean_travel_time=static_cast<Real>(sum_travel_time)/static_cast<Real>(N_vehicles_complete_travel);
   }
  else
   {
    mean_travel_time = 0;
   }
  
  unsigned sum_queue_size = 0;
  // Get the number of queues 
  const unsigned n_queues = queues_length.size();
  for (unsigned i = 0; i < n_queues; i++)
   {
    sum_queue_size+=queues_length[i];
   }

  if (n_queues > 0)
   {
    mean_queue_length = static_cast<Real>(sum_queue_size)/static_cast<Real>(n_queues);
   }
  else
   {
    mean_queue_length = 0;
   }

  // Emssions
  if (Current_number_of_vehicles > 0)
   {
    mean_co2=static_cast<Real>(sum_co2)/static_cast<Real>(Current_number_of_vehicles);
    mean_nox=static_cast<Real>(sum_nox)/static_cast<Real>(Current_number_of_vehicles);
    mean_voc=static_cast<Real>(sum_voc)/static_cast<Real>(Current_number_of_vehicles);
    mean_pm=static_cast<Real>(sum_pm)/static_cast<Real>(Current_number_of_vehicles);
   }
  else
   {
    mean_co2=0;
    mean_nox=0;
    mean_voc=0;
    mean_pm=0;
   } 
 
 }

 // ----------------------------------------------------------------
 // Update the lane status
 // ---------------------------------------------------------------- 
 void KKWPBC::update()
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
    
   }
  
 }
  
 // ----------------------------------------------------------------
 // Prints the lane status
 // ---------------------------------------------------------------- 
 void KKWPBC::print(bool print_velocities)
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
  
 // ----------------------------------------------------------------
 // Output lane status
 // ---------------------------------------------------------------- 
 void KKWPBC::output_time_space(std::ofstream &output_file)
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

 // ----------------------------------------------------------------
 // Synchronization distance, it may implement a linear or a quatratic
 // function. Requires current velocity and vehicles length. Beta
 // value based on "Cellular Automata approach to three-phase traffic
 // theory", Boris S Kerner, Sergey L Klenov and Dietrich E Wolf,
 // Journal of Physics A: Mathematical and General, 35, 9971-10013,
 // (2002)
 // ----------------------------------------------------------------
 Real KKWPBC::D(const unsigned v, const unsigned l)
 {
  if (kkw_d_function == KKW_D_Function::LINEAR)
   {
    return l + K*v;
   }
  else
   {
    const Real beta = 0.05;
    return l + v + beta*v*v;
   }
   
 }
 
 // ----------------------------------------------------------------
 // Compute the distance to the nearest bump
 // ----------------------------------------------------------------
 unsigned KKWPBC::distance_to_nearest_bump(unsigned position)
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
 
} // namespace CA
