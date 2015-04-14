//-*-C++-*-

#ifndef EVALUATE_BASIS_FUNCTION_HERMITE_POLYNOMIAL_CUBIC_H
#define EVALUATE_BASIS_FUNCTION_HERMITE_POLYNOMIAL_CUBIC_H

namespace TRAFOR
{
  template<typename lh_dmn_type, typename rh_dmn_type>
  class evaluate_basis_function<lh_dmn_type, rh_dmn_type, HERMITE_POLYNOMIAL_CUBIC>
  {
  public:

    const static bool ORTHOGONAL = false;

    const static BOUNDARY_CONDITIONS_type BC = lh_dmn_type::parameter_type::BOUNDARY_CONDITION;

    typedef typename lh_dmn_type::parameter_type::scalar_type scalartype;

  public:

    template<typename f_scalartype>
    static void execute(function<f_scalartype, lh_dmn_type>& f, int j)
    {
      scalartype alpha = -0.5;

      scalartype y = rh_dmn_type::get_elements()[j];
      

      for(int i=0; i<f.size(); i++)
	{
	  scalartype x = lh_dmn_type::get_elements()[i];

	  f(i) = spline(x, y, d, alpha);
	}
    }

    template<typename f_scalartype>
    static void norm(function<f_scalartype, rh_dmn_type>& f)
    {
      for(int i=0; i<f.size(); i++)
	f(i) = 1.;
    }

    template<typename f_scalartype>
    static void inner_product_function(function<f_scalartype, lh_dmn_type>& f)
    {

    }

    /*
      c_{i,j} = \int ds u_i^{lhs}(s)*u_j^{rhs}(s)
     */
    template<typename f_scalartype>
    static scalartype contraction(int i, int j)
    {
      
    }

  private:
    
    /*
      u( 0 < |s| < 1 ) = (a+2)*s^3-(a+3)*s^2+1 
      u( 1 < |s| < 2 ) = a*s^3-5as^2+8as-4a
      u( 2 < |s| )     = 0
     */
    static scalartype spline(scalartype x, scalartype y, scalartype d, scalartype a)
    {
      scalartype delta = std::abs((y-x)/d);

      if(delta>=0-1.e-6 and delta<1)
	return (a+2)*std::pow(delta,3)-(a+3)*std::pow(delta,2)+1;

      if(delta>=1 and delta<2)
	return a*std::pow(delta,3) - 5*a*std::pow(delta,2)+8*a*std::pow(delta,1)-4*a;

      return 0;
    }
    

  };

}

#endif