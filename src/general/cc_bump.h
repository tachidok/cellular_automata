#ifndef CC_BUMP_H
#define CC_BUMP_H

#include "common_includes.h"

namespace CA
{

 class CCBump
 {

 public:

  // Constructor
  CCBump(const unsigned position, bool enabled = true);

  // Destructor (empty)
  virtual ~CCBump();

  // Get position
  inline unsigned position() const {return Position;}

  // Enable bump
  inline void enable(){Enabled = true;}

  // Disable bump
  inline void disable(){Enabled = false;}

  // Check whether is enabled or not
  inline bool is_enabled(){return Enabled;}

 protected:

  const unsigned Position;
  bool Enabled;

 };

} // namespace CA

#endif // #ifndef CC_BUMP_H
