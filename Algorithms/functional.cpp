#include "functional.hh"

#include "Interface/hessian.hh"
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

  double Functional::d1(const FunctionSpaceElement& x, const FunctionSpaceElement &dx) const
  {
    return ( *impl().d1(x.impl()) )(dx.impl());
  }

  FunctionSpaceElement Functional::d1(const FunctionSpaceElement &x) const
  {
    return FunctionSpaceElement( impl().d1(x.impl()) );
  }

  FunctionSpaceElement Functional::d2(const FunctionSpaceElement& x, const FunctionSpaceElement &dx) const
  {
    return FunctionSpaceElement( (*impl().hessian( x.impl() ) )( dx.impl() ) );
  }

  double Functional::d2(const FunctionSpaceElement& x, const FunctionSpaceElement &dx, const FunctionSpaceElement& dy) const
  {
    return dy( d2(x,dx) );
  }

  LinearOperator Functional::hessian(const FunctionSpaceElement& x) const
  {
    return LinearOperator( impl().hessian( x.impl() ) );
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
