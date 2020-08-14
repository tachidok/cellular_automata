#include "../../../../src/general/common_includes.h"
#include "../../../../src/general/utilities.h"
#include "../../../../src/general/initialise.h"

#include "../../../../src/general/cc_vehicle.h"
#include "../../../../src/nasch/cc_nasch_pbc.h"

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

//#define MAX_VELOCITY 4

//#define N_BUMPS 10

#define BREAK_PROBABILITY_STEP   0.1
//#define DENSITY_STEP             0.1
//#define DENSITY_STEP           1.1

//#define OUTPUT_TIME_SPACE

//#define OUTPUT_CURRENT_VS_TIME // If you want to get this output then
// fix the density and set the density
// step to some number larger than one
// such that the density loop be
// performed only once

// Use the namespace of the framework
using namespace CA;

// Used to define arguments
struct Args {
 argparse::ArgValue<unsigned> vmax;
 argparse::ArgValue<Real> rho;
 argparse::ArgValue<Real> bp;
 argparse::ArgValue<unsigned> n_bumps_min;
 argparse::ArgValue<unsigned> n_bumps_max;
 argparse::ArgValue<unsigned> bumps_step;
};

int main(int argc, const char** argv)
{
 // Initialise ca
 initialise_ca();
 
 // Instantiate parser
 Args args;
 auto parser = argparse::ArgumentParser(argv[0], "NaSch algorithm");
 
 // Add arguments
 
 // Optional
 parser.add_argument<unsigned>(args.vmax, "--vmax")
  .help("Maximum velocity, in cells numbers")
  .default_value("4");
 
 parser.add_argument<Real>(args.rho, "--rho")
  .help("Density")
  .default_value("0.1");
 
 parser.add_argument<Real>(args.bp, "--bp")
  .help("Break probability")
  .default_value("0.1"); 
 
 parser.add_argument<unsigned>(args.n_bumps_min, "--n_bumps_min")
  .help("Minimum number of bumps")
  .default_value("1");
 
 parser.add_argument<unsigned>(args.n_bumps_max, "--n_bumps_max")
  .help("Maximum number of bumps")
  .default_value("10");
 
 parser.add_argument<unsigned>(args.bumps_step, "--bumps_step")
  .help("The step to increment the number of bumps")
  .default_value("1");
 
 // Parse the input arguments
 parser.parse_args(argc, argv);
 
 const unsigned long lane_size = LANE_SIZE;
 const unsigned maximum_velocity = args.vmax;
 const Real density = args.rho;
 const Real break_probability = args.bp;
 const unsigned min_nbumps = args.n_bumps_min;
 const unsigned max_nbumps = args.n_bumps_max;
 const unsigned bumps_step = args.bumps_step;
 
 // Check whether minimum number of bumps is smaller than maximum 
 // numbe of bumps
 if (min_nbumps > max_nbumps)
  {
   // Error message
   std::ostringstream error_message;
   error_message << "The minimum number of bumps is larger than the maximum number of bumps\n"
                 << std::endl;
   throw CALibError(error_message.str(),
                    CA_CURRENT_FUNCTION,
                    CA_EXCEPTION_LOCATION);
  }
 
 // Output for testing/validation
 std::ostringstream output_filename;
 output_filename << "RESLT/output_" << "bp_" << break_probability << "_rho_" << density << ".dat";
 // Output for testing/validation
 std::ofstream output_file((output_filename.str()).c_str(), std::ios_base::out);
 output_file << "density" << "\t"
             << "nbumps" << "\t"
             << "mean_velocity" << "\t"
             << "mean_current" << "\t"
             << "mean_CO2" << "\t"
             << "mean_NOx" << "\t"
             << "mean_VOC" << "\t"
             << "mean_PM" << "\t"
             << "std_V: " << "\t"
             << "std_CO2: " << "\t"
             << "std_NOx: " << "\t"
             << "std_VOC: " << "\t"
             << "std_PM: " << std::endl; 
 
 // Loop over number of bumps
 unsigned n_bumps = min_nbumps;
 while (n_bumps <= max_nbumps)
  {
#ifdef OUTPUT_TIME_SPACE 
   std::ostringstream lane_status_filename;
   lane_status_filename << "RESLT/lane_" << "bp" << break_probability << "_rho_" << density << "_nbumps_" << n_bumps << ".dat";
   std::ofstream lane_status_file((lane_status_filename.str()).c_str(), std::ios_base::out);
#endif // #ifdef OUTPUT_TIME_SPACE
   
   // Averaged configurations mean velocity
   Real averaged_configurations_mean_velocity = 0;
   // Averaged configurations mean current
   Real averaged_configurations_mean_current = 0;
   // Averaged configurations mean CO2
   Real averaged_configurations_mean_CO2 = 0;
   // Averaged configurations mean NOx
   Real averaged_configurations_mean_NOx = 0;
   // Averaged configurations mean VOC
   Real averaged_configurations_mean_VOC = 0;
   // Averaged configurations mean PM
   Real averaged_configurations_mean_PM = 0;
   // Averaged configurations std velocity
   Real averaged_configurations_std_velocity = 0;
   // Averaged configurations std CO2
   Real averaged_configurations_std_CO2 = 0;
   // Averaged configurations std NOx
   Real averaged_configurations_std_NOx = 0;
   // Averaged configurations std VOC
   Real averaged_configurations_std_VOC = 0;
   // Averaged configurations std PM
   Real averaged_configurations_std_PM = 0;
   
   for (unsigned i_configuration = 0; i_configuration < N_CONFIGURATIONS; i_configuration++)
    {
     CCNaSchPBC lane;
     lane.initialise(lane_size, maximum_velocity, break_probability);
     
#ifdef OUTPUT_CURRENT_VS_TIME
     std::ostringstream current_vs_time_filename;
     current_vs_time_filename << "RESLT/current_vs_time" << "_bp_" << break_probability << "_rho_" << density << ".dat";
     std::ofstream current_vs_time_file((current_vs_time_filename.str()).c_str(), std::ios_base::out);
#endif // #ifdef OUTPUT_CURRENT_VS_TIME

     // Add vehicles to the lane based on the given density
     lane.fill_in_vehicles(density);
     //lane.print(true); 
     
     // Add bumps to the lane, if any
     std::vector<unsigned> bumps_positions;
     // Equidistance bumps
     //const unsigned h_bump = std::ceil((double)lane_size / (double)(n_bumps + 1));
     const Real h_bump = (Real)lane_size / (Real)(n_bumps + 1);
     //DEB(h_bump);
     for (unsigned kk = 1; kk <= n_bumps; kk++)
      {
       // Add bump
       //DEB(kk);
       //DEB(h_bump*kk);
       bumps_positions.push_back(h_bump*kk);
      }
     lane.set_bumps(bumps_positions);
     
     Real sum_mean_velocity = 0;
     Real sum_mean_current = 0;
     Real sum_mean_co2 = 0;
     Real sum_mean_nox = 0;
     Real sum_mean_voc = 0;
     Real sum_mean_pm = 0;
     Real sum_std_velocity = 0;
     Real sum_std_co2 = 0;
     Real sum_std_nox = 0;
     Real sum_std_voc = 0;
     Real sum_std_pm = 0;
     
     const unsigned long monte_carlo_max_loop = MAX_MONTE_CARLO_LOOP;
     const unsigned long monte_carlo_stabilization_phase = MONTE_CARLO_STAB_PHASE;
     
     // Monte-Carlo loop
     for (unsigned long i = 0; i < monte_carlo_max_loop; i++)
      {
       // Compute the occupancy index
       lane.update_vehicles_list();
       
       // Apply NaSch rules
       lane.apply_nasch();
       
       // Update lane status
       lane.update();
         
       //lane.print(true);
       //lane.print(false); 
         
       // Apply only after stabilization phase
       if (i > monte_carlo_stabilization_phase)
        {             
         //Real mean_velocity = static_cast<Real>(sum_velocity) / static_cast<Real>(lane.current_number_of_vehicles());
         sum_mean_velocity+=lane.mean_velocity();
         //Real mean_current = static_cast<Real>(sum_velocity) / static_cast<Real>(lane.lane_size());
         sum_mean_current+=lane.mean_current();
         
         // Emissions
         sum_mean_co2+=lane.mean_co2();
         sum_mean_nox+=lane.mean_nox();
         sum_mean_voc+=lane.mean_voc();
         sum_mean_pm+=lane.mean_pm();

         // Standard deviation
         sum_std_velocity+=lane.std_velocity();
         sum_std_co2+=lane.std_co2();
         sum_std_nox+=lane.std_nox();
         sum_std_voc+=lane.std_voc();
         sum_std_pm+=lane.std_pm();

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
     // Total mean CO2
     const Real total_mean_CO2 = sum_mean_co2 / total_number_of_instances;
     // Total mean NOx
     const Real total_mean_NOx = sum_mean_nox / total_number_of_instances;
     // Total mean VOC
     const Real total_mean_VOC = sum_mean_voc / total_number_of_instances;
     // Total mean PM
     const Real total_mean_PM = sum_mean_pm / total_number_of_instances;
     // Total std velocity
     const Real total_std_velocity = sum_std_velocity / total_number_of_instances;
     // Total std CO2
     const Real total_std_co2 = sum_std_co2 / total_number_of_instances;
     // Total std NOx
     const Real total_std_nox = sum_std_nox / total_number_of_instances;
     // Total std VOC
     const Real total_std_voc = sum_std_voc / total_number_of_instances;
     // Total std PM
     const Real total_std_pm = sum_std_pm / total_number_of_instances;
     
     averaged_configurations_mean_velocity+=total_mean_velocity;
     averaged_configurations_mean_current+=total_mean_current;
     averaged_configurations_mean_CO2+=total_mean_CO2;
     averaged_configurations_mean_NOx+=total_mean_NOx;
     averaged_configurations_mean_VOC+=total_mean_VOC;
     averaged_configurations_mean_PM+=total_mean_PM;
     averaged_configurations_std_velocity+=total_std_velocity;
     averaged_configurations_std_CO2+=total_std_co2;
     averaged_configurations_std_NOx+=total_std_nox;
     averaged_configurations_std_VOC+=total_std_voc;
     averaged_configurations_std_PM+=total_std_pm;
     
    } // for (i_configuration < N_CONFIGURATIONS)
   
   averaged_configurations_mean_velocity=averaged_configurations_mean_velocity/static_cast<Real>(N_CONFIGURATIONS);
   averaged_configurations_mean_current=averaged_configurations_mean_current/static_cast<Real>(N_CONFIGURATIONS);
   averaged_configurations_mean_CO2=averaged_configurations_mean_CO2/static_cast<Real>(N_CONFIGURATIONS);
   averaged_configurations_mean_NOx=averaged_configurations_mean_NOx/static_cast<Real>(N_CONFIGURATIONS);
   averaged_configurations_mean_VOC=averaged_configurations_mean_VOC/static_cast<Real>(N_CONFIGURATIONS);
   averaged_configurations_mean_PM=averaged_configurations_mean_PM/static_cast<Real>(N_CONFIGURATIONS);
   averaged_configurations_std_velocity=averaged_configurations_std_velocity/static_cast<Real>(N_CONFIGURATIONS);
   averaged_configurations_std_CO2=averaged_configurations_std_CO2/static_cast<Real>(N_CONFIGURATIONS);
   averaged_configurations_std_NOx=averaged_configurations_std_NOx/static_cast<Real>(N_CONFIGURATIONS);
   averaged_configurations_std_VOC=averaged_configurations_std_VOC/static_cast<Real>(N_CONFIGURATIONS);
   averaged_configurations_std_PM=averaged_configurations_std_PM/static_cast<Real>(N_CONFIGURATIONS); 
   
   std::cerr << "bp: " << break_probability
             << "\trho: " << density
             << "\tnbumps: " << n_bumps
             << "\tmJ: " << averaged_configurations_mean_current
             << "\tmV: " << averaged_configurations_mean_velocity
             << "\tmCO2: " <<averaged_configurations_mean_CO2
             << "\tmNOx: " <<averaged_configurations_mean_NOx
             << "\tmVOC: " <<averaged_configurations_mean_VOC
             << "\tmPM: " <<averaged_configurations_mean_PM
             << "\tstdV: " <<averaged_configurations_std_velocity
             << "\tstdCO2: " <<averaged_configurations_std_CO2
             << "\tstdNOx: " <<averaged_configurations_std_NOx
             << "\tstdVOC: " <<averaged_configurations_std_VOC
             << "\tstdPM: " <<averaged_configurations_std_PM << std::endl; 
   
   // -----------------------------------------------------------------------------------------
   // Output data
   // -----------------------------------------------------------------------------------------
   output_file << density << "\t"
               << n_bumps << "\t"
               << averaged_configurations_mean_velocity << "\t"
               << averaged_configurations_mean_current << "\t"
               << averaged_configurations_mean_CO2 << "\t"
               << averaged_configurations_mean_NOx << "\t"
               << averaged_configurations_mean_VOC << "\t"
               << averaged_configurations_mean_PM << "\t"
               << averaged_configurations_std_velocity << "\t"
               << averaged_configurations_std_CO2 << "\t"
               << averaged_configurations_std_NOx << "\t"
               << averaged_configurations_std_VOC << "\t"
               << averaged_configurations_std_PM << std::endl; 
   
#ifdef OUTPUT_TIME_SPACE
   lane_status_file.close();
#endif // #ifdef OUTPUT_TIME_SPACE
   
   // Increase the number of bumps
   n_bumps+=bumps_step;
   
  } // while (n_bumps <= max_nbumps) 
  
 output_file.close();

 // Finalise ca
 finalise_ca();
 
 return 0;
 
}

