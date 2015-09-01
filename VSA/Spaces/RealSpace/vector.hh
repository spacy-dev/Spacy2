#ifndef ALGORITHM_REAL_SPACE_VECTOR_HH
#define ALGORITHM_REAL_SPACE_VECTOR_HH

#include "VSA/Util/Base/vectorBase.hh"

namespace VSA
{
  namespace Real
  {
    /**
     * @ingroup RealGroup
     * @brief Real number.
     */
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
      Vector(double x, const VectorSpace& space);

      /**
       * @brief Construct real number with initial value 0.
       * @param space associated function space (RealSpace)
       */
      explicit Vector(const VectorSpace& space);

      /**
       * @brief Assignment from double.
       * @param y value to assign
       */
      Vector& operator=(double y);

      /**
       * @brief Implicit conversion to double.
       * @return value of this vector
       */
      operator double() const;

      /**
       * @brief Apply as dual element.
       * @param y primal vector
       * @return \f$x(y)\f$
       */
      double operator()(const Vector& y) const;
    };
  }
}

#endif // ALGORITHM_REAL_SPACE_VECTOR_HH
