#ifndef SPACY_PRIMAL_DUAL_PRODUCT_SPACE_VECTOR_HH
#define SPACY_PRIMAL_DUAL_PRODUCT_SPACE_VECTOR_HH

#include <memory>
#include <vector>

#include "Spacy/vector.hh"
#include "Spacy/Util/Base/vectorBase.hh"

namespace Spacy
{
  /// @cond
  class Real;
  /// @endcond

  namespace PrimalDualProductSpace
  {
    /// @cond
    class VectorCreator;
    /// @endcond

    /**
     * @ingroup ProductSpaceGroup
     * @brief Product space vector.
     *
     * Represents a vector \f$x=(x_0,x_1,\ldots,x_n)\f$ of a product space \f$X = \{X_0,X_1,\ldots,X_n\}\f$. This structure may be further
     * restricted by specifying if variables act as "primal" or "dual" variables. Then we get
     * \f$ x = (x_p,x_d) \in X=\{ X_p, X_d \}\f$, where \f$X_p=\{X_i\}_i,\ i\in I \f$ is associated to the primal variables and \f$X_d=\{X_j\}_j,\ j\in J\f$
     * is associated to the dual variables.
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
       * Calling primal(y), resp. dual(y), before summation yields
       * a restriction of the summation operation to primal, resp dual, variables.
       *
       * @param y vector to add
       * @return x+y
       */
      Vector& operator+=(const Vector& y);

  //    /// Axpy-operation \f$x = x + ay\f$.
      //    AbstractVector& axpy(double a, const AbstractVector& y);

      /**
       * @brief In-place subtraction.
       *
       * Calling primal(y), resp. dual(y), before subtraction yields
       * a restriction of the subtraction operation to primal, resp dual, variables.
       *
       * @param y vector to subtract
       * @return x-y
       */
      Vector& operator-=(const Vector& y);

      /**
       * @brief In-place multiplication.
       *
       * Calling primal(y), resp. dual(y), before multiplication yields
       * a restriction of the multiplication operation to primal, resp dual, variables.
       *
       * @param a scaling factor
       * @return ax
       */
      Vector& operator*=(double a);

      /**
       * @brief Negation.
       *
       * Calling primal(y), resp. dual(y), before multiplication yields
       * a restriction of the multiplication operation to primal, resp dual, variables.
       *
       * @return -x.
       */
      Vector operator- () const;

      /**
       * @brief Equality comparison.
       *
       * Calling primal(y), resp. dual(y), before comparison yields
       * a restriction of the comparison operation to primal, resp dual, variables.
       *
       * @return true, if x=y, else false
       */
      bool operator==(const Vector& y) const;

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
       * @brief Access primal variables as product space vector.
       * @return \f$x_p\f$
       */
      ::Spacy::Vector& primalComponent();

      /**
       * @brief Access primal variables as product space vector.
       * @return \f$x_p\f$
       */
      const ::Spacy::Vector& primalComponent() const;

      /**
       * @brief Set primal vector \f$x_p\f$.
       * @param y product space vector
       */
      void setPrimalComponent(::Spacy::Vector y);

      /**
       * @brief Access dual variables as product space vector.
       * @return \f$x_d\f$
       */
      ::Spacy::Vector& dualComponent();

      /**
       * @brief Access dual variables as product space vector.
       * @return \f$x_d\f$
       */
      const ::Spacy::Vector& dualComponent() const;

      /**
       * @brief Set dual variable \f$x_d\f$.
       * @param y product space vector.
       */
      void setDualComponent(::Spacy::Vector y);

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

#endif // SPACY_PRIMAL_DUAL_PRODUCT_SPACE_VECTOR_HH
