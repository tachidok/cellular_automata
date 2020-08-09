#ifndef CC_FLOORFIELD_H
#define CC_FLOORFIELD_H

#include "common_includes.h"
#include "utilities.h"

#include "../../../src/general/cc_person.h"

/// This floor field is based on the paper Kirchner, Ansgar and
/// Schadschneider, Andreas, Simulation of evacuation processes using
/// a bionics-inspired cellular automaton model for pedestrian
/// dynamics, Physica A, Elsevier, 2002.
namespace CA
{

#define OCCUPIED_CELL 1
#define NON_OCCUPIED_CELL 0
#define OBSTACLE_CELL 1
 
 /// Implements the floor field used to implement the dynamics people
 /// uses to move on different stages
 class CCFloorField
 {
  
 public:
  
  /// Constructor (specify the size of the floor field)
  CCFloorField(const unsigned m, const unsigned n);
  
  /// Destructor
  virtual ~CCFloorField();
  
  /// Get the m-dimension of the floor field
  inline const unsigned m() const {return M;}
  
  /// Get the n-dimension of the floor field
  inline const unsigned n() const {return N;}
  
  /// Initialise floor field with emergency exits
  void initialise();
  
  /// Add emergency exits to the stage
  void add_emergency_exit(const unsigned i, const unsigned j);
  
  /// Get the number of emergency exits
  inline const unsigned n_emergency_exit() {return Emergency_exit.size();}
  
  /// Initialise floor field with emergency exits
  void fill_with_people(const Real density);
  
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
  
  /// Returns the value of the static field
  const Real static_field(const unsigned i, const unsigned j);
  
  /// Returns the value of the dynamic field
  const Real dynamic_field(const unsigned i, const unsigned j);
  
  /// Is a given cell occupied
  bool is_occupied(const unsigned i, const unsigned j);
  
  /// Is there an obstacle
  bool is_obstacle(const unsigned i, const unsigned j);
  
  // Get the number of people in the field
  inline const unsigned n_people() {return People_pt.size()}
  
  /// Get the i-th people in the field
  CCPerson *people_pt(const unsigned i);
  
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
  
  /// Store the position of the emergency exits
  std::vector<std::vector<unsigned> > Emergency_exit;
  
  /// People vector pointer
  std::vector<CCPerson *> People_pt;
  
  /// Add a person to field (check whether there are no obstacles or
  /// the positon is already occupied by another person)
  bool add_person_to_field(CCPerson *person_pt);
  
  /// Clean up emergency exits
  inline void clean_emergency_exits() {Emergency_exit.clear();}
  
  /// Allocate matrices memory
  void allocate_matrices_memory();
  
  /// Set stage boundary configuration
  void set_boundary_configuration();
  
  /// Build wall at the boundary of the floor field
  void build_wall_at_boundary();
  
  /// Configure emergency exits
  void set_emergency_exits();
  
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

