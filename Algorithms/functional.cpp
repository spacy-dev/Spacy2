#include "functional.hh"

#include "Interface/hessian.hh"
#include "Util/castTo.hh"

#include <utility>

namespace Algorithm
{
  Functional::Functional(std::unique_ptr<Interface::AbstractFunctional>&& implementation)
    : Mixin::UniqueImpl<Interface::AbstractFunctional>(std::move(implementation))
  {}

  double Functional::operator()(const Vector& x) const
  {
    return impl()(x.impl());
  }

  double Functional::d1(const Vector& x, const Vector &dx) const
  {
    return ( *impl().d1(x.impl()) )(dx.impl());
  }

  Vector Functional::d1(const Vector &x) const
  {
    return impl().d1(x.impl());
  }

  Vector Functional::d2(const Vector& x, const Vector &dx) const
  {
    return impl().d2(x.impl(),dx.impl());
  }

  double Functional::d2(const Vector& x, const Vector &dx, const Vector& dy) const
  {
    return dy( d2(x,dx) );
  }

  LinearOperator Functional::hessian(const Vector& x) const
  {
    return LinearOperator( impl().hessian( x.impl() ) );
  }

  VectorSpace* Functional::domain_ptr() const
  {
    return impl().domain_ptr();
  }

  VectorSpace& Functional::domain()
  {
    return *impl().domain_ptr();
  }

  const VectorSpace& Functional::domain() const
  {
    return impl().domain();
  }
}
