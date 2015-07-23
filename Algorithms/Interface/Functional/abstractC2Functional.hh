#ifndef ALGORITHM_INTERFACE_ABSTRACT_C2_FUNCTIONAL_HH
#define ALGORITHM_INTERFACE_ABSTRACT_C2_FUNCTIONAL_HH

#include "abstractC1Functional.hh"

namespace Algorithm
{
  namespace Interface
  {
    class AbstractLinearOperator;
    class Hessian;

    class AbstractC2Functional : public AbstractC1Functional
    {
    public:
      AbstractC2Functional(std::shared_ptr<AbstractBanachSpace> domain);

      Hessian getHessian(const AbstractFunctionSpaceElement& x) const;

      virtual std::unique_ptr<AbstractFunctionSpaceElement> d2(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& dx) const = 0;

      virtual double d2(const AbstractFunctionSpaceElement &x, const AbstractFunctionSpaceElement &dx, const AbstractFunctionSpaceElement& dy) const;

    protected:
      friend class Hessian;

      virtual Hessian makeHessian(const AbstractFunctionSpaceElement& x) const;

      mutable std::shared_ptr<AbstractLinearSolver> solver_;
    };
  }
}

#endif // ALGORITHM_INTERFACE_ABSTRACT_C2_FUNCTIONAL_HH
