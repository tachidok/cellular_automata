#include "cc_nasch_pbcwb.h"

namespace CA
{

 // ----------------------------------------------------------------
 // Constructor -- do nothing
 // ----------------------------------------------------------------
 NaSchPBCwb::NaSchPBCwb()
  : NaSchPBC()
 {
  
 }
 
 // ----------------------------------------------------------------
 // Constructor
 // ----------------------------------------------------------------
 NaSchPBCwb::NaSchPBCwb(unsigned lane_size, unsigned maximum_velocity,
                        Real break_probability)
  : NaSchPBC(lane_size,  maximum_velocity, break_probability)
 {
  
 }
 
 // ----------------------------------------------------------------
 // Destructor - do nothing
 // ----------------------------------------------------------------
 NaSchPBCwb::~NaSchPBCwb()
 {
  clear();
 } 

 // ----------------------------------------------------------------
 // Clear data structures
 // ----------------------------------------------------------------
 void NaSchPBCwb::clear()
 {
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
 
  // Call parent class
  //this->clear();
 
 }

 // ----------------------------------------------------------------
 // Set bumps
 // ----------------------------------------------------------------
 void NaSchPBCwb::set_bumps(std::vector<unsigned> &bumps_positions)
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
 // Update lane based on NaSchPBCwb rules
 // ----------------------------------------------------------------
 void NaSchPBCwb::apply_nasch(Real &mean_velocity, Real &mean_current, Real &mean_delay,
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
    
    // -----------------------------------------------------------------
    // Compute the spatial headway (the empty spaces between vehicles)
    // -----------------------------------------------------------------
    unsigned spatial_headway = 0;
    // Is this the last vehicle
    if (i + 1 == current_number_of_vehicles())
     {
      Vehicle *next_vehicle_pt = this->Vehicles_pt[0];
      spatial_headway = (next_vehicle_pt->position() + lane_size()) - current_position - 1;
     }
   else
    {
     Vehicle *next_vehicle_pt = this->Vehicles_pt[i+1];
     spatial_headway = next_vehicle_pt->position() - current_position - 1;
    }
   
   //std::cerr << i + 1 << ":" << current_position << "-" << spatial_headway << std::endl;
   
   // -----------------------------------------------------------------
   // NaSchPBC rules
   // -----------------------------------------------------------------
   
   // First rule (acceleration)
   unsigned new_velocity = std::min(current_velocity + 1, Maximum_velocity);
   
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
   
   // Third rule (randomization)
   const Real r = dis(gen); 
   if (r <= Break_probability)
    {
     new_velocity = std::max(int(new_velocity - 1), 0);
     //std::cerr << "NV: " << new_velocity << std::endl;
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
// Compute the distance to the nearest bump
// ----------------------------------------------------------------
 unsigned NaSchPBCwb::distance_to_nearest_bump(unsigned position)
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
