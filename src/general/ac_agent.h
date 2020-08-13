#ifndef AC_AGENT_H
#define AC_AGENT_H

#include "common_includes.h"
#include "utilities.h"

namespace CA
{
 
 /// The basic class to represent an agent in a lattice.
 class ACAgent
 {
  
 public:
  
  /// Constructor
  ACAgent(const unsigned position, const unsigned one_dimensional_neighbourhood_size = 1);
  
  // Constructor
  ACAgent(std::vector<unsigned> &position, std::vector<unsigned> &neighbourhood_size);
  
  /// Destructor (empty)
  virtual ~ACAgent();
  
  /// Set i-th position at time t
  inline unsigned &position(const unsigned i = 0, const unsigned t = 0) {return Position[i][t];}
  /// Get i-th position at time t
  inline unsigned position(const unsigned i = 0, const unsigned t = 0) const {return Position[i][t];}
  
  /// Get the number of dimensions (based on the neighbourhood)
  inline unsigned n_dimension() const {return Neighbourhood_size.size();}
  
  /// Get the number of neighbours in the i-th dimension
  inline unsigned neighbourhood_size(const unsigned i) const {return Neighbourhood_size[i];}
  
  /// In charge of updating the position of the agent
  void update_position();
  
  /// Reset movement of the agen for the current time step (should be
  /// called prior to update position)
  void reset_movement();
  
 protected:
  
  /// In charge of initialise any structure required by the agent
  void initialise(std::vector<unsigned> &position);
  
  /// Stores the position of the agent/particle
  /// Position[i][0] - Particle dimension i at current time
  /// Position[i][1] - Particle dimension i at next time
  std::vector<std::vector<unsigned> > Position;
  
  /// Neighbourhood size
  std::vector<unsigned> Neighbourhood_size;
  
 };
 
} // namespace CA

#endif // #ifndef AC_AGENT_H
