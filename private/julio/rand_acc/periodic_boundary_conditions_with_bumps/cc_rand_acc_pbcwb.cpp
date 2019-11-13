#include "cc_rand_acc_pbcwb.h"

// ----------------------------------------------------------------
// Constructor -- do nothing
// ----------------------------------------------------------------
RandAccPBCwb::RandAccPBCwb()
 : RandAccPBC()
{
  
}
 
// ----------------------------------------------------------------
// Constructor
// ----------------------------------------------------------------
RandAccPBCwb::RandAccPBCwb(unsigned long lane_size, unsigned maximum_velocity,
                           double p0, double p1)
 : RandAccPBC(lane_size, maximum_velocity, p0, p1)
{
 
}

// ----------------------------------------------------------------
// Destructor - do nothing
// ----------------------------------------------------------------
RandAccPBCwb::~RandAccPBCwb()
{
 clear();
} 

// ----------------------------------------------------------------
// Clear data structures
// ----------------------------------------------------------------
void RandAccPBCwb::clear()
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
void RandAccPBCwb::set_bumps(std::vector<unsigned long> &bumps_positions)
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
// Update lane based on RandAccPBCwb rules
// ----------------------------------------------------------------
void RandAccPBCwb::apply_rand_acc(Real &mean_velocity, Real &mean_current, unsigned &mean_travel_time, unsigned &mean_delay)
{
 // Accumulated velocity
 unsigned sum_velocity = 0;
 // Accumulated travel time
 unsigned sum_travel_time = 0;
 // Accumulated delay
 unsigned sum_delay = 0;
 
 // Used to get a seed for the random number engine
 std::random_device rd;
 // Standard mersenne_twister_engine seeded with rd()
 std::mt19937 gen(rd());
 
 // Use dist to generate a random number into a double in the range
 // [0,1)
 std::uniform_real_distribution<> dis(0.0, 1.0);
 
 for (unsigned long i = 0; i < Current_number_of_vehicles; i++)
  {
   // Get a pointer to the current vehicle
   Vehicle *current_vehicle_pt = Vehicles_pt[i];
   const unsigned long current_position = current_vehicle_pt->position();
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
    const double r = dis(gen);
    // Compute acceleration based on random number and as a function of
    // the spatial headway and the maximum velocity. Also make sure
    // that there is at least one acceleration step
    unsigned r_acc = std::max(static_cast<unsigned>(r * std::min(Maximum_velocity, spatial_headway)), static_cast<unsigned>(1));
    //DEB(r_acc);
   
    //const unsigned r_acc = 1;
   
    // First rule (acceleration)
    unsigned new_velocity = std::min(current_velocity + r_acc, Maximum_velocity);
   
    // Second rule (deceleration)
   // Are there bumps?
   const unsigned n_bumps = nbumps();
   if (n_bumps > 0)
    {
     // Get the distance to the closest bump
     const unsigned long distance_to_closest_bump = distance_to_nearest_bump(current_position);
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
       new_velocity = std::min(static_cast<unsigned>(distance_to_closest_bump), spatial_headway);
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
   
   // Delay probability
   const double p_0 = P_0;
   const double p_1 = P_1;
   
   // Third rule (randomization) if new velocity is equal to zero
   if (new_velocity == 0)
    {
     const double r0 = dis(gen); 
     if (r0 <= p_0)
      {
       new_velocity = std::max(int(new_velocity - 1), 0);
       //std::cerr << "NV: " << new_velocity << std::endl;
      }
    }
   else // (new_velocity > 0)
    {
     const double r1 = dis(gen); 
     if (r1 <= p_1)
      {
       new_velocity = std::max(int(new_velocity - 1), 0);
       //std::cerr << "NV: " << new_velocity << std::endl;
      }
    }

   // Fourth rule (movement)
   unsigned long new_position = current_position + new_velocity;
   if (new_position >= Lane_size)
    {
     // Increase the number of vehicles that have traverse the lane
     N_vehicles_complete_travel++;
     // Compute new average travel time
     Travel_time+=current_vehicle_pt->travel_time(); 
     
     // Prepare to clear statistics at next update
     current_vehicle_pt->enable_clear_statistics_at_next_update();
     
     // Set new position for vehicle
     new_position = new_position - Lane_size;
     
     // Get the current travel time of the vehicle and add it up to
     // the travel time of the lane
     sum_delay+=current_vehicle_pt->delay();
    }
   
   // Update velocity and positon of vehicle
   current_vehicle_pt->velocity(1) = new_velocity;
   current_vehicle_pt->position(1) = new_position;
   
   sum_velocity+=new_velocity;
   
  } // for (i < Current_number_of_vehicles)
 
 mean_velocity=static_cast<Real>(sum_velocity)/static_cast<Real>(Current_number_of_vehicles);
 mean_current=static_cast<Real>(sum_velocity)/static_cast<Real>(Lane_size);
 if (N_vehicles_complete_travel>0)
  {
   mean_travel_time=static_cast<Real>(Travel_time)/static_cast<Real>(N_vehicles_complete_travel);
  }
 mean_travel_time = 0;
 mean_delay=
  // The waiting time should be the sum of the current dealys of each
  // vehicle divided by the number of vehicles, there is no need for
  // history of delays
  
  }

  // ----------------------------------------------------------------
  // Compute the distance to the nearest bump
  // ----------------------------------------------------------------
unsigned long RandAccPBCwb::distance_to_nearest_bump(unsigned long position)
{
 // Get the number of bumps in the lane
 const unsigned n_bumps = Bumps_pt.size();
 unsigned long gap_to_nearest_bump = this->lane_size();
 // Loop over the bumps and compute the distance to the nearest bump
 for (unsigned i = 0; i < n_bumps; i++)
  {
   const unsigned long i_bump_position = Bumps_pt[i]->position();
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
