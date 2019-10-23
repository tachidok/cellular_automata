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

// ----------------------------------------------------------------------
// Checking with these initial configurations
//#define MAX_MONTE_CARLO_LOOP     500
//#define MONTE_CARLO_STAB_PHASE   100 // %10 of MAX_MONTE_CARLO_LOOP
//#define LANE_SIZE              10000 // %1 of MAX_MONTE_CARLO_LOOP

//#define MAX_MONTE_CARLO_LOOP    5000
//#define MONTE_CARLO_STAB_PHASE  1000 // %10 of MAX_MONTE_CARLO_LOOP
//#define LANE_SIZE               1000 // %1 of MAX_MONTE_CARLO_LOOP

//#define MAX_MONTE_CARLO_LOOP    2000
//#define MONTE_CARLO_STAB_PHASE    50 // %10 of MAX_MONTE_CARLO_LOOP
//#define LANE_SIZE                 50 // %1 of MAX_MONTE_CARLO_LOOP

#define N_CONFIGURATIONS          40 // Different number of initial
                                     // vehicles positions
#define MAX_MONTE_CARLO_LOOP   20000
#define MONTE_CARLO_STAB_PHASE 15000
#define LANE_SIZE               1000
// ----------------------------------------------------------------------

#define MAX_VELOCITY 5

#define BREAK_PROBABILITY_STEP   0.1
#define DENSITY_STEP             0.01
//#define DENSITY_STEP           1.1

//#define OUTPUT_CURRENT_VS_TIME // If you want to get this output then
// fix the density and set the density
// step to some number larger than one
// such that the density loop be
// performed only once

int main()
{ 
 const unsigned long lane_size = LANE_SIZE;
 const unsigned maximum_velocity = MAX_VELOCITY;
 
 const double maximum_break_probability = 1.0;
 const double break_probability_step = BREAK_PROBABILITY_STEP;
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
   const double density_step = DENSITY_STEP;
   double density = 0.0;
   //double density = 0.8;
   
   // Loop over density
   while (density <= maximum_density)
    {
     
     // Averaged configurations mean velocity
     double averaged_configurations_mean_velocity = 0;
     // Averaged configurations mean current
     double averaged_configurations_mean_current = 0;
     
     for (unsigned i_configuration = 0; i_configuration < N_CONFIGURATIONS; i_configuration++)
      {
       NaSch lane;
       lane.initialise(lane_size, maximum_velocity, break_probability);
       
#ifdef OUTPUT_CURRENT_VS_TIME
       std::ostringstream current_vs_time_filename;
       current_vs_time_filename << "RESLT/current_vs_time" << "_bp_" << break_probability << "_rho_" << density << ".dat";
       std::ofstream current_vs_time_file((current_vs_time_filename.str()).c_str(), std::ios_base::out);
#endif // #ifdef OUTPUT_CURRENT_VS_TIME
       
       double sum_mean_velocity = 0;
       double sum_mean_current = 0;
       
       // Add vehicles to the lane based on the given density
       lane.fill_in_vehicles(density);
       //lane.print(true); 
       
       bool print = false;
       
       const unsigned long monte_carlo_max_loop = MAX_MONTE_CARLO_LOOP;
       const unsigned long monte_carlo_stabilization_phase = MONTE_CARLO_STAB_PHASE;
       
       // Monte-Carlo loop
       for (unsigned long i = 0; i < monte_carlo_max_loop; i++)
        {
         // Compute the occupancy index
         lane.update_vehicles_list();
         
         // Apply NaSch rules
         unsigned long sum_velocity = lane.apply_nasch(print);
         // Update lane status
         lane.update();
         
         //lane.print(true);
         //lane.print(false); 
         
#ifdef OUTPUT_CURRENT_VS_TIME
         double mean_current = double(sum_velocity) / double(lane.lane_size());
#endif // #ifdef OUTPUT_CURRENT_VS_TIME
         
         // Apply only after stabilization phase
         if (i > monte_carlo_stabilization_phase)
          {
           //std::cerr << lane.number_of_vehicles() << " " << sum_velocity << std::endl;
           double mean_velocity = double(sum_velocity) / double(lane.number_of_vehicles());
           sum_mean_velocity+=mean_velocity;
           double mean_current = double(sum_velocity) / double(lane.lane_size());
#ifdef OUTPUT_CURRENT_VS_TIME
           mean_current = double(sum_velocity) / double(lane.lane_size());
#endif // #ifdef OUTPUT_CURRENT_VS_TIME
           sum_mean_current+=mean_current;
           //print = true;
          }
         
#ifdef OUTPUT_CURRENT_VS_TIME
         current_vs_time_file << i << "\t" << mean_current << std::endl;
#endif // #ifdef OUTPUT_CURRENT_VS_TIME
         
        } // for (i < monte_carlo_max_loop)
       
#ifdef OUTPUT_CURRENT_VS_TIME
       current_vs_time_file.close();
#endif // #ifdef OUTPUT_CURRENT_VS_TIME
       
       const double total_number_of_instances =
        monte_carlo_max_loop - monte_carlo_stabilization_phase - 1;
       
       // Total mean velocity
       const double total_mean_velocity = sum_mean_velocity / total_number_of_instances;
       // Total mean current
       const double total_mean_current = sum_mean_current / total_number_of_instances;
       
       averaged_configurations_mean_velocity+=total_mean_velocity;
       averaged_configurations_mean_current+=total_mean_current;
       
      } // for (i_configuration < N_CONFIGURATIONS)
     
     averaged_configurations_mean_velocity=averaged_configurations_mean_velocity/double(N_CONFIGURATIONS);
     averaged_configurations_mean_current=averaged_configurations_mean_current/double(N_CONFIGURATIONS);
     
     //std::cerr << "bp: " << break_probability << "\trho: " << density << "\tavcmJ: " << averaged_configurations_mean_current << "\tavcmV: " << averaged_configurations_mean_velocity << "\tSmJ: " << sum_mean_current << "\tSmv: " << sum_mean_velocity << "\tTNI: " << total_number_of_instances << std::endl;
     std::cerr << "bp: " << break_probability << "\trho: " << density << "\tavcmJ: " << averaged_configurations_mean_current << "\tavcmV: " << averaged_configurations_mean_velocity << std::endl;
     current_file << density << "\t" << averaged_configurations_mean_current << std::endl;
     velocity_file << density << "\t" << averaged_configurations_mean_velocity << std::endl;
     
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

