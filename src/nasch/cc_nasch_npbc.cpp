#include "cc_nasch_npbc.h"

namespace CA
{

 // ----------------------------------------------------------------
 // Constructor -- do nothing
 // ----------------------------------------------------------------
 NaSchNPBC::NaSchNPBC()
 {

 }

 // ----------------------------------------------------------------
 // Constructor
 // ----------------------------------------------------------------
 NaSchNPBC::NaSchNPBC(unsigned lane_size, unsigned maximum_velocity, Real break_probability, Real alpha, Real beta)
 {
  // Set lane configuration
  initialise(lane_size, maximum_velocity, break_probability, alpha, beta);
 }

 // ----------------------------------------------------------------
 // Destructor - do nothing
 // ----------------------------------------------------------------
 NaSchNPBC::~NaSchNPBC()
 {
  clear();
 }

 // ----------------------------------------------------------------
 // Initialise lane configuration
 // ----------------------------------------------------------------
 void NaSchNPBC::initialise(unsigned lane_size, unsigned maximum_velocity, Real break_probability, Real alpha, Real beta)
 {
  // Set lane configuration
  Allowed_number_of_vehicles = Lane_size = lane_size;
  Maximum_velocity = maximum_velocity;
  Break_probability = break_probability;

  // Initialise entry and exit probability
  Alpha = alpha; // entry probability
  Beta = beta; // exit probability

  // No vehicle has traverse the lane
  N_vehicles_complete_travel = 0;
  travel_time_computation = false;
  sum_travel_time = 0;

  // Initialise data structures
  clear();
 }

 // ----------------------------------------------------------------
 // Clear data structures
 // ----------------------------------------------------------------
 void NaSchNPBC::clear()
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
  Number_of_crosswalks = 0;
 }


 // ----------------------------------------------------------------
 // Set bumps
 // ----------------------------------------------------------------
 void NaSchNPBC::set_bumps(std::vector<unsigned> &bumps_positions)
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
 // Set crosswalks
 // ----------------------------------------------------------------
 void NaSchNPBC::add_crosswalk(unsigned position, unsigned crosswalk_length, unsigned crosswalk_width, unsigned maximum_speed, Real alpha, float safety_factor, unsigned N_c, Real go_back_prob)
 {
  Crosswalk *crossing = new Crosswalk(position, crosswalk_length, crosswalk_width, maximum_speed, alpha, safety_factor, N_c, go_back_prob);
  Cwalks_pt.push_back(crossing);
  Number_of_crosswalks++;
 }


 // ----------------------------------------------------------------
 // Update vehicles list
 // ----------------------------------------------------------------
 unsigned NaSchNPBC::update_vehicles_list()
 {
   std::random_device rd;
   // Standard mersenne_twister_engine seeded with rd()
   std::mt19937 gen(rd());

   // Use dist to generate a random number into a Real in the range
   // [0,1)
   std::uniform_real_distribution<> dis(0.0, 1.0);

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
    const Real r = dis(gen);

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
 /// Set all statistics values to zero
 // ----------------------------------------------------------------
 void NaSchNPBC::reset_statistics()
   {
    Mean_velocity = 0;
    Mean_delay = 0;
    Mean_travel_time = 0;
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
 // Update lane based on NaSchNPBC rules
 // ----------------------------------------------------------------
 void NaSchNPBC::apply_nasch()
   {
     std::random_device rd;
     // Standard mersenne_twister_engine seeded with rd()
     std::mt19937 gen(rd());

     // Use dist to generate a random number into a Real in the range
     // [0,1)
     std::uniform_real_distribution<> dis(0.0, 1.0);

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

     // Accumulated delay
     unsigned sum_delay = 0;

     // Emissions
     Real sum_co2 = 0.0;
     Real sum_nox = 0.0;
     Real sum_voc = 0.0;
     Real sum_pm = 0.0;

     // -----------------------------------------------------------------
     // Compute the pedestrians' dynamics for every crosswalk
     // -----------------------------------------------------------------
     for (unsigned cwi = 0; cwi < Number_of_crosswalks; cwi++)
       {
         //Entrance of pedestrians to the waiting zone
         Cwalks_pt[cwi]->pedestrians_entrance();
         Cwalks_pt[cwi]->select_next_location();
         Cwalks_pt[cwi]->apply_motion_rules();

         if(!Cwalks_pt[cwi]->is_empty())
            {
              if (Cwalks_pt[cwi]->is_safe_to_cross(Vehicles_pt, Maximum_velocity) || Cwalks_pt[cwi]->is_occupied())
                 Cwalks_pt[cwi]->update_crosswalk();
            }
       }

    // -----------------------------------------------------------------
    // Compute the lane's dynamics for every vehicle in Vehicles_pt[]
    // -----------------------------------------------------------------
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


        // -----------------------------------------------------------------
        // NaSchNPBC rules
        // -----------------------------------------------------------------

        // First rule (acceleration)
        unsigned new_velocity = std::min(current_velocity + 1, Maximum_velocity);

        // Second rule (deceleration)
        new_velocity = std::min(new_velocity, spatial_headway);

        // Are there bumps?
        const unsigned n_bumps = nbumps();
        const unsigned n_crosswalks = ncrosswalks();

        if (n_bumps > 0 || n_crosswalks > 0)
         {
          // Get the distance to the closest bump
          const unsigned distance_to_closest_bump = distance_to_nearest_bump(current_position);

          // Get the distance to the closest crosswalk
          Crosswalk* nearest_cwalk = get_the_nearest_crosswalk(current_position);
          unsigned distance_to_closest_crosswalk = this->Lane_size;

          // There are vehicles reaching (before) the crosswalk?
          if(nearest_cwalk != NULL)
            {
              distance_to_closest_crosswalk = nearest_cwalk->position()-current_position;
            }


        // deceleration to reach a bump
          if(distance_to_closest_bump < distance_to_closest_crosswalk)
            {
              const unsigned max_velocity_to_pass_bump = 1;
              // If the car is already at the bump then reduce the velocity to that allowed by the bump
              if (distance_to_closest_bump == 0)
               {
                // This may be a function of the type of bump
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

        // deceleration to reach a crosswalk
          else
            {
              unsigned max_velocity_to_pass_crosswalk = 1;

              if(nearest_cwalk != NULL)
                {
                  if (nearest_cwalk->is_occupied())
                    {
                      max_velocity_to_pass_crosswalk = 0;
                      spatial_headway = distance_to_closest_crosswalk - nearest_cwalk->no_cells_prior_to_CW() - 1; //X_p - X_i(t) - 1
                    }
                }

              // If the car is already at the crosswalk then reduce the velocity to that allowed by the crosswalk
              if (distance_to_closest_crosswalk == 0)
                {
                  new_velocity = std::min(max_velocity_to_pass_crosswalk, spatial_headway);
                }

                // If the cw is close then reduce velocity accordingly
              else if (distance_to_closest_crosswalk < new_velocity)
                {
                  new_velocity = std::min(distance_to_closest_crosswalk, spatial_headway);
                }
              // If cw is no close enough then use new velocity
              else
                {
                  new_velocity = std::min(new_velocity, spatial_headway);
                }
            }
          }
      // No bumps
      else
       {
        new_velocity = std::min(new_velocity, spatial_headway);
       }

      // Third rule (randomization)

      //Compute a random number
      const Real r = dis(gen);

      if ((r / RAND_MAX) <= Break_probability)
       {
        new_velocity = std::max(int(new_velocity - 1), 0);
       }

      // Fourth rule (movement)
      unsigned new_position = current_position + new_velocity;

      bool was_deleted = false;
      // This only applies to the last vehicle on the lane. Check whether
      // we allow it to leave the lane or not. We decide this based on
      // the Beta parameter
      if (i + 1 == Current_number_of_vehicles && new_position >= Lane_size)
       {
        // Check whether we allow it to leave
        if (Beta > 0.0)
         {
          // Compute a random number
          const Real r1 = dis(gen);

          // Is the computed probability is less or equal than the Beta
          // probability
          if ((r1 / RAND_MAX) <= Beta)
           {

            if(travel_time_computation)
              {
                N_vehicles_complete_travel++;
                sum_travel_time+=current_vehicle_pt->travel_time();
                Mean_travel_time=static_cast<Real>(sum_travel_time)/static_cast<Real>(N_vehicles_complete_travel);
              }

            // Delete vehicle
            delete current_vehicle_pt;
            was_deleted = true;
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


//Statistics computation
      sum_velocity+=new_velocity;

      // Emissions
      Real tmp_co2 = 0.0;
      Real tmp_nox = 0.0;
      Real tmp_voc = 0.0;
      Real tmp_pm = 0.0;

      if(!was_deleted)
        {
          current_vehicle_pt->compute_emissions(tmp_co2, tmp_nox, tmp_voc, tmp_pm);
          sum_delay+=current_vehicle_pt->delay();
        }



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

     }
     // for (i < Current_number_of_vehicles)

     if (Current_number_of_vehicles > 0)
      {
       Mean_velocity=static_cast<Real>(sum_velocity)/static_cast<Real>(Current_number_of_vehicles);
       Mean_delay=static_cast<Real>(sum_delay)/static_cast<Real>(Current_number_of_vehicles);
      }
     else
      {
       Mean_velocity=0;
       Mean_delay=0;
      }

     Mean_current=static_cast<Real>(sum_velocity)/static_cast<Real>(Lane_size);

     Density = static_cast<Real>(Current_number_of_vehicles)/static_cast<Real>(Lane_size);

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
 // Update the lane status
 // ----------------------------------------------------------------
 void NaSchNPBC::update()
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
 // Compute the distance to the nearest bump
 // ----------------------------------------------------------------
 unsigned NaSchNPBC::distance_to_nearest_bump(unsigned position)
 {
  // Get the number of bumps in the lane
  const unsigned n_bumps = nbumps();
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
 // Get the nearest crosswalk relative to a given position
 // ----------------------------------------------------------------
 Crosswalk* NaSchNPBC::get_the_nearest_crosswalk(unsigned position)
 {
  // Get the number of bumps in the lane
  const unsigned n_crosswalks = Cwalks_pt.size();
  unsigned gap_to_nearest_crosswalk = this->lane_size();
  Crosswalk* nearest_crosswalk = NULL;
  // Loop over the bumps and compute the distance to the nearest bump
  for (unsigned i = 0; i < n_crosswalks; i++)
   {
    const unsigned i_cw_position = Cwalks_pt[i]->position();
    if (i_cw_position >= position)
     {
      const int long distance = i_cw_position - position;
      if (distance < static_cast<int long>(gap_to_nearest_crosswalk))
       {
        gap_to_nearest_crosswalk = distance;
        nearest_crosswalk = Cwalks_pt[i];
       }
     }
   }

  return nearest_crosswalk;

 }


 // ----------------------------------------------------------------
 // Prints the lane status
 // ----------------------------------------------------------------
 void NaSchNPBC::print(bool print_velocities)
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



 void NaSchNPBC::output_time_space(std::ofstream &output_file)
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
