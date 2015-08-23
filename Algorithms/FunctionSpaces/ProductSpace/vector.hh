#ifndef ALGORITHM_PRODUCT_SPACE_VECTOR_HH
#define ALGORITHM_PRODUCT_SPACE_VECTOR_HH

#include <memory>
#include <vector>

#include "../../vector.hh"
#include "Util/Base/vectorBase.hh"
#include "Util/Mixins/primalDualSwitch.hh"

namespace Algorithm
{
  namespace ProductSpace
  {
    /// \cond
    class SpaceCreator;
    /// \endcond

    /**
   * @brief Product space element.
   */
    class Vector : public VectorBase<Vector>, public Mixin::PrimalDualSwitch
    {
    public:
      /**
     * @brief Construct real number with initial value 0.
     * @param space associated function space (RealSpace)
     */
      explicit Vector(const VectorSpace& space);

      Vector(const Vector& other);

      /// Assignment.
      Vector& operator=(const Vector& y);

      /// In-place summation.
      Vector& operator+=(const Vector& y);

      /// Axpy-operation \f$x = x + ay\f$.
      //    AbstractVector& axpy(double a, const AbstractVector& y);

      /// In-place subtraction.
      Vector& operator-=(const Vector& y);

      /// In-place multiplication.
      Vector& operator*=(double);

      /// Get -x.
      Vector operator- () const;

      bool operator==(const Vector& y) const;

      ::Algorithm::Vector& variable(unsigned i);

      const ::Algorithm::Vector& variable(unsigned i) const;

      Vector& primalComponent();

      const Vector& primalComponent() const;

      void setPrimalComponent(const ::Algorithm::Vector& y);

      Vector& dualComponent();

      const Vector& dualComponent() const;

      void setDualComponent(const ::Algorithm::Vector& y);

      const SpaceCreator& productSpace() const;

      bool isPrimalDualProductSpaceElement() const;

      /// Apply as dual element.
      double operator()(const Vector& y) const;

    private:
      std::vector< ::Algorithm::Vector > variables_ ;
    };
  }
}

#endif // ALGORITHM_PRODUCT_SPACE_VECTOR_HH
