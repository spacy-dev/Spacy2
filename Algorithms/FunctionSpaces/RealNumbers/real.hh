#ifndef ALGORITHM_REAL_NUMBERS_REAL_HH
#define ALGORITHM_REAL_NUMBERS_REAL_HH

#include <memory>
#include "Interface/vectorBase.hh"
#include "Util/Mixins/eps.hh"

namespace Algorithm
{
  /// \cond
  class VectorSpace;
  /// \endcond

  /// Real number.
  class Real : public VectorBase<Real> , public Mixin::Eps
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
     * @brief copy data to y.
     */
//    void copyTo(Interface::AbstractVector& y) const ;

    /// Print to os.
//    void print(std::ostream& os) const ;

    /// Assignment.
//    Real& operator=(const Real& y);

//    Real(const Real& y);

    /// In-place summation.
    Real& operator+=(const Real& y);

    /// In-place subtraction.
    Real& operator-=(const Real& y);

    /// In-place multiplication.
    Real& operator*=(double a);

    /// Get -x.
    Real operator- () const;

    /// Access value.
    double& coefficient(unsigned);

    /// Access value.
    const double& coefficient(unsigned) const;

    /// Number of entries in coefficient vector (=1).
    unsigned size() const ;

    operator double() const
    {
      return x_;
    }

    bool operator==(const Real& y) const;

    double operator()(const Real& y) const;

  private:
    /**
     * @brief Get a copy of this real number.
     */
//    Real* cloneImpl() const ;

    /// Apply as dual element.

    friend class RealProduct;
    double x_;
  };
}

#endif // ALGORITHM_REAL_NUMBERS_REAL_HH
