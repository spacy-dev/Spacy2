#ifndef ALGORITHM_INTERFACE_ABSTRACT_OPERATOR_HH
#define ALGORITHM_INTERFACE_ABSTRACT_OPERATOR_HH

#include <memory>

#include "Util/Mixins/cloneable.hh"

namespace Algorithm
{
  /// \cond
  class VectorSpace;
  /// \endcond
  namespace Interface
  {
    /// \cond
    class AbstractVector;
    class AbstractLinearSolver;
    class LinearizedOperator;
    /// \endcond

    /**
     * @brief Abstract interface for operators \f$A:\ X\rightarrow Y\f$, between function spaces \f$X\f$ and \f$Y\f$.
     */
    class AbstractOperator : public Mixin::Cloneable<AbstractOperator>
    {
    public:
      using Base = AbstractOperator;

      /**
       * @brief Constructor.
       * @param domain domain space \f$X\f$.
       * @param range range space \f$Y\f$.
       */
      AbstractOperator(VectorSpace* domain, VectorSpace* range);

      virtual ~AbstractOperator();

      /// Apply operator, i.e. compute \f$y=Ax\f$.
      virtual std::unique_ptr<AbstractVector> operator()(const AbstractVector&) const = 0;

      /// Apply first derivative of operator, i.e. compute \f$y=A'(x)dx\f$.
      virtual std::unique_ptr<AbstractVector> d1(const AbstractVector& x,
                                                               const AbstractVector& dx) const;

      /// Apply second derivative of operator, i.e. compute \f$y=A''(x)(dx,dy)\f$.
      virtual std::unique_ptr<AbstractVector> d2(const AbstractVector& x,
                                                               const AbstractVector& dx,
                                                               const AbstractVector& dy) const;

      /// Get linearization \f$A'(x): X \rightarrow Y\f$.
      std::unique_ptr<LinearizedOperator> linearization(const AbstractVector& x) const;

      /// Access domain space \f$X\f$.
      VectorSpace& domain();

      /// Access domain space \f$X\f$.
      const VectorSpace& domain() const;

      /// Access shared domain space \f$X\f$.
      VectorSpace* domain_ptr() const;

      /// Access range space \f$Y\f$.
      VectorSpace& range();

      /// Access range space \f$Y\f$.
      const VectorSpace& range() const;

      /// Access shared range space \f$Y\f$.
      VectorSpace* range_ptr() const;

    protected:
      friend class LinearizedOperator;

      virtual std::unique_ptr<LinearizedOperator> makeLinearization(const AbstractVector& x) const;

      virtual std::unique_ptr<AbstractLinearSolver> makeSolver() const;

      virtual std::unique_ptr<AbstractLinearSolver> makeAdjointSolver() const;

    private:
      VectorSpace *domain_, *range_;
    };
  }
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_OPERATOR_HH
