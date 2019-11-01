#include "general.h"

#ifndef CC_BUMP_H
#define CC_BUMP_H

class CCBump
{
 
 public:
 
 // Constructor
 CCBump(const unsigned long position, bool enabled = true);
 
 // Destructor (empty)
 virtual ~CCBump();
 
 // Get position
 inline unsigned long position() const {return Position;}
 
 // Enable bump
 inline void enable(){Enabled = true;}
 
 // Disable bump
 inline void disable(){Enabled = false;}
 
 // Check whether is enabled or not
 inline bool is_enabled(){return Enabled;}
 
 protected:
 
 const unsigned long Position;
 bool Enabled;
 
};

#endif // #ifndef CC_BUMP_H

