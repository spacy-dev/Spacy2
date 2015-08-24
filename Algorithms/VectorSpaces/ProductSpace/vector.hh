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
    class VectorCreator;
    /// \endcond

    /**
     * @ingroup ProductSpaceGroup
     * @brief Product space vector.
     */
    class Vector : public VectorBase<Vector>, public Mixin::PrimalDualSwitch
    {
    public:
      /**
       * @brief Construct product space vector.
       * @param space associated vector space
       */
      explicit Vector(const VectorSpace& space);

      /// Copy constructor.
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

      /// Equality comparison.
      bool operator==(const Vector& y) const;

      /// Access i-th variables.
      ::Algorithm::Vector& variable(unsigned i);

      /// Access i-th variables.
      const ::Algorithm::Vector& variable(unsigned i) const;

      /// Access primal variables as product space vector.
      Vector& primalComponent();

      /// Access primal variables as product space vector.
      const Vector& primalComponent() const;

      /**
       * @brief Set primal variable from a product space variable.
       * @param y product space variable.
       */
      void setPrimalComponent(const ::Algorithm::Vector& y);

      /// Access dual variables as product space vector.
      Vector& dualComponent();

      /// Access dual variables as product space vector.
      const Vector& dualComponent() const;

      /**
       * @brief Set dual variable from a product space variable.
       * @param y product space variable.
       */
      void setDualComponent(const ::Algorithm::Vector& y);

      /**
       * @brief Access VectorCreator object.
       * @see ProductSpace::VectorCreator
       */
      const VectorCreator& creator() const;

      /**
       * @brief Checks whether this product space has primal-dual structure or is a simple product space.
       * @return true if this product space has primal-dual structure, else false
       */
      bool isPrimalDual() const;

      /// Apply as dual element.
      double operator()(const Vector& y) const;

    private:
      std::vector< ::Algorithm::Vector > variables_ ;
    };
  }
}

#endif // ALGORITHM_PRODUCT_SPACE_VECTOR_HH
