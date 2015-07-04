#ifndef ALGORITHM_INDUCED_SCALARPRODUCT_HH
#define ALGORITHM_INDUCED_SCALARPRODUCT_HH

#include "abstractScalarProduct.hh"
#include "Interface/Functional/abstractC2Functional.hh"
#include "Interface/Functional/hessian.hh"
#include "Util/Mixins/impl.hh"

namespace Algorithm
{
  class C2Functional;
  class FunctionSpaceElement;

  namespace Interface
  {
    class InducedScalarProduct : public AbstractScalarProduct, public Mixin::UniqueImpl<Hessian>
    {
    public:
      InducedScalarProduct(const AbstractC2Functional& F, const AbstractFunctionSpaceElement& x);

      InducedScalarProduct(const C2Functional& F, const FunctionSpaceElement& x);

      double operator()(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y) const final override;
    };
  }
}

#endif // ALGORITHM_INDUCED_SCALARPRODUCT_HH
