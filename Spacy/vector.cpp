#include "vector.hh"

#include "Spacy/Util/cast.hh"
#include "Spacy/Util/Exceptions/incompatibleSpaceException.hh"

#include "Spacy/Spaces/RealSpace/real.hh"
#include "Spacy/vectorSpace.hh"

#include <cassert>

namespace Spacy
{
//  Vector& Vector::operator+=(const Vector& y)
//  {
//    return add_(y);
//  }

//  Vector& Vector::operator-=(const Vector& y)
//  {
//    return subtract_(y);
//  }

//  Vector& Vector::operator*=(double a)
//  {
//    return multiply_(a);
//  }

//  Vector Vector::operator-() const
//  {
//    return negate_();
//  }

//  bool Vector::operator==(const Vector& y) const
//  {
//    return compare_(y);
//  }

//  Vector Vector::operator()(const Vector& y) const
//  {
//    return asDual_(y);
//  }

//  const VectorSpace& Vector::space() const
//  {
//    return *space_();
//  }

//  Vector::operator bool() const
//  {
//    return add_ && subtract_ && multiply_ && negate_ && compare_ && asDual_ && space_;
//  }

//  Vector::Vector(AnyVector v)
//    : Mixin::Impl<AnyVector>(std::move(v))
//  {}




//  Vector::operator AnyVector() const
//  {
//    return impl();
//  }

//  Vector& Vector::operator+=(const Vector& y)
//  {
//    impl() += y.impl();
//    return *this;
//  }

//  Vector& Vector::operator-=(const Vector& y)
//  {
//    impl() -= y.impl();
//    return *this;
//  }

//  Vector& Vector::operator*=(double a)
//  {
//    impl() *= a;
//    return *this;
//  }

//  Vector Vector::operator-() const
//  {
//    Vector v(impl());
//    v *= -1;
//    return v;
//  }

//  Vector Vector::operator()(const Vector& y) const
//  {
//    return impl()(y.impl());
//  }

//  bool Vector::operator==(const Vector& y) const
//  {
//    return impl() == y.impl();
//  }

//  const VectorSpace& Vector::space() const
//  {
//    return *impl().space();
//  }



  Real Vector::operator()(const Vector& x) const
  {
    assert(base_);
    return base_->operator ()(x);
  }

  Vector& Vector::operator+=(const Vector& y)
  {
    assert(base_);
    base_->add(y);
    return *this;
  }

  Vector& Vector::operator-=(const Vector& y)
  {
    assert(base_);
    base_->subtract(y);
    return *this;
  }

  Vector& Vector::operator*=(double a)
  {
    assert(base_);
    base_->multiply(a);
    return *this;
  }

  Vector Vector::operator-() const
  {
    assert(base_);
    return base_->negate();
  }

  bool Vector::operator==(const Vector& y) const
  {
    if(!base_ && !y) return true;
    if(!base_ && y) return false;
    if(base_ && !y) return false;
    return base_->compare(y);
  }

  const VectorSpace& Vector::space() const
  {
    assert(base_);
    return *base_->space();
  }

  Vector::operator bool() const
  {
    return base_;
  }



  // free functions
//  Vector operator*(double a, Vector x)
//  {
//    return x*=a;//Scale<Arithmetic>{a,x};
//  }

  Vector operator*(Vector x, double a)
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
