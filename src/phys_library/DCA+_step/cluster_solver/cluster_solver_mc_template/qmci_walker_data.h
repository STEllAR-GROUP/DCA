//-*-C++-*-

#ifndef DCA_QMCI_WALKER_DATA_H
#define DCA_QMCI_WALKER_DATA_H

namespace DCA
{
  namespace QMCI
  {
    /*!
     * \class   MC_walker
     * \ingroup MONTE-CARLO-INTEGRATOR
     * \brief   empty template for the matrices in a Monte Carlo walker.
     * \author  Peter Staar
     * \version 1.0
     */
    template<QMCI_NAMES NAME, LIN_ALG::device_type device_t, typename parameters_type>
    class MC_walker_data
    {
    public:

      MC_walker_data(parameters_type& parameters);

      ~MC_walker_data();
    };
  }

}

#endif