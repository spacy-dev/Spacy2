#include "vector.hh"

#include "Interface/abstractVector.hh"
#include "FunctionSpaces/RealNumbers/real.hh"
#include "FunctionSpaces/ProductSpace/productSpaceElement.hh"
#include "Util/Exceptions/invalidArgumentException.hh"
#include "vectorSpace.hh"

#include <utility>

namespace Algorithm
{
  using Interface::AbstractVector;

  Vector::Vector()
    : Mixin::UniqueImpl<AbstractVector>(nullptr)
  {}

  Vector::Vector(std::unique_ptr<AbstractVector>&& implementation)
    : Mixin::UniqueImpl<AbstractVector>(std::move(implementation))
  {}

  Vector::Vector(const AbstractVector& implementation)
    : Vector(clone(implementation))
  {}

//  Vector::Vector(const Vector& y)
//    : Vector( clone(y.impl()) )
//  {}

  Vector& Vector::operator=(const Vector& y)
  {
    if( !implIsNullPtr() )
      y.impl().copyTo(impl());
    else
      setImpl( clone( y.impl() ) );
    return *this;
  }

  Vector& Vector::operator=(const AbstractVector& implementation)
  {
    if( !implIsNullPtr() )
      implementation.copyTo(impl());
    else
      setImpl( clone( implementation ) );
    return *this;
  }

  void Vector::print(std::ostream& os) const
  {
    return impl().print(os);
  }

  unsigned Vector::spaceIndex() const
  {
    return impl().space().index();
  }

  bool Vector::operator==(const Vector& y) const
  {
    return impl().equals( y.impl() );
  }


  Vector& Vector::operator+=(const Vector& y)
  {
    impl() += y.impl();
    return *this;
  }

  Vector& Vector::operator-=(const Vector& y)
  {
    impl() -= y.impl();
    return *this;
  }

  Vector Vector::operator-() const
  {
    return Vector(-impl());
  }

  double Vector::operator()(const Vector& y) const
  {
    return impl()( y.impl() );
  }

  double& Vector::coefficient(unsigned i)
  {
    return impl().coefficient(i);
  }

  const double& Vector::coefficient(unsigned i) const
  {
    return impl().coefficient(i);
  }

  unsigned Vector::size() const
  {
    return impl().size();
  }

  bool Vector::isAdmissible() const
  {
    return impl().isAdmissible();
  }

  double Vector::norm() const
  {
    return impl().space().norm()( *this );
  }



  // free functions
  Vector operator+(Vector x, const Vector& y)
  {
    return x += y;
  }

  Vector operator-(Vector x, const Vector& y)
  {
    return x -= y;
  }

  Vector operator+(double a, Vector x)
  {
    if( !is<Real>(x.impl()) ) throw InvalidArgumentException("operator+(double,const Vector&)");

    castTo<Real>(x.impl()).coefficient(0) += a;

    return x;
  }

  Vector operator+(const Vector& x, double a)
  {
    return a + x;
  }

  auto operator*(const Vector& x, const Vector& y) -> decltype(x.impl()*y.impl())
  {
    return x.impl() * y.impl();
  }

  double norm(const Vector& x)
  {
    return x.norm();
  }

  std::ostream& operator<<(std::ostream& os, const Vector& x)
  {
    return os << x.impl();
  }
}
