// Copyright (C) 2009-2016 ETH Zurich
// Copyright (C) 2007?-2016 Center for Nanophase Materials Sciences, ORNL
// All rights reserved.
//
// See LICENSE.txt for terms of usage.
// See CITATION.txt for citation guidelines if you use this code for scientific publications.
//
// Author: Peter Staar (taa@zurich.ibm.com)
//
// Triangular lattice.

#ifndef DCA_PHYS_MODELS_ANALYTIC_HAMILTONIANS_TRIANGULAR_LATTICE_HPP
#define DCA_PHYS_MODELS_ANALYTIC_HAMILTONIANS_TRIANGULAR_LATTICE_HPP

#include <cmath>
#include <stdexcept>
#include <vector>

#include "dca/function/domains.hpp"
#include "dca/function/function.hpp"
#include "dca/phys/domains/cluster/symmetries/point_groups/no_symmetry.hpp"
#include "dca/util/type_list.hpp"

namespace dca {
namespace phys {
namespace models {
// dca::phys::models::

template <typename DCA_point_group_type>
class triangular_lattice {
public:
  typedef domains::no_symmetry<2> LDA_point_group;
  typedef DCA_point_group_type DCA_point_group;
  const static int DIMENSION = 2;
  const static int BANDS = 1;

  static double* initialize_r_DCA_basis();

  static double* initialize_r_LDA_basis();

  static std::vector<int> get_flavors();
  static std::vector<std::vector<double>> get_a_vectors();

  template <class domain, class parameters_type>
  static void initialize_H_interaction(func::function<double, domain>& H_interaction,
                                       parameters_type& parameters);

  template <class domain>
  static void initialize_H_symmetry(func::function<int, domain>& H_symmetry);

  // Initializes the tight-binding (non-interacting) part of the momentum space Hamiltonian.
  // Preconditions: The elements of KDmn are two-dimensional (access through index 0 and 1).
  template <typename ParametersType, typename ScalarType, typename BandSpinDmn, typename KDmn>
  static void initialize_H_0(
      const ParametersType& parameters,
      func::function<ScalarType, func::dmn_variadic<BandSpinDmn, BandSpinDmn, KDmn>>& H_0);
};

template <typename DCA_point_group_type>
double* triangular_lattice<DCA_point_group_type>::initialize_r_DCA_basis() {
  static double* r_DCA = new double[4];

  r_DCA[0] = cos(M_PI / 3.);
  r_DCA[1] = sin(M_PI / 3.);
  r_DCA[2] = cos(-M_PI / 3.);
  r_DCA[3] = sin(-M_PI / 3.);

  return r_DCA;
}

// template<typename DCA_point_group_type>
// double* triangular_lattice<DCA_point_group_type>::initialize_k_DCA_basis()
// {
//   static double* k_DCA = new double[4];

//   k_DCA[0] = 2*M_PI;  k_DCA[1] = 2*M_PI/sqrt(3.);
//   k_DCA[2] = 2*M_PI;  k_DCA[3] = -2*M_PI/sqrt(3.);

//   return k_DCA;
// }

template <typename DCA_point_group_type>
double* triangular_lattice<DCA_point_group_type>::initialize_r_LDA_basis() {
  static double* r_LDA = new double[4];

  r_LDA[0] = cos(M_PI / 3.);
  r_LDA[1] = sin(M_PI / 3.);
  r_LDA[2] = cos(-M_PI / 3.);
  r_LDA[3] = sin(-M_PI / 3.);

  return r_LDA;
}

template <typename DCA_point_group_type>
std::vector<int> triangular_lattice<DCA_point_group_type>::get_flavors() {
  static std::vector<int> flavors(BANDS);

  for (int i = 0; i < BANDS; i++)
    flavors[i] = i;

  return flavors;
}

template <typename DCA_point_group_type>
std::vector<std::vector<double>> triangular_lattice<DCA_point_group_type>::get_a_vectors() {
  static std::vector<std::vector<double>> a_vecs(BANDS, std::vector<double>(DIMENSION, 0.));
  return a_vecs;
}

template <typename DCA_point_group_type>
template <class domain, class parameters_type>
void triangular_lattice<DCA_point_group_type>::initialize_H_interaction(
    func::function<double, domain>& H_interaction, parameters_type& parameters) {
  double U = parameters.get_U();

  H_interaction(0, 0, 0) = 0;
  H_interaction(0, 1, 0) = U;
  H_interaction(1, 0, 0) = U;
  H_interaction(1, 1, 0) = 0;
}

template <typename DCA_point_group_type>
template <class domain>
void triangular_lattice<DCA_point_group_type>::initialize_H_symmetry(
    func::function<int, domain>& H_symmetries) {
  H_symmetries(0, 0) = 0;
  H_symmetries(0, 1) = -1;
  H_symmetries(1, 0) = -1;
  H_symmetries(1, 1) = 0;
}

template <typename point_group_type>
template <typename ParametersType, typename ScalarType, typename BandSpinDmn, typename KDmn>
void triangular_lattice<point_group_type>::initialize_H_0(
    const ParametersType& parameters,
    func::function<ScalarType, func::dmn_variadic<BandSpinDmn, BandSpinDmn, KDmn>>& H_0) {
  static_assert(util::Length<typename BandSpinDmn::this_type>::value == 2,
                "BandSpinDmn has two subdomains, the band domain and the spin domain.");

  using BandDmn = typename util::TypeAt<0, typename BandSpinDmn::template domain_typelist<0>>::type;
  using SpinDmn = typename util::TypeAt<0, typename BandSpinDmn::template domain_typelist<1>>::type;

  if (BandDmn::get_size() != BANDS)
    throw std::logic_error("Triangular lattice has one band.");
  if (SpinDmn::get_size() != 2)
    throw std::logic_error("Spin domain size must be 2.");

  const auto& k_vecs = KDmn::get_elements();

  const auto t = parameters.get_t();

  H_0 = ScalarType(0);

  for (int k_ind = 0; k_ind < KDmn::dmn_size(); ++k_ind) {
    const auto& k = k_vecs[k_ind];
    const auto val =
        -2. * t * std::cos(k[0]) - 4. * t * std::cos(sqrt(3.) * k[1] / 2.) * std::cos(k[0] / 2.);

    H_0(0, 0, 0, 0, k_ind) = val;
    H_0(0, 1, 0, 1, k_ind) = val;
  }
}

}  // models
}  // phys
}  // dca

#endif  // DCA_PHYS_MODELS_ANALYTIC_HAMILTONIANS_TRIANGULAR_LATTICE_HPP
