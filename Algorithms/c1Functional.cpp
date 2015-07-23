#include "c1Functional.hh"

#include "functionSpaceElement.hh"
#include "Interface/Functional/abstractFunctional.hh"
#include "Interface/Functional/abstractC1Functional.hh"

namespace Algorithm
{
  C1Functional::C1Functional(std::unique_ptr<Interface::AbstractC1Functional>&& impl)
    : Functional(std::unique_ptr<Interface::AbstractFunctional>(impl.release()))
  {}

  double C1Functional::d1(const FunctionSpaceElement& x, const FunctionSpaceElement &dx) const
  {
    return ( *impl().d1(x.impl()) )(dx.impl());
  }

  FunctionSpaceElement C1Functional::d1(const FunctionSpaceElement &x) const
  {
    return FunctionSpaceElement( impl().d1(x.impl()) );
  }

  Interface::AbstractC1Functional& C1Functional::impl()
  {
    return dynamic_cast<Interface::AbstractC1Functional&>( Functional::impl() );
  }

  const Interface::AbstractC1Functional& C1Functional::impl() const
  {
    return dynamic_cast<const Interface::AbstractC1Functional&>( Functional::impl() );
  }
}
