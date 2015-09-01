#ifndef ALGORITHM_REAL_NUMBERS_REALPRODUCT_HH
#define ALGORITHM_REAL_NUMBERS_REALPRODUCT_HH

#include "VSA/vector.hh"

namespace VSA
{
  namespace Real
  {
    /**
     * @ingroup RealGroup
     * @brief Scalar product for space of real numbers.
     */
    class ScalarProduct
    {
    public:
      /**
       * @brief Compute scalar product.
       * @param x vector (Real::Vector)
       * @param y vector (Real::Vector)
       * @return \f$(x,y) = x*y \f$.
       */
      double operator()(const ::VSA::Vector& x, const ::VSA::Vector& y) const;
    };
  }
}

#endif // ALGORITHM_REAL_NUMBERS_REALPRODUCT_HH
