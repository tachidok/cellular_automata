#include "cc_floor_field.h"

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
 CCFloorField::~CCFloorField()
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
  //initialise_occupancy_matrix();
  
  /// Initialise obstacle matrix
  //initialise_obstacle_matrix();
 }

 // ----------------------------------------------------------------
 /// Add emergency exits to the stage
 // ----------------------------------------------------------------
 void CCFloorField::add_emergency_exit(const unsigned i, const unsigned j)
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
  const unsigned n_people = static_cast<Real>(field_size)*density;
  
  // Set the initial density
  Initial_density = density;
  
  // If the number of people to be added to the floor field is larger
  // or equal to the size of the foor field then fill is with people
  if (n_people >= field_size)
   {
    for (unsigned i = 0; i < M; i++)
     {
      for (unsigned j = 0; j < N; j++)
       {
        // The position of the person
        std::vector<unsigned> tmp_pos(2);
        tmp_pos[0] = i;
        tmp_pos[1] = j;
        // The size of neighbourhood
        std::vector<unsigned> tmp_neighbourhood(2);
        tmp_neighbourhood[0] = 3;
        tmp_neighbourhood[1] = 3;
        CCPerson *new_person_pt = new CCPerson(tmp_pos, tmp_neighbourhood);
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
        std::vector<unsigned> tmp_pos(2);
        tmp_pos[0] = i;
        tmp_pos[1] = j;
        // The size of neighbourhood
        std::vector<unsigned> tmp_neighbourhood(2);
        tmp_neighbourhood[0] = 3;
        tmp_neighbourhood[1] = 3;
        CCPerson *new_person_pt = new CCPerson(tmp_pos, tmp_neighbourhood);
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
  const unsigned i = person_pt->position(0);
  const unsigned j = person_pt->position(1);
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
 
 /// Update the transition probability matrix of a person using the
 /// floor field's information
 void CCFloorField::update_transition_probability_matrix(CCPerson *person_pt)
 {
  // Get the position of the person at current time
  const unsigned i_p = person_pt->position(0);
  const unsigned j_p = person_pt->position(1);
  
  // Get the full probability matrix of the person
  std::vector<std::vector<Real> > P = person_pt->p();
  // Get size of dimensions of the transitions probability matrix
  const unsigned p_m = P.size();
  const unsigned p_n = P[0].size();
  
  // Initialise offsets
  unsigned i_offset = -p_m/2;
  for (unsigned i = 0; i < p_m; i++)
   {
    // Initialise offsets
    unsigned j_offset = -p_n/2;
    for (unsigned j = 0; j < p_n; j++)
     {
      // Check whether the cell is occupied or is an obstacle
      if (is_occupied(i_p + i_offset, j_p + j_offset) ||
          is_obstacle(i_p + i_offset, j_p + j_offset))
       {
        // Set the probability to zero
        P[i][j] = 0.0;
       }
      else
       {
        // Static field contribution
        const Real sf = K_s * static_field(i_p + i_offset, j_p + j_offset);
        // Dynamic field contribution
        const Real df = K_d * dynamic_field(i_p + i_offset, j_p + j_offset);
        
        // Set the probability for the transition matrix
        P[i][j] = std::exp(sf) * std::exp(df);
        
       }
      
      // Increase the offset
      j_offset++;
      
     } // for (j < p_n)
    
    // Increase the offset
    i_offset++;
    
   } // for (i < p_m)
  
 }
 
 /// Compute next position for a given person
 void CCFloorField::update_next_position(CCPerson *person_pt)
 {  
  // Compute the indexes with the maximum probability
  unsigned i_max = 0;
  unsigned j_max = 0;
  person_pt->compute_max_probability_position(i_max, j_max);
  
  // Get size of neighboorhood in each dimension for the person
  const unsigned p_m = person_pt->neighbourhood_size(0);
  const unsigned p_n = person_pt->neighbourhood_size(1);
  
  const unsigned i_offset = -(p_m/2) + i_max;
  const unsigned j_offset = -(p_n/2) + j_max;

  // Get the position of the person at current time
  const unsigned i_p = person_pt->position(0);
  const unsigned j_p = person_pt->position(1);
  
  // Set the next position
  person_pt->position(0, 1) = i_p + i_offset;
  person_pt->position(1, 1) = j_p + j_offset;
  
 }
 
 // ----------------------------------------------------------------
 /// Take a simulation step
 // ----------------------------------------------------------------
 void CCFloorField::simulation_step()
 {
  // Loop over each person and compute its next position
  
  // Get the number of people on the stage
  const unsigned npeople = n_people();
  for (unsigned k = 0; k < npeople; k++)
   {
    // Get the k-th person on the stage
    CCPerson* person_pt = people_pt(k);
    
    // Compute the values for the transition probability matrix
    update_transition_probability_matrix(person_pt);
    
    // Compute the next position for the person
    update_next_position(person_pt);
    
   }
  
  // Solve for conflicts
  solve_people_position_conflicts();
  
 }
 
 // ----------------------------------------------------------------
 /// Update floor fields
 // ----------------------------------------------------------------
 void CCFloorField::update()
 {
  /// Move people based on their next position
    
  // Get the number of people
  const unsigned npeople = n_people();
  for (unsigned i = 0; i < npeople; i++)
   {
    // Update status
    people_pt(i)->update();
   }
  
  /// Update static field matrix
  update_static_field_matrix();
  
  /// Update dynamic field matrix
  update_dynamic_field_matrix();
  
  /// Update occupancy matrix
  update_occupancy_matrix();
  
  /// Update obstacle matrix
  update_obstacle_matrix();
 }
 
 // ----------------------------------------------------------------
 /// Output static field
 // ----------------------------------------------------------------
 void CCFloorField::output_static_field(std::ostringstream &output_folder_name)
 {
  // Create file name (add any stuff at the end of the string - ate)
  std::ostringstream output_static_field_filename(output_folder_name.str(), std::ostringstream::ate);
  output_static_field_filename << "static_field.dat";
  // File
  std::ofstream static_field_file((output_static_field_filename.str()).c_str(), std::ios_base::out);

  // Loop over the static field
  for (unsigned i = 0; i < M; i++)
   {
    for (unsigned j = 0; j < N-1; j++)
     {
      static_field_file << Static_field[i][j] << ",";
     }
    
    // The last column values
    if (i != M-1) // Is this not the last row as well
     {
      static_field_file << Static_field[i][N-1] << std::endl;
     }
    else // If this is the last column and the last row the do not
         // include the "\n" at the end of the line
     {
      static_field_file << Static_field[i][N-1];
     }
    
   }
  // Close file
  static_field_file.close();
 }
 
 // ----------------------------------------------------------------
 /// Output dynamic field
 // ----------------------------------------------------------------
 void CCFloorField::output_dynamic_field(std::ostringstream &output_folder_name)
 {
  // Create file name (add any stuff at the end of the string - ate)
  std::ostringstream output_dynamic_field_filename(output_folder_name.str(), std::ostringstream::ate);
  output_dynamic_field_filename << "dynamic_field.dat";
  // File
  std::ofstream dynamic_field_file((output_dynamic_field_filename.str()).c_str(), std::ios_base::out);

  // Loop over the dynamic field
  for (unsigned i = 0; i < M; i++)
   {
    for (unsigned j = 0; j < N-1; j++)
     {
      dynamic_field_file << Dynamic_field[i][j] << ",";
     }
    
    // The last column values
    if (i != M-1) // Is this not the last row as well
     {
      dynamic_field_file << Dynamic_field[i][N-1] << std::endl;
     }
    else // If this is the last column and the last row the do not
         // include the "\n" at the end of the line
     {
      dynamic_field_file << Dynamic_field[i][N-1];
     }
    
   }
  // Close file
  dynamic_field_file.close();
 }
 
 // ----------------------------------------------------------------
 /// Output occupancy matrix
 // ----------------------------------------------------------------
 void CCFloorField::output_occupancy_matrix(std::ostringstream &output_folder_name)
 {
  // Create file name (add any stuff at the end of the string - ate)
  std::ostringstream output_occupancy_matrix_filename(output_folder_name.str(), std::ostringstream::ate);
  output_occupancy_matrix_filename << "occupancy_matrix.dat";
  // File
  std::ofstream occupancy_matrix_file((output_occupancy_matrix_filename.str()).c_str(), std::ios_base::out);

  // Loop over the dynamic field
  for (unsigned i = 0; i < M; i++)
   {
    for (unsigned j = 0; j < N-1; j++)
     {
      if (Occupancy_matrix[i][j])
       {
        occupancy_matrix_file << "1,";
       }
      else
       {
        occupancy_matrix_file << "0,";
       }
      
     }
    
    // The last column values
    if (i != M-1) // Is this not the last row as well
     {
      if (Occupancy_matrix[i][N-1])
       {
        occupancy_matrix_file << "1" << std::endl;
       }
      else
       {
        occupancy_matrix_file << "0" << std::endl;
       }
      
     }
    else // If this is the last column and the last row the do not
         // include the "\n" at the end of the line
     {
      if (Occupancy_matrix[i][N-1])
       {
        occupancy_matrix_file << "1";
       }
      else
       {
        occupancy_matrix_file << "0";
       }
      
     }
    
   }
  // Close file
  occupancy_matrix_file.close();
 }
 
 // ----------------------------------------------------------------
 /// Output obstacle matrix
 // ----------------------------------------------------------------
 void CCFloorField::output_obstacle_matrix(std::ostringstream &output_folder_name)
 {
  // Create file name (add any stuff at the end of the string - ate)
  std::ostringstream output_obstacle_matrix_filename(output_folder_name.str(), std::ostringstream::ate);
  output_obstacle_matrix_filename << "obstacle_matrix.dat";
  // File
  std::ofstream obstacle_matrix_file((output_obstacle_matrix_filename.str()).c_str(), std::ios_base::out);

  // Loop over the dynamic field
  for (unsigned i = 0; i < M; i++)
   {
    for (unsigned j = 0; j < N-1; j++)
     {
      if (Obstacle_matrix[i][j])
       {
        obstacle_matrix_file << "1,";
       }
      else
       {
        obstacle_matrix_file << "0,";
       }
      
     }
    
    // The last column values
    if (i != M-1) // Is this not the last row as well
     {
      if (Obstacle_matrix[i][N-1])
       {
        obstacle_matrix_file << "1" << std::endl;
       }
      else
       {
        obstacle_matrix_file << "0" << std::endl;
       }
      
     }
    else // If this is the last column and the last row the do not
         // include the "\n" at the end of the line
     {
      if (Obstacle_matrix[i][N-1])
       {
        obstacle_matrix_file << "1";
       }
      else
       {
        obstacle_matrix_file << "0";
       }
      
     }
    
   }
  // Close file
  obstacle_matrix_file.close();
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
 const Real CCFloorField::static_field(const unsigned i, const unsigned j)
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
 /// Is there no more people on the stage
 // ----------------------------------------------------------------
 bool CCFloorField::is_empty()
 {
  if (n_people() > 0)
   {
    return false;
   }
  return true;
 }
 
 // ----------------------------------------------------------------
 /// Set stage boundary configuration
 // ----------------------------------------------------------------
 void CCFloorField::set_boundary_configuration()
 {
  // Initialise obstacles matrix (wall at boundary and any other
  // obstacles)
  initialise_obstacle_matrix();
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
  /// The computation of the static field is described in Appendix A
  /// of the paper "Kirchner, Ansgar and Schadschneider, Andreas,
  /// Simulation of evacuation processes using a bionics-inspired
  /// cellular automaton model for pedestrian dynamics, Physica A,
  /// Elsevier, 2002".

  /// In the mentioned paper, the euclidean distance is used therefore
  /// obstacles have no effect on the values of the field. If we would
  /// want to consider obstacles we may use the Manhattan distance
  /// instead
  
  // Initialise the field with zeroes
  for (unsigned i = 0; i < M; i++)
   {
    Static_field[i].assign(N, 0.0);
   }
  
  // For each emergency exit on the stage compute the maximum
  // (euclidean) distance to all cells on the lattice
  
  // Get the number of emergency exits
  const unsigned nemergency_exit = n_emergency_exit();
  // Store the maximum distance to each emergency exit
  std::vector<Real> max_distance(nemergency_exit, 0.0);
  for (unsigned k = 0; k < nemergency_exit; k++)
   {
    // Get the position of the exit
    const unsigned exit_i = Emergency_exit[k][0];
    const unsigned exit_j = Emergency_exit[k][1];
    // Compute the euclidean distance from each exit to all the cells
    // in the lattice and store the maximum in max_distance
    for (unsigned i = 0; i < M; i++)
     {
      for (unsigned j = 0; j < N; j++)
       {
        Real i_d = static_cast<Real>(exit_i) - static_cast<Real>(i);
        Real j_d = static_cast<Real>(exit_j) - static_cast<Real>(j);
        Real tmp_squared = std::sqrt((i_d*i_d)+(j_d*j_d));
        // Get the maximum of the distances
        if (tmp_squared > max_distance[k])
         {
          max_distance[k] = tmp_squared;
         }
        
       } // for (j < N)
      
     } // for (i < M)

   } // for (k < nemergency_exit)
  
  // The values in "max_distance" are used to normalise the entries in
  // the Static field matrix
  
  // Loop over all the exits
  for (unsigned k = 0; k < nemergency_exit; k++)
   {
    // ... and all the cells again to compute the final values for the
    // Static field
    
    // Get the position of the exit
    const unsigned exit_i = Emergency_exit[k][0];
    const unsigned exit_j = Emergency_exit[k][1];
    
    for (unsigned i = 0; i < M; i++)
     {
      for (unsigned j = 0; j < N; j++)
       {
        // Compute the euclidean distance from the current exit to the
        // current cell
        Real i_d = static_cast<Real>(exit_i) - static_cast<Real>(i);
        Real j_d = static_cast<Real>(exit_j) - static_cast<Real>(j);
        Real tmp_squared = std::sqrt((i_d*i_d)+(j_d*j_d));
        Real tmp_difference = max_distance[k] - tmp_squared;
        
        // If this is the first emergency exit then copy the value
        // into the static field, otherwise choose the minimum
        if (k == 0)
         {
          Static_field[i][j] = tmp_difference;
         }
        else // k != 0
         {
          // Get the minimum of the distances
          if (tmp_difference < Static_field[i][j])
           {
            Static_field[i][j] = tmp_difference;
           }
          
         }
        
       } // for (j < N)
      
     } // for (i < M)
    
   } // for (k < nemergency_exit)
  
 }

 // ----------------------------------------------------------------
 /// Initialise dynamic field matrix
 // ----------------------------------------------------------------
 void CCFloorField::initialise_dynamic_field_matrix()
 {
  // Initialise the field with zeroes
  for (unsigned i = 0; i < M; i++)
   {
    Dynamic_field[i].assign(N, 0.0);
   }
  
 }

 /*
 // ----------------------------------------------------------------
 /// Initialise occupancy matrix
 // ----------------------------------------------------------------
 void CCFloorField::initialise_occupancy_matrix()
 {
  /// Occupancy matrix
  std::vector<std::vector<bool> > Occupancy_matrix;  
 }
 */

 // ----------------------------------------------------------------
 /// Initialise obstacle matrix
 // ----------------------------------------------------------------
 void CCFloorField::initialise_obstacle_matrix()
 {
  // Build the wall
  build_wall_at_boundary();
 }

 // ----------------------------------------------------------------
 /// Solve people's positions conflicts
 // ----------------------------------------------------------------
 void CCFloorField::solve_people_position_conflicts()
 {
  
 }
 
 // ----------------------------------------------------------------
 /// Update static field matrix
 // ----------------------------------------------------------------
 void CCFloorField::update_static_field_matrix()
 {
  // Do nothing
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
  // Restart the occupancy matrix with zeroes
  for (unsigned i = 0; i < M; i++)
   {
    Occupancy_matrix[i].assign(N, 0);
   }
  
  // Get the number of people
  const unsigned npeople = n_people();
  // Loop over all people still on the stage
  for (unsigned k = 0; k < npeople; k++)
   {
    // Get the k-th person on the stage
    CCPerson* person_pt = people_pt(k);
    // Get the position of the i-th person
    const unsigned i = person_pt->position(0);
    const unsigned j = person_pt->position(1);
    
    // Update the status of the occupancy matrix
    Occupancy_matrix[i][j] = OCCUPIED_CELL;
    
   }
  
 }

 // ----------------------------------------------------------------
 /// Update obstacle matrix
 // ----------------------------------------------------------------
 void CCFloorField::update_obstacle_matrix()
 {
  // Do nothing
 }
 
} // namespace CA
