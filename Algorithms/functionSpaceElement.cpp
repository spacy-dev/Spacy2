#include "functionSpaceElement.hh"

#include "Interface/abstractBanachSpace.hh"
#include "Interface/abstractFunctionSpaceElement.hh"
#include "FunctionSpaces/RealNumbers/real.hh"
#include "FunctionSpaces/ProductSpace/productSpaceElement.hh"
#include "Util/Exceptions/invalidArgumentException.hh"
#include "banachSpace.hh"

#include <utility>

namespace Algorithm
{
  using Interface::AbstractFunctionSpaceElement;

  FunctionSpaceElement::FunctionSpaceElement()
    : Mixin::UniqueImpl<AbstractFunctionSpaceElement>(nullptr)
  {}

  FunctionSpaceElement::FunctionSpaceElement(std::unique_ptr<AbstractFunctionSpaceElement>&& implementation)
    : Mixin::UniqueImpl<AbstractFunctionSpaceElement>(std::move(implementation))
  {}

  FunctionSpaceElement::FunctionSpaceElement(const AbstractFunctionSpaceElement& implementation)
    : FunctionSpaceElement(clone(implementation))
  {}

//  FunctionSpaceElement::FunctionSpaceElement(const FunctionSpaceElement& y)
//    : FunctionSpaceElement( clone(y.impl()) )
//  {}

  FunctionSpaceElement& FunctionSpaceElement::operator=(const FunctionSpaceElement& y)
  {
    if( !implIsNullPtr() )
      y.impl().copyTo(impl());
    else
      setImpl( clone( y.impl() ) );
    return *this;
  }

  FunctionSpaceElement& FunctionSpaceElement::operator=(const AbstractFunctionSpaceElement& implementation)
  {
    if( !implIsNullPtr() )
      implementation.copyTo(impl());
    else
      setImpl( clone( implementation ) );
    return *this;
  }

  void FunctionSpaceElement::print(std::ostream& os) const
  {
    return impl().print(os);
  }

  unsigned FunctionSpaceElement::spaceIndex() const
  {
    return impl().space().index();
  }

  bool FunctionSpaceElement::operator==(const FunctionSpaceElement& y) const
  {
    return impl().equals( y.impl() );
  }


  FunctionSpaceElement& FunctionSpaceElement::operator+=(const FunctionSpaceElement& y)
  {
    impl() += y.impl();
    return *this;
  }

  FunctionSpaceElement& FunctionSpaceElement::operator-=(const FunctionSpaceElement& y)
  {
    impl() -= y.impl();
    return *this;
  }

  FunctionSpaceElement FunctionSpaceElement::operator-() const
  {
    return FunctionSpaceElement(-impl());
  }

  double FunctionSpaceElement::operator()(const FunctionSpaceElement& y) const
  {
    return impl()( y.impl() );
  }

  double& FunctionSpaceElement::coefficient(unsigned i)
  {
    return impl().coefficient(i);
  }

  const double& FunctionSpaceElement::coefficient(unsigned i) const
  {
    return impl().coefficient(i);
  }

  unsigned FunctionSpaceElement::size() const
  {
    return impl().size();
  }

  bool FunctionSpaceElement::isAdmissible() const
  {
    return impl().isAdmissible();
  }

  double FunctionSpaceElement::norm() const
  {
    return impl().space().norm()( impl() );
  }



  // free functions
  FunctionSpaceElement operator+(FunctionSpaceElement x, const FunctionSpaceElement& y)
  {
    return x += y;
  }

  FunctionSpaceElement operator-(FunctionSpaceElement x, const FunctionSpaceElement& y)
  {
    return x -= y;
  }

  FunctionSpaceElement operator+(double a, FunctionSpaceElement x)
  {
    if( !isRealElement(x.impl()) ) throw InvalidArgumentException("operator+(double,const FunctionSpaceElement&)");

    dynamic_cast<Real&>(x.impl()).coefficient(0) += a;

    return x;
  }

  FunctionSpaceElement operator+(const FunctionSpaceElement& x, double a)
  {
    return a + x;
  }

  auto operator*(const FunctionSpaceElement& x, const FunctionSpaceElement& y) -> decltype(x.impl()*y.impl())
  {
    return x.impl() * y.impl();
  }

  double norm(const FunctionSpaceElement& x)
  {
    return x.norm();
  }

  std::ostream& operator<<(std::ostream& os, const FunctionSpaceElement& x)
  {
    return os << x.impl();
  }
}
