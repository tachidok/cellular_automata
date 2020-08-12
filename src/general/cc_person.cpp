#include "cc_person.h"

namespace CA
{

 // ----------------------------------------------------------------
 /// Constructor
 // ----------------------------------------------------------------
 CCPerson::CCPerson(std::vector<unsigned> &position, std::vector<unsigned> &neighbourhood_sizes)
  : ACAgent(position, neighbourhood_sizes)
 {
  allocate_memory();
  initialise_preference_matrix();
 }
 
 // ----------------------------------------------------------------
 /// Destructor
 // ----------------------------------------------------------------
 CCPerson::~CCPerson()
 {
  
 }
 
 // ----------------------------------------------------------------
 /// Allocate memory for internal structures and initialise
 /// information used by the person to move: preference matrix M
 void CCPerson::allocate_memory()
 {
#ifdef CELLULAR_AUTOMATON_PANIC_MODE
  // Get the number of dimensions
  const unsigned ndim = n_dimension();
  if (ndim != 2)
   {
    // Error message
    std::ostringstream error_message;
    error_message << "The number of dimensions is different of 2, the current implementation\n"
                  << "of the CCPerson class only supports 2-dimensional lattices\n"
                  << "If you require a different number of dimensions please check the other\n"
                  << "derived classes from ACAgent or implement one that fullfils your needs\n"
                  << std::endl;
    throw CALibError(error_message.str(),
                     CA_CURRENT_FUNCTION,
                     CA_EXCEPTION_LOCATION);
   }
#endif // #ifdef CELLULAR_AUTOMATON_RANGE_CHECK
  
  // The dimension of the preference matrix
  const unsigned m = this->neighbourhood_size(0);
  const unsigned n = this->neighbourhood_size(1);
  M.resize(m);
  P.resize(m);
  for (unsigned i = 0 ; i < m; i++)
   {
    M[i].resize(n, 0.0);
    P[i].resize(n, 0.0);
   }
 }
 
 // ----------------------------------------------------------------
 /// Compute maximum probability indexes
 // ----------------------------------------------------------------
 void CCPerson::compute_max_probability_position(unsigned &i_max, unsigned &j_max)
 {
  // Compute the next position using the current values in the matrix
  // P (transition probabiliies matrix) and the matrix M (preferences
  // matrix)
  
  // The next position is that where the maximum value of the P matrix
  // and the weight given by the matrix M is found
  
  // The dimension of the preference matrix
  const unsigned m = this->neighbourhood_size(0);
  const unsigned n = this->neighbourhood_size(1);
  
  // Store the position of the maximum value (initialise with the
  // center value)
  unsigned tmp_i_max = m/2;
  unsigned tmp_j_max = n/2;
  // Store the maximum value, initialise it with the value at the
  // center of the transition matrix
  Real max_value = P[m/2][n/2] * M[m/2][n/2];
  
  // Loop over the transition probability matrix
  for (unsigned i = 0; i < m; i++)
   {
    for (unsigned j = 0; j < n; j++)
     {
      const Real test_value = P[i][j] * M[i][j];
      if (test_value > max_value)
       {
        tmp_i_max = i;
        tmp_j_max = j;
        max_value = test_value;
       }
      
     } // for (j < p_n)
    
   } // for (i < p_m)
  
  i_max = tmp_i_max;
  j_max = tmp_j_max;
  
 }
 
 // ----------------------------------------------------------------
 /// Update person's status
 // ----------------------------------------------------------------
 void CCPerson::update()
 {
  // Update position (copy the next position into the current
  // position)
  this->update_position();
 }

 // ----------------------------------------------------------------
 /// Initialise preference matrix
 // ----------------------------------------------------------------
 void CCPerson::initialise_preference_matrix()
 {
  default_initialisation_preference_matrix();
 }

 // ----------------------------------------------------------------
 /// Default initialisation preference matrix, all movements are
 /// allowed
 // ----------------------------------------------------------------
 void CCPerson::default_initialisation_preference_matrix()
 {
  // Default initialisation of the preference matrix (all movements
  // are allowed)
  const unsigned m = this->neighbourhood_size(0);
  const unsigned n = this->neighbourhood_size(1);
  for (unsigned i = 0; i < m; i++)
   {
    for (unsigned j = 0; j < n; j++)
     {
      M[i][j] = 1.0;
     }
   }
 }
 
} // namespace CA
