#ifndef CC_PERSON_H
#define CC_PERSON_H

#include "ac_agent.h"

namespace CA
{
 
 /// Implements a person represented as a particle that moves in a
 /// lattice
 class CCPerson : virtual public ACAgent
 {
  
 public:
  
  /// Constructor
  CCPerson(std::vector<unsigned> &position, std::vector<unsigned> &neighbourhood_sizes);
  
  /// Destructor
  virtual ~CCPerson();

  /// Initialise information used by the person to move:
  /// - Preference Matrix M
  void initialise();
  
  /// Update person's status
  void update();
  
  /// Set preference matrix entries
  inline unsigned &m(const unsigned i, const unsigned j) {return M[i][j];}
  /// Get preference matrix entries
  inline unsigned m(const unsigned i, const unsigned j) const {return M[i][j];}
  
  /// Set transition probabilities matrix entries
  inline unsigned &p(const unsigned i, const unsigned j) {return P[i][j];}
  /// Get transition probabilities matrix entries
  inline unsigned p(const unsigned i, const unsigned j) const {return P[i][j];}
  
 protected:
  
  /// Transition probabilities matrix
  std::vector<std::vector<Real> > P;
  
  /// Preference matrix
  std::vector<std::vector<Real> > M;
  
  /// Initialise preference matrix
  void initialise_preference_matrix();
  
  /// Default initialisation preference matrix, all movements are
  /// allowed
  void default_initialisation_preference_matrix();
  
 };
 
} // namespace CA

#endif // #ifndef CC_PERSON_H

