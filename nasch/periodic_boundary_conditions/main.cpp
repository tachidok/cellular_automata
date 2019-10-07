#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <algorithm>

#include "general.h"
#include "cc_vehicle.h"
#include "cc_nasch.h"

//#define MAX_MONTE_CARLO_LOOP 500
//#define MONTE_CARLO_STAB_PHASE 100
//#define LANE_SIZE 10000

#define MAX_MONTE_CARLO_LOOP  20000
#define MONTE_CARLO_STAB_PHASE 1000 // %10 of MAX_MONTE_CARLO_LOOP
#define LANE_SIZE               100 // %1 of MAX_MONTE_CARLO_LOOP

#define MAX_VELOCITY 5

int main()
{ 
 const unsigned lane_size = LANE_SIZE;
 const unsigned maximum_velocity = MAX_VELOCITY;
 
 const double maximum_break_probability = 1.0;
 //const double break_probability_step = 0.1;
 const double break_probability_step = 0.1;
 double break_probability = 0.0;
 
 // Loop over break probability
 while (break_probability <= maximum_break_probability)
  {
   // Output for testing/validation
   std::ostringstream velocity_filename;
   std::ostringstream current_filename;
   velocity_filename << "RESLT/velocity_" << "bp" << break_probability << ".dat";
   current_filename << "RESLT/current_" << "bp" << break_probability << ".dat";
   // Output for testing/validation
   std::ofstream velocity_file((velocity_filename.str()).c_str(), std::ios_base::out);
   std::ofstream current_file((current_filename.str()).c_str(), std::ios_base::out);
   
   const double maximum_density = 1.0;
   const double density_step = 0.01;
   double density = 0.0;
   
   // Loop over density
   while (density <= maximum_density)
    {
     NaSch lane;
     lane.initialise(lane_size, maximum_velocity, break_probability);
     
     double sum_mean_velocity = 0;
     double sum_mean_current = 0;
     
     // Add vehicles to the lane based on the given density
     lane.fill_in_vehicles(density);
     //lane.print(true); 
     
     const unsigned monte_carlo_max_loop = MAX_MONTE_CARLO_LOOP;
     const unsigned monte_carlo_stabilization_phase = MONTE_CARLO_STAB_PHASE;
     // Monte-Carlo loop
     for (unsigned i = 0; i < monte_carlo_max_loop; i++)
      {
       // Compute the occupancy index
       lane.update_vehicles_list();
       
       // Apply NaSch rules
       unsigned sum_velocity = lane.apply_nasch();
       // Update lane status
       lane.update();
       
       //lane.print(true);
       //lane.print(false);
       
       //std::cerr << lane.number_of_vehicles() << " " << sum_velocity << std::endl;
       
       // Apply only after stabilization phase
       if (i > monte_carlo_stabilization_phase)
        {
         double mean_velocity = double(sum_velocity) / double(lane.number_of_vehicles());
         sum_mean_velocity+=mean_velocity;
         double mean_current = double(sum_velocity) / double(lane.lane_size());
         sum_mean_current+=mean_current;
        }
       
      } // for (i < monte_carlo_max_loop)
     
     const double total_number_of_instances =
      monte_carlo_max_loop - monte_carlo_stabilization_phase - 1;
     
     // Total mean velocity
     const double total_mean_velocity = sum_mean_velocity / total_number_of_instances;
     // Total mean current
     const double total_mean_current = sum_mean_current / total_number_of_instances;
     
     std::cerr << "p: " << break_probability << "\trho: " << density << "\tmJ: " << total_mean_current << "\tmV: " << total_mean_velocity << std::endl;
     velocity_file << density << "\t" << total_mean_velocity << std::endl;
     current_file << density << "\t" << total_mean_current << std::endl;
     
     // Increase density
     density+=density_step;
     
    } // while (density <= maximum_density)
   
    // Increase density
   break_probability+=break_probability_step;
   
   velocity_file.close();
   current_file.close();
   
  } // while (break_probability <= maximum_break_probability)
 
 return 0;
 
}

