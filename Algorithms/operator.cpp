#include "operator.hh"

#include "functionSpaceElement.hh"
#include "linearOperator.hh"

#include "Interface/Operator/linearizedOperator.hh"
#include "Util/castTo.hh"


#include <utility>

namespace Algorithm
{
  Operator::Operator(std::unique_ptr<Interface::AbstractOperator>&& implementation)
    : Mixin::UniqueImpl<Interface::AbstractOperator>(std::move(implementation)),
      domain_( impl().sharedDomain() ),
      range_( impl().sharedRange() )
  {}

  FunctionSpaceElement Operator::operator()(const FunctionSpaceElement& x) const
  {
    return impl()(x.impl());
  }

  FunctionSpaceElement Operator::d1(const FunctionSpaceElement& x, const FunctionSpaceElement& dx) const
  {
    return impl().d1( x.impl() , dx.impl() );
  }

  FunctionSpaceElement Operator::d2(const FunctionSpaceElement& x, const FunctionSpaceElement& dx, const FunctionSpaceElement& dy) const
  {
    return impl().d2(x.impl(),dx.impl(),dy.impl());
  }

  LinearOperator Operator::linearization(const FunctionSpaceElement& x) const
  {
    return LinearOperator( impl().linearization(x.impl()) );
  }

  FunctionSpace& Operator::domain()
  {
    return domain_;
  }

  const FunctionSpace& Operator::domain() const
  {
    return domain_;
  }

  FunctionSpace& Operator::range()
  {
    return range_;
  }

  const FunctionSpace& Operator::range() const
  {
    return range_;
  }
}
