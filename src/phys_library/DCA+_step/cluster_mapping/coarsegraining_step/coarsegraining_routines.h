//-*-C++-*-

#ifndef DCA_COARSEGRAINING_ROUTINES_H
#define DCA_COARSEGRAINING_ROUTINES_H

namespace DCA
{

  template<typename scalar_type, typename k_dmn_t, typename q_dmn_t>
  struct coarsegraining_functions
  {
#include "type_definitions.h"

    int K_ind;
    int w_ind;

    scalar_type mu;

    FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, k_dmn_t> >* H_k;
    FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, k_dmn_t> >* A_k;

    FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t> > I_q;
    FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t> > H_q;
    FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t> > A_q;
    FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t> > S_q;
    FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t> > G_q;
  };


  template<typename parameters_type, typename K_dmn>
  class coarsegraining_routines
  {
#include "type_definitions.h"

    typedef typename K_dmn::parameter_type k_cluster_type;

    const static int DIMENSION = K_dmn::parameter_type::DIMENSION;

    typedef typename parameters_type::concurrency_type concurrency_type;

    typedef dmn_0<MATH_ALGORITHMS::tetrahedron_mesh<K_dmn> >             tetrahedron_dmn;

    typedef MATH_ALGORITHMS::gaussian_quadrature_domain<tetrahedron_dmn> quadrature_dmn;

  public:

    coarsegraining_routines(parameters_type& parameters_ref);

    ~coarsegraining_routines();

  protected:

    void compute_tetrahedron_mesh(int k_mesh_refinement,
                                  int number_of_periods);

    void compute_gaussian_mesh(int k_mesh_refinement,
                               int gaussian_quadrature_rule,
                               int number_of_periods);

    template<typename scalar_type, typename k_dmn_t, typename q_dmn_t>
    void wannier_interpolation(int K_ind,
                               FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, k_dmn_t> >& f_k,
                               FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t> >& f_q);

    template<typename scalar_type, typename tmp_scalar_type, typename q_dmn_t>
    void compute_I_q(tmp_scalar_type value,
                     FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t> >& I_q);

    template<typename scalar_type, typename k_dmn_t, typename q_dmn_t>
    void compute_H_q(int K_ind,
                     FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, k_dmn_t> >& H_k,
                     FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t> >& H_q);

    /***********************************************************************************
     ***
     ***         Routines for DCA
     ***
     ***********************************************************************************/

    template<typename scalar_type, typename q_dmn_t>
    void compute_S_q(int K_ind, int w_ind,
                     FUNC_LIB::function<std::complex<scalar_type>, dmn_4<nu, nu, K_dmn  , w> >& S_K_w,
                     FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t   > >& S_q);

    template<typename scalar_type, typename k_dmn_t, typename q_dmn_t>
    void compute_G_q_w(int K_ind, int w_ind,
                       FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, k_dmn_t   > >& H_k,
                       FUNC_LIB::function<std::complex<scalar_type>, dmn_4<nu, nu, K_dmn  , w> >& S_K,
                       FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t   > >& I_q,
                       FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t   > >& H_q,
                       FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t   > >& S_q,
                       FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t   > >& G_q);

    template<typename scalar_type, typename q_dmn_t>
    void compute_S_q(int K_ind, int w_ind,
                     FUNC_LIB::function<std::complex<scalar_type>, dmn_4<nu, nu, K_dmn  , w_REAL> >& S_K_w,
                     FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t   > >& S_q);

    template<typename scalar_type, typename k_dmn_t, typename q_dmn_t>
    void compute_G_q_w(int K_ind, int w_ind,
                       FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, k_dmn_t        > >& H_k,
                       FUNC_LIB::function<std::complex<scalar_type>, dmn_4<nu, nu, K_dmn  , w_REAL> >& S_K,
                       FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t        > >& I_q,
                       FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t        > >& H_q,
                       FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t        > >& S_q,
                       FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t        > >& G_q);

    template<typename scalar_type, typename k_dmn_t, typename q_dmn_t>
    void compute_G_q_t(int K_ind, int t_ind,
                       FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, k_dmn_t> >& H_k,
                       FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t> >& I_q,
                       FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t> >& H_q,
                       FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t> >& G_q);

    /***********************************************************************************
     ***                                                                             ***
     ***         Routines for DCA+
     ***                                                                             ***
     ***********************************************************************************/

    template<typename scalar_type, typename k_dmn_t, typename q_dmn_t>
    void compute_S_q_from_A_k(int K_ind, int w_ind,
                              FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, k_dmn_t> >& A_k,
                              FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t> >& A_q,
                              FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t> >& S_q);

    template<typename scalar_type, typename k_dmn_t, typename q_dmn_t>
    void compute_G_q_w(int K_ind, int w_ind,
                       FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, k_dmn_t> >& H_k,
                       FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, k_dmn_t> >& A_k,
                       FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t> >& I_q,
                       FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t> >& H_q,
                       FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t> >& A_q,
                       FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t> >& S_q,
                       FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t> >& G_q);

    template<typename scalar_type, typename k_dmn_t, typename q_dmn_t>
    void compute_G_q_w(coarsegraining_functions<scalar_type, k_dmn_t, q_dmn_t>& coarsegraining_functions_ref);

  protected:

    parameters_type&  parameters;
    concurrency_type& concurrency;
  };

  template<typename parameters_type, typename K_dmn>
  coarsegraining_routines<parameters_type, K_dmn>::coarsegraining_routines(parameters_type& parameters_ref):
    parameters (parameters_ref),
    concurrency(parameters.get_concurrency())
  {
  }

  template<typename parameters_type, typename K_dmn>
  coarsegraining_routines<parameters_type, K_dmn>::~coarsegraining_routines()
  {}

  template<typename parameters_type, typename K_dmn>
  void coarsegraining_routines<parameters_type, K_dmn>::compute_tetrahedron_mesh(int k_mesh_refinement,
                                                                                 int number_of_periods)
  {
    MATH_ALGORITHMS::tetrahedron_mesh<typename K_dmn::parameter_type> mesh(k_mesh_refinement);

    quadrature_dmn::translate_according_to_period(number_of_periods, mesh);

    {
      coarsegraining_domain<K_dmn, TETRAHEDRON_K>::get_size()     = 0;
      coarsegraining_domain<K_dmn, TETRAHEDRON_K>::get_weights() .resize(0);
      coarsegraining_domain<K_dmn, TETRAHEDRON_K>::get_elements().resize(0);

      for(int l=0; l<mesh.size(); l++)
        mesh[l].update_tetrahedron_domain(coarsegraining_domain<K_dmn, TETRAHEDRON_K>::get_size(),
                                          coarsegraining_domain<K_dmn, TETRAHEDRON_K>::get_weights(),
                                          coarsegraining_domain<K_dmn, TETRAHEDRON_K>::get_elements());


      coarsegraining_domain<K_dmn, TETRAHEDRON_ORIGIN>::get_size()     = 0;
      coarsegraining_domain<K_dmn, TETRAHEDRON_ORIGIN>::get_weights() .resize(0);
      coarsegraining_domain<K_dmn, TETRAHEDRON_ORIGIN>::get_elements().resize(0);

      for(int l=0; l<mesh.size(); l++)
        mesh[l].update_tetrahedron_domain(coarsegraining_domain<K_dmn, TETRAHEDRON_ORIGIN>::get_size(),
                                          coarsegraining_domain<K_dmn, TETRAHEDRON_ORIGIN>::get_weights(),
                                          coarsegraining_domain<K_dmn, TETRAHEDRON_ORIGIN>::get_elements());
    }

    // cout << mesh.size() << endl;
    
    // #ifdef ALLOW_GNUPLOT
    //     if(false)
    //       {
    //         Gnuplot plot_obj("lines");

    //         for(int l=0; l<mesh.size(); l++)
    //           {
    //             mesh[l].plot(plot_obj);
    //             //mesh[l].plot_q_vecs(plot_obj);
    //           }

    //         plot_obj.showonscreen();
    //       }
    // #endif
      }

  template<typename parameters_type, typename K_dmn>
  void coarsegraining_routines<parameters_type, K_dmn>::compute_gaussian_mesh(int k_mesh_refinement,
                                                                              int gaussian_quadrature_rule,
                                                                              int number_of_periods)
  {
    //     quadrature_dmn::initialize_Brillouin_zone(parameters.get_k_mesh_refinement(),
    //                                               parameters.get_gaussian_quadrature_rule(),
    //                                               parameters.get_number_of_periods());

    {
      quadrature_dmn::initialize_Brillouin_zone(k_mesh_refinement,
                                                gaussian_quadrature_rule,
                                                number_of_periods);

      //      SHOW::plot_points(K_dmn::get_elements());
      //SHOW::plot_points(quadrature_dmn::get_elements());
    }

    {
      coarsegraining_domain<K_dmn, ORIGIN>::get_size()     = quadrature_dmn::get_size();
      coarsegraining_domain<K_dmn, ORIGIN>::get_weights()  = quadrature_dmn::get_weights();
      coarsegraining_domain<K_dmn, ORIGIN>::get_elements() = quadrature_dmn::get_elements();
    }

    {
      coarsegraining_domain<K_dmn, K>::get_size()     = quadrature_dmn::get_size();
      coarsegraining_domain<K_dmn, K>::get_weights()  = quadrature_dmn::get_weights();
      coarsegraining_domain<K_dmn, K>::get_elements() = quadrature_dmn::get_elements();
    }

    {
      coarsegraining_domain<K_dmn, K_PLUS_Q>::get_size()     = quadrature_dmn::get_size();
      coarsegraining_domain<K_dmn, K_PLUS_Q>::get_weights()  = quadrature_dmn::get_weights();
      coarsegraining_domain<K_dmn, K_PLUS_Q>::get_elements() = quadrature_dmn::get_elements();
    }

    {
      coarsegraining_domain<K_dmn, Q_MINUS_K>::get_size()     = quadrature_dmn::get_size();
      coarsegraining_domain<K_dmn, Q_MINUS_K>::get_weights()  = quadrature_dmn::get_weights();
      coarsegraining_domain<K_dmn, Q_MINUS_K>::get_elements() = quadrature_dmn::get_elements();
    }
  }

  template<typename parameters_type, typename K_dmn>
  template<typename scalar_type, typename k_dmn_t, typename q_dmn_t>
  void coarsegraining_routines<parameters_type, K_dmn>::wannier_interpolation(int K_ind,
                                                                              FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, k_dmn_t> >& f_k,
                                                                              FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t> >& f_q)
  {
    typedef interpolation_matrices<scalar_type, k_dmn_t, q_dmn_t> interpolation_matrices_type;

    if(not interpolation_matrices_type::is_initialized())
      interpolation_matrices_type::initialize(concurrency);

    LIN_ALG::matrix<scalar_type, LIN_ALG::CPU>& T = interpolation_matrices_type::get(K_ind);

    scalar_type alpha(1.);
    scalar_type beta (0.);

    scalar_type* A_ptr = &(reinterpret_cast<scalar_type(&)[2]>(f_k(0))[0]);  //&real(f_k(0));
    scalar_type* B_ptr = &T(0,0);
    scalar_type* C_ptr = &(reinterpret_cast<scalar_type(&)[2]>(f_q(0))[0]);  //&real(f_q(0));

    int M = 2*nu_nu::dmn_size();
    int K = k_dmn_t::dmn_size();
    int N = q_dmn_t::dmn_size();

    int LDA = 2*nu_nu::dmn_size();
    int LDB = T.get_global_size().first;
    int LDC = 2*nu_nu::dmn_size();

    LIN_ALG::GEMM<LIN_ALG::CPU>::execute('N', 'T', M, N, K, alpha, A_ptr, LDA, B_ptr, LDB, beta, C_ptr, LDC);
  }

  template<typename parameters_type, typename K_dmn>
  template<typename scalar_type, typename tmp_scalar_type, typename q_dmn_t>
  void coarsegraining_routines<parameters_type, K_dmn>::compute_I_q(tmp_scalar_type value,
                                                                    FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t> >& I_q)
  {
    for(int q_ind=0; q_ind<q_dmn_t::dmn_size(); q_ind++)
      for(int j=0; j<nu::dmn_size(); j++)
        for(int i=0; i<nu::dmn_size(); i++)
          I_q(i,j,q_ind) = i==j? value : 0.;

//     if(parameters.get_double_counting_method() == "coarsegraining")
//       {
// 	std::vector<int> interacting_bands = parameters.get_interacting_bands();
	
// 	double           Delta             = parameters.get_double_counting_correction_for_this_iteration();
	
// 	for(int q_ind=0; q_ind<q_dmn_t::dmn_size(); q_ind++)
// 	  for(int s_ind=0; s_ind<s::dmn_size(); s_ind++)
// 	    for(int b_ind=0; b_ind<interacting_bands.size(); b_ind++)
// 	      I_q(interacting_bands[b_ind], s_ind,
// 		  interacting_bands[b_ind], s_ind,
// 		  q_ind) += Delta; 
//       }
  }

  template<typename parameters_type, typename K_dmn>
  template<typename scalar_type, typename k_dmn_t, typename q_dmn_t>
  void coarsegraining_routines<parameters_type, K_dmn>::compute_H_q(int K_ind,
                                                                    FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, k_dmn_t> >& H_k,
                                                                    FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t> >& H_q)
  {
    wannier_interpolation(K_ind, H_k, H_q);
  }

  /*****************************************
   ***                                   ***
   ***         Routines for DCA          ***
   ***                                   ***
   *****************************************/

  template<typename parameters_type, typename K_dmn>
  template<typename scalar_type, typename q_dmn_t>
  void coarsegraining_routines<parameters_type, K_dmn>::compute_S_q(int K_ind, int w_ind,
                                                                    FUNC_LIB::function<std::complex<scalar_type>, dmn_4<nu, nu, K_dmn  , w> >& S_K_w,
                                                                    FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t   > >& S_q)
  {
    for(int q_ind=0; q_ind<q_dmn_t::dmn_size(); q_ind++)
      for(int j=0; j<nu::dmn_size(); j++)
        for(int i=0; i<nu::dmn_size(); i++)
          S_q(i,j,q_ind) = S_K_w(i,j,K_ind,w_ind);
  }

  template<typename parameters_type, typename K_dmn>
  template<typename scalar_type, typename k_dmn_t, typename q_dmn_t>
  void coarsegraining_routines<parameters_type, K_dmn>::compute_G_q_w(int K_ind, int w_ind,
                                                                      FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, k_dmn_t   > >& H_k,
                                                                      FUNC_LIB::function<std::complex<scalar_type>, dmn_4<nu, nu, K_dmn  , w> >& S_K,
                                                                      FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t   > >& I_q,
                                                                      FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t   > >& H_q,
                                                                      FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t   > >& S_q,
                                                                      FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t   > >& G_q)
  {
    {
      std::complex<scalar_type> i_wm_min_mu;

      i_wm_min_mu.real( parameters.get_chemical_potential() );
      i_wm_min_mu.imag( w::get_elements()[w_ind] );

      compute_I_q(i_wm_min_mu, I_q);
    }

    compute_H_q(K_ind, H_k, H_q);

    compute_S_q(K_ind, w_ind, S_K, S_q);

    /*
      LIN_ALG::matrix<std::complex<scalar_type>, LIN_ALG::CPU> G_inv("G_inv", nu::dmn_size());

      for(int q_ind=0; q_ind<q_dmn_t::dmn_size(); q_ind++){

      for(int j=0; j<nu::dmn_size(); j++)
      for(int i=0; i<nu::dmn_size(); i++)
      G_inv(i,j) = I_q(i,j,q_ind)-H_q(i,j,q_ind)-S_q(i,j,q_ind);

      if(false)
      LIN_ALG::GEINV<LIN_ALG::CPU>::execute(G_inv);
      else
      LIN_ALG::GEINV<LIN_ALG::CPU>::execute_on_Green_function_matrix(G_inv);

      for(int j=0; j<nu::dmn_size(); j++)
      for(int i=0; i<nu::dmn_size(); i++)
      G_q(i,j,q_ind) = G_inv(i,j);
      }
    */

    int nr_threads = parameters.get_nr_coarsegraining_threads();

    if(nr_threads==1)
      quadrature_integration<parameters_type, q_dmn_t>::quadrature_integration_G_q_w_st(I_q, H_q, S_q, G_q);
    else
      quadrature_integration<parameters_type, q_dmn_t>::quadrature_integration_G_q_w_mt(nr_threads, I_q, H_q, S_q, G_q);
  }

  template<typename parameters_type, typename K_dmn>
  template<typename scalar_type, typename q_dmn_t>
  void coarsegraining_routines<parameters_type, K_dmn>::compute_S_q(int K_ind, int w_ind,
                                                                    FUNC_LIB::function<std::complex<scalar_type>, dmn_4<nu, nu, K_dmn  , w_REAL> >& S_K_w,
                                                                    FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t        > >& S_q)
  {
    for(int q_ind=0; q_ind<q_dmn_t::dmn_size(); q_ind++)
      for(int j=0; j<nu::dmn_size(); j++)
        for(int i=0; i<nu::dmn_size(); i++)
          S_q(i,j,q_ind) = S_K_w(i,j,K_ind,w_ind);
  }

  template<typename parameters_type, typename K_dmn>
  template<typename scalar_type, typename k_dmn_t, typename q_dmn_t>
  void coarsegraining_routines<parameters_type, K_dmn>::compute_G_q_w(int K_ind, int w_ind,
                                                                      FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, k_dmn_t        > >& H_k,
                                                                      FUNC_LIB::function<std::complex<scalar_type>, dmn_4<nu, nu, K_dmn  , w_REAL> >& S_K,
                                                                      FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t        > >& I_q,
                                                                      FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t        > >& H_q,
                                                                      FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t        > >& S_q,
                                                                      FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t        > >& G_q)
  {
    {
      std::complex<scalar_type> i_wm_min_mu;

      real(i_wm_min_mu) = w_REAL::get_elements()[w_ind]+parameters.get_chemical_potential();
      imag(i_wm_min_mu) = parameters.get_real_frequencies_off_set();

      compute_I_q(i_wm_min_mu, I_q);
    }

    compute_H_q(K_ind, H_k, H_q);

    compute_S_q(K_ind, w_ind, S_K, S_q);

    /*
    {
      for(int q_ind=0; q_ind<q_dmn_t::dmn_size(); q_ind++)
        {
          if(abs(q_dmn_t::get_elements()[q_ind][0])<1.e-6)
	    {
	      double kx = q_dmn_t::get_elements()[q_ind][0];
	      double ky = q_dmn_t::get_elements()[q_ind][1];
	      double kz = q_dmn_t::get_elements()[q_ind][2];
	      
// 	      cout << kx << "\t"
// 		   << ky << "\t"
// 		   << kz << "\t"
// 		   << real(H_q(0,0,q_ind)) << "\t" << -2.*(cos(kx)+cos(ky)+cos(kz)) << "\n";

	      if(abs(real(H_q(0,0,q_ind))+2.*(cos(kx)+cos(ky)+cos(kz)))>1.e-6)
		throw std::logic_error(__FUNCTION__);

	      if(abs(real(H_q(1,1,q_ind))+2.*(cos(kx)+cos(ky)+cos(kz)))>1.e-6)
		throw std::logic_error(__FUNCTION__);

	    }
        }
    }
    */

    /*
      LIN_ALG::matrix<std::complex<scalar_type>, LIN_ALG::CPU> G_inv("G_inv", nu::dmn_size());

      for(int q_ind=0; q_ind<q_dmn_t::dmn_size(); q_ind++){

      for(int j=0; j<nu::dmn_size(); j++)
      for(int i=0; i<nu::dmn_size(); i++)
      G_inv(i,j) = I_q(i,j,q_ind)-H_q(i,j,q_ind)-S_q(i,j,q_ind);

      if(false)
      LIN_ALG::GEINV<LIN_ALG::CPU>::execute(G_inv);
      else
      LIN_ALG::GEINV<LIN_ALG::CPU>::execute_on_Green_function_matrix(G_inv);

      for(int j=0; j<nu::dmn_size(); j++)
      for(int i=0; i<nu::dmn_size(); i++)
      G_q(i,j,q_ind) = G_inv(i,j);
      }
    */

    int nr_threads = parameters.get_nr_coarsegraining_threads();

    if(nr_threads==1)
      quadrature_integration<parameters_type, q_dmn_t>::quadrature_integration_G_q_w_st(            I_q, H_q, S_q, G_q);
    else
      quadrature_integration<parameters_type, q_dmn_t>::quadrature_integration_G_q_w_mt(nr_threads, I_q, H_q, S_q, G_q);
  }

  /*!
   *
   * p 122 AGD
   *
   */
  template<typename parameters_type, typename K_dmn>
  template<typename scalar_type, typename k_dmn_t, typename q_dmn_t>
  void coarsegraining_routines<parameters_type, K_dmn>::compute_G_q_t(int K_ind, int t_ind,
                                                                      FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, k_dmn_t> >& H_k,
                                                                      FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t> >& I_q,
                                                                      FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t> >& H_q,
                                                                      FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t> >& G_q)
  {
    scalar_type f_val = 1;
    scalar_type t_val = t::get_elements()[t_ind];
    scalar_type beta  = parameters.get_beta();

    f_val = t_val<0? 1          : -1;
    t_val = t_val<0? t_val+beta : t_val;

    compute_I_q(parameters.get_chemical_potential(), I_q);

    compute_H_q(K_ind, H_k, H_q);

    G_q = 0.;

    /*
      {
      LIN_ALG::matrix<std::complex<scalar_type>, LIN_ALG::CPU> H_m("H_m", nu::dmn_size());

      LIN_ALG::vector<scalar_type              , LIN_ALG::CPU> L("e_l", nu::dmn_size());
      LIN_ALG::matrix<std::complex<scalar_type>, LIN_ALG::CPU> V("V_l", nu::dmn_size());

      LIN_ALG::vector<scalar_type              , LIN_ALG::CPU> G_t("e_l", nu::dmn_size());

      for(int q_ind=0; q_ind<q_dmn_t::dmn_size(); q_ind++){

      for(int j=0; j<nu::dmn_size(); j++)
      for(int i=0; i<nu::dmn_size(); i++)
      H_m(i,j) = H_q(i,j,q_ind)-I_q(i,j,q_ind);

      if(false)
      LIN_ALG::GEEV<LIN_ALG::CPU>::execute('V', 'U', H_m, L, V);
      else
      LIN_ALG::GEEV<LIN_ALG::CPU>::execute_on_Greens_function_matrix('V', 'U', H_m, L, V);

      for(int i=0; i<nu::dmn_size(); i++){

      if(L[i]<0)
      G_t[i] = f_val*std::exp(L[i]*(beta-t_val))/(std::exp(L[i]*beta)+1.);
      else
      G_t[i] = f_val*std::exp(-L[i]*t_val)/(std::exp(-L[i]*beta)+1.);

      if(G_t[i]!=G_t[i]){
      cout << "\n\t warning in compute_G_q_t --> G_t[i] : " << G_t[i] << "\n";
      cout << "\n\tL[i] : " << L[i] << "\n";
      cout << "\n\tbeta : " << beta << "\n";
      cout << "\n\ttau  : " << t_val << "\n";
      cout << "\n\tstd::exp(L[i]*beta)  : "        << std::exp(L[i]*beta)         << "\n";
      cout << "\n\tstd::exp(L[i]*(beta-t_val)) : " << std::exp(L[i]*(beta-t_val)) << "\n";

      throw std::logic_error(__FUNCTION__);
      }

      //         G_t[i] = f_val*std::exp(L[i]*(beta-t_val))/(std::exp(L[i]*beta)+1.);
      //         if(G_t[i]!=G_t[i]){
      //           cout << "\n\t warning in compute_G_q_t --> G_t[i] : " << G_t[i] << "(L[i]*(beta-t_val) : " << L[i]*(beta-t_val) << ", L[i]*beta : " << L[i]*beta << ")\n";
      //         }
      }

      for(int j=0; j<nu::dmn_size(); j++)
      for(int i=0; i<nu::dmn_size(); i++)
      for(int l=0; l<nu::dmn_size(); l++)
      G_q(i,j,q_ind) += G_t[l]*real(conj(V(l,i))*V(l,j));
      }
      }
    */

    {
      int nr_threads = parameters.get_nr_coarsegraining_threads();

      if(nr_threads==1)
        quadrature_integration<parameters_type, q_dmn_t>::quadrature_integration_G_q_t_st(            beta, f_val, t_val, I_q, H_q, G_q);
      else
        quadrature_integration<parameters_type, q_dmn_t>::quadrature_integration_G_q_t_mt(nr_threads, beta, f_val, t_val, I_q, H_q, G_q);
    }
  }

  /*****************************************
   ***                                   ***
   ***         Routines for DCA+         ***
   ***                                   ***
   *****************************************/

  template<typename parameters_type, typename K_dmn>
  template<typename scalar_type, typename k_dmn_t, typename q_dmn_t>
  void coarsegraining_routines<parameters_type, K_dmn>::compute_S_q_from_A_k(int K_ind, int w_ind,
                                                                             FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, k_dmn_t> >& A_k,
                                                                             FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t> >& A_q,
                                                                             FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t> >& S_q)
  {
    wannier_interpolation(K_ind, A_k, A_q);

    scalar_type alpha = w::get_elements()[w_ind]>0 ? 1 : -1;

    transform_to_alpha::backward(alpha, S_q, A_q);
  }

  template<typename parameters_type, typename K_dmn>
  template<typename scalar_type, typename k_dmn_t, typename q_dmn_t>
  void coarsegraining_routines<parameters_type, K_dmn>::compute_G_q_w(int K_ind, int w_ind,
                                                                      FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, k_dmn_t> >& H_k,
                                                                      FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, k_dmn_t> >& A_k,
                                                                      FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t> >& I_q,
                                                                      FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t> >& H_q,
                                                                      FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t> >& A_q,
                                                                      FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t> >& S_q,
                                                                      FUNC_LIB::function<std::complex<scalar_type>, dmn_3<nu, nu, q_dmn_t> >& G_q)
  {
    {
      std::complex<scalar_type> i_wm_min_mu;

      real(i_wm_min_mu) = parameters.get_chemical_potential();
      imag(i_wm_min_mu) = w::get_elements()[w_ind];

      compute_I_q(i_wm_min_mu, I_q);
    }

    compute_H_q(K_ind, H_k, H_q);

    compute_S_q_from_A_k(K_ind, w_ind, A_k, A_q, S_q);

    /*
      LIN_ALG::matrix<std::complex<scalar_type>, LIN_ALG::CPU> G_inv("G_inv", nu::dmn_size());

      for(int q_ind=0; q_ind<q_dmn_t::dmn_size(); q_ind++){

      for(int j=0; j<nu::dmn_size(); j++)
      for(int i=0; i<nu::dmn_size(); i++)
      G_inv(i,j) = I_q(i,j,q_ind)-H_q(i,j,q_ind)-S_q(i,j,q_ind);

      LIN_ALG::GEINV<LIN_ALG::CPU>::execute_on_Green_function_matrix(G_inv);

      for(int j=0; j<nu::dmn_size(); j++)
      for(int i=0; i<nu::dmn_size(); i++)
      G_q(i,j,q_ind) = G_inv(i,j);
      }
    */

    int nr_threads = parameters.get_nr_coarsegraining_threads();

    if(nr_threads==1)
      quadrature_integration<parameters_type, q_dmn_t>::quadrature_integration_G_q_w_st(I_q, H_q, S_q, G_q);
    else
      quadrature_integration<parameters_type, q_dmn_t>::quadrature_integration_G_q_w_mt(nr_threads, I_q, H_q, S_q, G_q);
  }

}

#endif