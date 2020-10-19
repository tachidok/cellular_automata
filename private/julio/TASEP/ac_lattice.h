#ifndef AC_LATTICE_H
#define AC_LATTICE_H

#include "../../../src/general/common_includes.h"
#include "../../../src/general/utilities.h"

#include "../../../src/general/ac_agent.h"

namespace CA
{

#define LATTICE_OCCUPIED_SITE 1
#define LATTICE_NON_OCCUPIED_SITE 0
 
 /// Implements the interfaces for a rectangular square lattice where
 /// agents can occupy a single position on the lattice, each agent
 /// undergoes its own dynamics and interacts with neighbouring agents
 /// following its implemented behaviour
 class ACLattice
 {
  
 public:
  
  /// Constructor (specify the size of the lattice) and the number of force fields
  ACLattice(std::vector<unsigned> &dimension_sizes, const unsigned n_force_fields = 0)
  
  /// Destructor
  virtual ~ACLattice();
  
  /// Get the i-dimension of the lattice
  const unsigned dimension_size(const unsigned i);
  
  /// Get the number of dimensions
  inline const unsigned n_dimensions() const {return Dimension_size.size();}
  
  /// Initialise the lattice
  virtual void initialise();
  
  /// Populate with cells
  virtual void populate_lattice();
  
  /// Add an agent to the lattice (check whether the positon is not
  /// already occupied by another agent)
  bool add_agent_to_lattice(ACAgent *agent_pt);
  
  /// Is there no agents on the lattice
  inline bool is_empty() {return Agents_pt.empty();}
  
  // Get the number of agents on stage
  inline const unsigned n_agents() {return Agents_pt.size();}
  
  // Get the number of already occupied spaces on stage
  const unsigned n_occupied();
  
  /// -------------------------------------------------------------------
  /// Simulation
  /// -------------------------------------------------------------------
  
  /// Implements the set of actions that should be taken previous the
  /// execution of agents rules
  virtual void actions_before_agents_rules();
  
  /// Implements the set of actions that should be taken after the
  /// execution of agents rules
  virtual void actions_after_agents_rules();
  
  /// Implements the set of actions that should be taken previous the
  /// simulation step
  virtual void actions_before_simulation_step();
  
  /// Implements the set of actions that should be taken after the
  /// simulation step
  virtual void actions_after_simulation_step();
  
  /// Take a simulation step
  void simulation_step();
  
  /// Get the i-th force field weight
  const Real force_field_weight(const unsigned i) const;
  
  /// Set the i-th force field weight
  Real &force_field_weight(const unsigned i);
  
  /// Set force fields weight parameters
  void set_force_fields_weights(std::vector<Real> &force_field_weights);
  
  /// Get the force fields weight vector
  std::vector<Real> &get_force_fields_weights() {return Force_fields_weights;}
  
  /// Returns the k-th force field
  std::vector<std::vector<Real> >&force_field(const unsigned k);
  
  /// Output force fields
  void output_force_fields(std::ostringstream &output_folder_name);
  
  /// Output lattice occupation
  void output_lattice_occupation(std::ostringstream &output_folder_name);
  
  /// Set index for files
  inline unsigned &index_files() {return Index_files;}
  
  /// Read index for files
  inline unsigned index_files() const {return Index_files;}
  
 protected:
  
  /// Stores the size of the dimensions of the lattice
  std::vector<unsigned> Dimension_sizes;
  
  /// Number of force fields
  const unsigned N_force_fields;
  
  /// Lattice matrix
  std::vector<std::vector<ACAgent *> > Lattice_pt;
  
  /// The force fields weights vector
  std::vector<Real> Force_fields_weights;
  
  /// Force fields
  std::vector<std::vector<std::vector<Real> > > Force_fields;
  
  /// List of pointers to agents
  std::list<ACAgent *> Agents_pt;
  
  /// Allocate force fields memory
  void allocate_force_fields_memory();
  
  /// Initialise force fields
  void initialise_force_fields();
  
  /// Update force fields
  void update_force_fields();
  
  /// Index for files (default to 0 in constructor)
  unsigned Index_files;
  
 };
 
} // namespace CA

#endif // #ifndef AC_LATTICE_H

