#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <algorithm>

#include "general.h"
#include "cc_vehicle.h"
#include "cc_rand_acc.h"

#define MAX_MONTE_CARLO_LOOP  200000
#define MONTE_CARLO_STAB_PHASE 10000 // %5 of MAX_MONTE_CARLO_LOOP
#define LANE_SIZE               1000 // %0.5 of MAX_MONTE_CARLO_LOOP

//#define MAX_MONTE_CARLO_LOOP  10000
//#define MONTE_CARLO_STAB_PHASE 1000 // %10 of MAX_MONTE_CARLO_LOOP
//#define LANE_SIZE               100 // %1 of MAX_MONTE_CARLO_LOOP

#define MAX_VELOCITY 5

int main()
{ 
 const unsigned lane_size = LANE_SIZE;
 const unsigned maximum_velocity = MAX_VELOCITY;
 
 const double maximum_break_probability = 1.0;
 //const double break_probability_step = 0.1;
 const double break_probability_step = 1.1;
 double break_probability = 0.0;
 
 // Loop over break probability
 while (break_probability <= maximum_break_probability)
  {
   const double alpha_min = 0.0;
   const double alpha_max = 1.0;
   const double beta_min = 0.0;
   const double beta_max = 1.0;
   const double alpha_step = 0.01;
   const double beta_step = 0.1;
   for (double beta = beta_min; beta <= beta_max; beta+=beta_step)
    {
     std::cerr << "Working with [beta=" << beta << "] ..." << std::endl;

     std::ostringstream density_filename;
     std::ostringstream velocity_filename;
     std::ostringstream current_filename;
     density_filename << "RESLT/density_" << "b" << beta << ".dat";
     velocity_filename << "RESLT/velocity_" << "b" << beta << ".dat";
     current_filename << "RESLT/current_" << "b" << beta << ".dat";
     // Output for testing/validation
     std::ofstream density_file((density_filename.str()).c_str(), std::ios_base::out);
     std::ofstream velocity_file((velocity_filename.str()).c_str(), std::ios_base::out);
     std::ofstream current_file((current_filename.str()).c_str(), std::ios_base::out);
     
     for (double alpha = alpha_min; alpha <= alpha_max; alpha+=alpha_step)
      {
       RandAcc lane;
       lane.initialise(lane_size, maximum_velocity, break_probability, alpha, beta);
       
       double sum_mean_velocity = 0;
       double sum_mean_current = 0;
       
       // Initial state of the lane
       //lane.print(true); 
       
       const unsigned monte_carlo_max_loop = MAX_MONTE_CARLO_LOOP;
       const unsigned monte_carlo_stabilization_phase = MONTE_CARLO_STAB_PHASE;
       // Monte-Carlo loop
       for (unsigned i = 0; i < monte_carlo_max_loop; i++)
        {
         // Compute the occupancy index
         lane.update_vehicles_list();
     
         // Apply NaSch rules
         unsigned sum_velocity = lane.apply_rand_acc();
         //DEB(sum_velocity);
         //DEB(alpha);
         // Update lane status
         lane.update();
         
         //lane.print(true);
         //lane.print(false);
     
         //const double density = lane.density();
         //DEB(density);
         //std::cerr << lane.current_number_of_vehicles() << " " << sum_velocity << std::endl;
     
         // Apply only after stabilization phase
         if (i > monte_carlo_stabilization_phase)
          {
           double mean_velocity = double(sum_velocity) / double(lane.current_number_of_vehicles());
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
       
       const double density = lane.density();
       //std::cerr << "p: " << break_probability << "\talpha: " << alpha << "\tbeta: " << beta <<  "\tmJ: " << total_mean_current << "\tmV: " << total_mean_velocity << std::endl;
       density_file << alpha << "\t" << density << std::endl;
       velocity_file << alpha << "\t" << total_mean_velocity << std::endl;
       current_file << alpha << "\t" << total_mean_current << std::endl;
              
      } // for (alpha < alpha_max)

     density_file.close();
     velocity_file.close();
     current_file.close();
     
    } // for (beta < beta_max)
   
   // Increase break probability
   break_probability+=break_probability_step;
   
  } // while (break_probability <= maximum_break_probability)
  
 return 0;
 
}

