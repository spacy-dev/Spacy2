#include "functional.hh"

#include "functionSpaceElement.hh"
#include "hilbertSpace.hh"
#include "Interface/abstractHilbertSpace.hh"
#include "Util/castTo.hh"

#include <utility>

namespace Algorithm
{
  Functional::Functional(std::unique_ptr<Interface::AbstractFunctional>&& implementation)
    : Mixin::UniqueImpl<Interface::AbstractFunctional>(std::move(implementation))
  {
    if(is<Interface::AbstractHilbertSpace>(impl().domain()))
      domain_ = std::make_shared<HilbertSpace>(std::static_pointer_cast<Interface::AbstractHilbertSpace>(impl().sharedDomain()));
    else
      domain_ = std::make_shared<BanachSpace>(impl().sharedDomain());
  }

  double Functional::operator()(const FunctionSpaceElement& x) const
  {
    return impl()(x.impl());
  }

  BanachSpace& Functional::domain()
  {
    return *domain_;
  }

  const BanachSpace& Functional::domain() const
  {
    return *domain_;
  }

  std::shared_ptr<BanachSpace> Functional::sharedDomain() const
  {
    return domain_;
  }

}
