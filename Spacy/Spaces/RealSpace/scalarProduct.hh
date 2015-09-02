#ifndef ALGORITHM_REAL_NUMBERS_REALPRODUCT_HH
#define ALGORITHM_REAL_NUMBERS_REALPRODUCT_HH

#include "Spacy/vector.hh"

namespace Spacy
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
      double operator()(const ::Spacy::Vector& x, const ::Spacy::Vector& y) const;
    };
  }
}

#endif // ALGORITHM_REAL_NUMBERS_REALPRODUCT_HH
