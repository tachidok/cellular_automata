#include "cc_crosswalk.h"

namespace CA
  {
    // ----------------------------------------------------------------
    /// Constructor
    // ----------------------------------------------------------------
    Crosswalk::Crosswalk(unsigned position, unsigned crosswalk_length, unsigned crosswalk_width, unsigned maximum_speed, Real alpha, float safety_factor, unsigned N_c, Real go_back_prob)
      {

        initialise(position, crosswalk_length, crosswalk_width, maximum_speed, alpha, safety_factor, N_c, go_back_prob);
      }

    // ----------------------------------------------------------------
    /// Destructor
    // ----------------------------------------------------------------
    Crosswalk::~Crosswalk()
      {
        clear();
      }

    // ----------------------------------------------------------------
    /// Initialise the crosswalk deployment
    // ----------------------------------------------------------------
    void Crosswalk::initialise(unsigned position, unsigned crosswalk_length, unsigned crosswalk_width, unsigned maximum_speed, Real alpha, float safety_factor, unsigned N_c, Real go_back_prob)
      {
        Position = position;
        crossing_length = crosswalk_length;
        crossing_width = crosswalk_width;

        // Set the total number of pedestrians to be kept in a matrix of crosswalk_length x crosswalk_width
        Allowed_number_of_pedestrians = crosswalk_length * crosswalk_width;

        Maximum_speed = maximum_speed;
        Avg_desired_speed = 0;
        // Initialise entry and exit probability
        Alpha = alpha; // entry probability
        //Beta = beta; // exit probability
        PGoBack = go_back_prob;

        Safety_factor = safety_factor;

        Cells_prior_to_CW = N_c; // cells between the intersection and the crosswalk

        // Initialise data structures
        clear();
      }


    // ----------------------------------------------------------------
    /// Clear data structures
    // ----------------------------------------------------------------
    void Crosswalk::clear()
      {
        //crossing.clear();
        crossing.resize(crossing_length, std::vector<Pedestrian*>(crossing_width));
        for (unsigned i = 0; i < crossing_length; i++)
          {
            for (unsigned j = 0; j < crossing_width; j++)
              {
                if (crossing[i][j] != 0)
                  {
                    delete crossing[i][j];
                    crossing[i][j] = 0;
                  }
              }
          }

      Pedestrian_pt.clear();

      Current_number_of_pedestrians = 0;

      }

      // ----------------------------------------------------------------
      /// Determines the empty places within the waiting zone
      // ----------------------------------------------------------------
      std::vector <unsigned> Crosswalk::allowed_waiting_places()
        {
          std::vector<unsigned> wplaces;
          //unsigned positions=0;
          for (unsigned j = 0; j < crossing_width; j++)
            {
              if (crossing[0][j] == 0)
                wplaces.push_back(j);
            }
          return wplaces;
        }


    // ----------------------------------------------------------------
    /// Performs the dynamic entrance of pedestrians and updates their listing
    // ----------------------------------------------------------------
    unsigned Crosswalk::pedestrians_entrance()
      {
        unsigned n_pedestrians = 0;
        // Used to get a seed for the random number engine
        std::random_device rd;
        // Standard mersenne_twister_engine seeded with rd()
        std::mt19937 gen(rd());

        // Use dist to generate a random number into a Real in the range
        // [0,1)
        std::uniform_real_distribution<> dis(0.0, 1.0);

        // Clear current pedestrians list
        Pedestrian_pt.clear();
        // Reserve space for storing data in pedestrians vector.
        //Pedestrian_pt.reserve(Allowed_number_of_pedestrians + 1);

        // Determines the empty places within the waiting zone
        std::vector <unsigned> w_places = allowed_waiting_places();
        const unsigned nw_places = w_places.size();

        // Before adding, check whether the crosswalk has the capacity to hold
        //new pedestrians and whether the waiting zone is empty
        if (Current_number_of_pedestrians < Allowed_number_of_pedestrians && nw_places > 0 && Alpha > 0.0)
          {
            //Compute a random number
            const Real r = dis(gen);

            //New pedestrians are allowed to enter the waiting zone with probability Alpha
            if (r <= Alpha)
              {
              //Each allowed empty waiting place will be occupied by a
              //pedestrian, with probability Alpha
              for (unsigned i = 0; i < nw_places; i++)
                {
                  Real s = dis(gen);
                  //A new pedestrian enters the waiting zone with probability Alpha
                  if (s <= Alpha && crossing[0][w_places[i]] == 0)
                    {
                      Pedestrian *new_pedestrian_pt = new Pedestrian(0, w_places[i], 0);
                      crossing[0][w_places[i]] = new_pedestrian_pt;
                      Current_number_of_pedestrians++;
                    }
                }
              }
          }

        //Update current pedestrians list
        for (unsigned i = 0; i < crossing_length; i++)
          {
            for (unsigned j = 0; j < crossing_width; j++)
              {
                if (crossing[i][j] != 0)
                  {
                    Pedestrian *pedestrian_pt = crossing[i][j];

                    Pedestrian_pt.push_back(pedestrian_pt);

                    n_pedestrians++;
                  }
              }
          }
        return n_pedestrians;
      }


    // ----------------------------------------------------------------
    /// Inserts a pedestrian at location x,y with a given speed
    // ----------------------------------------------------------------
    void Crosswalk::insert_pedestrian(unsigned x, unsigned y, unsigned speed)
      {
        Pedestrian *new_pedestrian_pt = new Pedestrian(x, y, speed);
        crossing[x][y] = new_pedestrian_pt;
        Pedestrian_pt.push_back(new_pedestrian_pt);
        Current_number_of_pedestrians++;
      }

      void Crosswalk::insert_pedestrian(unsigned x, unsigned y, unsigned nextx, unsigned nexty, unsigned speed, unsigned nextspeed)
        {
          Pedestrian *new_pedestrian_pt = new Pedestrian(x, y, nextx, nexty, speed, nextspeed);
          crossing[x][y] = new_pedestrian_pt;
          Pedestrian_pt.push_back(new_pedestrian_pt);
          Current_number_of_pedestrians++;
        }

    // ----------------------------------------------------------------
    /// Verifies whether are pedestrians crossing the lane
    // ----------------------------------------------------------------
    bool Crosswalk::pedestrians_waiting()
      {
        bool waiting = false;
        for (unsigned j = 0; j < crossing_width && waiting == false; j++)
          {
            if (crossing[0][j] != 0)
              {
                waiting = true;
              }
          }
        return waiting;
      }


    // ----------------------------------------------------------------
    /// Verifies whether are pedestrians crossing the lane
    // ----------------------------------------------------------------
    bool Crosswalk::is_occupied()
      {
        bool occupied = false;

        // Verifies the crosswalk occupancy excluding the waiting zone
        for (unsigned i = 1; i < crossing_length && occupied == false; i++)
          {
            for (unsigned j = 0; j < crossing_width && occupied == false; j++)
              {

                if (crossing[i][j] != 0)
                  {
                    occupied = true;
                  }
              }
          }

        return occupied;
      }


      // ----------------------------------------------------------------
      /// Verifies whether are pedestrians within the whole crosswalk
      // ----------------------------------------------------------------
      bool Crosswalk::is_empty()
        {
          bool empty = true;


          if(Current_number_of_pedestrians > 0)
            empty = false;

          return empty;
        }


    // ----------------------------------------------------------------
    /// Identify the pedestrians that are within the neighborhood and can become
    /// an obstacle for the next movement
    // ----------------------------------------------------------------
    std::vector<Pedestrian*> Crosswalk::get_neighbors(unsigned x, unsigned y)
      {
        unsigned t_neighbors = 0;
        unsigned n_neighbors = 3;
        unsigned start_y = 0;
        unsigned neighborhood_width = crossing_width-1;
        // Each pedestrian may have up to 3 neighbors in front and 3 neighbors in back
        std::vector<Pedestrian*> neighbors;


        // Bound the neighborhood width according to the current position
        if (y > 0 && y < crossing_width - 1)
          {
            start_y = y - 1;
            neighborhood_width = y + 1;
            if(x>0 && PGoBack != 0)
              neighbors.assign(6, 0);
            else
              neighbors.assign(3, 0);
          }
        else if (y == 0)
          {
            neighborhood_width = y + 1;
            n_neighbors = 2;
            if(x > 0 && PGoBack != 0)
              neighbors.assign(4, 0);
            else
              neighbors.assign(2, 0);
          }
        else if (y == crossing_width - 1)
          {
            start_y = crossing_width - 2;
            n_neighbors = 2;
            if(x > 0 && PGoBack != 0)
              neighbors.assign(4, 0);
            else
              neighbors.assign(2, 0);
          }


        // Identify the nearest pedestrians within the neighborhood
        for (unsigned l = x + 1; l < crossing_length && l <= x + Maximum_speed; l++)
          {
            unsigned w = 0;
            for (unsigned j = start_y; j <= neighborhood_width; j++)
              {
                if (crossing[l][j] != 0 && neighbors[w] == 0)
                  {
                    Pedestrian *neighbor = crossing[l][j];
                    neighbors[w] = neighbor;
                    t_neighbors++;
                  }
                w++;
              }
            if (t_neighbors == n_neighbors)
              break;
          }

          t_neighbors = 0;

/*
      //
          if(x>0 && pr <= PGoBack)
            {
            for (short l = x - 1; l >= 0 && l >= x - Maximum_speed ; l--)
              {

                unsigned w = 0;
                for (unsigned j = start_y; j <= neighborhood_width; j++)
                  {
                    if (crossing[l][j] != 0 && neighbors[n_neighbors+ w] == 0)
                      {
                        Pedestrian *neighbor = crossing[l][j];
                        neighbors[n_neighbors+ w] = neighbor;
                        t_neighbors++;
                      }
                    w++;
                  }
              }
            }*/

        return neighbors;
      }

    // ----------------------------------------------------------------
    /// For each pedestrian choose the next location within the neighborhood
    // ----------------------------------------------------------------
    void Crosswalk::select_next_location()
      {
        // Used to get a seed for the random number engine
        std::random_device rd;
        // Standard mersenne_twister_engine seeded with rd()
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0, 1.0);

        //For each pedestrian select the next location
        for (unsigned i = 0; i < Current_number_of_pedestrians; i++)
          {
            // Get a pointer to the current pedestrian
            Pedestrian *current_pedestrian = Pedestrian_pt[i];
            const unsigned current_position_x = current_pedestrian->position_x();
            const unsigned current_position_y = current_pedestrian->position_y();

            std::vector<Pedestrian*> next_pedestrians = get_neighbors(current_position_x, current_position_y);
            const unsigned n_neighbors = next_pedestrians.size();

            unsigned start = 0;
            if (current_position_y > 0)
              {
                start = current_position_y - 1;
              }

            // For each neighbor compute the respective space gap and the probability
            // for all the possible forward positions
            Real maxp = 0.0;
            unsigned next_col = 0;
            short space_gap = 0;

            for (unsigned y = 0, yc = start; y < n_neighbors; y++, yc++)
              {
                Real rp = dis(gen);
                short gs = 0;

                //If all places in front have been checked, the selection continues
                //with the back places and the space gap is set to a negative value
                short sgsign = 1;
                if(yc >= crossing_width)
                  {
                    yc = start;
                    sgsign = -1;
                  }

                if(next_pedestrians[y] != 0)
                    {
                      if(next_pedestrians[y]->position_x() > current_position_x)
                        gs = (int)(next_pedestrians[y]->position_x() - current_position_x - 1);
                      else
                        gs = (int)(next_pedestrians[y]->position_x() - current_position_x + 1);
                    }
                else
                    gs = Maximum_speed * sgsign;


/*if(next_pedestrians[y] != 0)
  std::cout<<"rp: " << rp << " n["<< y <<"]: " << next_pedestrians[y]->position_x() << "," << next_pedestrians[y]->position_y() <<" gs: " << gs << std::endl;
else
  std::cout<<"rp: " << rp << " n["<< y <<"]: " << 0 <<" gs: " << gs <<std::endl;*/

                // Choose the location with the great probability, preferring the forward movement
                if(gs != 0)
                  {
                    if (current_position_y == yc) //forward movement
                      {
                        next_col = yc;
                        space_gap = gs;
                        break;
                      }
                    else if(rp > maxp)
                      {
                        maxp = rp;
                        next_col = yc;
                        space_gap = gs;
                      }
                  }
              }



          // Update the future speed and position according to the previous selection
            if((int)current_position_x + space_gap >= 0)
              {
                current_pedestrian->speed(1) = std::abs(space_gap);
                current_pedestrian->position_x(1) = current_position_x + space_gap;
              }
            else
              {
                //This case prevents that a pedestrian go back to a place out of the crosswalk
                current_pedestrian->speed(1) = current_position_x;
                current_pedestrian->position_x(1) = 0;
              }

          current_pedestrian->position_y(1) = next_col;


//std::cout << "P"<< i <<"[" << current_position_x << "," << current_position_y << "]" << " next cell:[" <<  current_pedestrian->position_x(1) << "," << current_pedestrian->position_y(1) << "], speed: " << current_pedestrian->speed(1) << std::endl<< std::endl;
          }
        //Endfor Select the next location for each pedestrian
      }


    // ----------------------------------------------------------------
    /// Computes the crosswalk's dynamics depending on the pedestrian motion rules
    // ----------------------------------------------------------------
    void Crosswalk::apply_motion_rules()
      {
        // Used to get a seed for the random number engine
        std::random_device rd;
        // Standard mersenne_twister_engine seeded with rd()
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, Maximum_speed);

        //unsigned min_speed = Maximum_speed;
        unsigned sum_speed = 0;

        //For each pedestrian select the next location
        for (unsigned i = 0; i < Current_number_of_pedestrians; i++)
          {
            // Get a pointer to the current pedestrian

            Pedestrian *current_pedestrian = Pedestrian_pt[i];
            const unsigned current_position_x = current_pedestrian->position_x();
            const unsigned current_position_y = current_pedestrian->position_y();
            unsigned current_speed = current_pedestrian->speed();
            unsigned allowed_speed = current_pedestrian->speed(1);
            unsigned new_speed = 0;

            /*if(current_speed == 0)
              std::cout<< "current_speed = 0 -> P" << i << " current_pos: [" << current_position_x << "," << current_position_y << "], allowed_speed: " << allowed_speed << ", new_position: ["<< current_pedestrian->position_x(1) <<"," << current_pedestrian->position_y(1) <<"]"<< std::endl;*/

            // First rule (acceleration), allowed only for new pedestrians within the waiting zone
            if(current_position_x == 0 && current_speed == 0)
              {
                unsigned rnd_speed = dis(gen);
                new_speed = std::min(rnd_speed, allowed_speed);
              }
            else if(current_position_x > 0 && current_speed == 0)
              new_speed = allowed_speed;

            // Second rule (deceleration), allowed only for pedestrians outside the waiting zone
            //if(current_position_x > 0)
            if(current_position_x > 0 && current_speed > 0)
              {
                new_speed = std::min(current_speed, allowed_speed);
              }


          /*  if(new_speed < min_speed)
              min_speed = new_speed;*/

            sum_speed += new_speed;


            // Third rule (movement)
            unsigned new_position_x = 0;
            if(current_position_x < current_pedestrian->position_x(1))
              new_position_x = current_position_x + new_speed;
            else
              new_position_x = current_position_x - new_speed;

            //The pedestrians that reach the end of the crosswalk can leave it depending on their new positions
            if(new_position_x >= crossing_length)
              {
//                std::cout<< "exit P" << i << " current_position: [" << current_position_x << "," << current_position_y<<"]"<<std::endl;

                unsigned col = current_position_y;
                delete current_pedestrian;
                current_pedestrian = NULL;
                //Pedestrian_pt[i] = 0;
                //std::cout<< "Pedestrian_pt[i] = 0";
                Pedestrian_pt.erase(Pedestrian_pt.begin()+i);
                crossing[current_position_x][col] = 0;
                Current_number_of_pedestrians--;
                i--;
              }
            else
              {
                // Update the future speed
                current_pedestrian->speed(1) = new_speed;
                current_pedestrian->position_x(1) = new_position_x;
                //current_pedestrian->position_y(1) = current_position_y;
              }
          }

        /*if (Avg_desired_speed != min_speed)
          Avg_desired_speed = min_speed;*/

          Avg_desired_speed = round((float)sum_speed/(float)Current_number_of_pedestrians);

        //update_crosswalk();

      }

    // ----------------------------------------------------------------
    // Update the crosswalk status
    /// This method must be invoked if and only if the security space gap between
    /// the crosswalk and the nearest vehicle is satisfied
    // ----------------------------------------------------------------
    void Crosswalk::update_crosswalk()
      {
        for (unsigned i = 0; i < Current_number_of_pedestrians; i++)
          {
            // Get a pointer to the current pedestrian
            Pedestrian *current_pedestrian = Pedestrian_pt[i];
            const unsigned current_position_x = current_pedestrian->position_x();
            const unsigned current_position_y = current_pedestrian->position_y();
            const unsigned new_position_x = current_pedestrian->position_x(1);
            const unsigned new_position_y = current_pedestrian->position_y(1);


            if(crossing[new_position_x][new_position_y] == 0 )
              {
                crossing[new_position_x][new_position_y] = current_pedestrian;
                crossing[current_position_x][current_position_y] = 0;
                current_pedestrian->update();
              }
            else
              {
                current_pedestrian->remain();
              }

            //std::cout<< "P" << i << " current_position: [" << current_position_x << "," << current_position_y << "], new_speed: " << current_pedestrian->speed() << ", new_position: ["<< current_pedestrian->position_x() <<"," << current_pedestrian->position_y() <<"]"<< std::endl;
          }
      }

    // ----------------------------------------------------------------
    /// Verifies whether the space gap to the crosswalk is safe
    // ----------------------------------------------------------------
    bool Crosswalk::is_safe_to_cross(std::vector<Vehicle*> vehicles, unsigned maximum_velocity)
      {
        bool is_safe = false;
        int min_gap = -1;
        unsigned i = 0;
        std::random_device rd;  //Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
        //std::uniform_int_distribution<> distrib(0, Safety_factor);
        std::uniform_real_distribution<> distrib(0.0, Safety_factor);
        int number_of_vehicles = vehicles.size();


        if(number_of_vehicles > 0)
          {
          while (i < number_of_vehicles && vehicles[i]->position() <= Position)
            {
              min_gap = Position - vehicles[i]->position();
              i++;
            }

          std::uniform_real_distribution<> dis(0.0, 1.0);
          const Real r = dis(gen);
          float Tsf = 0;


          if (r <= 1/Safety_factor)
            Tsf = distrib(gen);


          int critical_gap = round((((float)crossing_length-1) * 0.6)/(float)Avg_desired_speed) + Tsf;

          if(min_gap >= critical_gap)
            is_safe = true;

          //std::cout<< "***************** 1/Safety_factor = "<< 1/Safety_factor << ", Tsf = "<< Tsf << ", AVG_Desired_speed = " << Avg_desired_speed <<", gap_to_CW: "<<min_gap << ", Veh = "<< i-1 <<", critical_gap = "<<critical_gap << std::endl;
          }
        else
          is_safe = true;



        return is_safe;
      }


    // ----------------------------------------------------------------
    // Prints the crosswalk status
    // ----------------------------------------------------------------
    void Crosswalk::print(bool print_speed)
      {
        for (unsigned i = 0; i < crossing_length; i++)
          {
            for (unsigned j = 0; j < crossing_width; j++)
              {
                Pedestrian *pedestrian_pt = crossing[i][j];
                if (pedestrian_pt != 0)
                  {
                    if (print_speed)
                      {
                        const unsigned speed = pedestrian_pt->speed();
                        std::cout << speed;
                      }
                    else
                      {
                        std::cout << "*";
                      }
                  }
                else
                  {
                    std::cout << ".";
                  }
              }
            std::cout << std::endl;
          }
        std::cout << std::endl;
      }

  }
