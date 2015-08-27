#ifndef ALGORITHM_ADAPTER_FENICS_SCALARPRODUCTS
#define ALGORITHM_ADAPTER_FENICS_SCALARPRODUCTS

#include "../../vector.hh"

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
      /// Compute l2 product \f$(x,y) = \sum_i x_i y_i \f$.
      double operator()(const ::Algorithm::Vector& x, const ::Algorithm::Vector& y) const;
    };
  }
}

#endif // ALGORITHM_ADAPTER_FENICS_SCALARPRODUCTS

