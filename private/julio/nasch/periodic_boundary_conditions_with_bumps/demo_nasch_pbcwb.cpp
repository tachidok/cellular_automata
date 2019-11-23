#include "../../../../src/general/general.h"
#include "cc_nasch_pbcwb.h"

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

#define N_CONFIGURATIONS          1 // Different number of initial
                                    // vehicles positions
#define MAX_MONTE_CARLO_LOOP   20000
#define MONTE_CARLO_STAB_PHASE 15000
#define LANE_SIZE               1500
// ----------------------------------------------------------------------

#define MAX_VELOCITY 5

#define OUTPUT_TIME_SPACE

#define BREAK_PROBABILITY_STEP   0.1
#define DENSITY_STEP             0.01
//#define DENSITY_STEP           1.1

#define OUTPUT_TIME_SPACE

//#define OUTPUT_CURRENT_VS_TIME // If you want to get this output then
// fix the density and set the density
// step to some number larger than one
// such that the density loop be
// performed only once

// Use the namespace of the framework
using namespace CA;

int main(int argc, const char** argv)
{ 
 const unsigned long lane_size = LANE_SIZE;
 const unsigned maximum_velocity = MAX_VELOCITY;
 
 const Real maximum_break_probability = 1.0;
 const Real break_probability_step = BREAK_PROBABILITY_STEP;
 Real break_probability = 0.0;
 
 // Loop over break probability
 while (break_probability <= maximum_break_probability)
  {
   // Output for testing/validation
   std::ostringstream output_filename;
   output_filename << "RESLT/output_" << "bp_" << break_probability << ".dat";
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
   //Real density = 0.8;
   
   // Loop over density
   while (density <= maximum_density)
    {
#ifdef OUTPUT_TIME_SPACE 
     std::ostringstream lane_status_filename;
     lane_status_filename << "RESLT/lane_" << "bp" << break_probability << "_rho_" << density << ".dat";
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
       NaSchPBCwb lane;
       lane.initialise(lane_size, maximum_velocity, break_probability);
       
#ifdef OUTPUT_CURRENT_VS_TIME
       std::ostringstream current_vs_time_filename;
       current_vs_time_filename << "RESLT/current_vs_time" << "_bp_" << break_probability << "_rho_" << density << ".dat";
       std::ofstream current_vs_time_file((current_vs_time_filename.str()).c_str(), std::ios_base::out);
#endif // #ifdef OUTPUT_CURRENT_VS_TIME
       
       // Add vehicles to the lane based on the given density
       lane.fill_in_vehicles(density);

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
       
       // Add bumps to the lane
       std::vector<unsigned> bumps_positions;
       // Add one bump at the center of the lane
       bumps_positions.push_back(lane_size/2);
       // With bump at middle of the lane
       lane.set_bumps(bumps_positions);
       
       const unsigned long monte_carlo_max_loop = MAX_MONTE_CARLO_LOOP;
       const unsigned long monte_carlo_stabilization_phase = MONTE_CARLO_STAB_PHASE;
       
       // Monte-Carlo loop
       for (unsigned long i = 0; i < monte_carlo_max_loop; i++)
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

         // Apply NaSch rules
         lane.apply_nasch(mean_velocity, mean_current,
                          mean_delay, sum_travel_time, mean_travel_time, mean_queue_length,
                          mean_co2, mean_nox, mean_voc, mean_pm);
         
         // Update lane status
         lane.update(); 
         
#ifdef OUTPUT_CURRENT_VS_TIME
         Real mean_current = Real(sum_velocity) / Real(lane.lane_size());
#endif // #ifdef OUTPUT_CURRENT_VS_TIME
         
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
           
#ifdef OUTPUT_CURRENT_VS_TIME
           mean_current = Real(sum_velocity) / Real(lane.lane_size());
#endif // #ifdef OUTPUT_CURRENT_VS_TIME
           
#ifdef OUTPUT_TIME_SPACE
           // Output lane status
           if (i_configuration == 0)
            {
             if ((int(density * 1000)) % 10 == 0)
              {
               lane.output_time_space(lane_status_file);
              }
            }
#endif // #ifdef OUTPUT_TIME_SPACE 
           
          }
         
#ifdef OUTPUT_CURRENT_VS_TIME
         current_vs_time_file << i << "\t" << mean_current << std::endl;
#endif // #ifdef OUTPUT_CURRENT_VS_TIME
         
        } // for (i < monte_carlo_max_loop)
       
#ifdef OUTPUT_CURRENT_VS_TIME
       current_vs_time_file.close();
#endif // #ifdef OUTPUT_CURRENT_VS_TIME
       
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

     std::cerr << "bp: " << break_probability
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
     
     //std::cerr << "bp: " << break_probability << "\trho: " << density << "\tavcmJ: " << averaged_configurations_mean_current << "\tavcmV: " << averaged_configurations_mean_velocity << "\tSmJ: " << sum_mean_current << "\tSmv: " << sum_mean_velocity << "\tTNI: " << total_number_of_instances << std::endl;

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
   
   // Increase density
   break_probability+=break_probability_step;

   output_file.close();
   
  } // while (break_probability <= maximum_break_probability)
 
 return 0;
 
}

