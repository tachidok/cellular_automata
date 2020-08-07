#include "cc_person.h"

namespace CA
{
 
 // ----------------------------------------------------------------
 /// Constructor (specify the size of the floor field)
 // ----------------------------------------------------------------
 CCFloorField::CCFloorField(const unsigned m, const unsigned n)
  : M(m), N(n)
 {
  initialise();
 }

 // ----------------------------------------------------------------
 /// Destructor
 // ----------------------------------------------------------------
 virtual ~CCFloorField::CCFloorField()
 {
  
 }

 // ----------------------------------------------------------------
 /// Initialise floor field
 // ----------------------------------------------------------------
 void CCFloorField::initialise()
 {
    /// Initialise static field matrix
  void initialise_static_field_matrix();
  
  /// Initialise dynamic field matrix
  void initialise_dynamic_field_matrix();
  
  /// Initialise occupancy matrix
  void initialise_occupancy_matrix();
  
  /// Initialise obstacle matrix
  void initialise_obstacle_matrix();
 }
 
 // ----------------------------------------------------------------
 /// Update floor fields
 // ----------------------------------------------------------------
 void CCFloorField::update()
 {
    /// Update static field matrix
  void update_static_field_matrix();

  /// Update dynamic field matrix
  void update_dynamic_field_matrix();
  
  /// Update occupancy matrix
  void update_occupancy_matrix();
  
  /// Update obstacle matrix
  void update_obstacle_matrix();
 }

 
  // Returns the value of the static field
  inline const Real static_field(const unsigned i, const unsigned j) {return Static_field[i][j];}
  
  // Returns the value of the dynamic field
  inline const Real dynamic_field(const unsigned i, const unsigned j) {return Dynamic_field[i][j];}
  
  /// Is a given cell occupied
  inline bool is_occupied(const unsigned i, const unsigned j) {return Occupancy_matrix[i][j];}
  
  /// Is there an obstacle
  inline bool is_obstacle(const unsigned i, const unsigned j) {return Obstacle_matrix[i][j];}
 
 // ----------------------------------------------------------------
 /// Initialise static field matrix
 // ----------------------------------------------------------------
 void CCFloorField::initialise_static_field_matrix()
 {
  /// Static field
  std::vector<std::vector<Real> > Static_field; 
 }

 // ----------------------------------------------------------------
 /// Initialise dynamic field matrix
 // ----------------------------------------------------------------
 void CCFloorField::initialise_dynamic_field_matrix()
 {
  /// Dynamic field
  std::vector<std::vector<Real> > Dynamic_field;  
 }

 // ----------------------------------------------------------------
 /// Initialise occupancy matrix
 // ----------------------------------------------------------------
 void CCFloorField::initialise_occupancy_matrix()
 {
  /// Occupancy matrix
  std::vector<std::vector<bool> > Occupancy_matrix;  
 }

 // ----------------------------------------------------------------
 /// Initialise obstacle matrix
 // ----------------------------------------------------------------
 void CCFloorField::initialise_obstacle_matrix()
 {
  /// Obstacle matrix (0 represents a forbidden cell, e.g. wall, 1 for
  /// anything else)
  std::vector<std::vector<bool> > Obstacle_matrix;  
 }

 // ----------------------------------------------------------------
 /// Update static field matrix
 // ----------------------------------------------------------------
 void CCFloorField::update_static_field_matrix()
 {
  
 }

 // ----------------------------------------------------------------
 /// Update dynamic field matrix
 // ----------------------------------------------------------------
 void CCFloorField::update_dynamic_field_matrix()
 {
  
 }

 // ----------------------------------------------------------------
 /// Update occupancy matrix
 // ----------------------------------------------------------------
 void CCFloorField::update_occupancy_matrix()
 {
  
 }

 // ----------------------------------------------------------------
 /// Update obstacle matrix
 // ----------------------------------------------------------------
 void CCFloorField::update_obstacle_matrix()
 {
  
 }
 
} // namespace CA
