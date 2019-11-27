#include "cc_rand_acc_pbc.h"

namespace CA
{

 // ----------------------------------------------------------------
 // Constructor -- do nothing
 // ----------------------------------------------------------------
 RandAccPBC::RandAccPBC() 
 {
  
 }
 
 // ----------------------------------------------------------------
 // Constructor
 // ----------------------------------------------------------------
 RandAccPBC::RandAccPBC(unsigned lane_size, unsigned maximum_velocity,
                        Real p0, Real p1)
 {
  // Set lane configuration
  initialise(lane_size, maximum_velocity, p0, p1);
 }

 // ----------------------------------------------------------------
 // Destructor - do nothing
 // ----------------------------------------------------------------
 RandAccPBC::~RandAccPBC()
 {
  clear();
 }
 
 // ----------------------------------------------------------------
 // Initialise lane configuration
 // ----------------------------------------------------------------
 void RandAccPBC::initialise(unsigned lane_size, unsigned maximum_velocity, Real p0, Real p1)
 {
  // Set lane configuration
  Lane_size = lane_size;
  Maximum_velocity = maximum_velocity;
  P_0 = p0;
  P_1 = p1;
 
  // No vehicle has traverse the lane
  N_vehicles_complete_travel = 0;
 
  // Initialise data structures
  clear();
 }

 // ----------------------------------------------------------------
 // Clear data structures
 // ----------------------------------------------------------------
 void RandAccPBC::clear()
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
 void RandAccPBC::fill_in_vehicles(Real density)
 {
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
      Vehicle *new_vehicle_pt = new Vehicle(initial_velocity, initial_position); 
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
        Vehicle *new_vehicle_pt = new Vehicle(initial_velocity, initial_position); 
        // Add a vehicle
        Lane[k] = new_vehicle_pt;
        // Increase the number of added vehicles to the lane
        i++;
       }
      
     } // while(i < Current_number_of_vehicles)
    
   }
  
 }

 // ----------------------------------------------------------------
 // Update vehicles list
 // ----------------------------------------------------------------
 unsigned RandAccPBC::update_vehicles_list()
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
 // Update lane based on RandAccPBC rules
 // ----------------------------------------------------------------
 void RandAccPBC::apply_rand_acc(Real &mean_velocity, Real &mean_current, Real &mean_delay,
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
    const Real r = dis(gen);
    // Compute acceleration based on random number and as a function of
    // the spatial headway and the maximum velocity. Also make sure
    // that there is at least one acceleration step
    unsigned r_acc = std::max(static_cast<unsigned>(r * std::min(Maximum_velocity, spatial_headway)), static_cast<unsigned>(1));
    //DEB(r_acc);
   
    //const unsigned r_acc = 1;
   
    // First rule (acceleration)
    unsigned new_velocity = std::min(current_velocity + r_acc, Maximum_velocity);
   
    // Second rule (deceleration)
    new_velocity = std::min(new_velocity, spatial_headway);
   
    // Delay probability
    const Real p_0 = P_0;
    const Real p_1 = P_1;
    
    // Third rule (randomization) if new velocity is equal to zero
    if (new_velocity == 0)
     {
      const Real r0 = dis(gen);
      if (r0 <= p_0)
       {
        new_velocity = std::max(int(new_velocity - 1), 0);
        //std::cerr << "NV: " << new_velocity << std::endl;
       }
     }
    else // (new_velocity > 0)
     {
      const Real r1 = dis(gen); 
      if (r1 <= p_1)
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
    
   } // for (i < Current_number_of_vehicles)

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
 void RandAccPBC::update()
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
 void RandAccPBC::print(bool print_velocities)
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
 void RandAccPBC::output_time_space(std::ofstream &output_file)
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
