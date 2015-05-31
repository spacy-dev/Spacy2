#ifndef ALGORITHM_CONJUGATE_GRADIENTS_JACOBIPRECONDITIONER_HH
#define ALGORITHM_CONJUGATE_GRADIENTS_JACOBIPRECONDITIONER_HH

#include <memory>
#include <vector>

#include "../Interface/abstractC0Operator.hh"
#include "../functionSpace.hh"
#include "../c0Operator.hh"

namespace Algorithm
{
  class FunctionSpaceElement;

  class JacobiPreconditioner : public AbstractC0Operator
  {
  public:
    JacobiPreconditioner(const C0Operator& A);

    std::unique_ptr<AbstractC0Operator> clone() const;

    std::unique_ptr<AbstractFunctionSpaceElement> operator()(const AbstractFunctionSpaceElement& x) const override;

  private:
    std::vector<double> diag_;
  };

  C0Operator jacobi(const C0Operator& A)
  {
    return C0Operator(std::make_shared<JacobiPreconditioner>(A));
  }
}
#endif // JACOBIPRECONDITIONER_HH
