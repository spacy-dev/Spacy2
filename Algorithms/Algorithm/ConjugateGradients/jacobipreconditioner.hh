#ifndef ALGORITHM_CONJUGATE_GRADIENTS_JACOBIPRECONDITIONER_HH
#define ALGORITHM_CONJUGATE_GRADIENTS_JACOBIPRECONDITIONER_HH

#include <vector>

#include "../Interface/abstractOperator.hh"
#include "../functionSpace.hh"
#include "../operator.hh"

namespace Algorithm
{
  class FunctionSpaceElement;

  class JacobiPreconditioner : public AbstractOperator
  {
  public:
    JacobiPreconditioner(const Operator& A);

    void setArgument(const FunctionSpaceElement& x) override;

    FunctionSpaceElement operator()() const override;

    const FunctionSpace& getDomain() const override;

    const FunctionSpace& getRange() const override;

  private:
    std::vector<double> diag_;
    const FunctionSpace& domain_;
    const FunctionSpace& range_;
    FunctionSpaceElement result_;
  };

  Operator jacobi(const Operator& A)
  {
    return Operator(std::make_shared<JacobiPreconditioner>(A));
  }
}
#endif // JACOBIPRECONDITIONER_HH
