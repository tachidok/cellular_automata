#include "../../../../src/general/general.h"
#include "cc_rand_acc_pbcwb.h"

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
#define MAX_MONTE_CARLO_LOOP   2000
#define MONTE_CARLO_STAB_PHASE 1500
#define LANE_SIZE               150
// ----------------------------------------------------------------------

#define MAX_VELOCITY 5

//#define MAXIMUM_BREAK_PROBABILITY_P0   0.6
//#define MAXIMUM_BREAK_PROBABILITY_P1   0.1
#define MAXIMUM_BREAK_PROBABILITY_P0   1.0
#define MAXIMUM_BREAK_PROBABILITY_P1   1.0
#define BREAK_PROBABILITY_STEP_P0   0.2
#define BREAK_PROBABILITY_STEP_P1   0.2
#define DENSITY_STEP             0.01
//#define DENSITY_STEP           1.1

#define MAX_VELOCITY 5

#define N_BUMPS 0

#define OUTPUT_TIME_SPACE

// Use the namespace of the framework
using namespace CA;

int main(int argc, const char** argv)
{ 
 const unsigned lane_size = LANE_SIZE;
 const unsigned maximum_velocity = MAX_VELOCITY;
 
 const Real maximum_break_probability_p0 = MAXIMUM_BREAK_PROBABILITY_P0;
 const Real break_probability_step_p0 = BREAK_PROBABILITY_STEP_P0;
 Real break_probability_p0 = 0.0;
 
 // Loop over break probability
 while (break_probability_p0 <= maximum_break_probability_p0)
  {
   Real maximum_break_probability_p1 = break_probability_p0;
   if (break_probability_p0 > MAXIMUM_BREAK_PROBABILITY_P1)
    {
     maximum_break_probability_p1 = MAXIMUM_BREAK_PROBABILITY_P1;
    }
   
   const Real break_probability_step_p1 = BREAK_PROBABILITY_STEP_P1;
   Real break_probability_p1 = 0.0;
   
   // Loop over break probability
   while (break_probability_p1 <= maximum_break_probability_p1)
    {
     // Output for testing/validation
     std::ostringstream output_filename;
     output_filename << "RESLT/output_" << "bp_p0_" << break_probability_p0 << "_p1_"<< break_probability_p1 << ".dat";
     // Output for testing/validation
     std::ofstream output_file((output_filename.str()).c_str(), std::ios_base::out);
     output_file << "density" << "\t"
                     << "mean_velocity" << "\t"
                     << "mean_current" << "\t"
                     << "mean_delay" << "\t"
                     << "mean_travel_time" << "\t"
                     << "mean_queue_length" << "\t"
                     << "mean_CO2" << "\t"
                     << "mean_NOx" << "\t"
                     << "mean_VOC" << "\t"
                     << "mean_PM" << std::endl;
     
         const Real maximum_density = 1.0;
         const Real density_step = DENSITY_STEP;
         Real density = 0.0;
     
         while (density <= maximum_density)
          {
#ifdef OUTPUT_TIME_SPACE 
           std::ostringstream lane_status_filename;
           lane_status_filename << "RESLT/lane_" << "bp_p0_" << break_probability_p0 << "_p1_"<< break_probability_p1 << "_rho_" << density << ".dat";
           std::ofstream lane_status_file((lane_status_filename.str()).c_str(), std::ios_base::out);
#endif // #ifdef OUTPUT_TIME_SPACE
       
       // Averaged configurations mean velocity
       Real averaged_configurations_mean_velocity = 0;
       // Averaged configurations mean current
       Real averaged_configurations_mean_current = 0;
       // Averaged configurations mean delay
       Real averaged_configurations_mean_delay = 0;
       // Averaged configurations mean travel time
       Real averaged_configurations_mean_travel_time = 0;
       // Averaged configurations mean queue length
       Real averaged_configurations_mean_queue_length = 0;
       // Averaged configurations mean CO2
       Real averaged_configurations_mean_CO2 = 0;
       // Averaged configurations mean NOx
       Real averaged_configurations_mean_NOx = 0;
       // Averaged configurations mean VOC
       Real averaged_configurations_mean_VOC = 0;
       // Averaged configurations mean PM
       Real averaged_configurations_mean_PM = 0;
       
       for (unsigned i_configuration = 0; i_configuration < N_CONFIGURATIONS; i_configuration++)
        {
         RandAccPBCwb lane;
         lane.initialise(lane_size, maximum_velocity, break_probability_p0, break_probability_p1);
         
         // Add vehicles to the lane based on the given density
         lane.fill_in_vehicles(density);
         
         // Add bumps to the lane
         std::vector<unsigned> bumps_positions;
         const unsigned h_bump = lane_size / (N_BUMPS + 1);
         for (unsigned kk = 1; kk <= N_BUMPS; kk++)
          {
           // Add bump at the center of the lane
           bumps_positions.push_back(h_bump*kk);
           //DEB(h_bump*kk);
          }
         // No bumps
         lane.set_bumps(bumps_positions);
         // Initial state of the lane
         //lane.print(true); 

         Real sum_mean_velocity = 0;
         Real sum_mean_current = 0;
         Real sum_mean_delay = 0;
         Real sum_mean_travel_time = 0;
         unsigned sum_travel_time = 0;
         Real sum_mean_queue_length = 0;
         Real sum_mean_co2 = 0;
         Real sum_mean_nox = 0;
         Real sum_mean_voc = 0;
         Real sum_mean_pm = 0;
         
         bool first_time_reset_travel_time = false;
         
         const unsigned monte_carlo_max_loop = MAX_MONTE_CARLO_LOOP;
         const unsigned monte_carlo_stabilization_phase = MONTE_CARLO_STAB_PHASE;
         // Monte-Carlo loop
         for (unsigned i = 0; i < monte_carlo_max_loop; i++)
          {
           // Compute the occupancy index
           lane.update_vehicles_list();
          
           Real mean_velocity = 0;
           Real mean_current = 0;
           Real mean_delay = 0;
           Real mean_travel_time = 0; 
           Real mean_queue_length = 0;
           Real mean_co2 = 0.0;
           Real mean_nox = 0.0;
           Real mean_voc = 0.0;
           Real mean_pm = 0.0;
          
           // Apply Rand-Acc WB rules
           lane.apply_rand_acc(mean_velocity, mean_current,
                               mean_delay, sum_travel_time, mean_travel_time, mean_queue_length,
                               mean_co2, mean_nox, mean_voc, mean_pm);
           //unsigned sum_velocity = lane.apply_rand_acc();
           //DEB(sum_velocity);
           //DEB(alpha);
           // Update lane status
           lane.update();
          
           //lane.print(true);
           //lane.print(false);
          
           //const Real density = lane.density();
           //DEB(density);
           //std::cerr << lane.current_number_of_vehicles() << " " << sum_velocity << std::endl;
          
           // Apply only after stabilization phase
           if (i > monte_carlo_stabilization_phase)
            {
             // Reset statistics after stabilisation phase
             if (!first_time_reset_travel_time)
              {
               sum_travel_time = 0;
               lane.reset_n_vehicles_complete_travel();
               mean_travel_time=0;
               first_time_reset_travel_time = true;
              
              }
            
             //Real mean_velocity = static_cast<Real>(sum_velocity) / static_cast<Real>(lane.current_number_of_vehicles());
             sum_mean_velocity+=mean_velocity;
             //Real mean_current = static_cast<Real>(sum_velocity) / static_cast<Real>(lane.lane_size());
             sum_mean_current+=mean_current;
             sum_mean_delay+=mean_delay;
             sum_mean_travel_time+=mean_travel_time;
             sum_mean_queue_length+=mean_queue_length;
            
             // Emissions
             sum_mean_co2+=mean_co2;
             sum_mean_nox+=mean_nox;
             sum_mean_voc+=mean_voc;
             sum_mean_pm+=mean_pm;
            
#ifdef OUTPUT_TIME_SPACE
             // Output lane status
             if (i_configuration == 0)
              {
               if ((int(density * 100)) % 10 == 0)
                {
                 lane.output_time_space(lane_status_file);
                }
              }
#endif // #ifdef OUTPUT_TIME_SPACE 
            
            }
          
          } // for (i < monte_carlo_max_loop)
         
         const Real total_number_of_instances =
          monte_carlo_max_loop - monte_carlo_stabilization_phase - 1;
         
         // Total mean velocity
         const Real total_mean_velocity = sum_mean_velocity / total_number_of_instances;
         // Total mean current
         const Real total_mean_current = sum_mean_current / total_number_of_instances;
         // Total mean delay
         const Real total_mean_delay = sum_mean_delay / total_number_of_instances;
         // Total mean travel time
         const Real total_mean_travel_time = sum_mean_travel_time / total_number_of_instances;
         // Total mean queue length
         const Real total_mean_queue_length = sum_mean_queue_length / total_number_of_instances;
         // Total mean CO2
         const Real total_mean_CO2 = sum_mean_co2 / total_number_of_instances;
         // Total mean NOx
         const Real total_mean_NOx = sum_mean_nox / total_number_of_instances;
         // Total mean VOC
         const Real total_mean_VOC = sum_mean_voc / total_number_of_instances;
         // Total mean PM
         const Real total_mean_PM = sum_mean_pm / total_number_of_instances;
         
         averaged_configurations_mean_velocity+=total_mean_velocity;
         averaged_configurations_mean_current+=total_mean_current;
         averaged_configurations_mean_delay+=total_mean_delay;
         averaged_configurations_mean_travel_time+=total_mean_travel_time;
         averaged_configurations_mean_queue_length+=total_mean_queue_length;
         averaged_configurations_mean_CO2+=total_mean_CO2;
         averaged_configurations_mean_NOx+=total_mean_NOx;
         averaged_configurations_mean_VOC+=total_mean_VOC;
         averaged_configurations_mean_PM+=total_mean_PM;
         
        } // for (i_configuration < N_CONFIGURATIONS)
       
       averaged_configurations_mean_velocity=averaged_configurations_mean_velocity/static_cast<Real>(N_CONFIGURATIONS);
       averaged_configurations_mean_current=averaged_configurations_mean_current/static_cast<Real>(N_CONFIGURATIONS);
       averaged_configurations_mean_delay=averaged_configurations_mean_delay/static_cast<Real>(N_CONFIGURATIONS);
       averaged_configurations_mean_travel_time=averaged_configurations_mean_travel_time/static_cast<Real>(N_CONFIGURATIONS);
       averaged_configurations_mean_queue_length=averaged_configurations_mean_queue_length/static_cast<Real>(N_CONFIGURATIONS);
       averaged_configurations_mean_CO2=averaged_configurations_mean_CO2/static_cast<Real>(N_CONFIGURATIONS);
       averaged_configurations_mean_NOx=averaged_configurations_mean_NOx/static_cast<Real>(N_CONFIGURATIONS);
       averaged_configurations_mean_VOC=averaged_configurations_mean_VOC/static_cast<Real>(N_CONFIGURATIONS);
       averaged_configurations_mean_PM=averaged_configurations_mean_PM/static_cast<Real>(N_CONFIGURATIONS);
       
       std::cerr << "p0: " << break_probability_p0
                 << "\tp1: " << break_probability_p1
                 << "\trho: " << density
                 << "\tmJ: " << averaged_configurations_mean_current
                 << "\tmV: " << averaged_configurations_mean_velocity
                 << "\tmDelay: " <<averaged_configurations_mean_delay
                 << "\tmTript: " <<averaged_configurations_mean_travel_time
                 << "\tmQlen: " <<averaged_configurations_mean_queue_length
                 << "\tmCO2: " <<averaged_configurations_mean_CO2
                 << "\tmNOx: " <<averaged_configurations_mean_NOx
                 << "\tmVOC: " <<averaged_configurations_mean_VOC
                 << "\tmPM: " <<averaged_configurations_mean_PM << std::endl;
       
       // -----------------------------------------------------------------------------------------
       // Output data
       // -----------------------------------------------------------------------------------------
       output_file << density << "\t"
                   << averaged_configurations_mean_velocity << "\t"
                   << averaged_configurations_mean_current << "\t"
                   << averaged_configurations_mean_delay << "\t"
                   << averaged_configurations_mean_travel_time << "\t"
                   << averaged_configurations_mean_queue_length << "\t"
                   << averaged_configurations_mean_CO2 << "\t"
                   << averaged_configurations_mean_NOx << "\t"
                   << averaged_configurations_mean_VOC << "\t"
                   << averaged_configurations_mean_PM << std::endl;
       
       // Increase density
       density+=density_step;
       
#ifdef OUTPUT_TIME_SPACE
       lane_status_file.close();
#endif // #ifdef OUTPUT_TIME_SPACE 
       
      } // while (density <= maximum_density) 
     
     // Increase break probability p1
     break_probability_p1+=break_probability_step_p1;
     
     output_file.close();
     
    } // while (break_probability_p1 <= maximum_break_probability_p1)
   
   // Increase break probability p0
   break_probability_p0+=break_probability_step_p0;
   
  } // while (break_probability_p0 <= maximum_break_probability_p0)
 
 return 0;
 
}

