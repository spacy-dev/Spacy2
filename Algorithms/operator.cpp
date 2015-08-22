#include "operator.hh"

#include "vector.hh"
#include "linearOperator.hh"

#include "Interface/Operator/linearizedOperator.hh"
#include "Util/castTo.hh"


#include <utility>

namespace Algorithm
{
  Operator::Operator(std::unique_ptr<Interface::AbstractOperator>&& implementation)
    : Mixin::UniqueImpl<Interface::AbstractOperator>(std::move(implementation))
  {}

  Vector Operator::operator()(const Vector& x) const
  {
    return impl()(x);
  }

  Vector Operator::d1(const Vector& x, const Vector& dx) const
  {
    return impl().d1( x , dx );
  }

  Vector Operator::d2(const Vector& x, const Vector& dx, const Vector& dy) const
  {
    return impl().d2(x,dx,dy);
  }

  LinearOperator Operator::linearization(const Vector& x) const
  {
    return LinearOperator( impl().linearization(x) );
  }

  VectorSpace* Operator::domain_ptr() const
  {
    return impl().domain_ptr();
  }

  const VectorSpace& Operator::domain() const
  {
    return impl().domain();
  }

  VectorSpace* Operator::range_ptr() const
  {
    return impl().range_ptr();
  }

  const VectorSpace& Operator::range() const
  {
    return impl().range();
  }
}
