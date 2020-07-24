#ifndef AC_AGENT_H
#define AC_AGENT_H

#include "common_includes.h"

namespace CA
{
 
 /// The basic class to represent an agent in a lattice.
 class ACAgent
 {
  
 public:
  
  /// Constructor
  ACAgent(const unsigned position);
  
  // Constructor
  ACAgent(std::vector<unsigned> &position, const unsigned dim = 2);
  
  /// Destructor (empty)
  virtual ~ACAgent();
  
  /// Set i-th position at time t
  inline unsigned &position(const unsigned i = 0, const unsigned t = 0) {return Position[i][t];}
  /// Get i-th position at time t
  inline unsigned position(const unsigned i = 0, const unsigned t = 0) const {return Position[i][t];}
  
  /// Gets access to the dimension of the agent
  inline const unsigned dim() const {return Dim;}
  
  /// In charge of updating the position of the agent
  void update_position();
  
 protected:
  
  /// In charge of initialise any structure required by the agent
  void initialise(std::vector<unsigned> &position);
  
  /// Stores the position of the agent/particle at the current [0] and
  /// next state [1]
  std::vector<std::vector<unsigned> > Position;

  /// The dimension where the agent live on
  const unsigned Dim;
  
 };
 
} // namespace CA

#endif // #ifndef AC_AGENT_H
