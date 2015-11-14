#ifndef SPACY_REAL_SPACE_VECTOR_HH
#define SPACY_REAL_SPACE_VECTOR_HH

#include <ostream>

#include <boost/type_erasure/any.hpp>

#include "Spacy/Util/Base/vectorBase.hh"
#include "Spacy/Util/Concepts/vectorConcept.hh"
#include "Spacy/Util/Mixins/impl.hh"

namespace Spacy
{
  /// @cond
  class Vector;
  /// @endcond

  /**
   * @ingroup RealGroup
   * @brief Real number.
   */
  class Real :
      public VectorBase ,
      public Mixin::Impl<double> ,
      public SupportedOperatorBase<Real>
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

    /**
     * @brief Construction from double.
     * @param y value to assign
     */
    Real(double x);

    /**
     * @brief Construction form vector.
     * @throws if y does not hold an object of type Real
     * @param y value to assign
     */
    Real(const Vector& y);

    /**
     * @brief Assignment from double.
     * @param y value to assign
     */
    Real& operator=(double y);

    /**
     * @brief In-place summation.
     * @throws if y does not hold an object of type Real
     * @param y vector to add
     * @return \f$x+y\f$
     */
    Real& operator+=(const Real& y);

    /**
     * @brief In-place subtraction.
     * @throws if y does not hold an object of type Real
     * @param y vector to subtract
     * @return \f$x-y\f$
     */
    Real& operator-=(const Real& y);

    /**
     * @brief Assignment from Real stored in a ::Spacy::Vector.
     * @param y value to assign
     * @throws InvalidArgumentException if casting y to const Real::Vector& fails
     */
    Real& operator=(const Vector& y);

    Real& operator*=(const Real& y);

    /**
     * @brief Implicit conversion to double.
     * @return value of this vector
     */
   // operator double() const;

//      operator double&();

    /**
     * @brief Apply as dual element.
     * @param y primal vector
     * @return \f$x(y)\f$
     */
    Real operator()(const Real& y) const;

    friend double toDouble(const Real& x);

    friend double& toDouble(Real& x);
  };


  double toDouble(const Vector& x);

  double& toDouble(Vector& x);

  /// Compute \f$x/y\f$.
  Real operator/(Real x, const Real& y);

  /// Compute \f$x/y\f$.
  Real operator/(double x, const Real& y);

  /// Compute \f$x/y\f$.
  Real operator/(const Real& x, double y);


  /// Compute \f$x*y\f$.
  Real operator*(Real x, const Real& y);

  /// Compute \f$x*y\f$.
  Real operator*(double x, Real y);

  /// Compute \f$x*y\f$.
  Real operator*(Real x, double y);

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
}

#endif // SPACY_REAL_SPACE_VECTOR_HH
