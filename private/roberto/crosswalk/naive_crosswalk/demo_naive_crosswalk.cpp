#include "../../../../src/general/common_includes.h"
#include "../../../../src/general/cc_pedestrian.h"
#include "../../../../src/crosswalk/cc_crosswalk.h"

#define CROSSING_LENGTH 7
#define CROSSING_WIDTH 3
#define CROSSING_MAX_SPEED 3
#define CROSSING_ALPHA 0.5
#define SAFETY_FACTOR 2.5 //the higher the level the more prudence
#define NC 2



using namespace CA;

int main()
  {
    //Crosswalk crossing(CROSSING_LENGTH, CROSSING_WIDTH, MAX_SPEED, ALPHA);
    Crosswalk crossing(50, CROSSING_LENGTH, CROSSING_WIDTH, CROSSING_MAX_SPEED, CROSSING_ALPHA, SAFETY_FACTOR, NC);

    crossing.insert_pedestrian(0, 0, 1, 0, 0, 1);
    crossing.insert_pedestrian(0, 1, 2, 2, 0, 2);
    crossing.insert_pedestrian(0, 2, 2, 2, 0, 2);

    crossing.insert_pedestrian(1, 1, 2, 1, 1, 1);

    crossing.insert_pedestrian(2, 0, 5, 0, 1, 3);

    crossing.insert_pedestrian(3, 1, 6, 1, 1, 3);
    crossing.insert_pedestrian(3, 2, 6, 2, 1, 3);

    crossing.insert_pedestrian(6, 0, 9, 0, 2, 3);

    for(unsigned i =0; i < 6; i++)
      {
        //crossing.pedestrians_entrance();

      /*  if(crossing.pedestrians_waiting())
          std::cout << "Pedestrians waiting" << std::endl;
        else
          std::cout << "FREE" << std::endl;

    crossing.insert_pedestrian(1, 1, 2);
    crossing.insert_pedestrian(3, 2, 2);
    crossing.insert_pedestrian(4, 1, 2);
    crossing.insert_pedestrian(4, 2, 2);*/



        crossing.print(true);



        crossing.select_next_location();

        crossing.apply_motion_rules();

        crossing.update_crosswalk();


        /*if(crossing.is_occupied())
          std::cout << "is occupied---------------" << std::endl;
        else
          std::cout << "is free---------------" << std::endl;*/

        //crossing.print(true);
      }




    return 0;
  }
