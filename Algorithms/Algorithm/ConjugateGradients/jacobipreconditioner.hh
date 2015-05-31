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

    JacobiPreconditioner(const JacobiPreconditioner& other);

    std::unique_ptr<AbstractC0Operator> clone() const;

    void setArgument(const AbstractFunctionSpaceElement& x);

    std::unique_ptr<AbstractFunctionSpaceElement> d0() const override;

    std::unique_ptr<AbstractFunctionSpaceElement> operator()(const AbstractFunctionSpaceElement& x) const override;

  private:
    std::vector<double> diag_;
    std::unique_ptr<AbstractFunctionSpaceElement> x_ = nullptr;
  };

  C0Operator jacobiPreconditioner(const C0Operator& A);
}
#endif // JACOBIPRECONDITIONER_HH
