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

    private:
      std::shared_ptr<AbstractFunctionSpace> domain_, range_;
    };
  }
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_OPERATOR_HH
