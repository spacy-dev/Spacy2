#include "functional.hh"

#include "functionSpaceElement.hh"
#include "Util/castTo.hh"

#include <utility>

namespace Algorithm
{
  Functional::Functional(std::unique_ptr<Interface::AbstractFunctional>&& implementation)
    : Mixin::UniqueImpl<Interface::AbstractFunctional>(std::move(implementation)),
      domain_( impl().sharedDomain() )
  {
//    if(is<Interface::AbstractHilbertSpace>(impl().domain()))
//      domain_ = std::make_shared<HilbertSpace>(std::static_pointer_cast<Interface::AbstractHilbertSpace>(impl().sharedDomain()));
//    else
//      domain_ = std::make_shared<FunctionSpace>(impl().sharedDomain());
  }

  double Functional::operator()(const FunctionSpaceElement& x) const
  {
    return impl()(x.impl());
  }

  FunctionSpace& Functional::domain()
  {
    return domain_;
  }

  const FunctionSpace& Functional::domain() const
  {
    return domain_;
  }
}
