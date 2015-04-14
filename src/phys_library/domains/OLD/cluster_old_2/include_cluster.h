
typedef representation_type cluster_representation_type;

#include "cluster_operations.h"
//#include "cluster_symmetry.h"

#include "DCA_lattice_parameters.h"
#include "LDA_lattice_parameters.h"
#include "PCM_lattice_parameters.h"
#include "host_lattice_parameters.h"
#include "host_vertex_lattice_parameters.h"

template<cluster_representation_type representation_type, class base_cluster_type>
class r_cluster
{};

template<cluster_representation_type representation_type, class base_cluster_type>
class k_cluster
{};

#include "cluster_symmetry.h"

#include "r_cluster.h"
#include "k_cluster.h"

#include "set_symmetry_matrices.h"
#include "search_maximal_symmetry_group.h"

#include "cluster_reduction.h"
//#include "cluster_reduction_spglib.h"

#include "cluster_initializer.h"
#include "cluster_initializer_2D.h"
#include "cluster_initializer_3D.h"

//#include "cluster_initializer_psimag_2D.h"

#include "cluster.h"





//#include "cluster_harmonics.h"
//#include "cluster_harmonic_k_basis_functions.h"
//#include "cluster_harmonic_r_basis_functions.h"