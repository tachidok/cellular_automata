#include "ac_agent.h"

namespace CA
{
 
 // ----------------------------------------------------------------
 /// Constructor
 // ----------------------------------------------------------------
 ACAgent::ACAgent(const unsigned position, const unsigned one_dimensional_neighbourhood_size)
 {
  // The number of dimensions
  Neighbourhood_size.resize(1);
  // The number of neighbours per dimension
  Neighbourhood_size[0] = one_dimensional_neighbourhood_size;
  std::vector<unsigned> tmp_position(1);
  tmp_position[0] = position;
  initialise(tmp_position);
 }
 
 // ----------------------------------------------------------------
 /// Constructor
 // ----------------------------------------------------------------
 ACAgent::ACAgent(std::vector<unsigned> &position, std::vector<unsigned> &neighbourhood_sizes)
 {
  const unsigned ndim = neighbourhood_sizes.size();
  // The number of dimensions
  Neighbourhood_size.resize(ndim);
  // The size or number of neighbours per dimension
  for (unsigned i = 0; i < ndim; i++)
   {
    Neighbourhood_size[i] = neighbourhood_sizes[i];
   }
  
  initialise(position);
  
 }
 
 // Destructor (empty)
 ACAgent::~ACAgent()
 {
  
 }

 // ----------------------------------------------------------------
 /// In charge of updating the position of the agent
 // ----------------------------------------------------------------
 void ACAgent::update_position()
 {
  // Get the number of dimension
  const unsigned ndim = n_dimension();
  // Loop over all dimensions and update the current position of the
  // agent
  for (unsigned i = 0; i < ndim; i++)
   {
    // Update each position
    Position[i][0] = Position[i][1];
   }
  
 }
 
 // ----------------------------------------------------------------
 /// In charge of initialise any structure required by the agent
 // ----------------------------------------------------------------
 void ACAgent::initialise(std::vector<unsigned> &position)
 {
  const unsigned ndim = n_dimension();
  /// Range check
#ifdef CELLULAR_AUTOMATON_RANGE_CHECK // CHAPCHOM_PANIC_MODE
  // Get the dimension of the input vector
  const unsigned tmp_dim = position.size();
  if (ndim != tmp_dim)
   {
    // Error message
    std::ostringstream error_message;
    error_message << "The dimension of the position vector is different\n"
                  << "from that indicated by the agent\n"
                  << "Position vector dimension: "<<tmp_dim<<"\n"
                  << "Agent dimension: "<<ndim<<"\n"
                  << std::endl;
    throw CALibError(error_message.str(),
                     CA_CURRENT_FUNCTION,
                     CA_EXCEPTION_LOCATION);
   }
#endif // #ifdef CELLULAR_AUTOMATON_RANGE_CHECK
  
  // Resize position vector
  Position.resize(ndim);
  for (unsigned i = 0; i < ndim; i++)
   {
    // Set space for the current and the next position. Current
    // position is at index 0, and the next position is at index 1
    Position[i].resize(2);
    
    // Set current and next i-position
    Position[i][0] = Position[i][1] = position[i];
   }
  
 }
 
} // namespace CA
