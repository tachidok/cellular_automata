#include "../../../../src/general/common_includes.h"
#include "../../../../src/general/cc_person.h"

// Use the namespace of the framework
using namespace CA;

// Used to define arguments
struct Args {
 argparse::ArgValue<Real> rho_m; // Minimum density
 argparse::ArgValue<Real> rho_M; // Maximum density
 argparse::ArgValue<Real> rho_h; // Density step from rho_m to rho_M
 argparse::ArgValue<unsigned> ls; // Lattice size
 argparse::ArgValue<unsigned> nc; // Number of configurations to test
};

int main(int argc, const char** argv)
{
 // Instantiate parser
 Args args;
 auto parser = argparse::ArgumentParser(argv[0], "Demo evacuation from a rectangular lattice");
 
 // Add arguments
 
 // Optional
 parser.add_argument<Real>(args.rho_m, "--rho_m")
  .help("Minimum density of people (number of people on the lattice)")
  .default_value("0.0");
 
 parser.add_argument<Real>(args.rho_M, "--rho_M")
  .help("Maximum density of people (number of people on the lattice)")
  .default_value("1.0");
 
 parser.add_argument<Real>(args.rho_h, "--rho_h")
  .help("Density step until reaching maximum density")
  .default_value("0.1");
 
 parser.add_argument<unsigned>(args.ls, "--ls")
  .help("Lattice size, consider only squared lattices")
  .default_value("10");
 
 parser.add_argument<unsigned>(args.nc, "--nc")
  .help("Number of configurations to test")
  .default_value("100");
 
 // Parse the input arguments
 parser.parse_args(argc, argv);
 
 const Real rho_m = args.rho_m; // Minimum density
 const Real rho_M = args.rho_M; // Maximum density
 const Real rho_h = args.rho_h; // Density step from rho_m to rho_M
 const unsigned lattice_size = args.ls; // Lattice size
 const unsigned n_configurations = args.nc; // Number of configurations to test
 
 // Output for testing/validation
 std::ostringstream output_filename;
 output_filename << "RESLT/output.dat";
 // Output for testing/validation
 std::ofstream output_file((output_filename.str()).c_str(), std::ios_base::out);
 output_file << "rho" << "\t"
             << "mean_iterations" << "\t"
             << "max_iterations" << "\t"
             << "min_iterations" << "\t"
             << "std_dev" << std::endl;
 
 Real current_density = rho_m;
 const unsigned n_steps = (rho_M-rho_m/rho_h) + 1;
 // A counter for the density steps
 unsigned i_rho_step = 0;
 
 // Loop over density
 while (i_rho_step < n_steps)
  {
   // Averaged iteartions
   Real averaged_iterations = 0;
   // Max iterations for current density
   unsigned max_iterations = 0;
   // Min iterations for current density
   unsgined min_iterations = lattice_size*lattice_size;
   
   for (unsigned i_configuration = 0; i_configuration < n_configurations; i_configuration++)
    {
     // Instantiate a stage
     CCStage stage;
     // Initialise stage (with doors and obstacles)
     stage.initialise();
     // Add people to the stage
     stage.fill_stage_with_people(current_density);
     
     // Keep track of the number of iterations
     unsigned n_iterations_current_configuration = 0;
     // Output initial configuration
     stage.output();
     
     // Iterate until stage is empty
     while(!stage.empty())
      {
       stage.simulation_step();
       // Increase the number of iterations
       n_iterations_current_configuration++;
       // Output current configuration
       stage.output();
      } // while(!stage.empty())
     
     // Add to the sum of iterations
     averaged_iterations+=n_iterations_current_configuration;

     // Update the maximum and minimum number of iterations
     if (max_iterations < n_iterations_current_configuration)
      {
       max_iterations = n_iterations_current_configuration;
      }

     if (min_iterations > n_iterations_current_configuration)
      {
       min_iterations = n_iterations_current_configuration;
      }
     
    } // for (i_configuration < n_configurations)
   
   // Compute the average number of iterations
   averaged_iterations=averaged_iterations/(Real)(n_configurations);

   // -----------------------------------------------------------------------------------------
   // Output data
   // -----------------------------------------------------------------------------------------
   output_file << "\trho: " << density
               << "\tmeanIt: " << averaged_iterations
               << "\tmaxIt: " << max_iterations
               << "\tminIt: " << min_iterations
               << std::endl;
   
   std::cerr << "\trho: " << density
             << "\tmeanIt: " << averaged_iterations
             << "\tmaxIt: " << max_iterations
             << "\tminIt: " << min_iterations
             << std::endl;
   
   // Increase density
   current_density+=rho_h;
   
   // Increase the density counter step
   i_rho_step++;
   
  } // while (i_rho_step < n_steps)
 
 std::cout << std::endl;
 std::cout << "[DONE]"<< std::endl;
 std::cout << std::endl;
 
 return 0;
 
}

