#ifndef ALGORITHM_INTERFACE_ENERGYSCALARPRODUCT_HH
#define ALGORITHM_INTERFACE_ENERGYSCALARPRODUCT_HH

#include "abstractScalarProduct.hh"
#include "Operator/abstractOperator.hh"
#include "Util/Mixins/impl.hh"

namespace Algorithm
{
  namespace Interface
  {
    class EnergyScalarProduct : public AbstractScalarProduct, public Mixin::UniqueImpl<AbstractOperator>
    {
    public:
      explicit EnergyScalarProduct(const AbstractOperator& impl);

      double operator()(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y) const final override;
    };
  }
}

#endif // ALGORITHM_INTERFACE_ENERGYSCALARPRODUCT_HH
