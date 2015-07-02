#include "fenics_scalarProducts.hh"

#include "fenics_functionSpaceElement.hh"

#include <dolfin.h>

namespace Algorithm
{
  double Fenics_DefaultScalarProduct::operator()(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y) const
  {
    return fenics_Vector(x).impl().vector()->inner( *fenics_Vector(y).impl().vector() );
  }

  Fenics_EnergyScalarProduct::Fenics_EnergyScalarProduct(const dolfin::GenericMatrix& A)
    : A_(A)
  {}

  double Fenics_EnergyScalarProduct::operator()(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y) const
  {
    const auto& x_ = fenics_Vector(x);
    const auto& y_ = fenics_Vector(y);

    auto z = clone(y_);

    A_.mult( *y_.impl().vector() , *z->impl().vector() );

    return x_.impl().vector()->inner( *z->impl().vector() );
  }
}
