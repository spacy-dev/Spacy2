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
  {
//    if(is<Interface::AbstractHilbertSpace>(impl().domain()))
//      domain_ = std::make_shared<HilbertSpace>(std::static_pointer_cast<Interface::AbstractHilbertSpace>(impl().sharedDomain()));
//    else
//      domain_ = std::make_shared<FunctionSpace>(impl().sharedDomain());

//    if(is<Interface::AbstractHilbertSpace>(impl().range()))
//      range_ = std::make_shared<HilbertSpace>(std::static_pointer_cast<Interface::AbstractHilbertSpace>(impl().sharedRange()));
//    else
//      range_ = std::make_shared<FunctionSpace>(impl().sharedRange());
  }

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
    return dynamic_cast<const Interface::AbstractOperator&>( impl() ).d2(x.impl(),dx.impl(),dy.impl());
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

//  std::shared_ptr<FunctionSpace> Operator::sharedDomain() const
//  {
//    return domain_;
//  }

  FunctionSpace& Operator::range()
  {
    return range_;
  }

  const FunctionSpace& Operator::range() const
  {
    return range_;
  }
}
