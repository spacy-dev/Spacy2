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

  enum { PRIMAL=0 , DUAL=1 };

  namespace ProductSpace
  {
    /// @cond
    class VectorCreator;
    /// @endcond

    /** @addtogroup ProductSpaceGroup
     * @{
     */

    /**
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

      /** @return \f$x+y\f$ */
      Vector& operator+=(const Vector& y);

  //    /// Axpy-operation \f$x = x + ay\f$.
      //    AbstractVector& axpy(double a, const AbstractVector& y);

      /** @return \f$x-y\f$ */
      Vector& operator-=(const Vector& y);

      /** @return \f$ax\f$ */
      Vector& operator*=(double a);

      /** @return \f$-x\f$ */
      Vector operator- () const;

      /// Equality comparison (possibly up to the maximal attainable accuracy).
      bool operator==(const Vector& y) const;

      unsigned numberOfVariables() const;

      /**
       * @brief Access k-th variable.
       * @param k global variable index
       * @return associated vector \f$x_k\f$
       */
      ::Spacy::Vector& component(unsigned k);

      /**
       * @brief Access k-th variable.
       * @param k global variable index
       * @return associated vector \f$x_k\f$
       */
      const ::Spacy::Vector& component(unsigned k) const;

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
      std::vector< ::Spacy::Vector > components_ = {};
    };
    /** @} */
  }
}

#endif // SPACY_SPACES_PRODUCT_SPACE_VECTOR_HH
