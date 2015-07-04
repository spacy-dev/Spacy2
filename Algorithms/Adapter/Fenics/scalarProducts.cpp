#include "scalarProducts.hh"

#include "vector.hh"

#include <dolfin.h>

namespace Algorithm
{
  using Interface::AbstractFunctionSpaceElement;

  namespace Fenics
  {
    double l2ScalarProduct::operator()(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y) const
    {
      return toVector(x).impl().vector()->inner( *toVector(y).impl().vector() );
    }

    EnergyScalarProduct::EnergyScalarProduct(const dolfin::GenericMatrix& A)
      : A_(A)
    {}

    double EnergyScalarProduct::operator()(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y) const
    {
      const auto& x_ = toVector(x);
      const auto& y_ = toVector(y);

      auto z = clone(y_);

      A_.mult( *y_.impl().vector() , *z->impl().vector() );

      return x_.impl().vector()->inner( *z->impl().vector() );
    }
  }
}
