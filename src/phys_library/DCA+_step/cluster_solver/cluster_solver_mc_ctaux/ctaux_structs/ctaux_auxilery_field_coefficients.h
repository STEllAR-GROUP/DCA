//-*-C++-*-

#ifndef DCA_QMCI_CT_AUX_CACHED_AUXILARY_FIELD_VALUES_H
#define DCA_QMCI_CT_AUX_CACHED_AUXILARY_FIELD_VALUES_H

namespace DCA
{
  namespace QMCI
  {
    /*!
     *  \class  CV
     *  \ingroup STRUCTURES
     *
     *  \author Peter Staar
     *  \version 1.0
     *  \brief  This class precomputes expensive exponents.
     *
     *   \f{eqnarray}{
     *   \exp_V(\nu, \mu, \sigma_{HS}, \sigma_{HF}, \Delta r) &=& e^{-\gamma(\nu, \mu, \Delta r)\:\sigma_{HS}\:\sigma_{HF}}
     *   \f}
     */
    template<typename parameters_type>
    class CV
    {
#include "type_definitions.h"

      typedef r_DCA r_dmn_t;

      typedef HS_spin_domain              HS_spin_domain_type;
      typedef HS_field_sign_domain        HS_field_sign_domain_type;

      typedef dmn_0<HS_spin_domain_type>        HS_s;
      typedef dmn_0<HS_field_sign_domain_type>  HS_f;

      typedef dmn_3<nu,nu,r_dmn_t>                nu_nu_r_dmn_t;
      typedef dmn_5<nu,nu,HS_s,HS_f,r_dmn_t>      nu_nu_HS_s_HS_f_r_dmn_t;
      typedef dmn_6<nu,nu,HS_s,HS_s,HS_f,r_dmn_t> nu_nu_HS_s_HS_s_HS_f_r_dmn_t;

    public:

      CV(parameters_type& parameters);
      ~CV();

      template<class stream_type>
      void to_JSON(stream_type& ss);

      static FUNC_LIB::function<double, nu_nu_r_dmn_t>& get_H_interaction();

      int    nu_nu_HS_s_HS_f_r_DCA_dmn_index(int spin_orbital_1,
                                             int spin_orbital_2,
                                             HS_spin_states_type HS_spin,
                                             HS_field_sign_type HS_field_sign,
                                             int site=0);

      /*!
       *   if U_{\nu, \mu} > 0
       *   --> 1
       *   else
       *   --> e^{- \gamma \sigma_{HS}}
       */
      template<typename vertex_singleton_t>
      double get_QMC_factor(vertex_singleton_t& v, HS_spin_states_type new_HS_spin);

      double exp_V(int linind);

      /*!
       *   \f{eqnarray}{
       *   \exp_V(\nu, \mu, \sigma_{HS}, \sigma_{HF}, \Delta r) &=& e^{-\gamma(\nu, \mu, \Delta r)\:\sigma_{HS}\:\sigma_{HF}}
       *   \f}
       */
      template<typename vertex_singleton_t>
      double exp_V(vertex_singleton_t& v);

      /*!
       *   \f{eqnarray}{
       *   \exp_V(\nu, \mu, \sigma_{HS}, \sigma_{HF}, \Delta r) &=& e^{-\gamma(\nu, \mu, \Delta r)\:\sigma_{HS}\:\sigma_{HF}}
       *   \f}
       */
      double exp_V(int spin_orbital_1,
                   int spin_orbital_2,
                   HS_spin_states_type HS_spin,
                   HS_field_sign_type HS_field_sign,
                   int site=0);

      /*
        double one__div__exp_V_function_min_one(int linind);

        double one__div__exp_V_function_min_one(int spin_orbital_1,
        int spin_orbital_2,
        HS_spin_states_type HS_spin,
        HS_field_sign_type HS_field_sign,
        int site=0);
      */

      double exp_delta_V(int linind);

      template<typename vertex_singleton_t>
      double exp_delta_V(vertex_singleton_t& v,
                         HS_spin_states_type new_HS_spin);

      template<typename vertex_singleton_t>
      double exp_minus_delta_V(vertex_singleton_t& v,
                               HS_spin_states_type new_HS_spin);

      double exp_delta_V(int spin_orbital_1,
                         int spin_orbital_2,
                         HS_spin_states_type HS_spin_1,
                         HS_spin_states_type HS_spin_2,
                         HS_field_sign_type HS_field_sign,
                         int site=0);

      double exp_minus_delta_V(int spin_orbital_1,
                               int spin_orbital_2,
                               HS_spin_states_type HS_spin_1,
                               HS_spin_states_type HS_spin_2,
                               HS_field_sign_type HS_field_sign,
                               int site=0);

      template<typename MOMS_type>
      void initialize(MOMS_type& MOMS);

    private:

      void initialize_gamma();
      void initialize_exp_V();
      void initialize_exp_delta_V();

    private:

      parameters_type& parameters;

      double BETA;
      double K_CT_AUX;
      double BANDS;
      double FULL_CLUSTER_SIZE;
      double CORRELATED_ORBITALS;

      nu_nu_HS_s_HS_f_r_dmn_t                        nu_nu_HS_s_HS_f_r_dmn;

      FUNC_LIB::function<double, nu_nu_r_dmn_t>                H_interaction;

      FUNC_LIB::function<double, nu_nu_r_dmn_t>                gamma_function;
      FUNC_LIB::function<double, nu_nu_HS_s_HS_f_r_dmn_t>      exp_V_function;

      //FUNC_LIB::function<double, nu_nu_HS_s_HS_f_r_dmn_t>      one__div__exp_V_function_min_one_function;

      FUNC_LIB::function<double, nu_nu_HS_s_HS_s_HS_f_r_dmn_t> exp_delta_V_function;
    };

    template<typename parameters_type>
    CV<parameters_type>::CV(parameters_type& parameters_ref):
      parameters(parameters_ref),

      gamma_function("gamma_function"),
      exp_V_function("exp_V_function"),
      //one__div__exp_V_function_min_one_function("one__div__exp_V_function_min_one_function"),
      exp_delta_V_function("exp_delta_V_function")
    {

    }

    template<typename parameters_type>
    CV<parameters_type>::~CV()
    {}

    template<typename parameters_type>
    template<class stream_type>
    void CV<parameters_type>::to_JSON(stream_type& ss)
    {
      ss << ",";

      gamma_function.to_JSON(ss);
      ss << ",";

      exp_V_function.to_JSON(ss);
      ss << ",";

      //     one__div__exp_V_function_min_one_function.to_JSON(ss);
      //     ss << ",";

      exp_delta_V_function.to_JSON(ss);
    }

    template<typename parameters_type>
    FUNC_LIB::function<double, typename CV<parameters_type>::nu_nu_r_dmn_t>& CV<parameters_type>::get_H_interaction()
    {
      static FUNC_LIB::function<double, nu_nu_r_dmn_t> H;
      return H;
    }

    template<typename parameters_type>
    inline int CV<parameters_type>::nu_nu_HS_s_HS_f_r_DCA_dmn_index(int spin_orbital_1,
                                                                    int spin_orbital_2,
                                                                    HS_spin_states_type HS_spin,
                                                                    HS_field_sign_type HS_field_sign,
                                                                    int site)
    {
//       int HS_spin_ind  = do_cast<int>::execute(HS_spin);
//       int HS_field_ind = do_cast<int>::execute(HS_field_sign);

      int HS_spin_ind  = HS_spin_domain      ::to_coordinate(HS_spin);
      int HS_field_ind = HS_field_sign_domain::to_coordinate(HS_field_sign);

      return nu_nu_HS_s_HS_f_r_dmn(spin_orbital_1, spin_orbital_2, HS_spin_ind, HS_field_ind, site);
    }

    template<typename parameters_type>
    inline double CV<parameters_type>::exp_V(int linind)
    {
      return exp_V_function(linind);
    }

    template<typename parameters_type>
    template<typename vertex_singleton_t>
    inline double CV<parameters_type>::get_QMC_factor(vertex_singleton_t& v, HS_spin_states_type new_HS_spin)
    {
      std::pair<int,int>& spin_orbitals = v.get_spin_orbitals();

      if(H_interaction(spin_orbitals.first, spin_orbitals.second, 0) > 1.e-3)
        {
          return 1.;
        }

      if(H_interaction(spin_orbitals.first, spin_orbitals.second, 0) < -1.e-3)
        {
          HS_spin_states_type old_HS_spin  = v.get_HS_spin();
          HS_field_sign_type  HS_field = HS_FIELD_UP;

          return std::exp(-gamma_function(spin_orbitals.first, spin_orbitals.second, 0)*(new_HS_spin-old_HS_spin)*HS_field);
          //return this->exp_V(spin_orbitals.first, spin_orbitals.second, HS_spin, HS_field);
        }

      return 0.;
    }

    template<typename parameters_type>
    template<typename vertex_singleton_t>
    inline double CV<parameters_type>::exp_V(vertex_singleton_t& v)
    {
      HS_spin_states_type HS_spin  = v.get_HS_spin();
      HS_field_sign_type  HS_field = v.get_HS_field();

      int spin_orbital         = v.get_spin_orbital();
      int spin_orbital_paired  = v.get_paired_spin_orbital();

      return this->exp_V(spin_orbital, spin_orbital_paired, HS_spin, HS_field);
    }

    template<typename parameters_type>
    inline double CV<parameters_type>::exp_V(int spin_orbital_1,
                                             int spin_orbital_2,
                                             HS_spin_states_type HS_spin,
                                             HS_field_sign_type HS_field_sign,
                                             int site)
    {
//       int HS_spin_ind  = do_cast<int>::execute(HS_spin);
//       int HS_field_ind = do_cast<int>::execute(HS_field_sign);

      int HS_spin_ind  = HS_spin_domain      ::to_coordinate(HS_spin);
      int HS_field_ind = HS_field_sign_domain::to_coordinate(HS_field_sign);

      return exp_V_function(spin_orbital_1, spin_orbital_2, HS_spin_ind, HS_field_ind, site);
    }

    template<typename parameters_type>
    inline double CV<parameters_type>::exp_delta_V(int linind)
    {
      return exp_delta_V_function(linind);
    }

    template<typename parameters_type>
    template<typename vertex_singleton_t>
    inline double CV<parameters_type>::exp_delta_V(vertex_singleton_t& v,
                                                   HS_spin_states_type new_HS_spin)
    {
      int spin_orbital_1  = v.get_spin_orbital();
      int spin_orbital_2  = v.get_paired_spin_orbital();

      HS_spin_states_type old_HS_spin  = v.get_HS_spin();
      HS_field_sign_type  HS_field     = v.get_HS_field();

      return this->exp_delta_V(spin_orbital_1, spin_orbital_2, new_HS_spin, old_HS_spin, HS_field, 0);
    }

    template<typename parameters_type>
    template<typename vertex_singleton_t>
    inline double CV<parameters_type>::exp_minus_delta_V(vertex_singleton_t& v,
                                                         HS_spin_states_type new_HS_spin)
    {
      int spin_orbital_1  = v.get_spin_orbital();
      int spin_orbital_2  = v.get_paired_spin_orbital();

      HS_spin_states_type old_HS_spin  = v.get_HS_spin();
      HS_field_sign_type  HS_field     = v.get_HS_field();

      return this->exp_minus_delta_V(spin_orbital_1, spin_orbital_2, new_HS_spin, old_HS_spin, HS_field, 0);
    }

    template<typename parameters_type>
    inline double CV<parameters_type>::exp_delta_V(int spin_orbital_1,
                                                   int spin_orbital_2,
                                                   HS_spin_states_type HS_spin_1,
                                                   HS_spin_states_type HS_spin_2,
                                                   HS_field_sign_type HS_field_sign,
                                                   int site)
    {
      assert(site==0);

//       int HS_spin_1_ind = do_cast<int>::execute(HS_spin_1);
//       int HS_spin_2_ind = do_cast<int>::execute(HS_spin_2);
//       int HS_field_ind  = do_cast<int>::execute(HS_field_sign);

      int HS_spin_1_ind = HS_spin_domain      ::to_coordinate(HS_spin_1);
      int HS_spin_2_ind = HS_spin_domain      ::to_coordinate(HS_spin_2);
      int HS_field_ind  = HS_field_sign_domain::to_coordinate(HS_field_sign);

      return exp_delta_V_function(spin_orbital_1, spin_orbital_2, HS_spin_1_ind, HS_spin_2_ind, HS_field_ind, site);
    }

    template<typename parameters_type>
    inline double CV<parameters_type>::exp_minus_delta_V(int spin_orbital_1,
                                                         int spin_orbital_2,
                                                         HS_spin_states_type HS_spin_1,
                                                         HS_spin_states_type HS_spin_2,
                                                         HS_field_sign_type HS_field_sign,
                                                         int site)
    {
      assert(site==0);

//       int HS_spin_1_ind = do_cast<int>::execute(HS_spin_1);
//       int HS_spin_2_ind = do_cast<int>::execute(HS_spin_2);
//       int HS_field_ind  = do_cast<int>::execute(HS_field_sign);

      int HS_spin_1_ind = HS_spin_domain      ::to_coordinate(HS_spin_1);
      int HS_spin_2_ind = HS_spin_domain      ::to_coordinate(HS_spin_2);
      int HS_field_ind  = HS_field_sign_domain::to_coordinate(HS_field_sign);

      return exp_delta_V_function(spin_orbital_1, spin_orbital_2, HS_spin_2_ind, HS_spin_1_ind, HS_field_ind, site);
    }

    template<class parameters_type>
    template<typename MOMS_type>
    void CV<parameters_type>::initialize(MOMS_type& MOMS)
    {
      BETA               = parameters.get_beta();
      K_CT_AUX           = parameters.get_K_CT_AUX();
      BANDS              = electron_band_domain_type::get_size();
      FULL_CLUSTER_SIZE  = r_dmn_t::dmn_size();

      H_interaction       = MOMS.H_interactions;
      get_H_interaction() = MOMS.H_interactions;

      CORRELATED_ORBITALS = 0;

      for(int nu_ind_i=0; nu_ind_i<2*BANDS; nu_ind_i++)
        for(int nu_ind_j=0; nu_ind_j<2*BANDS; nu_ind_j++)
          for(int r=0; r<FULL_CLUSTER_SIZE; r++)
            fabs(H_interaction(nu_ind_i, nu_ind_j, r)) > 1.e-3 ? CORRELATED_ORBITALS++ : CORRELATED_ORBITALS ;

      CORRELATED_ORBITALS = CORRELATED_ORBITALS/2.;

      initialize_gamma();
      initialize_exp_V();
      initialize_exp_delta_V();
    }

    template<class parameters_type>
    void  CV<parameters_type>::initialize_gamma()
    {
      //cout << __FUNCTION__ << endl;

      for(int nu_ind_i=0; nu_ind_i<2*BANDS; nu_ind_i++)
        {
          for(int nu_ind_j=0; nu_ind_j<2*BANDS; nu_ind_j++)
            {
              for(int r=0; r<FULL_CLUSTER_SIZE; r++)
                {
                  double U_i_j_r = fabs(H_interaction(nu_ind_i, nu_ind_j, r));

                  double coshgamma=1.+U_i_j_r*BETA*FULL_CLUSTER_SIZE*CORRELATED_ORBITALS/(2.*K_CT_AUX);

                  gamma_function(nu_ind_i, nu_ind_j, r) = acosh(coshgamma);
                }
            }
        }
    }

    template<class parameters_type>
    void CV<parameters_type>::initialize_exp_V()
    {
      //cout << __FUNCTION__ << endl;

      for(int nu_ind_i=0; nu_ind_i<2*BANDS; nu_ind_i++)
        {
          for(int nu_ind_j=0; nu_ind_j<2*BANDS; nu_ind_j++)
            {
              for(int HS_spin_ind=0; HS_spin_ind<3; HS_spin_ind++)
                {
                  for(int HS_field_ind=0; HS_field_ind<2; HS_field_ind++)
                    {
                      for(int r=0; r<FULL_CLUSTER_SIZE; r++)
                        {
                          HS_spin_states_type HS_spin  = HS_spin_domain_type      ::get_elements()[HS_spin_ind];
                          HS_field_sign_type  HS_field = HS_field_sign_domain_type::get_elements()[HS_field_ind];

                          //if(H_interaction(nu_ind_i, nu_ind_j, r)>1.e-3)
                          {
                            exp_V_function(nu_ind_i, nu_ind_j, HS_spin_ind, HS_field_ind, r)
                              = std::exp(-gamma_function(nu_ind_i, nu_ind_j, r)*HS_spin*HS_field);

                            //                    if(fabs(exp_V_function(nu_ind_i, nu_ind_j, HS_spin_ind, HS_field_ind, r)-1.) > 1.e-16)
                            //                      one__div__exp_V_function_min_one_function(nu_ind_i, nu_ind_j, HS_spin_ind, HS_field_ind, r)
                            //                        = 1./(exp_V_function(nu_ind_i, nu_ind_j, HS_spin_ind, HS_field_ind, r)-1.);
                          }

                          if(H_interaction(nu_ind_i, nu_ind_j, r)<-1.e-3)
                            {
                              //                            cout << r_dmn_t::get_r_cluster()[r][0] << "\t"
                              //                                 << r_dmn_t::get_r_cluster()[r][1] << "\t"
                              //                                 << H_interaction(nu_ind_i, nu_ind_j, r) << endl;

                              HS_field = HS_FIELD_UP;

                              exp_V_function(nu_ind_i, nu_ind_j, HS_spin_ind, HS_field_ind, r)
                                = std::exp(-gamma_function(nu_ind_i, nu_ind_j, r)*HS_spin*HS_field);

                              //                            if(fabs(exp_V_function(nu_ind_i, nu_ind_j, HS_spin_ind, HS_field_ind, r)-1.) > 1.e-16)
                              //                              one__div__exp_V_function_min_one_function(nu_ind_i, nu_ind_j, HS_spin_ind, HS_field_ind, r)
                              //                                = 1./(exp_V_function(nu_ind_i, nu_ind_j, HS_spin_ind, 1, r)-1.);
                            }
                        }
                    }
                }
            }
        }
    }

    template<class parameters_type>
    void CV<parameters_type>::initialize_exp_delta_V()
    {
      for(int nu_ind_i=0; nu_ind_i<2*BANDS; nu_ind_i++)
        {
          for(int nu_ind_j=0; nu_ind_j<2*BANDS; nu_ind_j++)
            {
              for(int HS_spin_1_ind=0; HS_spin_1_ind<3; HS_spin_1_ind++)
                {
                  for(int HS_spin_2_ind=0; HS_spin_2_ind<3; HS_spin_2_ind++)
                    {
                      for(int HS_field_ind=0; HS_field_ind<2; HS_field_ind++)
                        {
                          for(int r=0; r<FULL_CLUSTER_SIZE; r++)
                            {
                              HS_spin_states_type HS_spin_1  = HS_spin_domain_type::get_elements()[HS_spin_1_ind];
                              HS_spin_states_type HS_spin_2  = HS_spin_domain_type::get_elements()[HS_spin_2_ind];

                              HS_field_sign_type  HS_field = HS_field_sign_domain_type::get_elements()[HS_field_ind];

                              //if(H_interaction(nu_ind_i, nu_ind_j, r)>1.e-3)
                              {
                                exp_delta_V_function(nu_ind_i, nu_ind_j, HS_spin_1_ind, HS_spin_2_ind, HS_field_ind, r) =
                                  std::exp(-gamma_function(nu_ind_i, nu_ind_j, r)*(HS_spin_1-HS_spin_2)*HS_field);
                              }

                              if(H_interaction(nu_ind_i, nu_ind_j, r)<-1.e-3)
                                {
                                  HS_field = HS_FIELD_UP;

                                  exp_delta_V_function(nu_ind_i, nu_ind_j, HS_spin_1_ind, HS_spin_2_ind, HS_field_ind, r) =
                                    std::exp(-gamma_function(nu_ind_i, nu_ind_j, r)*(HS_spin_1-HS_spin_2)*HS_field);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

  }

}

#endif