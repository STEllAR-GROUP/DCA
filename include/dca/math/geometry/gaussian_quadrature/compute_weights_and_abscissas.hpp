// Copyright (C) 2009-2016 ETH Zurich
// Copyright (C) 2007?-2016 Center for Nanophase Materials Sciences, ORNL
// All rights reserved.
//
// See LICENSE.txt for terms of usage.
// See CITATION.txt for citation guidelines if you use this code for scientific publications.
//
// Author: Peter Staar (peter.w.j.staar@gmail.com)
//         Urs R. Haehner (haehneru@itp.phys.ethz.ch)
//
// This file provides functions to compute the Gaussian quadrature weights and abscissas for 1D, 2D
// and 3D. It makes use of the SIMPLEX_GM_RULE C++ library
// (https://people.sc.fsu.edu/~jburkardt/cpp_src/simplex_gm_rule/simplex_gm_rule.html).

#ifndef DCA_MATH_GEOMETRY_GAUSSIAN_QUADRATURE_COMPUTE_WEIGHTS_AND_ABSCISSAS_HPP
#define DCA_MATH_GEOMETRY_GAUSSIAN_QUADRATURE_COMPUTE_WEIGHTS_AND_ABSCISSAS_HPP

#include "dca/math/geometry/gaussian_quadrature/simplex_gm_rule.hpp"
#include "math_library/geometry_library/tetrahedron_mesh/tetrahedron/tetrahedron.hpp"

namespace dca {
namespace math {
namespace gaussquad {
// dca::math::gaussquad::

// Empty template declaration
template <int dimension>
void computeWeightsAndAbscissas(const int rule, math_algorithms::tetrahedron<dimension>& tet);

// Template specializations
// 1D
template <>
void computeWeightsAndAbscissas(const int rule, math_algorithms::tetrahedron<1>& tet);
// 2D
template <>
void computeWeightsAndAbscissas(const int rule, math_algorithms::tetrahedron<2>& tet);
// 3D
template <>
void computeWeightsAndAbscissas(const int rule, math_algorithms::tetrahedron<3>& tet);

}  // gaussquad
}  // math
}  // dca

#endif  // DCA_MATH_GEOMETRY_GAUSSIAN_QUADRATURE_COMPUTE_WEIGHTS_AND_ABSCISSAS_HPP