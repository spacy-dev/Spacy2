#include "fenics_l2scalarProduct.hh"

#include "fenics_functionSpaceElement.hh"

namespace Algorithm
{
  double Fenics_DefaultScalarProduct::operator()(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y) const
  {
    if( !isFenicsVector(x) || !isFenicsVector(y) ) throw InvalidArgumentException("Fenics_DefaultScalarProduct");

    const auto& x_ = dynamic_cast< const Fenics_Vector& >(x);
    const auto& y_ = dynamic_cast< const Fenics_Vector& >(y);

    return x_.impl().vector()->inner( *y_.impl().vector() );
  }
}
