#pragma once

#include <Spacy/Util/Base/vectorBase.hh>
#include <Spacy/Util/Mixins/get.hh>

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

    Real& operator=(double y);

    /// In-place summation \f$x+y\f$.
    Real& operator+=(const Real& y);

    /// In-place subtraction \f$x-y\f$
    Real& operator-=(const Real& y);

    /// Negation \f$ -x\f$.
    Real operator-() const;

    /// Comparison operator \f$ x==y\f$.
    bool operator==(const Real& y) const;

    Real& operator*=(const Real& y);

    Real& operator*=(double a);

    Real& operator/=(const Real& y);

    /// Apply as dual element (i.e. compute \f$x*y\f$).
    Real operator()(const Real& y) const;
  };


  double toDouble(const Vector& x);

  double& toDouble(Vector& x);

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
