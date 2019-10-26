#include "../../../../src/general/general.h"
#include "../../../../src/general/cc_vehicle.h"
#include "../../../../src/rand_acc/cc_rand_acc_pbc.h"

//#define MAX_MONTE_CARLO_LOOP  200000
//#define MONTE_CARLO_STAB_PHASE 10000 // %5 of MAX_MONTE_CARLO_LOOP
//#define LANE_SIZE               1000 // %0.5 of MAX_MONTE_CARLO_LOOP

//#define MAX_MONTE_CARLO_LOOP  10000
//#define MONTE_CARLO_STAB_PHASE 1000 // %10 of MAX_MONTE_CARLO_LOOP
//#define LANE_SIZE               100 // %1 of MAX_MONTE_CARLO_LOOP

//#define MAX_MONTE_CARLO_LOOP    200000
//#define MONTE_CARLO_STAB_PHASE   10000 // %10 of MAX_MONTE_CARLO_LOOP
//#define LANE_SIZE                 1000 // %1 of MAX_MONTE_CARLO_LOOP

#define N_CONFIGURATIONS          1 // Different number of initial
// vehicles positions
#define MAX_MONTE_CARLO_LOOP   20000
#define MONTE_CARLO_STAB_PHASE 15000
#define LANE_SIZE               1000
// ----------------------------------------------------------------------

#define MAX_VELOCITY 5

#define MAXIMUM_BREAK_PROBABILITY_P0   0.6
#define MAXIMUM_BREAK_PROBABILITY_P1   0.1
#define BREAK_PROBABILITY_STEP_P0   0.1
#define BREAK_PROBABILITY_STEP_P1   0.05
#define DENSITY_STEP             0.1
//#define DENSITY_STEP           1.1

#define MAX_VELOCITY 5

int main()
{ 
 const unsigned long lane_size = LANE_SIZE;
 const unsigned maximum_velocity = MAX_VELOCITY;
 
 const double maximum_break_probability_p0 = MAXIMUM_BREAK_PROBABILITY_P0;
 const double break_probability_step_p0 = BREAK_PROBABILITY_STEP_P0;
 double break_probability_p0 = 0.0;
 
 // Loop over break probability
 while (break_probability_p0 <= maximum_break_probability_p0)
  {
   double maximum_break_probability_p1 = break_probability_p0;
   if (break_probability_p0 > MAXIMUM_BREAK_PROBABILITY_P1)
    {
     maximum_break_probability_p1 = MAXIMUM_BREAK_PROBABILITY_P1;
    }
   
   const double break_probability_step_p1 = BREAK_PROBABILITY_STEP_P1;
   double break_probability_p1 = 0.0;
   
   // Loop over break probability
   while (break_probability_p1 <= maximum_break_probability_p1)
    {
     // Output for testing/validation
     std::ostringstream velocity_filename;
     std::ostringstream current_filename;
     velocity_filename << "RESLT/velocity_" << "bp_p0_" << break_probability_p0 << "_p1_"<< break_probability_p1 << ".dat";
     current_filename << "RESLT/current_" << "bp_p0_" << break_probability_p0 << "_p1_" << break_probability_p1 << ".dat";
     // Output for testing/validation
     std::ofstream velocity_file((velocity_filename.str()).c_str(), std::ios_base::out);
     std::ofstream current_file((current_filename.str()).c_str(), std::ios_base::out);
     
     const double maximum_density = 1.0;
     const double density_step = DENSITY_STEP;
     double density = 0.0;
     
     while (density <= maximum_density)
      {
       
       // Averaged configurations mean velocity
       double averaged_configurations_mean_velocity = 0;
       // Averaged configurations mean current
       double averaged_configurations_mean_current = 0;
       
       for (unsigned i_configuration = 0; i_configuration < N_CONFIGURATIONS; i_configuration++)
        {
       
         RandAccPBC lane;
         lane.initialise(lane_size, maximum_velocity, break_probability_p0, break_probability_p1);
     
         double sum_mean_velocity = 0;
         double sum_mean_current = 0;
     
         // Add vehicles to the lane based on the given density
         lane.fill_in_vehicles(density);
     
         // Initial state of the lane
         //lane.print(true); 
       
         const unsigned monte_carlo_max_loop = MAX_MONTE_CARLO_LOOP;
         const unsigned monte_carlo_stabilization_phase = MONTE_CARLO_STAB_PHASE;
         // Monte-Carlo loop
         for (unsigned i = 0; i < monte_carlo_max_loop; i++)
          {
           // Compute the occupancy index
           lane.update_vehicles_list();
     
           // Apply Rand-Acc rules
           unsigned long sum_velocity = lane.apply_rand_acc();
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

         averaged_configurations_mean_velocity+=total_mean_velocity;
         averaged_configurations_mean_current+=total_mean_current;
       
        } // for (i_configuration < N_CONFIGURATIONS)
       
       averaged_configurations_mean_velocity=averaged_configurations_mean_velocity/double(N_CONFIGURATIONS);
       averaged_configurations_mean_current=averaged_configurations_mean_current/double(N_CONFIGURATIONS);
       
       std::cerr << "p0: " << break_probability_p0 << "\tp1: " << break_probability_p1 << "\trho: " << density << "\tavcmJ: " << averaged_configurations_mean_current << "\tavcmV: " << averaged_configurations_mean_velocity << std::endl;
       velocity_file << density << "\t" << averaged_configurations_mean_velocity << std::endl;
       current_file << density << "\t" << averaged_configurations_mean_current << std::endl;
       
       // Increase density
       density+=density_step;
       
      } // while (density <= maximum_density) 
     
     // Increase break probability p1
     break_probability_p1+=break_probability_step_p1;
     
     velocity_file.close();
     current_file.close();
     
    } // while (break_probability_p1 <= maximum_break_probability_p1)
   
   // Increase break probability p0
   break_probability_p0+=break_probability_step_p0;
   
  } // while (break_probability_p0 <= maximum_break_probability_p0)
 
 return 0;
 
}

