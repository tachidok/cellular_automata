#ifndef CC_PEDESTRIAN_H
#define CC_PEDESTRIAN_H

#include "common_includes.h"

namespace CA
  {

    class Pedestrian
      {
      public:
        // ----------------------------------------------------------------
        // Constructor
        // ----------------------------------------------------------------
        Pedestrian();

        Pedestrian(unsigned positionx, unsigned positiony, unsigned speed);

        Pedestrian(unsigned current_positionx, unsigned current_positiony, unsigned next_positionx, unsigned next_positiony, unsigned current_speed, unsigned next_speed);

        // ----------------------------------------------------------------
        // Destructor
        // ----------------------------------------------------------------
        virtual ~Pedestrian();

        void initialise(unsigned positionx, unsigned positiony, unsigned speed);

        // ----------------------------------------------------------------
        // Set the new current (x,y) position and speed
        // ----------------------------------------------------------------
        void update();

        // ----------------------------------------------------------------
        // Update the future status with the current values
        // ----------------------------------------------------------------
        void remain();

        // Set speed
        inline unsigned &speed(unsigned i = 0) {return Speed[i];}
        // Get speed
        inline unsigned speed(unsigned i = 0) const {return Speed[i];}
        // Set X position
        inline unsigned &position_x(unsigned i = 0) {return Position_x[i];}
        // Get X position
        inline unsigned position_x(unsigned i = 0) const {return Position_x[i];}
        // Set Y position
        inline unsigned &position_y(unsigned i = 0) {return Position_y[i];}
        // Get Y position
        inline unsigned position_y(unsigned i = 0) const {return Position_y[i];}

      protected:

        unsigned Speed[2];

        unsigned Position_x[2];

        unsigned Position_y[2];


      };


  }

#endif
