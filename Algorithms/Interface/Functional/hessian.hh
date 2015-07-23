#ifndef ALGORITHM_INTERFACE_HESSIAN_HH
#define ALGORITHM_INTERFACE_HESSIAN_HH

#include <memory>

#include "Interface/Functional/abstractC2Functional.hh"
#include "Interface/Operator/abstractLinearOperator.hh"
#include "Util/Mixins/impl.hh"

namespace Algorithm
{
  namespace Interface
  {
    class AbstractFunctionSpaceElement;

    class Hessian : public AbstractLinearOperator, public Mixin::UniqueImpl<AbstractC2Functional>
    {
    public:
      Hessian(std::unique_ptr<AbstractC2Functional>&& A, const AbstractFunctionSpaceElement& x);

      Hessian(std::unique_ptr<AbstractC2Functional>&& A, const AbstractFunctionSpaceElement& x, std::shared_ptr<AbstractLinearSolver> solver);

      std::unique_ptr<AbstractFunctionSpaceElement> operator ()(const AbstractFunctionSpaceElement& dx) const final override;

      std::shared_ptr<AbstractLinearSolver> getSolver() const final override;

      void setSolver(std::shared_ptr<AbstractLinearSolver> solver);

    private:
      Hessian* cloneImpl() const;

      std::unique_ptr<AbstractFunctionSpaceElement> x_;
      std::shared_ptr<AbstractLinearSolver> solver_ = nullptr;
    };
  }
}

#endif // ALGORITHM_INTERFACE_HESSIAN_HH
