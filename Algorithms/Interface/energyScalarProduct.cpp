#include "energyScalarProduct.hh"

#include "abstractFunctionSpaceElement.hh"

namespace Algorithm
{
  namespace Interface
  {
    EnergyScalarProduct::EnergyScalarProduct(const AbstractOperator& impl)
      : Mixin::UniqueImpl<AbstractOperator>(clone(impl))
    {}

    double EnergyScalarProduct::operator()(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y) const
    {
      return (*impl()(y))(x);
    }
  }
}
