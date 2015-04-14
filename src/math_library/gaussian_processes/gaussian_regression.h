//-*-C++-*-

#ifndef GAUSSIAN_REGRESSION_H
#define GAUSSIAN_REGRESSION_H

namespace MATH_LIBRARY
{
  template<typename scalartype, typename lhs_dmn_t, typename rhs_dmn_t>
  class gaussian_regression
  {
    typedef typename lhs_dmn_t::parameter_type::scalar_type lhs_scalar_type;
    typedef typename rhs_dmn_t::parameter_type::scalar_type rhs_scalar_type;

    typedef typename lhs_dmn_t::parameter_type::element_type lhs_element_type;
    typedef typename rhs_dmn_t::parameter_type::element_type rhs_element_type;

  public:

    gaussian_regression();
    ~gaussian_regression();

    void set_X(LIN_ALG::matrix<scalartype, LIN_ALG::CPU>& X_ref);
    void set_S(LIN_ALG::matrix<scalartype, LIN_ALG::CPU>& S_ref);

    void compute_S(scalartype s_f, scalartype l, scalartype sigma);
    void compute_A(double eps);

    void execute(FUNC_LIB::function<scalartype, lhs_dmn_t>& y,
                 FUNC_LIB::function<scalartype, rhs_dmn_t>& w);

    void execute(FUNC_LIB::function<scalartype, lhs_dmn_t>& y,
                 FUNC_LIB::function<scalartype, rhs_dmn_t>& w,
                 FUNC_LIB::function<scalartype, rhs_dmn_t>& wm,
                 FUNC_LIB::function<scalartype, rhs_dmn_t>& wp);

  private:

    int Nr;
    int Nc;

    // y = X w
    LIN_ALG::matrix<scalartype, LIN_ALG::CPU> X;

    LIN_ALG::matrix<scalartype, LIN_ALG::CPU> S;
    LIN_ALG::matrix<scalartype, LIN_ALG::CPU> S_inv;

    LIN_ALG::matrix<scalartype, LIN_ALG::CPU> A;
    LIN_ALG::matrix<scalartype, LIN_ALG::CPU> A_inv;
    LIN_ALG::matrix<scalartype, LIN_ALG::CPU> A_inv_X;
  };

  template<typename scalartype, typename lhs_dmn_t, typename rhs_dmn_t>
  gaussian_regression<scalartype, lhs_dmn_t, rhs_dmn_t>::gaussian_regression():
    Nr(rhs_dmn_t::dmn_size()),
    Nc(lhs_dmn_t::dmn_size()),

    X("X", std::pair<int,int>(Nr, Nc)),

    S    ("S"    , std::pair<int,int>(Nc, Nc)),
    S_inv("S_inv", std::pair<int,int>(Nc, Nc)),

    A      ("A"      , std::pair<int,int>(Nc, Nc)),
    A_inv  ("A_inv"  , std::pair<int,int>(Nc, Nc)),
    A_inv_X("A_inv_X", std::pair<int,int>(Nc, Nr))
  {}

  template<typename scalartype, typename lhs_dmn_t, typename rhs_dmn_t>
  gaussian_regression<scalartype, lhs_dmn_t, rhs_dmn_t>::~gaussian_regression()
  {}

  template<typename scalartype, typename lhs_dmn_t, typename rhs_dmn_t>
  void gaussian_regression<scalartype, lhs_dmn_t, rhs_dmn_t>::execute(FUNC_LIB::function<scalartype, lhs_dmn_t>& y,
                                                                      FUNC_LIB::function<scalartype, rhs_dmn_t>& w)
  {
    w = 0;
    for(int j=0; j<Nr; j++)
      for(int i=0; i<Nc; i++)
        w(i) += A_inv_X(i,j)*y(j);
  }

  template<typename scalartype, typename lhs_dmn_t, typename rhs_dmn_t>
  void gaussian_regression<scalartype, lhs_dmn_t, rhs_dmn_t>::execute(FUNC_LIB::function<scalartype, lhs_dmn_t>& y,
                                                                      FUNC_LIB::function<scalartype, rhs_dmn_t>& w,
                                                                      FUNC_LIB::function<scalartype, rhs_dmn_t>& wm,
                                                                      FUNC_LIB::function<scalartype, rhs_dmn_t>& wp)
  {
    w = 0;
    for(int j=0; j<Nr; j++)
      for(int i=0; i<Nc; i++)
        w(i) += A_inv_X(i,j)*y(j);

    for(int i=0; i<Nc; i++)
      wm(i) = w(i)-A_inv(i,i);

    for(int i=0; i<Nc; i++)
      wp(i) = w(i)+A_inv(i,i);
  }

  template<typename scalartype, typename lhs_dmn_t, typename rhs_dmn_t>
  void gaussian_regression<scalartype, lhs_dmn_t, rhs_dmn_t>::set_X(LIN_ALG::matrix<scalartype, LIN_ALG::CPU>& X_ref)
  {
    X.copy_from(X_ref);
  }

  // page 19, eqn 2.31, Rasmussen and Williams
  template<typename scalartype, typename lhs_dmn_t, typename rhs_dmn_t>
  void gaussian_regression<scalartype, lhs_dmn_t, rhs_dmn_t>::compute_S(scalartype s_f, scalartype l, scalartype sigma)
  {
    for(int i=0; i<Nc; i++){
      for(int j=0; j<Nc; j++){

        S(i,j) = 0;

        lhs_element_type x_i = lhs_dmn_t::get_elements()[i];
        lhs_element_type x_j = lhs_dmn_t::get_elements()[j];

        //       x_j[0] = x_j[0] + l0*2*M_PI + l1*0;
        //       x_j[1] = x_j[1] + l0*0      + l1*2*M_PI;

        S(i,j) += (s_f*s_f)*std::exp(-0.5*VECTOR_OPERATIONS::L2_NORM(x_i, x_j)/(l*l));
      }

      S(i,i) += sigma*sigma;
    }

    //S.print();
  }

  /*
    template<typename scalartype, typename lhs_dmn_t, typename rhs_dmn_t>
    void gaussian_regression<scalartype, lhs_dmn_t, rhs_dmn_t>::compute_S_periodic(scalartype s_f, scalartype l, scalartype sigma)
    {
    for(int i=0; i<Nc; i++){
    for(int j=0; j<Nc; j++){

    S(i,j) = 0;

    lhs_element_type x_i = lhs_dmn_t::get_elements()[i];
    lhs_element_type x_j = lhs_dmn_t::get_elements()[j];

    x_j[0] = x_j[0] + l0*2*M_PI + l1*0;
    x_j[1] = x_j[1] + l0*0      + l1*2*M_PI;

    S(i,j) += (s_f*s_f)*std::exp(-0.5*VECTOR_OPERATIONS::L2_NORM(x_i, x_j)/(l*l));
    }

    S(i,i) += sigma*sigma;
    }

    //S.print();
    }
  */

  // page 9, Rasmussen and Williams
  template<typename scalartype, typename lhs_dmn_t, typename rhs_dmn_t>
  void gaussian_regression<scalartype, lhs_dmn_t, rhs_dmn_t>::compute_A(double eps)
  {
    LIN_ALG::PSEUDO_INVERSE<LIN_ALG::CPU>::execute(S, S_inv);

    LIN_ALG::matrix<scalartype, LIN_ALG::CPU> Xt_X("Xt_X", std::pair<int,int>(Nc, Nc));

    LIN_ALG::GEMM<LIN_ALG::CPU>::execute('C', 'N', X, X, Xt_X);

    for(int j=0; j<Nc; j++)
      for(int i=0; i<Nc; i++)
        A(i,j) = Xt_X(i,j)+S(i,j);

    //   for(int i=0; i<Nc; i++)
    //     A(i,i) += eps;

    LIN_ALG::PSEUDO_INVERSE<LIN_ALG::CPU>::execute(A, A_inv);

    LIN_ALG::GEMM<LIN_ALG::CPU>::execute('N', 'C', A_inv, X, A_inv_X);

    //A_inv_X.print();
  }

}

#endif