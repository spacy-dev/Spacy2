// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_REAL_SPACE_VECTOR_HH
#define SPACY_REAL_SPACE_VECTOR_HH

#include <ostream>

#include "Spacy/Util/Base/vectorBase.hh"
#include "Spacy/Util/Mixins/get.hh"

namespace Spacy
{
  /// @cond
  class Vector;
  /// @endcond

  /** @addtogroup RealGroup @{ */

  /// %Real number.
  class Real :
      public VectorBase ,
      public Mixin::Get<double>
  {
  public:
    /**
     * @brief Construct real number with value x.
     * @param x initial value
     * @param space associated function space (RealSpace)
     */
    Real(double x, const VectorSpace& space);

    /**
     * @brief Construct real number with initial value 0.
     * @param space associated function space (RealSpace)
     */
    explicit Real(const VectorSpace& space);

    /// Construction from double, is associated to Space::R;
    Real(double x);

    /**
     * @brief Construction from vector.
     * @throws if y does not hold an object of type Real
     */
    Real(const Vector& y);

    Real& operator=(double y);

    /// In-place summation \f$x+y\f$.
    Real& operator+=(const Real& y);

    /// In-place subtraction \f$x-y\f$
    Real& operator-=(const Real& y);

    /// Negation \f$ -x\f$.
    Real operator-() const;

    /// Comparison operator \f$ x==y\f$.
    bool operator==(const Real& y) const;

    /**
     * @brief Assignment from Real stored in a ::Spacy::Vector.
     * @throws InvalidArgumentException if casting y to const Real::Vector& fails
     */
    Real& operator=(const Vector& y);

    Real& operator*=(const Real& y);

    Real& operator*=(double a)
    {
      get() *= a;
      return *this;
    }

    /// Apply as dual element (i.e. compute \f$x*y\f$).
    Real operator()(const Real& y) const;
  };


  double toDouble(const Vector& x);

  double& toDouble(Vector& x);

  double& toDouble(Real& x);

  double toDouble(const Real &x);

  /// Compute \f$x/y\f$.
  Real operator/(Real x, const Real& y);

  /// Compute \f$x/y\f$.
  Real operator/(double x, const Real& y);

  /// Compute \f$x/y\f$.
  Real operator/(const Real& x, double y);


  /// Compute \f$x*y\f$.
  Real operator*(Real x, const Real& y);

  /// Compute \f$x*y\f$.
  template <class Arithmetic,
            class = std::enable_if_t<std::is_arithmetic<Arithmetic>::value> >
  Real operator*(Arithmetic x, Real y)
  {
    return y *= x;
  }

  /// Compute \f$x*y\f$.
  template <class Arithmetic,
            class = std::enable_if_t<std::is_arithmetic<Arithmetic>::value> >
  Real operator*(Real x, Arithmetic y)
  {
    return x *= y;
  }

  /// Compute \f$a*y\f$.
  Vector operator*(Real a, Vector x);

  /// Compute \f$a*y\f$.
  Vector operator*(Vector x, Real a);

  /// Compute \f$x+y\f$.
  Real operator+(Real x, const Real& y);

  /// Compute \f$x+y\f$.
  Real operator+(Real x, double y);

  /// Compute \f$x+y\f$.
  Real operator+(double x, Real y);

  /// Compute \f$x+y\f$.
  Real operator+(Real x, const Vector& y);

  /// Compute \f$x+y\f$.
  Real operator+(const Vector& x, Real y);

  /// Compute \f$x-y\f$.
  Real operator-(Real x, const Real& y);

  /// Compute \f$x-y\f$.
  Real operator-(Real x, double y);

  /// Compute \f$x-y\f$.
  Real operator-(double x, Real y);

  /// Evaluate \f$ x<y \f$.
  bool operator<(const Real& x, const Real& y);

  /// Evaluate \f$ x<y \f$.
  bool operator<(const Real& x, double y);

  /// Evaluate \f$ x<y \f$.
  bool operator<(double x, const Real& y);

  /// Evaluate \f$ x>y \f$.
  bool operator>(const Real& x, const Real& y);

  /// Evaluate \f$ x>y \f$.
  bool operator>(const Real& x, double y);

  /// Evaluate \f$ x>y \f$.
  bool operator>(double x, const Real& y);

  /// Evaluate \f$ x>=y \f$.
  bool operator>=(double x, const Real& y);

  /// Evaluate \f$ x>=y \f$.
  bool operator>=(const Real& x, double y);

  /// Evaluate \f$ x>=y \f$.
  bool operator>=(const Real& x, const Real& y);

  /// Evaluate \f$ x>=y \f$.
  bool operator<=(const Real& x, const Real& y);

  /// Evaluate \f$ x<=y \f$.
  bool operator<=(double x, const Real& y);

  /// Evaluate \f$ x<=y \f$.
  bool operator<=(const Real& x, double y);

  /// Evaluate \f$ x==y \f$.
  bool operator==(const Real& x, double y);

  /// Evaluate \f$ x==y \f$.
  bool operator==(double x, const Real& y);

  /// Write to std::ostream.
  std::ostream& operator<<(std::ostream& os, const Real& x);

  /// Compute absolute value.
  Real abs(Real x);

  /// Compute maximum.
  Real max(const Real& x, const Real& y);

  /// Compute minimum.
  Real min(const Real& x, const Real& y);

  /// Compute \f$x^y\f$.
  Real pow(Real x, double y);

  /// Compute \f$\sqrt{x}\f$.
  Real sqrt(Real x);

  /// Compute \f$ x^{1/3} \f$.
  Real cbrt(Real x);

  /** @} */
}

#endif // SPACY_REAL_SPACE_VECTOR_HH
