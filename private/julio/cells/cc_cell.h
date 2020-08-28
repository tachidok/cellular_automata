#ifndef CC_CELL_H
#define CC_CELL_H

#include "../../../src/general/ac_agent.h"

namespace CA
{
 
 /// Implements a cell represented as a particle that moves in a
 /// lattice
 class CCCell : virtual public ACAgent
 {
  
 public:

  // JCPS, needs to define and implement methods
  
  /// Constructor
  CCCell(std::vector<unsigned> &position, std::vector<unsigned> &neighbourhood_sizes);
  
  /// Destructor
  virtual ~CCCell();
  
  /// Allocate memory for internal structures and initialise
  /// information used by the person to move: preference matrix M
  void allocate_memory();
  
  /// Get the indexes of the entry with the maximum probability in the
  /// P matrix, return the max probability
  Real get_indexes_with_max_probability(unsigned &i_max, unsigned &j_max);
  
  /// Update person's status
  void update();
  
  /// Get access to the full preference matrix
  inline std::vector<std::vector<Real> > &m() {return M;}
  
  /// Set preference matrix entries
  inline Real &m(const unsigned i, const unsigned j) {return M[i][j];}
  /// Get preference matrix entries
  inline Real m(const unsigned i, const unsigned j) const {return M[i][j];}
  
  /// Get access to the full transitions probabily matrix
  inline std::vector<std::vector<Real> > &p() {return P;}
  
  /// Set transition probabilities matrix entries
  inline Real &p(const unsigned i, const unsigned j) {return P[i][j];}
  /// Get transition probabilities matrix entries
  inline Real p(const unsigned i, const unsigned j) const {return P[i][j];}
  
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

#endif // #ifndef CC_CELL_H
