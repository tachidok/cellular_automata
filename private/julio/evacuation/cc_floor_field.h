#ifndef CC_FLOORFIELD_H
#define CC_FLOORFIELD_H

#include "common_includes.h"
#include "utilities.h"

namespace CA
{
 
 /// Implements the floor field used to implement the dynamics people
 /// uses to move on different stages
 class CCFloorField
 {
  
 public:
  
  /// Constructor (specify the size of the floor field)
  CCFloorField(const unsigned m, const unsigned n);
  
  /// Destructor
  virtual ~CCFloorField();
  
  /// Initialise floor field
  void initialise();
  
  /// Update floor fields
  void update();
  
  /// Set static field weight parameter
  inline Real &k_s() {return K_s;}
  
  /// Get static field weight parameter
  inline Real k_s() const {return K_s;}
  
  /// Set dynamic field weight parameter
  inline Real &k_d() {return K_d;}
  
  /// Get dynamic field weight parameter
  inline Real k_d() const {return K_d;}
  
  // Returns the value of the static field
  inline const Real static_field(const unsigned i, const unsigned j) {return Static_field[i][j];}
  
  // Returns the value of the dynamic field
  inline const Real dynamic_field(const unsigned i, const unsigned j) {return Dynamic_field[i][j];}
  
  /// Is a given cell occupied
  inline bool is_occupied(const unsigned i, const unsigned j) {return Occupancy_matrix[i][j];}
  
  /// Is there an obstacle
  inline bool is_obstacle(const unsigned i, const unsigned j) {return Obstacle_matrix[i][j];}
  
 protected:
  
  /// Dimension of the floor field
  const unsigned M;
  const unsigned N;
  
  /// Static field weight parameter
  const Real K_s;
  
  /// Dynamic field weight parameter
  const Real K_d;
  
  /// Static field
  std::vector<std::vector<Real> > Static_field;
  
  /// Dynamic field
  std::vector<std::vector<Real> > Dynamic_field;
  
  /// Occupancy matrix
  std::vector<std::vector<bool> > Occupancy_matrix;
  
  /// Obstacle matrix (0 represents a forbidden cell, e.g. wall, 1 for
  /// anything else)
  std::vector<std::vector<bool> > Obstacle_matrix;
  
  /// Initialise static field matrix
  void initialise_static_field_matrix();
  
  /// Initialise dynamic field matrix
  void initialise_dynamic_field_matrix();
  
  /// Initialise occupancy matrix
  void initialise_occupancy_matrix();
  
  /// Initialise obstacle matrix
  void initialise_obstacle_matrix();

  /// Update static field matrix
  void update_static_field_matrix();

  /// Update dynamic field matrix
  void update_dynamic_field_matrix();
  
  /// Update occupancy matrix
  void update_occupancy_matrix();
  
  /// Update obstacle matrix
  void update_obstacle_matrix();
  
 };
 
} // namespace CA

#endif // #ifndef CC_FLOORFIELD_H

