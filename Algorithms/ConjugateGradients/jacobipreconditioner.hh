#ifndef ALGORITHM_CONJUGATE_GRADIENTS_JACOBIPRECONDITIONER_HH
#define ALGORITHM_CONJUGATE_GRADIENTS_JACOBIPRECONDITIONER_HH

#include <vector>

#include "../operator.hh"

namespace Algorithm
{
  class FunctionSpaceElement;

  class JacobiPreconditioner : public Operator
  {
  public:
    JacobiPreconditioner(const Operator& A);

    void update(const FunctionSpaceElement& x);

    FunctionSpaceElement operator()(const FunctionSpaceElement& x) const;

    FunctionSpaceElement d1(const FunctionSpaceElement& dx) const;

  private:
    std::vector<double> diag;
  };
}
#endif // JACOBIPRECONDITIONER_HH
