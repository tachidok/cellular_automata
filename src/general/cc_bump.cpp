#include "cc_bump.h"

namespace CA
{

 // ----------------------------------------------------------------
 // Constructor
 // ----------------------------------------------------------------
 CCBump::CCBump(const unsigned long position, bool enabled)
  : Position(position)
 {
  Enabled = enabled;
 }

 // Destructor (empty)
 CCBump::~CCBump()
 {
 
 }

} // namespace CA
