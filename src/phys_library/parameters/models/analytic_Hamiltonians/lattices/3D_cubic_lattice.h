//-*-C++-*-

#ifndef CUBIC_LATTICE_3D_H
#define CUBIC_LATTICE_3D_H

/*!
 *  \author peter staar
 */
template<typename point_group_type>
class cubic_lattice
{
public:

  typedef no_symmetry<3>   LDA_point_group;
  typedef point_group_type DCA_point_group;

  const static int DIMENSION = 3;
  const static int BANDS     = 1;

  static double* initialize_r_DCA_basis();
  static double* initialize_r_LDA_basis();

  static std::vector<int>                  get_flavors();
  static std::vector<std::vector<double> > get_a_vectors();

  static std::vector<std::pair<std::pair<int,int>, std::pair<int,int> > > get_orbital_permutations();

  template<class domain, class parameters_type>
  static void initialize_H_interaction(FUNC_LIB::function<double , domain >& H_interaction,
                                       parameters_type&            parameters);

  template<class domain>
  static void initialize_H_symmetry(FUNC_LIB::function<int , domain>& H_symmetry);

  template<class parameters_type>
  static std::complex<double> get_LDA_Hamiltonians(parameters_type& parameters,
                                                   std::vector<double> k, int b1, int s1, int b2, int s2);
};

template<typename point_group_type>
double* cubic_lattice<point_group_type>::initialize_r_DCA_basis()
{
  static double* r_DCA = new double[9];

  r_DCA[0] = 1.;  r_DCA[3] = 0.; r_DCA[6] = 0.;
  r_DCA[1] = 0.;  r_DCA[4] = 1.; r_DCA[7] = 0.;
  r_DCA[2] = 0.;  r_DCA[5] = 0.; r_DCA[8] = 1.;

  return r_DCA;
}


template<typename point_group_type>
double* cubic_lattice<point_group_type>::initialize_r_LDA_basis()
{
  static double* r_LDA = new double[9];

  r_LDA[0] = 1.;  r_LDA[3] = 0.; r_LDA[6] = 0.;
  r_LDA[1] = 0.;  r_LDA[4] = 1.; r_LDA[7] = 0.;
  r_LDA[2] = 0.;  r_LDA[5] = 0.; r_LDA[8] = 1.;

  return r_LDA;
}

template<typename point_group_type>
std::vector<int> cubic_lattice<point_group_type>::get_flavors()
{
  static std::vector<int> flavors(BANDS);

  for(int i=0; i<BANDS; i++)
    flavors[i]=i;

  return flavors;
}

template<typename point_group_type>
std::vector<std::vector<double> > cubic_lattice<point_group_type>::get_a_vectors()
{
  static std::vector<std::vector<double> > a_vecs(BANDS, std::vector<double>(DIMENSION, 0.));
  return a_vecs;
}

template<typename point_group_type>
std::vector<std::pair<std::pair<int,int>, std::pair<int,int> > > cubic_lattice<point_group_type>::get_orbital_permutations()
{
  static std::vector<std::pair<std::pair<int,int>, std::pair<int,int> > > permutations(0);
  return permutations;
}

template<typename point_group_type>
template<class domain, class parameters_type>
void cubic_lattice<point_group_type>::initialize_H_interaction(FUNC_LIB::function<double , domain >& H_interaction,
                                                                parameters_type&            parameters)
{
  double U = parameters.get_U();

  H_interaction(0,0,0) = 0;  H_interaction(0,1,0) = U;
  H_interaction(1,0,0) = U;  H_interaction(1,1,0) = 0;
}

template<typename point_group_type>
template<class domain>
void cubic_lattice<point_group_type>::initialize_H_symmetry(FUNC_LIB::function<int , domain>& H_symmetries)
{
  H_symmetries(0,0)= 0; H_symmetries(0,1)=-1;
  H_symmetries(1,0)=-1; H_symmetries(1,1)= 0;
}

template<typename point_group_type>
template<class parameters_type>
std::complex<double> cubic_lattice<point_group_type>::get_LDA_Hamiltonians(parameters_type& parameters,
                                                                            std::vector<double> k,
                                                                            int b1, int s1, int b2, int s2)
{
  assert(k.size()==DIMENSION);

  std::complex<double> H_LDA = 0.;

  double t       = parameters.get_t();
  double t_prime = parameters.get_t_prime();

  if( (b1 == 0 && b2 == 0) && ((s1 == 0 && s2 == 0) || (s1 == 1 && s2 == 1)))
    H_LDA = -2.* t * ( cos(k[0]) + cos(k[1]) + cos(k[2]) ) - 4.*t_prime*cos(k[0])*cos(k[1])*cos(k[2]);

  return H_LDA;
}

#endif