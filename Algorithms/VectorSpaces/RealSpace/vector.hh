#ifndef ALGORITHM_REAL_SPACE_VECTOR_HH
#define ALGORITHM_REAL_SPACE_VECTOR_HH

#include "Util/Base/vectorBase.hh"
#include "Util/Mixins/eps.hh"
#include "Util/Mixins/impl.hh"

namespace Algorithm
{
  namespace Real
  {
    /// Real number.
    class Vector :
        public VectorBase<Vector,double> ,
        public SupportedOperatorBase<Vector>
    {
    public:
      /**
       * @brief Construct real number with value x.
       * @param x initial value
       * @param space associated function space (RealSpace)
       */
      Vector(double x, const ::Algorithm::VectorSpace& space);

      /**
       * @brief Construct real number with initial value 0.
       * @param space associated function space (RealSpace)
       */
      explicit Vector(const ::Algorithm::VectorSpace& space);

      /// Assignment.
      Vector& operator=(double y);

      /// Number of entries in coefficient vector (=1).
      unsigned size() const ;

      /// Implicit conversion to double.
      operator double() const;

      /// Apply as dual element.
      double operator()(const Vector& y) const;
    };
  }
}

#endif // ALGORITHM_REAL_SPACE_VECTOR_HH
