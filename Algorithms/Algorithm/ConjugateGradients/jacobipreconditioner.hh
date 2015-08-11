#ifndef ALGORITHM_CONJUGATE_GRADIENTS_JACOBIPRECONDITIONER_HH
#define ALGORITHM_CONJUGATE_GRADIENTS_JACOBIPRECONDITIONER_HH

#include <memory>
#include <vector>

#include "Interface/Operator/abstractOperator.hh"
#include "operator.hh"
#include "linearOperator.hh"

namespace Algorithm
{
  class Vector;

  class JacobiPreconditioner : public Interface::AbstractOperator
  {
  public:
    JacobiPreconditioner(const Operator& A);

    JacobiPreconditioner(const LinearOperator& A);

    JacobiPreconditioner(const JacobiPreconditioner& other);

    std::unique_ptr<Interface::AbstractVector> operator()(const Interface::AbstractVector& x) const override;

  private:
    JacobiPreconditioner* cloneImpl() const;

    std::vector<double> diag_;
    std::unique_ptr<Interface::AbstractVector> x_ = nullptr;
  };

  Operator jacobiPreconditioner(const Operator& A);

  Operator jacobiPreconditioner(const LinearOperator& A);
}
#endif // JACOBIPRECONDITIONER_HH
