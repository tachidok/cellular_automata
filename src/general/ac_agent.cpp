#include "ac_agent.h"

namespace CA
{
 
 // ----------------------------------------------------------------
 // Constructor
 // ----------------------------------------------------------------
 ACAgent::ACAgent(const unsigned position)
  : Dim(1)
 {
  std::vector<unsigned> tmp_position(1);
  tmp_position[0] = position;
  initialise(tmp_position);
 }
 
 // ----------------------------------------------------------------
 // Constructor
 // ----------------------------------------------------------------
 ACAgent::ACAgent(std::vector<unsigned> &position, const unsigned dim)
  : Dim(dim)
 {
  initialise(position);
 }
 
 // Destructor (empty)
 ACAgent::~ACAgent()
 {
  
 }

 // ----------------------------------------------------------------
 // In charge of initialise any structure required by the agent
 // ----------------------------------------------------------------
 void ACAgent::initialise(std::vector<unsigned> &position)
 {
  /// Range check
#ifdef CELLULAR_AUTOMATON_RANGE_CHECK
  // Get the dimension of the input vector
  const unsigned tmp_dim = position.size();
  if (Dim != tmp_dim)
   {
    // Error message
    std::ostringstream error_message;
    error_message << "The dimension of the input position vector is different\n"
                  << "from that indicated for the agent\n"
                  << "Input position dimension: "<<tmp_dim<<"\n"
                  << "Indicated agent dimension: "<<Dim<<"\n"
                  << std::endl;
    throw CALibError(error_message.str(),
                     CA_CURRENT_FUNCTION,
                     CA_EXCEPTION_LOCATION);
   }
#endif // #ifdef CELLULAR_AUTOMATON_RANGE_CHECK
  
  // Resize position vector
  Position.resize(Dim);
  for (unsigned i = 0; i < Dim; i++)
   {
    // Set space for the current and the next position. Current
    // position is at index 0, and the next position is at index 1
    Position[i].resize(2);
    
    // Set current and next i-position
    Position[i][0] = Position[i][1] = position[i];
   }
  
 }
 
} // namespace CA
