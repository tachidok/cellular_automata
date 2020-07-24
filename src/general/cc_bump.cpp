#include "cc_bump.h"

namespace CA
{

 // ----------------------------------------------------------------
 // Constructor
 // ----------------------------------------------------------------
 CCBump::CCBump(const unsigned position, bool enabled)
  : ACAgent(position)
 {
  Enabled = enabled;
 }

 // Destructor (empty)
 CCBump::~CCBump()
 {
 
 }

} // namespace CA
