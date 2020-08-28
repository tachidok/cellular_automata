#ifndef CC_LATTICE_H
#define CC_LATTICE_H

#include "../../../src/general/common_includes.h"
#include "../../../src/general/utilities.h"

#include "../../../src/general/cc_person.h"

namespace CA
{

#define OCCUPIED_CELL 1
#define NON_OCCUPIED_CELL 0
#define OBSTACLE_CELL 1
 
 /// Implements a rectangular square lattice where cells can occupy a
 /// single position on the domain, each cells undergoes its own
 /// dynamics and interacts with neighbouring cells following its
 /// implemented behaviour
 class CCLattice
 {
  
 public:
  
  /// Constructor (specify the size of the lattice)
  CCLattice(std::vector<unsigned> &dimension_sizes);
  
  /// Destructor
  virtual ~CCLattice();
  
  /// Initialise the lattice
  virtual void initialise();
  
  /// Populate with cells
  virtual void populate_lattice();
  
  /// Take a simulation step
  void simulation_step();
  
  /// Update force fields
  void update_force_fields();
  
  /// Output static field
  void output_force_fields(std::ostringstream &output_folder_name);
  
  /// Get the i-th force field weight
  const Real force_fields_weight(const unsigned i) const;
  
  /// Set the i-th force field weight
  Real &force_fields_weight(const unsigned i);
  
  /// Set force fields weight parameters
  void set_force_fields_weights(std::vector<Real> &force_field_weights);
  
  /// Get the force fields weight vector
  std::vector<Real> &get_force_fields_weights() {return Force_fields_weights;}
  
  /// Get the i-dimension of the lattice
  const unsigned dimension_size(const unsigned i);
  
  /// Get the number of dimensions
  inline const unsigned n_dimensions() const {Dimension_size.size();}
  
  /// Returns the k-th force field
  std::vector<Real> &force_field(const unsigned k);
  
  /// Is there no cells on the lattice
  inline bool is_empty() {return Cells_pt.empty();}
  
  // Get the number of cells on stage
  inline const unsigned n_cells() {return Cells_pt.size();}
  
  // Get the number of already occupied spaces on stage
  const unsigned n_occupied();
  
  /// Set index for files
  inline unsigned &index_files() {return Index_files;}
  
  /// Read index for files
  inline unsigned index_files() const {return Index_files;}
  
 protected:
  
  /// Stores the size of the dimensions of the lattice
  std::vector<unsigned> Dimension_sizes;
  
  /// The force fields weights vector
  std::vector<Real> Force_fields_weights;
  
  /// Force fields
  std::vector<std::vector<std::vector<Real> > > Force_fields;
  
  /// List of pointers to cells
  std::list<CCCell *> Cells_pt;
  
  /// Add a cell to the lattice (check whether the positon is not
  /// already occupied by another cell)
  bool add_cell_to_lattice(CCCell *cell_pt);
  
  /// Allocate force fields memory
  void allocate_force_fields_memory();
  
  /// Set boundary conditions
  void set_boundary_conditions();
  
  /// Initialise force fields
  void initialise_force_fields();
  
  /// Update force fields
  void update_force_fields();
  
  /// Solve cells's positions conflicts (using max probability)
  void solve_cells_position_conflicts(std::map<std::pair<unsigned, unsigned>, std::vector<CCCell *> > &next_peoples_positions, std::map<CCCell *, Real> &max_probability_of_person);
  
  /// Index for files (default to 0 in constructor)
  unsigned Index_files;
    
 };
 
} // namespace CA

#endif // #ifndef CC_LATTICE_H

