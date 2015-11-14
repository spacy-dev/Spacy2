#ifndef SPACY_SPACES_PRODUCT_SPACE_VECTOR_HH
#define SPACY_SPACES_PRODUCT_SPACE_VECTOR_HH

#include <memory>
#include <vector>

#include "Spacy/vector.hh"
#include "Spacy/Util/Base/vectorBase.hh"

namespace Spacy
{
  /// @cond
  class Real;
  /// @endcond

  namespace ProductSpace
  {
    /// @cond
    class VectorCreator;
    /// @endcond

    /**
     * @ingroup ProductSpaceGroup
     * @brief Product space vector.
     *
     * Represents a vector \f$x=(x_0,x_1,\ldots,x_n)\f$ of a product space \f$X = \{X_0,X_1,\ldots,X_n\}\f$.
     */
    class Vector : public VectorBase
    {
    public:
      /**
       * @brief Construct product space vector.
       * @param space associated vector space
       */
      explicit Vector(const VectorSpace& space);

      /**
       * @brief In-place summation.
       *
       * @param y vector to add
       * @return \f$x+y\f$
       */
      Vector& operator+=(const Vector& y);

  //    /// Axpy-operation \f$x = x + ay\f$.
      //    AbstractVector& axpy(double a, const AbstractVector& y);

      /**
       * @brief In-place subtraction.
       *
       * @param y vector to subtract
       * @return \f$x-y\f$
       */
      Vector& operator-=(const Vector& y);

      /**
       * @brief In-place multiplication.
       *
       * @param a scaling factor
       * @return \f$ax\f$
       */
      Vector& operator*=(double a);

      /**
       * @brief Negation.
       * @return \f$-x\f$
       */
      Vector operator- () const;

      /**
       * @brief Equality comparison.
       * @return true, if \f$x=y\f$, else false
       */
      bool operator==(const Vector& y) const;

      unsigned numberOfVariables() const;

      /**
       * @brief Access k-th variable.
       * @param k global variable index
       * @return associated vector \f$x_k\f$
       */
      ::Spacy::Vector& variable(unsigned k);

      /**
       * @brief Access k-th variable.
       * @param k global variable index
       * @return associated vector \f$x_k\f$
       */
      const ::Spacy::Vector& variable(unsigned k) const;

      /**
       * @brief Access VectorCreator object.
       * @see ProductSpace::VectorCreator
       */
      const VectorCreator& creator() const;

      /**
       * @brief Apply as dual element.
       * @param y primal vector
       * @return \f$x(y)\f$
       */
      Real operator()(const Vector& y) const;

    private:
      std::vector< ::Spacy::Vector > variables_ = {};
    };
  }
}

#endif // SPACY_SPACES_PRODUCT_SPACE_VECTOR_HH
