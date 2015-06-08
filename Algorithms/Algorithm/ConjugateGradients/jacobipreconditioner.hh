#ifndef ALGORITHM_CONJUGATE_GRADIENTS_JACOBIPRECONDITIONER_HH
#define ALGORITHM_CONJUGATE_GRADIENTS_JACOBIPRECONDITIONER_HH

#include <memory>
#include <vector>

#include "Interface/Operator/abstractOperator.hh"
#include "functionSpace.hh"
#include "operator.hh"
#include "linearOperator.hh"

namespace Algorithm
{
  class FunctionSpaceElement;

  class JacobiPreconditioner : public AbstractOperator
  {
  public:
    JacobiPreconditioner(const Operator& A);

    JacobiPreconditioner(const LinearOperator& A);

    JacobiPreconditioner(const JacobiPreconditioner& other);

    void setArgument(const AbstractFunctionSpaceElement& x);

//    std::unique_ptr<AbstractFunctionSpaceElement> d0() const override;

    std::unique_ptr<AbstractFunctionSpaceElement> operator()(const AbstractFunctionSpaceElement& x) const override;

  private:
    JacobiPreconditioner* cloneImpl() const;

    std::vector<double> diag_;
    std::unique_ptr<AbstractFunctionSpaceElement> x_ = nullptr;
  };

  Operator jacobiPreconditioner(const Operator& A);

  Operator jacobiPreconditioner(const LinearOperator& A);
}
#endif // JACOBIPRECONDITIONER_HH
