#pragma once

#include <Spacy/Spaces/ScalarSpace/Real.hh>
#include <Spacy/vectorSpace.hh>
#include <Spacy/Util/Exceptions/incompatibleSpaceException.hh>
#include <Spacy/Util/Mixins/Get.hh>
#include <functional>
#include <type_traits>

namespace Spacy
{
  /// Type-erased vector.
  class Vector
  {
  public:
    /// Apply as dual space element.
    Real operator()(const Vector& x) const;

    Vector& operator+=(const Vector& y);

    Vector& operator-=(const Vector& y);

    Vector& operator*=(double a);

    Vector operator-() const;

    bool operator==(const Vector& y) const;

    /// Access underlying space.
    const VectorSpace& space() const;
  };

  /// Multiplication with arithmetic types (double,float,int,...).
  template <class Arithmetic,
            class = std::enable_if_t< std::is_arithmetic<Arithmetic>::value > >
  Vector operator*(Arithmetic a, Vector x)
  {
    return x *= a;
  }

  /// Multiplication with arithmetic types (double,float,int,...).
  template <class Arithmetic,
            class = std::enable_if_t< std::is_arithmetic<Arithmetic>::value > >
  Vector operator*(Vector x, Arithmetic a)
  {
    return x *= a;
  }

  /// Sum of vectors \f$z=x+y\f$.
  Vector operator+(Vector x, const Vector& y)
  {
      return x += y;
  }

  /// Subtract vectors \f$z=x-y\f$.
  Vector operator-(Vector x, const Vector& y)
  {
      return x -= y;
  }

  /// Compute scalar product \f$z=x*y=(x,y)\f$.
  Real operator*(const Vector& x, const Vector& y)
  {
      return x.space().scalarProduct()(x,y);
  }

  /// Compute norm, where the norm associated with the underlying function space is used \f$ z = \|x\| \f$.
  Real norm(const Vector& x)
  {
      return x.space().norm()(x);
  }

  void checkDualPairing(const Vector& x, const Vector& y)
  {
    if( !y.space().isPrimalWRT(x.space()) )
      throw IncompatibleSpaceException(x.space().index(),y.space().index());
  }


  template <class T,
            typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
  Vector operator*(const Mixin::Get<T>& x, Vector y)
  {
    return y *= get(x);
  }

  template <class T,
            typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
  Vector operator*(const Vector& x, const Mixin::Get<T>& y)
  {
    return y * x;
  }

  Vector operator*(const Mixin::Get<Real>& x, Vector y)
  {
    return y *= get(get(x));
  }

  Vector operator*(const Vector& x, const Mixin::Get<Real>& y)
  {
    return y * x;
  }
}
