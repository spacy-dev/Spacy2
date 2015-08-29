#ifndef ALGORITHM_PRODUCT_SPACE_PRODUCT_HH
#define ALGORITHM_PRODUCT_SPACE_PRODUCT_HH

#include "Algorithms/vector.hh"

namespace Algorithm
{
  namespace ProductSpace
  {
    /**
     * @ingroup ProductSpaceGroup
     * @brief Canonical scalar product on product spaces.
     */
    class ScalarProduct
    {
    public:
      /// Compute \f$(x,y)=\sum_i (x_i,y_i)\f$.
      double operator()(const ::Algorithm::Vector& x, const ::Algorithm::Vector& y) const;
    };
  }
}

#endif // ALGORITHM_PRODUCT_SPACE_PRODUCT_HH
