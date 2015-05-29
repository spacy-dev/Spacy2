#ifndef ALGORITHM_CONJUGATE_GRADIENTS_JACOBIPRECONDITIONER_HH
#define ALGORITHM_CONJUGATE_GRADIENTS_JACOBIPRECONDITIONER_HH

#include <memory>
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

    std::unique_ptr<AbstractOperator> clone() const;

    std::unique_ptr<AbstractFunctionSpaceElement> operator()(const AbstractFunctionSpaceElement& x) const override;

    const AbstractBanachSpace& getDomain() const override;

    const AbstractBanachSpace& getRange() const override;

  private:
    std::vector<double> diag_;
    const AbstractBanachSpace& domain_;
    const AbstractBanachSpace& range_;
  };

  Operator jacobi(const Operator& A)
  {
    return Operator(std::make_shared<JacobiPreconditioner>(A));
  }
}
#endif // JACOBIPRECONDITIONER_HH
