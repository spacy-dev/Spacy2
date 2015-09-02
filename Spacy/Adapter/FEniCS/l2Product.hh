#ifndef ALGORITHM_ADAPTER_FENICS_L2_PRODUCT_HH
#define ALGORITHM_ADAPTER_FENICS_L2_PRODUCT_HH

#include "Spacy/vector.hh"

namespace Spacy
{
  namespace FEniCS
  {
    /**
     * @ingroup FenicsGroup
     * @brief l2 product \f$(x,y) = \sum_i x_i y_i \f$ for %FEniCS.
     * @tparam Description %Kaskade::VariableSetDescription
     */
    class l2Product
    {
    public:
      /**
       * @brief Compute l2 product.
       * @param x vector (FEniCS::Vector)
       * @param y vector (FEniCS::Vector)
       * @return \f$(x,y) = \sum_i x_i y_i \f$.
       */
      double operator()(const ::Spacy::Vector& x, const ::Spacy::Vector& y) const;
    };
  }
}

#endif // ALGORITHM_ADAPTER_FENICS_L2_PRODUCT_HH

