#include "vector.hh"

#include "Spacy/Util/cast.hh"
#include "Spacy/Util/Exceptions/incompatibleSpaceException.hh"

#include "Spacy/Spaces/RealSpace/real.hh"
#include "Spacy/vectorSpace.hh"


namespace Spacy
{
//  Vector::Vector(AnyVector v)
//    : Mixin::Impl<AnyVector>(std::move(v))
//  {}

  Vector::operator AnyVector() const
  {
    return impl();
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

  Vector& Vector::operator*=(double a)
  {
    impl() *= a;
    return *this;
  }

  Vector Vector::operator-() const
  {
    Vector v(impl());
    v *= -1;
    return v;
  }

  Vector Vector::operator()(const Vector& y) const
  {
    return impl()(y.impl());
  }

  bool Vector::operator==(const Vector& y) const
  {
    return impl() == y.impl();
  }

  const VectorSpace& Vector::space() const
  {
    return *impl().space();
  }

  // free functions
  Vector operator*(double a, Vector x)
  {
    return x*=a;//Scale<Arithmetic>{a,x};
  }

  Vector operator+(Vector x, const Vector& y)
  {
    return x += y;
  }

  Vector operator-(Vector x, const Vector& y)
  {
    return x -= y;
  }

  Real operator*(const Vector& x, const Vector& y)
  {
    return x.space().scalarProduct()(x,y);
  }

  Real norm(const Vector& x)
  {
    return x.space().norm()(x);
  }

  void checkDualPairing(const Vector& x, const Vector& y)
  {
    if( !y.space().isPrimalWRT(x.space()) )
      throw IncompatibleSpaceException(x.space().index(),y.space().index());
  }
}
