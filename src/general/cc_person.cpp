#include "cc_person.h"

namespace CA
{

 // ----------------------------------------------------------------
 /// Constructor
 // ----------------------------------------------------------------
 CCPerson::CCPerson(std::vector<unsigned> &position)
  : ACAgent(position, 2) // Dimension is 2
 {
  initialise();
 }
 
 // ----------------------------------------------------------------
 /// Destructor
 // ----------------------------------------------------------------
 virtual ~CCPerson::CCPerson()
 {
  
 }
 
 // ----------------------------------------------------------------
 /// Initialise information used by the person to move:
 /// - Preference Matrix M
 // ----------------------------------------------------------------
 void CCPerson::initialise()
 {
  // The dimension of the preference matrix
  const unsigned m = 3;
  const unsigned n = 3;
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
  const unsigned m = M.size();
  for (unsigned i = 0; i < m; i++)
   {
    const unsigned n = M[i].size();
    for (unsigned j = 0; j < n; j++)
     {
      M[i][j] = 1.0;
     }
   }
 }
 
} // namespace CA
