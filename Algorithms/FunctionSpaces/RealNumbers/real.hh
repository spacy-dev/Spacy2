#ifndef ALGORITHM_REAL_NUMBERS_REAL_HH
#define ALGORITHM_REAL_NUMBERS_REAL_HH

#include "Util/Base/vectorBase.hh"
#include "Util/Mixins/eps.hh"
#include "Util/Mixins/impl.hh"

namespace Algorithm
{
  /// Real number.
  class Real :
      public VectorBase<Real,double> ,
      public SupportedOperatorBase<Real> ,
      public Mixin::Eps
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

    /// Assignment.
    Real& operator=(double y);

    /// Number of entries in coefficient vector (=1).
    unsigned size() const ;

    operator double() const;

    bool operator==(const Real& y) const;

    double operator()(const Real& y) const;
  };
}

#endif // ALGORITHM_REAL_NUMBERS_REAL_HH
