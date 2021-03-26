#ifndef CC_CROSSWALK_H
#define CC_CROSSWALK_H

#include "../general/common_includes.h"
#include "../general/cc_pedestrian.h"
#include "../general/cc_vehicle.h"

namespace CA
  {

    //Implements the cellular automata model to describe the motion of a pedestrian through a crosswalk
    class Crosswalk
      {

      public:

        // ----------------------------------------------------------------
        /// Constructor
        // ----------------------------------------------------------------
        Crosswalk(unsigned position, unsigned crosswalk_length, unsigned crosswalk_width, unsigned maximum_speed, Real alpha = 0.0, float safety_factor = 2.5, unsigned N_c = 3, Real go_back_prob = 0.0);

        // ----------------------------------------------------------------
        /// Destructor
        // ----------------------------------------------------------------
        ~Crosswalk();

        // ----------------------------------------------------------------
        /// Initialise the crosswalk deployment
        // ----------------------------------------------------------------
        void initialise(unsigned position, unsigned crosswalk_length, unsigned crosswalk_width, unsigned maximum_speed, Real alpha = 0.0, float safety_factor = 2.5, unsigned N_c = 3, Real go_back_prob = 0.0);

        // Get position
        inline unsigned position() const {return Position;}

        // Get the number of cells between the intersection and the crosswalk
        inline unsigned no_cells_prior_to_CW() const {return Cells_prior_to_CW;}

        // ----------------------------------------------------------------
        /// Clear data structures
        // ----------------------------------------------------------------
        void clear();


        // ----------------------------------------------------------------
        /// Determines the empty places within the waiting zone
        // ----------------------------------------------------------------
        std::vector <unsigned> allowed_waiting_places();

        // ----------------------------------------------------------------
        /// Performs the dynamic entrance of pedestrians and updates their listing
        // ----------------------------------------------------------------
        unsigned pedestrians_entrance();

        // ----------------------------------------------------------------
        /// Inserts a pedestrian at location x,y with a given speed
        // ----------------------------------------------------------------
        void insert_pedestrian(unsigned x, unsigned y, unsigned speed);

        void insert_pedestrian(unsigned x, unsigned y, unsigned nextx, unsigned nexty, unsigned speed, unsigned nextspeed);

        // ----------------------------------------------------------------
        ///Verifies whether are pedestrians waiting to cross the lane
        // ----------------------------------------------------------------
        bool pedestrians_waiting();

        // ----------------------------------------------------------------
        /// Verifies whether are pedestrians crossing the lane
        // ----------------------------------------------------------------
        bool is_occupied();

        // ----------------------------------------------------------------
        /// Verifies whether are pedestrians within the whole crosswalk
        // ----------------------------------------------------------------
        bool is_empty();

        // ----------------------------------------------------------------
        /// Identify the pedestrians that are within the neighborhood and can become
        /// an obstacle for the forward movement
        // ----------------------------------------------------------------
        std::vector<Pedestrian*> get_neighbors(unsigned x, unsigned y);

        // ----------------------------------------------------------------
        /// For each pedestrian choose the next location within the neighborhood
        // ----------------------------------------------------------------
        void select_next_location();

        // ----------------------------------------------------------------
        /// Update crosswalk based on pedestrian motion rules
        // ----------------------------------------------------------------
        void apply_motion_rules();

        // ----------------------------------------------------------------
        /// Update the crosswalk status
        // ----------------------------------------------------------------
        void update_crosswalk();


        // ----------------------------------------------------------------
        /// Verifies whether the space gap to the crosswalk is safe
        // ----------------------------------------------------------------
        bool is_safe_to_cross(std::vector<Vehicle*> vehicles, unsigned maximum_velocity);

        // ----------------------------------------------------------------
        /// Prints the crosswalk status
        // ----------------------------------------------------------------
        void print(bool print_velocities = false);


      protected:



        unsigned Position;
        unsigned crossing_length;
        unsigned crossing_width;
        unsigned Maximum_speed;
        unsigned Avg_desired_speed;

        unsigned Cells_prior_to_CW; // Number of cells between the intersection and the crosswalk
        //Cells_prior_to_CW also can be used to smoothly decelerate during a vehicle reaching the crosswalk

        //The pedestrian's aggresiveness comprises the pedestrian start-up lost time and end clearance time
        float Safety_factor; //m'


        // Using non-periodic boundary conditions requires setting Alpha and
        // Beta values, they state the entry and exit probability for the
        // first and last vehicle in the lane, respectively.
        double Alpha; // entry probability
        //double Beta; // exit probability

        //Ability of the pedestrian to back off when there is an obstruction in front of it
        double PGoBack;


        // Allowed number of pedestrians
        unsigned Allowed_number_of_pedestrians;
        // Current number of vehicles (less or equal than
        // "Allowed_number_of_pedestrians")
        unsigned Current_number_of_pedestrians;

        std::vector< std::vector<Pedestrian*> > crossing;
        std::vector<Pedestrian*> Pedestrian_pt;

      };

  }

#endif
