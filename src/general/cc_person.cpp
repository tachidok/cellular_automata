#include "cc_person.h"

namespace CA
{

 // ----------------------------------------------------------------
 /// Constructor
 // ----------------------------------------------------------------
 CCPerson::CCPerson(std::vector<unsigned> &position, std::vector<unsigned> &neighbourhood_sizes)
  : ACAgent(position, neighbourhood_sizes)
 {
  initialise();
 }
 
 // ----------------------------------------------------------------
 /// Destructor
 // ----------------------------------------------------------------
 CCPerson::~CCPerson()
 {
  
 }
 
 // ----------------------------------------------------------------
 /// Initialise information used by the person to move:
 /// - Preference Matrix M
 // ----------------------------------------------------------------
 void CCPerson::initialise()
 {
  // The dimension of the preference matrix
  const unsigned m = this->neighbourhood_size(0);
  const unsigned n = this->neighbourhood_size(1);
  M.resize(m);
  for (unsigned i = 0 ; i < m; i++)
   {
    M[i].resize(n);
   }
 }
 
 // ----------------------------------------------------------------
 /// Update person's status
 // ----------------------------------------------------------------
 void CCPerson::update()
 {
  
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
