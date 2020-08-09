#include "cc_person.h"

namespace CA
{
 
 // ----------------------------------------------------------------
 /// Constructor (specify the size of the floor field)
 // ----------------------------------------------------------------
 CCFloorField::CCFloorField(const unsigned m, const unsigned n)
  : M(m), N(n)
 {
  // Allocate memory for matrices (static field, dynamic field,
  // occupancy and obstacles)
  allocate_matrices_memory();
  
  // Clear any emergency exit
  clean_emergency_exits();
 }
 
 // ----------------------------------------------------------------
 /// Destructor
 // ----------------------------------------------------------------
 virtual ~CCFloorField::CCFloorField()
 {
  // Clear any emergency exit
  clean_emergency_exits();
 }
 
 // ----------------------------------------------------------------
 /// Allocate matrices memory
 // ----------------------------------------------------------------
 void CCFloorField::allocate_matrices_memory()
 {
  // Allocate memory for all matrices
  Static_field.resize(M);
  Dynamic_field.resize(M);
  Occupancy_matrix.resize(M);
  Obstacle_matrix.resize(M);
  for (unsigned i = 0; i < M; i++)
   {
    Static_field[i].resize(N, 0);
    Dynamic_field[i].resize(N, 0);
    Occupancy_matrix[i].resize(N, NON_OCCUPIED_CELL);
    Obstacle_matrix[i].resize(N, NON_OCCUPIED_CELL);
   }
  
 }
 
 // ----------------------------------------------------------------
 /// Initialise floor field
 // ----------------------------------------------------------------
 void CCFloorField::initialise()
 { 
  // Set boundary configuration
  set_boundary_configuration();
  
  // Initialise static field matrix
  initialise_static_field_matrix();
  
  // Initialise dynamic field matrix
  initialise_dynamic_field_matrix();
  
  /// Initialise occupancy matrix
  initialise_occupancy_matrix();
  
  /// Initialise obstacle matrix
  initialise_obstacle_matrix();
 }

 // ----------------------------------------------------------------
 /// Add emergency exits to the stage
 // ----------------------------------------------------------------
 void CCFloorField::add_emergency_exit(const unsigned i, const unsigned j)
 {
  /// Range check
#ifdef CELLULAR_AUTOMATON_RANGE_CHECK
  if (i >= m || j >= n)
   {
    // Error message
    std::ostringstream error_message;
    error_message << "The entry you are trying to read is out of range\n"
                  << "You requested for the entry ("<<i<<", "<<j<<")\n"
                  << "The range for i should be in [0, "<<m-1<<"]\n"
                  << "The range for j should be in [0, "<<n-1<<"]\n"
                  << std::endl;
    throw CALibError(error_message.str(),
                     CA_CURRENT_FUNCTION,
                     CA_EXCEPTION_LOCATION);
   }
#endif // #ifdef CELLULAR_AUTOMATON_RANGE_CHECK
  std::vector<unsigned> tmp(2);
  tmp[0] = i;
  tmp[1] = j;
  Emergency_exit.push_back(tmp);
 }
 
 // ----------------------------------------------------------------
 /// Initialise floor field with emergency exits
 // ----------------------------------------------------------------
 void CCFloorField::fill_with_people(const Real density)
 {
  // Used to get a seed for the random number engine
  std::random_device rd;
  // Standard mersenne_twister_engine seeded with rd()
  std::mt19937 gen(rd());
  
  // Use dist to generate a random number into a Real in the range
  // [0,1)
  std::uniform_real_distribution<> dis(0.0, 1.0);
  
  // Floor field size
  const unsigned field_size = M*N;
  
  // Get the number of people to add to the floor field
  const unsigned n_people = static_cast<double>(field_size)*density;
  
  // If the number of people to be added to the floor field is larger
  // or equal to the size of the foor field then fill is with people
  if (n_people >= field_size)
   {
    for (unsigned i = 0; i < M; i++)
     {
      for (unsigned j = 0; j < N; j++)
       {
        // The position of the person
        std::vector<unsigned> tmp(2);
        tmp[0] = i;
        tmp[1] = j;
        CCPerson *new_person_pt = CCPerson(tmp);
        // Add the person to the field
        add_person_to_field(new_person_pt);
       }
     }
   }
  else
   {
    // Counter
    unsigned k = 0;
    // Add people to the floor field
    while (k < n_people)
     {
      // Random position to add a person
      const Real r_i = dis(gen);
      const Real r_j = dis(gen);
      unsigned i = r_i * M;
      unsigned j = r_j * N;      
      // Check whether this entry is already occupied
      if (!is_occupied(i, j) && !is_obstacle(i, j))
       {
        // The position of the person
        std::vector<unsigned> tmp(2);
        tmp[0] = i;
        tmp[1] = j;
        // Create the person
        CCPerson *new_person_pt = CCPerson(tmp);
        // Try to add the person to the field
        if (add_person_to_field(new_person_pt))
         {
          // Increase the number of people added to the floor field
          k++;
         }
        
       }
      
     } // while (k < n_people)
    
   }
  
 }

 // ----------------------------------------------------------------
 /// Add a person to field (check whether there are no obstacles or
 /// the positon is already occupied by another person)
 // ----------------------------------------------------------------
 bool CCFloorField::add_person_to_field(CCPerson *person_pt)
 {
  // Get the position of the person
  const unsigned i = person_pt.position(0);
  const unsigned j = person_pt.position(1);
  /// Range check
#ifdef CELLULAR_AUTOMATON_RANGE_CHECK
  if (i >= M || j >= N)
   {
    // Error message
    std::ostringstream error_message;
    error_message << "You are trying to add a person in the position ("<<i<<", "<<j<<")\n"
                  << "However, the field size is in the following range:\n"
                  << "The range for i should be in [0, "<<M-1<<"]\n"
                  << "The range for j should be in [0, "<<N-1<<"]\n"
                  << std::endl;
    throw CALibError(error_message.str(),
                     CA_CURRENT_FUNCTION,
                     CA_EXCEPTION_LOCATION);
   }
#endif // #ifdef CELLULAR_AUTOMATON_RANGE_CHECK
  
  /// Check whether the cell is already occupied by another person
  if (is_occupied(i, j))
   {
    // The cell is already occupied therefore we cannot add a person
    // in that position
    return false;
   }
  
  /// Check whether there is an obstacle in that position
  if (is_obstacle(i, j))
   {
    // There is an obstacle therefore we cannot add a person in that
    // position
    return false;
   }
  
  // The position is empty, add the person to the field
  People_pt.push_back(person_pt);
  
  // Update the status of the occupancy matrix
  Occupancy_matrix[i][j] = OCCUPIED_CELL;
  
  return true;
  
 }
 
 // ----------------------------------------------------------------
 /// Update floor fields
 // ----------------------------------------------------------------
 void CCFloorField::update()
 {
  /// Move people based on the transition probabilities matrix
  
  
  /// Update static field matrix
  void update_static_field_matrix();
  
  /// Update dynamic field matrix
  void update_dynamic_field_matrix();
  
  /// Update occupancy matrix
  void update_occupancy_matrix();
  
  /// Update obstacle matrix
  void update_obstacle_matrix();
 }
 
 // ----------------------------------------------------------------
 /// Get the i-th people in the field
 // ----------------------------------------------------------------
 CCPerson *CCFloorField::people_pt(const unsigned i)
 {
  /// Range check
#ifdef CELLULAR_AUTOMATON_RANGE_CHECK
  // Get the number of people in the field
  const unsigned npeople = n_people();
  if (i >= npeople)
   {
    // Error message
    std::ostringstream error_message;
    error_message << "You are trying to get the "<<i<<" entry in the people vector\n"
                  << "However, we only have "<<npeople<<" entries in the people vector\n"
                  << "The range should be in [0, "<<npeople-1<<"]\n"
                  << std::endl;
    throw CALibError(error_message.str(),
                     CA_CURRENT_FUNCTION,
                     CA_EXCEPTION_LOCATION);
   }
#endif // #ifdef CELLULAR_AUTOMATON_RANGE_CHECK
  return People_pt[i];
 }
 
 // ----------------------------------------------------------------
 /// Returns the value of the static field
 // ----------------------------------------------------------------
 inline const Real CCFloorField::static_field(const unsigned i, const unsigned j)
 {
  /// Range check
#ifdef CELLULAR_AUTOMATON_RANGE_CHECK
  if (i >= M || j >= N)
   {
    // Error message
    std::ostringstream error_message;
    error_message << "The entry you are trying to read is out of range\n"
                  << "You requested for the entry ("<<i<<", "<<j<<")\n"
                  << "The range for i should be in [0, "<<M-1<<"]\n"
                  << "The range for j should be in [0, "<<N-1<<"]\n"
                  << std::endl;
    throw CALibError(error_message.str(),
                     CA_CURRENT_FUNCTION,
                     CA_EXCEPTION_LOCATION);
   }
#endif // #ifdef CELLULAR_AUTOMATON_RANGE_CHECK
  return Static_field[i][j];
 }

 // ----------------------------------------------------------------
 /// Returns the value of the dynamic field
 // ----------------------------------------------------------------
 const Real CCFloorField::dynamic_field(const unsigned i, const unsigned j)
 {
  /// Range check
#ifdef CELLULAR_AUTOMATON_RANGE_CHECK
  if (i >= M || j >= N)
   {
    // Error message
    std::ostringstream error_message;
    error_message << "The entry you are trying to read is out of range\n"
                  << "You requested for the entry ("<<i<<", "<<j<<")\n"
                  << "The range for i should be in [0, "<<M-1<<"]\n"
                  << "The range for j should be in [0, "<<N-1<<"]\n"
                  << std::endl;
    throw CALibError(error_message.str(),
                     CA_CURRENT_FUNCTION,
                     CA_EXCEPTION_LOCATION);
   }
#endif // #ifdef CELLULAR_AUTOMATON_RANGE_CHECK  
  return Dynamic_field[i][j];
 }
 
 // ----------------------------------------------------------------
 /// Is a given cell occupied
 // ----------------------------------------------------------------
 bool CCFloorField::is_occupied(const unsigned i, const unsigned j)
 {
  /// Range check
#ifdef CELLULAR_AUTOMATON_RANGE_CHECK
  if (i >= M || j >= N)
   {
    // Error message
    std::ostringstream error_message;
    error_message << "The entry you are trying to read is out of range\n"
                  << "You requested for the entry ("<<i<<", "<<j<<")\n"
                  << "The range for i should be in [0, "<<M-1<<"]\n"
                  << "The range for j should be in [0, "<<N-1<<"]\n"
                  << std::endl;
    throw CALibError(error_message.str(),
                     CA_CURRENT_FUNCTION,
                     CA_EXCEPTION_LOCATION);
   }
#endif // #ifdef CELLULAR_AUTOMATON_RANGE_CHECK
  return Occupancy_matrix[i][j];
 }
 
 // ----------------------------------------------------------------
 /// Is there an obstacle
 // ----------------------------------------------------------------
 bool CCFloorField::is_obstacle(const unsigned i, const unsigned j)
 {
  /// Range check
#ifdef CELLULAR_AUTOMATON_RANGE_CHECK
  if (i >= M || j >= N)
   {
    // Error message
    std::ostringstream error_message;
    error_message << "The entry you are trying to read is out of range\n"
                  << "You requested for the entry ("<<i<<", "<<j<<")\n"
                  << "The range for i should be in [0, "<<M-1<<"]\n"
                  << "The range for j should be in [0, "<<N-1<<"]\n"
                  << std::endl;
    throw CALibError(error_message.str(),
                     CA_CURRENT_FUNCTION,
                     CA_EXCEPTION_LOCATION);
   }
#endif // #ifdef CELLULAR_AUTOMATON_RANGE_CHECK
  return Obstacle_matrix[i][j];
 }
 
 // ----------------------------------------------------------------
 /// Set stage boundary configuration
 // ----------------------------------------------------------------
 void CCFloorField::set_boundary_configuration()
 {
  // Build the wall
  build_wall_at_boundary();
  // And create the emergency exits
  set_emergency_exits();
 }
 
 // ----------------------------------------------------------------
 /// Build wall at the boundary of the floor field
 // ----------------------------------------------------------------
 void CCFloorField::build_wall_at_boundary()
 {
  // Use the obstacle matrix to represent a wall a the boundary
  // ----------------------
  // Vertical walls
  for (unsigned i = 0; i < M; i++)
   {
    Obstacle_matrix[i][0] = OBSTACLE_CELL;
    Obstacle_matrix[i][N-1] = OBSTACLE_CELL;
   }
  
  // ----------------------
  // Horizontal walls
  for (unsigned j = 1; j < N-1; j++)
   {
    Obstacle_matrix[0][j] = OBSTACLE_CELL;
    Obstacle_matrix[M-1][j] = OBSTACLE_CELL;
   }
 }
 
 // ----------------------------------------------------------------
 /// Configure emergency exits
 // ----------------------------------------------------------------
 void CCFloorField::set_emergency_exits()
 {
  // Get the number of emergency exits
  const unsigned nemergency_exits = Emergency_exit.size();
  
  // Check there is at least one emergency exit
  if (nemergency_exits == 0)
   {
    // Error message
    std::ostringstream error_message;
    error_message << "You have not established emergency exits"
                  << std::endl;
    throw CALibError(error_message.str(),
                     CA_CURRENT_FUNCTION,
                     CA_EXCEPTION_LOCATION);
   }
  
  // Loop over the emergency exits and change the entry in the
  // obstacle matrix to a non obstacle cell
  for (unsigned k = 0; k < nemergency_exits; k++)
   {
    // Get the position of the emergency exit
    const unsigned i = Emergency_exit[k][0];
    const unsigned j = Emergency_exit[k][1];
    // Change the entry in the obstacle matrix
    Obstacle_matrix[i][j] = NON_OCCUPIED_CELL;
   }
  
 }
 
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
