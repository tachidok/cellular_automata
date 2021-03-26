#include "../../../../src/general/common_includes.h"
#include "../../../../src/general/cc_vehicle.h"
#include "../../../../src/nasch/cc_nasch_npbc.h"
#include "../../../../src/general/cc_pedestrian.h"
#include "../../../../src/crosswalk/cc_crosswalk.h"



//#define MAX_MONTE_CARLO_LOOP     1000
//#define MONTE_CARLO_STAB_PHASE   500 // %10 of MAX_MONTE_CARLO_LOOP
#define MAX_MONTE_CARLO_LOOP   20000
#define MONTE_CARLO_STAB_PHASE 15000


#define LANE_SIZE 100
#define MAX_speed 5


#define CROSSING_LENGTH 7
#define CROSSING_WIDTH 3
#define CROSSING_MAX_SPEED 3
#define CROSSING_ALPHA 0.3
#define SAFETY_FACTOR 2.5 //the higher the level the more prudence
#define NC 0

//#define OUTPUT_TIME_SPACE

#define MACRO_VAR

// Use the namespace of the framework
using namespace CA;

// Used to define arguments
struct Args {
 argparse::ArgValue<Real> alphapMax;
 argparse::ArgValue<Real> alphapMin;
 argparse::ArgValue<Real> alphapStep;
 argparse::ArgValue<Real> alphaMax;
 argparse::ArgValue<Real> alphaMin;
 argparse::ArgValue<Real> alphaStep;
 argparse::ArgValue<Real> betaMax;
 argparse::ArgValue<Real> betaMin;
 argparse::ArgValue<Real> betaStep;
 argparse::ArgValue<unsigned> nconfigs;
};


int main(int argc, const char* argv[])
  {
    // Instantiate parser
    Args args;



    auto parser = argparse::ArgumentParser(argv[0], "NaSch algorithm");
    // Add arguments

    // Optional
    parser.add_argument<Real>(args.alphapMin, "--alpha_pMin")
     .help("Minimum probability for pedestrian entrance")
     .default_value("0.1");

    parser.add_argument<Real>(args.alphapMax, "--alpha_pMax")
     .help("Maximum probability for pedestrian entrance")
     .default_value("1.0");

    parser.add_argument<Real>(args.alphapStep, "--alpha_pStep")
      .help("Incremental step for maximum probability for pedestrian entrance")
      .default_value("0.1");

    parser.add_argument<Real>(args.betaMin, "--betaMin")
     .help("Minimum probability for vehicle exit")
     .default_value("0.1");

    parser.add_argument<Real>(args.betaMax, "--betaMax")
     .help("Maximum probability for vehicle exit")
     .default_value("1.0");

    parser.add_argument<Real>(args.betaStep, "--betaStep")
       .help("Incremental step for maximum probability for vehicle exit")
       .default_value("0.1");

    parser.add_argument<Real>(args.alphaMin, "--alphaMin")
      .help("Minimum probability for vehicle entrance")
      .default_value("0.1");

    parser.add_argument<Real>(args.alphaMax, "--alphaMax")
      .help("Maximum probability for vehicle entrance")
      .default_value("1.0");

    parser.add_argument<Real>(args.alphaStep, "--alphaStep")
     .help("Incremental step for maximum probability for vehicle entrance")
     .default_value("0.1");

    parser.add_argument<unsigned>(args.nconfigs, "--nconfigs")
      .help("Number of different simulation configurations")
      .default_value("100");

    // Parse the input arguments
    parser.parse_args(argc, argv);


    const unsigned lane_size = LANE_SIZE;
    const unsigned maximum_speed = MAX_speed;

    const Real maximum_break_probability = 0.5;
    const Real break_probability_step = 1.1;
    Real break_probability = 0;

    // Loop over break probability
    while (break_probability <= maximum_break_probability)
      {
        const Real alpha_min = args.alphaMin;
        const Real alpha_max = args.alphaMax;

        const Real beta_min = args.betaMin;
        const Real beta_max = args.betaMax;

        const Real alpha_p_min = args.alphapMin;
        const Real alpha_p_max = args.alphapMax;

        const Real alpha_step = args.alphaStep;
        const Real beta_step = args.betaStep;
        const Real alpha_p_step = args.alphapStep;

        const Real n_configurations = args.nconfigs;


// for (Alpha_p)
        for (Real alpha_p = alpha_p_min; alpha_p <= alpha_p_max; alpha_p += alpha_p_step)
            {

  // for (Beta)
              for (Real beta = beta_min; beta <= beta_max; beta+=beta_step)
                {
                  #ifdef MACRO_VAR
                    std::ostringstream output_filename;
                    output_filename << "RESLT/" << "AlphaP"<< alpha_p  << "_Beta" << beta << "bp" <<break_probability << ".tfc";//<< CROSSING_ALPHA << "_bp"<< break_probability << ".tfc";
                    // Output for testing/validation
                    std::ofstream output_file((output_filename.str()).c_str(), std::ios_base::out);
                    output_file << "Alpha_p="<< alpha_p <<" \t Beta=" << beta << std::endl;
                    output_file << "alpha \t flow \t density \t speed \t delay \t travel_time \t mean_CO2 \t mean_NOx \t mean_VOC \t mean_PM \t std_speed \t std_CO2 \t std_NOx \t std_VOC \t std_PM"<< std::endl;
                  #endif


  // for (Alpha)

                  for (Real alpha = alpha_min; alpha <= alpha_max; alpha+=alpha_step)
                    {
                      std::cerr << "Working with [alpha_p=" << alpha_p << "] [beta=" << beta << "], [bp=" << break_probability << "], [alpha="<< alpha <<"]" << std::endl;
                      // Averaged configurations mean velocity
                      Real averaged_configurations_mean_speed = 0;
                      // Averaged configurations mean delay
                      Real averaged_configurations_mean_delay = 0;
                      // Averaged configurations mean travel time
                      Real averaged_configurations_mean_travel_time = 0;
                      // Averaged configurations mean current
                      Real averaged_configurations_mean_flow = 0;
                      // Averaged configurations mean density
                      Real averaged_configurations_mean_density = 0;
                      // Averaged configurations mean CO2
                      Real averaged_configurations_mean_CO2 = 0;
                      // Averaged configurations mean NOx
                      Real averaged_configurations_mean_NOx = 0;
                      // Averaged configurations mean VOC
                      Real averaged_configurations_mean_VOC = 0;
                      // Averaged configurations mean PM
                      Real averaged_configurations_mean_PM = 0;
                      // Averaged configurations std velocity
                      Real averaged_configurations_std_speed = 0;
                      // Averaged configurations std CO2
                      Real averaged_configurations_std_CO2 = 0;
                      // Averaged configurations std NOx
                      Real averaged_configurations_std_NOx = 0;
                      // Averaged configurations std VOC
                      Real averaged_configurations_std_VOC = 0;
                      // Averaged configurations std PM
                      Real averaged_configurations_std_PM = 0;

                      for (unsigned i_configuration = 0; i_configuration < n_configurations; i_configuration++)
                        {
                          NaSchNPBC lane;
                          lane.initialise(lane_size, maximum_speed, break_probability, alpha, beta);

                          #ifdef OUTPUT_TIME_SPACE
                          std::ostringstream lane_status_filename;
                          lane_status_filename << "RESLT/lane_" << "alpha_p" << alpha_p << "bp" << break_probability << "_beta_"<< beta << "_alpha_"<< alpha << ".dat";
                          std::ofstream lane_status_file((lane_status_filename.str()).c_str(), std::ios_base::out);
                          #endif // #ifdef OUTPUT_TIME_SPACE

                          //Add the crosswalks to the lane
                          lane.add_crosswalk(lane_size/2, CROSSING_LENGTH, CROSSING_WIDTH, CROSSING_MAX_SPEED, alpha_p, SAFETY_FACTOR, NC);

                          Real sum_mean_speed = 0;
                          Real sum_mean_delay = 0;
                          Real sum_mean_travel_time = 0;
                          Real sum_mean_flow = 0;
                          Real sum_density = 0;
                          Real sum_mean_co2 = 0;
                          Real sum_mean_nox = 0;
                          Real sum_mean_voc = 0;
                          Real sum_mean_pm = 0;
                          Real sum_std_speed = 0;
                          Real sum_std_co2 = 0;
                          Real sum_std_nox = 0;
                          Real sum_std_voc = 0;
                          Real sum_std_pm = 0;

                          const unsigned monte_carlo_max_loop = MAX_MONTE_CARLO_LOOP;
                          const unsigned monte_carlo_stabilization_phase = MONTE_CARLO_STAB_PHASE;

    // Monte-Carlo loop
                          for (unsigned i = 0; i < monte_carlo_max_loop; i++)
                            {
                              // Compute the occupancy index
                              lane.update_vehicles_list();
                              // Apply NaSch rules
                              lane.apply_nasch();

                              // Update lane status
                              lane.update();

                              //density = lane.density();


                              // Apply only after stabilization phase
                              if (i > monte_carlo_stabilization_phase)
                                {
                                  if(i == monte_carlo_stabilization_phase+1)
                                    lane.allow_travel_time_computation();
                                  sum_mean_speed+=lane.mean_velocity();
                                  sum_mean_delay+=lane.mean_delay();
                                  sum_mean_travel_time+=lane.mean_travel_time();
                                  sum_mean_flow+=lane.mean_current();
                                  sum_density+=lane.density();

                                  // Emissions
                                  sum_mean_co2+=lane.mean_co2();
                                  sum_mean_nox+=lane.mean_nox();
                                  sum_mean_voc+=lane.mean_voc();
                                  sum_mean_pm+=lane.mean_pm();

                                  // Standard deviation
                                  sum_std_speed+=lane.std_velocity();
                                  sum_std_co2+=lane.std_co2();
                                  sum_std_nox+=lane.std_nox();
                                  sum_std_voc+=lane.std_voc();
                                  sum_std_pm+=lane.std_pm();

                                  #ifdef OUTPUT_TIME_SPACE
                                  // Output lane status
                                  /*if ((int(density * 100)) % 10 == 0)
                                      {*/
                                        lane.output_time_space(lane_status_file);
                                      //}
                                  #endif // #ifdef OUTPUT_TIME_SPACE
                                  }
                              }
    // endfor (Monte-Carlo)

                          const Real total_number_of_instances =
                          monte_carlo_max_loop - monte_carlo_stabilization_phase - 1;

                          // Total mean speed
                          const Real total_mean_speed = sum_mean_speed / total_number_of_instances;
                          // Total mean delay
                          const Real total_mean_delay = sum_mean_delay / total_number_of_instances;
                          // Total mean travel time
                          const Real total_mean_travel_time = sum_mean_travel_time / total_number_of_instances;
                          // Total mean flow
                          const Real total_mean_flow = sum_mean_flow / total_number_of_instances;
                          // Total mean density
                          const Real total_mean_density = sum_density / total_number_of_instances;

                          // Total mean CO2
                          const Real total_mean_CO2 = sum_mean_co2 / total_number_of_instances;
                          // Total mean NOx
                          const Real total_mean_NOx = sum_mean_nox / total_number_of_instances;
                          // Total mean VOC
                          const Real total_mean_VOC = sum_mean_voc / total_number_of_instances;
                          // Total mean PM
                          const Real total_mean_PM = sum_mean_pm / total_number_of_instances;
                          // Total std velocity
                          const Real total_std_speed = sum_std_speed / total_number_of_instances;
                          // Total std CO2
                          const Real total_std_co2 = sum_std_co2 / total_number_of_instances;
                          // Total std NOx
                          const Real total_std_nox = sum_std_nox / total_number_of_instances;
                          // Total std VOC
                          const Real total_std_voc = sum_std_voc / total_number_of_instances;
                          // Total std PM
                          const Real total_std_pm = sum_std_pm / total_number_of_instances;

                          averaged_configurations_mean_speed+=total_mean_speed;
                          averaged_configurations_mean_delay+=total_mean_delay;
                          averaged_configurations_mean_travel_time+=total_mean_travel_time;
                          averaged_configurations_mean_flow+=total_mean_flow;
                          averaged_configurations_mean_density+=total_mean_density;
                          averaged_configurations_mean_CO2+=total_mean_CO2;
                          averaged_configurations_mean_NOx+=total_mean_NOx;
                          averaged_configurations_mean_VOC+=total_mean_VOC;
                          averaged_configurations_mean_PM+=total_mean_PM;
                          averaged_configurations_std_speed+=total_std_speed;
                          averaged_configurations_std_CO2+=total_std_co2;
                          averaged_configurations_std_NOx+=total_std_nox;
                          averaged_configurations_std_VOC+=total_std_voc;
                          averaged_configurations_std_PM+=total_std_pm;

                        }
// endfor (n_configurations)

                      averaged_configurations_mean_speed=averaged_configurations_mean_speed/static_cast<Real>(n_configurations);
                      averaged_configurations_mean_delay=averaged_configurations_mean_delay/static_cast<Real>(n_configurations);
                      averaged_configurations_mean_travel_time=averaged_configurations_mean_travel_time/static_cast<Real>(n_configurations);
                      averaged_configurations_mean_flow=averaged_configurations_mean_flow/static_cast<Real>(n_configurations);
                      averaged_configurations_mean_density=averaged_configurations_mean_density/static_cast<Real>(n_configurations);
                      averaged_configurations_mean_CO2=averaged_configurations_mean_CO2/static_cast<Real>(n_configurations);
                      averaged_configurations_mean_NOx=averaged_configurations_mean_NOx/static_cast<Real>(n_configurations);
                      averaged_configurations_mean_VOC=averaged_configurations_mean_VOC/static_cast<Real>(n_configurations);
                      averaged_configurations_mean_PM=averaged_configurations_mean_PM/static_cast<Real>(n_configurations);
                      averaged_configurations_std_speed=averaged_configurations_std_speed/static_cast<Real>(n_configurations);
                      averaged_configurations_std_CO2=averaged_configurations_std_CO2/static_cast<Real>(n_configurations);
                      averaged_configurations_std_NOx=averaged_configurations_std_NOx/static_cast<Real>(n_configurations);
                      averaged_configurations_std_VOC=averaged_configurations_std_VOC/static_cast<Real>(n_configurations);
                      averaged_configurations_std_PM=averaged_configurations_std_PM/static_cast<Real>(n_configurations);

                      #ifdef MACRO_VAR
                        output_file << alpha <<
                        "\t" << averaged_configurations_mean_flow <<
                        "\t" << averaged_configurations_mean_density <<
                        "\t" << averaged_configurations_mean_speed<<
                        "\t" << averaged_configurations_mean_delay<<
                        "\t" << averaged_configurations_mean_travel_time<<
                        "\t" << averaged_configurations_mean_CO2<<
                        "\t" << averaged_configurations_mean_NOx<<
                        "\t" << averaged_configurations_mean_VOC<<
                        "\t" << averaged_configurations_mean_PM <<
                        "\t" << averaged_configurations_std_speed <<
                        "\t" << averaged_configurations_std_CO2 <<
                        "\t" << averaged_configurations_std_NOx <<
                        "\t" << averaged_configurations_std_VOC <<
                        "\t" << averaged_configurations_std_PM <<
                        std::endl;
                      #endif

                      #ifdef OUTPUT_TIME_SPACE
                        lane_status_file.close();
                      #endif
                    }
// endfor (Alpha)

                #ifdef MACRO_VAR
                  output_file.close();
                #endif
                }
// endfor (Beta)
            }
//endfor (Alpha_p)

        // Increase break probability
        break_probability+=break_probability_step;

      } // while (break_probability <= maximum_break_probability)

 return 0;

}
