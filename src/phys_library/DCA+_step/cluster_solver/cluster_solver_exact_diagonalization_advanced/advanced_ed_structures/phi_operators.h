//-*-C++-*-

#ifndef ADVANCED_FERMIONIC_PHI_OPERATORS_H
#define ADVANCED_FERMIONIC_PHI_OPERATORS_H

namespace DCA
{
  namespace ADVANCED_EXACT_DIAGONALIZATION
  {

    template<typename parameter_type, typename ed_options>       // N: size of bitset sequence
    class operators
    {
//       const static int N = ed_options::N;

      typedef typename ed_options::int_type  int_type;

//       typedef typename ed_options::scalar_type  scalar_type;
//       typedef typename ed_options::complex_type complex_type;

      typedef typename ed_options::phi_type phi_type;

    public:

      //       static void create_at    (int_type l, psi_state<parameter_type, ed_options>& Psi);
      //       static void annihilate_at(int_type l, psi_state<parameter_type, ed_options>& Psi);

      //       static int create(int l, phi_type& phi);

      static bool create_at    (int_type l, phi_type& phi, int& sign);
      static bool annihilate_at(int_type l, phi_type& phi, int& sign);
    };

    template<typename parameter_type, typename ed_options>
    bool operators<parameter_type, ed_options>::create_at(int_type l, phi_type& phi, int& sign)
    {
      int tmp_sgn = sign;

      if(phi.test(l))
        {
          return false;
        }
      else
        {
          phi.set(l);

          if(l != 0)
            {
              int_type tmp = (1<<l) - 1;

              //typename psi_state<parameter_type, ed_options>::phi_type mask(tmp);
              phi_type mask(tmp);

              bool change_sign = ((phi & mask).count()) & 1;

              if (change_sign)
                sign *= -1;
            }

          for(int i = 0; i < l; ++i){
            if(phi[i])
              tmp_sgn *= -1;
          }

          assert(tmp_sgn==sign);

          return true;
        }
    }

    template<typename parameter_type, typename ed_options>
    bool operators<parameter_type, ed_options>::annihilate_at(int_type l, phi_type& phi, int& sign)
    {
      int tmp_sgn = sign;

      if(phi.test(l))
        {
          phi.reset(l);

          if (l != 0)
            {
              int_type tmp = (1<<l) - 1;

              //typename psi_state<parameter_type, ed_options>::phi_type mask(tmp);
              phi_type mask(tmp);

              bool change_sign = ((phi & mask).count()) & 1;

              if (change_sign)
                sign *= -1;
            }

          for(int i = 0; i < l; ++i){
            if(phi[i])
              tmp_sgn *= -1;
          }

          assert(tmp_sgn==sign);

          return true;
        }
      else
        {
          return false;
        }
    }
  }
}

#endif