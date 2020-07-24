#ifndef CC_BUMP_H
#define CC_BUMP_H

#include "ac_agent.h"

namespace CA
{

 class CCBump : virtual public ACAgent
 {
  
 public:
  
  // Constructor
  CCBump(const unsigned position, bool enabled = true);
  
  // Destructor (empty)
  virtual ~CCBump();
  
  // Enable bump
  inline void enable(){Enabled = true;}
 
  // Disable bump
  inline void disable(){Enabled = false;}
 
  // Check whether is enabled or not
  inline bool is_enabled(){return Enabled;}
  
 protected:
  
  bool Enabled;
 
 };
 
} // namespace CA

#endif // #ifndef CC_BUMP_H
