#ifndef ALGORITHM_ALGORITHM_NEWTON_HH
#define ALGORITHM_ALGORITHM_NEWTON_HH

#include <functional>

#include "norm.hh"

#include "newtonParameter.hh"
#include "dampingStrategies.hh"

namespace Algorithm
{
  class FunctionSpaceElement;
  class C1Operator;
  class LinearSolver;


  class Newton : public NewtonParameter
  {
  public:
    explicit Newton(const C1Operator& F);

    FunctionSpaceElement solve() const;

    FunctionSpaceElement solve(const FunctionSpaceElement& x0) const;

    template <class DampingStrategy>
    void setDampingStrategy()
    {
      dampingFactor_ = DampingStrategy(*this,F_,norm_);
    }

  private:
    const C1Operator& F_;
    std::function<double(const LinearSolver&,const FunctionSpaceElement&,const FunctionSpaceElement&)> dampingFactor_;
    mutable Norm norm_;
  };

  Newton localNewton(const C1Operator& F);

  Newton affineCovariantNewton(const C1Operator& F);

  Newton affineContravariantNewton(const C1Operator& F);
}
#endif // ALGORITHM_ALGORITHM_NEWTON_HH
