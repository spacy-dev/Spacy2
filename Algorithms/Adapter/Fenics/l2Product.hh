#ifndef ALGORITHM_ADAPTER_FENICS_L2_PRODUCT_HH
#define ALGORITHM_ADAPTER_FENICS_L2_PRODUCT_HH

#include "Algorithms/vector.hh"

namespace Algorithm
{
  namespace Fenics
  {
    /**
     * @ingroup FenicsGroup
     * @brief l2 product \f$(x,y) = \sum_i x_i y_i \f$ for FEniCS.
     */
    class l2Product
    {
    public:
      /**
       * @brief Compute l2 product.
       * @param x vector
       * @param y vector
       * @return \f$(x,y) = \sum_i x_i y_i \f$.
       */
      double operator()(const ::Algorithm::Vector& x, const ::Algorithm::Vector& y) const;
    };
  }
}

#endif // ALGORITHM_ADAPTER_FENICS_L2_PRODUCT_HH

