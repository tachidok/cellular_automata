#include "cc_pedestrian.h"

namespace CA
  {

    // ----------------------------------------------------------------
    /// Constructor
    // ----------------------------------------------------------------
    Pedestrian::Pedestrian()
      {
        initialise(0, 0, 0);
      }

    Pedestrian::Pedestrian(unsigned positionx, unsigned positiony, unsigned speed)
      {
        initialise(positionx, positiony, speed);
      }


    Pedestrian::Pedestrian(unsigned current_positionx, unsigned current_positiony, unsigned next_positionx, unsigned next_positiony, unsigned current_speed, unsigned next_speed)
      {
        Position_x[0] = current_positionx;
        Position_x[1] = next_positionx;
        Position_y[0] = current_positiony;
        Position_y[1] = next_positiony;
        Speed[0] = current_speed;
        Speed[1] = next_speed;
      }
    // ----------------------------------------------------------------
    /// Destructor
    // ----------------------------------------------------------------
    Pedestrian::~Pedestrian()
        {

        }

      // ----------------------------------------------------------------
      /// Set the current (x,y) position and speed
      // ----------------------------------------------------------------
    void Pedestrian::initialise(unsigned positionx, unsigned positiony, unsigned speed)
        {
          Position_x[1] = Position_x[0] = positionx;
          Position_y[1] = Position_y[0] = positiony;
          Speed[1] = Speed[0] = speed;
        }

      // ----------------------------------------------------------------
      /// Set the new current (x,y) position and speed
      // ----------------------------------------------------------------
    void Pedestrian::update()
        {
          Position_x[0] = Position_x[1];
          Position_y[0] = Position_y[1];
          Speed[0] = Speed[1];
        }


    // ----------------------------------------------------------------
    /// Update the future status with the current values
    // ----------------------------------------------------------------
    void Pedestrian::remain()
      {
        Position_x[1] = Position_x[0];
        Position_y[1] = Position_y[0];
        Speed[1] = Speed[0];
      }
  }
