#include "../../../../src/general/common_includes.h"
#include "../../../../src/general/cc_vehicle.h"
#include "../../../../src/nasch/cc_nasch_npbc.h"

//#define MAX_MONTE_CARLO_LOOP   20000
//#define MONTE_CARLO_STAB_PHASE 15000

#define MAX_MONTE_CARLO_LOOP     500
#define MONTE_CARLO_STAB_PHASE   100 // %10 of MAX_MONTE_CARLO_LOOP

#define LANE_SIZE 100
#define MAX_VELOCITY 5

#define OUTPUT_TIME_SPACE

//#define MACRO_VAR

// Use the namespace of the framework
using namespace CA;

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
        const double alpha_step = 0.1;
        //const double alpha_step = 0.01;
        const double beta_step = 0.1;

        double density = 0.0;

// for (beta < beta_max)
        for (double beta = beta_min; beta <= beta_max; beta+=beta_step)
          {
            std::cerr << "Working with [beta=" << beta << "] and [bp=" << break_probability << "] ..." << std::endl;



            #ifdef MACRO_VAR
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
            #endif

// for (alpha < alpha_max)
            for (double alpha = alpha_min; alpha <= alpha_max; alpha+=alpha_step)
              {
                NaSchNPBC lane;
                lane.initialise(lane_size, maximum_velocity, break_probability, alpha, beta);

                #ifdef OUTPUT_TIME_SPACE
                  std::ostringstream lane_status_filename;
                  lane_status_filename << "RESLT/lane_" << "bp" << break_probability << "_beta_"<< beta << "_alpha_"<< alpha << ".dat";
                  std::ofstream lane_status_file((lane_status_filename.str()).c_str(), std::ios_base::out);
                #endif // #ifdef OUTPUT_TIME_SPACE

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
                    unsigned sum_velocity = lane.apply_nasch();
                    // Update lane status
                    lane.update();

                    //lane.print(true);
                    //lane.print(false);

                    //const double density = lane.density();
                    //DEB(density);
                    //std::cerr << lane.current_number_of_vehicles() << " " << sum_velocity << std::endl;
                    density = lane.density();
                    // Apply only after stabilization phase

                    #ifdef OUTPUT_TIME_SPACE
                      // Output lane status
                      /*if ((int(density * 100)) % 10 == 0)
                        {*/
                          lane.output_time_space(lane_status_file);
                        //}
                    #endif // #ifdef OUTPUT_TIME_SPACE

                    if (i > monte_carlo_stabilization_phase)
                      {
                        double mean_velocity = double(sum_velocity) / double(lane.current_number_of_vehicles());
                        sum_mean_velocity+=mean_velocity;
                        double mean_current = double(sum_velocity) / double(lane.lane_size());
                        sum_mean_current+=mean_current;
                      }

                  }
// for (i < monte_carlo_max_loop)

                const double total_number_of_instances =
                monte_carlo_max_loop - monte_carlo_stabilization_phase - 1;

                // Total mean velocity
                const double total_mean_velocity = sum_mean_velocity / total_number_of_instances;
                // Total mean current
                const double total_mean_current = sum_mean_current / total_number_of_instances;

                //const double density = lane.density();

                //std::cerr << "p: " << break_probability << "\talpha: " << alpha << "\tbeta: " << beta <<  "\tmJ: " << total_mean_current << "\tmV: " << total_mean_velocity << std::endl;
                #ifdef MACRO_VAR
                  density_file << alpha << "\t" << density << std::endl;
                  velocity_file << alpha << "\t" << total_mean_velocity << std::endl;
                  current_file << alpha << "\t" << total_mean_current << std::endl;
                #endif

                #ifdef OUTPUT_TIME_SPACE
                  lane_status_file.close();
                #endif // #ifdef OUTPUT_TIME_SPACE

              }
// for (alpha < alpha_max)


            #ifdef MACRO_VAR
              density_file.close();
              velocity_file.close();
              current_file.close();
            #endif

          }
// for (beta < beta_max)

        // Increase break probability
        break_probability+=break_probability_step;

      } // while (break_probability <= maximum_break_probability)

 return 0;

}
