#ifndef ALGORITHM_NEWTON_DAMPINGSTRATEGIES_HH
#define ALGORITHM_NEWTON_DAMPINGSTRATEGIES_HH

namespace Algorithm
{
  class C1Operator;
  class FunctionSpaceElement;
  class LinearSolver;
  class NewtonParameter;
  class Norm;

  namespace Newton_DampingStrategy
  {
    class AffineCovariant
    {
    public:
      AffineCovariant(const NewtonParameter& p, const C1Operator& F, const Norm& norm);

      double operator()(const LinearSolver& DFInv_, const FunctionSpaceElement& x, const FunctionSpaceElement& dx);

    private:
      const NewtonParameter& p_;
      const C1Operator& F_;
      const Norm& norm_;
    };

    class AffineContravariant
    {
    public:
      AffineContravariant(const NewtonParameter& p, const C1Operator& F, const Norm& norm);

      double operator()(const LinearSolver&, const FunctionSpaceElement& x, const FunctionSpaceElement& dx);

    private:
      const NewtonParameter& p_;
      const C1Operator& F_;
      const Norm& norm_;

      double muPrime = -1.;
      double norm_F_x_old = -1;
    };

    class Undamped
    {
    public:
      Undamped(const NewtonParameter&, const C1Operator& F, const Norm& norm);

      double operator()(const LinearSolver&, const FunctionSpaceElement&, const FunctionSpaceElement&);
    };
  }
}

#endif // ALGORITHM_NEWTON_DAMPINGSTRATEGIES_HH
