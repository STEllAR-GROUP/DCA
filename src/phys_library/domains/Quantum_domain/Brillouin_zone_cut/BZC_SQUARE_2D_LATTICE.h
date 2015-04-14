//-*-C++-*-

#ifndef BRILLOUIN_ZONE_CUT_SQUARE_2D_LATTICE_H
#define BRILLOUIN_ZONE_CUT_SQUARE_2D_LATTICE_H

/*!
 *  \author Peter Staar
 */
template<>
class brillouin_zone_path_domain<SQUARE_2D_LATTICE>
{
  const static int INTERPOLATION_NB = 64;

public:

  typedef std::vector<double>                           element_type;
  typedef brillouin_zone_path_domain<SQUARE_2D_LATTICE> this_type;

public:

  static std::string                get_name();  
  static int                        get_size();
  static std::vector<element_type>& get_elements();

private:

  static std::vector<element_type> initialize_elements();
};

std::string brillouin_zone_path_domain<SQUARE_2D_LATTICE>::get_name()  
{
  return std::string("SQUARE_2D_LATTICE");
}

int brillouin_zone_path_domain<SQUARE_2D_LATTICE>::get_size()  
{
  return get_elements().size();
}

std::vector<std::vector<double> >& brillouin_zone_path_domain<SQUARE_2D_LATTICE>::get_elements()
{
  static std::vector<element_type> v = initialize_elements();
  return v;
}

std::vector<std::vector<double> > brillouin_zone_path_domain<SQUARE_2D_LATTICE>::initialize_elements()
{
  std::vector<element_type> collection_k_vecs(0);

  std::vector<double> b0(2,0);
  std::vector<double> b1(2,0);

  b0[0] = 2*M_PI; b0[1] = 0;
  b1[0] = 0;      b1[1] = 2*M_PI;


  std::vector<double> k0(2);
  std::vector<double> k1(2);
  std::vector<double> k2(2);
  std::vector<double> k3(2);

  for(int i=0; i<2; i++)
    {
      k0[i]  = 0    *b0[i]+0.   *b1[i];
      k1[i]  = 1./2.*b0[i]+1./2.*b1[i];
      k2[i]  = 1./2.*b0[i]+0.   *b1[i];
      k3[i]  = 0.   *b0[i]+0.   *b1[i];
    }

  for(int l=0; l<INTERPOLATION_NB; l++)
    {
      std::vector<double> k(2,0);

      k[0] = (1.-double(l)/double(INTERPOLATION_NB))*k0[0] + double(l)/double(INTERPOLATION_NB)*k1[0];
      k[1] = (1.-double(l)/double(INTERPOLATION_NB))*k0[1] + double(l)/double(INTERPOLATION_NB)*k1[1];

      collection_k_vecs.push_back(k);
    }
	  
	  
  for(int l=0; l<INTERPOLATION_NB; l++)
    {
      std::vector<double> k(2,0);

      k[0] = (1.-double(l)/double(INTERPOLATION_NB))*k1[0] + double(l)/double(INTERPOLATION_NB)*k2[0];
      k[1] = (1.-double(l)/double(INTERPOLATION_NB))*k1[1] + double(l)/double(INTERPOLATION_NB)*k2[1];

      collection_k_vecs.push_back(k);
    }

  for(int l=0; l<INTERPOLATION_NB; l++)
    {
      std::vector<double> k(2,0);

      k[0] = (1.-double(l)/double(INTERPOLATION_NB))*k2[0] + double(l)/double(INTERPOLATION_NB)*k3[0];
      k[1] = (1.-double(l)/double(INTERPOLATION_NB))*k2[1] + double(l)/double(INTERPOLATION_NB)*k3[1];

      collection_k_vecs.push_back(k);
    }

  collection_k_vecs.push_back(k3);

  return collection_k_vecs;
}

#endif