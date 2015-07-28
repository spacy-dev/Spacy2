#include "operator.hh"

#include "banachSpace.hh"
#include "hilbertSpace.hh"
#include "Interface/abstractHilbertSpace.hh"

#include "functionSpaceElement.hh"

#include <utility>

namespace Algorithm
{
  Operator::Operator(std::unique_ptr<Interface::AbstractOperator>&& implementation)
    : Mixin::UniqueImpl<Interface::AbstractOperator>(std::move(implementation))
  {
    if(Interface::isHilbertSpace(impl().domain()))
      domain_ = std::make_shared<HilbertSpace>(std::static_pointer_cast<Interface::AbstractHilbertSpace>(impl().sharedDomain()));
    else
      domain_ = std::make_shared<BanachSpace>(impl().sharedDomain());

    if(Interface::isHilbertSpace(impl().range()))
      range_ = std::make_shared<HilbertSpace>(std::static_pointer_cast<Interface::AbstractHilbertSpace>(impl().sharedRange()));
    else
      range_ = std::make_shared<BanachSpace>(impl().sharedRange());
  }

  FunctionSpaceElement Operator::operator()(const FunctionSpaceElement& x) const
  {
    return impl()(x.impl());
  }

  BanachSpace& Operator::domain()
  {
    return *domain_;
  }

  const BanachSpace& Operator::domain() const
  {
    return *domain_;
  }

  std::shared_ptr<BanachSpace> Operator::sharedDomain() const
  {
    return domain_;
  }

  BanachSpace& Operator::range()
  {
    return *range_;
  }

  const BanachSpace& Operator::range() const
  {
    return *range_;
  }
}
