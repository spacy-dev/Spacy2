#include "functionSpaceElement.hh"

#include "Interface/abstractBanachSpace.hh"
#include "Interface/abstractFunctionSpaceElement.hh"
#include "FunctionSpaces/RealNumbers/real.hh"
#include "Util/invalidargumentexception.hh"
#include "banachSpace.hh"

#include <utility>

namespace Algorithm
{
  FunctionSpaceElement::FunctionSpaceElement()
    : impl_(nullptr)
  {}

  FunctionSpaceElement::FunctionSpaceElement(std::unique_ptr<AbstractFunctionSpaceElement>&& implementation)
    : impl_(std::move(implementation))
  {}

  FunctionSpaceElement::FunctionSpaceElement(const AbstractFunctionSpaceElement& implementation)
    : impl_(clone(implementation))
  {}


  FunctionSpaceElement::FunctionSpaceElement(const FunctionSpaceElement& y)
    : impl_( clone(y.impl()) )
  {}

  FunctionSpaceElement& FunctionSpaceElement::operator=(const FunctionSpaceElement& y)
  {
    if( impl_ != nullptr)
      y.impl().copyTo(*impl_);
    else
      impl_ = clone( y.impl() );
    return *this;
  }

  FunctionSpaceElement& FunctionSpaceElement::operator=(const AbstractFunctionSpaceElement& implementation)
  {
    if( impl_ != nullptr )
      implementation.copyTo(*impl_);
    else
      impl_ = clone( implementation );
    return *this;
  }

  void FunctionSpaceElement::print(std::ostream& os) const
  {
    return impl().print(os);
  }

  AbstractFunctionSpaceElement& FunctionSpaceElement::impl()
  {
    return *impl_;
  }

  const AbstractFunctionSpaceElement& FunctionSpaceElement::impl() const
  {
    return *impl_;
  }

  unsigned FunctionSpaceElement::spaceIndex() const
  {
    return impl().getSpace().index();
  }

  bool FunctionSpaceElement::operator==(const FunctionSpaceElement& y) const
  {
    return (this == &y);
  }


  FunctionSpaceElement& FunctionSpaceElement::operator+=(const FunctionSpaceElement& y)
  {
    impl() += y.impl();
    return *this;
  }

  FunctionSpaceElement& FunctionSpaceElement::operator+=(Scale&& s)
  {
    impl().axpy(s.a,s.x.impl());
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

  std::ostream& operator<<(std::ostream& os, const FunctionSpaceElement& x)
  {
    return os << x.impl();
  }
}
