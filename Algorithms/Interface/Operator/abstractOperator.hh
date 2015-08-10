#ifndef ALGORITHM_INTERFACE_ABSTRACT_OPERATOR_HH
#define ALGORITHM_INTERFACE_ABSTRACT_OPERATOR_HH

#include <memory>

#include "Util/Mixins/cloneable.hh"

namespace Algorithm
{
  namespace Interface
  {
    /// \cond
    class AbstractFunctionSpace;
    class AbstractFunctionSpaceElement;
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
      AbstractOperator(std::shared_ptr<AbstractFunctionSpace> domain, std::shared_ptr<AbstractFunctionSpace> range);

      virtual ~AbstractOperator();

      /// Apply operator, i.e. compute \f$y=Ax\f$.
      virtual std::unique_ptr<AbstractFunctionSpaceElement> operator()(const AbstractFunctionSpaceElement&) const = 0;

      /// Get linearization \f$A'(x): X \rightarrow Y\f$.
      std::unique_ptr<LinearizedOperator> linearization(const AbstractFunctionSpaceElement& x) const;

      /// Apply first derivative of operator, i.e. compute \f$y=A'(x)dx\f$.
      virtual std::unique_ptr<AbstractFunctionSpaceElement> d1(const AbstractFunctionSpaceElement& x,
                                                               const AbstractFunctionSpaceElement& dx) const;

      /// Apply second derivative of operator, i.e. compute \f$y=A''(x)(dx,dy)\f$.
      virtual std::unique_ptr<AbstractFunctionSpaceElement> d2(const AbstractFunctionSpaceElement& x,
                                                               const AbstractFunctionSpaceElement& dx,
                                                               const AbstractFunctionSpaceElement& dy) const;

      AbstractFunctionSpace& domain();

      /// Access domain space \f$X\f$.
      const AbstractFunctionSpace& domain() const;

      /// Access shared domain space \f$X\f$.
      std::shared_ptr<AbstractFunctionSpace> sharedDomain() const;

      /// Access range space \f$Y\f$.
      AbstractFunctionSpace& range();

      /// Access range space \f$Y\f$.
      const AbstractFunctionSpace& range() const;

      /// Access shared range space \f$Y\f$.
      std::shared_ptr<AbstractFunctionSpace> sharedRange() const;

    protected:
      friend class LinearizedOperator;

      virtual std::unique_ptr<LinearizedOperator> makeLinearization(const AbstractFunctionSpaceElement& x) const;

      virtual std::unique_ptr<AbstractLinearSolver> makeSolver() const;

    private:
      std::shared_ptr<AbstractFunctionSpace> domain_, range_;
    };
  }
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_OPERATOR_HH
