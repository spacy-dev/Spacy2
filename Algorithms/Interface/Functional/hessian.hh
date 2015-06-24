#ifndef ALGORITHM_INTERFACE_HESSIAN_HH
#define ALGORITHM_INTERFACE_HESSIAN_HH

#include <memory>

#include "Interface/Operator/abstractLinearOperator.hh"

namespace Algorithm
{
  class AbstractC2Functional;
  class AbstractFunctionSpaceElement;

  class Hessian : public AbstractLinearOperator
  {
  public:
    Hessian(const AbstractC2Functional& A, const AbstractFunctionSpaceElement& x);

    Hessian(const AbstractC2Functional& A, const AbstractFunctionSpaceElement& x, std::shared_ptr<AbstractLinearSolver> solver);

    std::unique_ptr<AbstractFunctionSpaceElement> operator ()(const AbstractFunctionSpaceElement& dx) const final override;

    std::shared_ptr<AbstractLinearSolver> getSolver() const final override;

    void setSolver(std::shared_ptr<AbstractLinearSolver> solver);

  private:
    Hessian* cloneImpl() const;

    const AbstractC2Functional& A_;
    const AbstractFunctionSpaceElement& x_;
    std::shared_ptr<AbstractLinearSolver> solver_ = nullptr;
  };
}

#endif // ALGORITHM_INTERFACE_HESSIAN_HH
