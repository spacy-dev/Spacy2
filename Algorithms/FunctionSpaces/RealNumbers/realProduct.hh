#ifndef ALGORITHM_REAL_NUMBERS_REALPRODUCT_HH
#define ALGORITHM_REAL_NUMBERS_REALPRODUCT_HH

#include "../../Interface/abstractFunctionSpaceElement.hh"
#include "../../Interface/abstractScalarProduct.hh"

namespace Algorithm
{
  class RealProduct : public AbstractScalarProduct
  {
    double operator()(const AbstractFunctionSpaceElement&, const AbstractFunctionSpaceElement&) const final override;
  };
}

#endif // ALGORITHM_REAL_NUMBERS_REALPRODUCT_HH
