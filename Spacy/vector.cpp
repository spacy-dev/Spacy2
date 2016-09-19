#include "vector.hh"

#include <Spacy/Util/cast.hh>
#include <Spacy/Util/Exceptions/incompatibleSpaceException.hh>

#include <Spacy/Spaces/RealSpace/real.hh>
#include <Spacy/vectorSpace.hh>
#include <Spacy/zeroVectorCreator.hh>

#include <cassert>

namespace Spacy
{
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
    return base_->space();
  }

  Vector::operator bool() const
  {
    return base_;
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

  Vector zero(const VectorSpace& space)
  {
      return space.creator()(&space);
  }
}
