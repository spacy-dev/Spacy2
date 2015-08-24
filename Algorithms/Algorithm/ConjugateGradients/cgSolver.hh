#ifndef ALGORITHM_CONJUGATE_GRADIENTS_CG_SOLVER_HH
#define ALGORITHM_CONJUGATE_GRADIENTS_CG_SOLVER_HH

#include "conjugateGradients.hh"
#include "operator.hh"

#include "Util/mixins.hh"
#include "Util/Exceptions/callOfUndefinedFunctionException.hh"
#include "Util/Base/operatorBase.hh"

namespace Algorithm
{
  class CGSolver :
      public OperatorBase ,
      public Mixin::AbsoluteAccuracy, public Mixin::RelativeAccuracy,
      public Mixin::Eps, public Mixin::Verbosity,
      public Mixin::IterativeRefinements , public Mixin::MaxSteps
  {
  public:
    CGSolver(Operator A_, CallableOperator P_, const std::string& type );

    CGSolver(const CGSolver& other);

    Vector operator()(const Vector& y) const;

    CGMethod& impl();

    bool isPositiveDefinite() const;

    const CallableOperator& P() const;

    const CallableOperator& A() const;

  private:
    mutable CGMethod cg;
  };

  template <class Op1, class Op2,
            class = std::enable_if_t<std::is_base_of<Operator,std::decay_t<Op1> >::value>,
            class = std::enable_if_t<std::is_base_of<Operator,std::decay_t<Op2> >::value> >
  auto makeCGSolver(Op1&& A, Op2&& P, double relativeAccuracy = 1e-15, double eps = 1e-15, bool verbose = false)
  {
    auto solver = CGSolver(std::forward<Op1>(A), std::forward<Op2>(P), "CG" );
    solver.setRelativeAccuracy(relativeAccuracy);
    solver.setEps(eps);
    solver.setVerbosity(verbose);
    return solver;
  }

  template <class Op1, class Op2,
            class = std::enable_if_t<std::is_base_of<Operator,std::decay_t<Op1> >::value>,
            class = std::enable_if_t<std::is_base_of<Operator,std::decay_t<Op2> >::value> >
  auto makeRCGSolver(Op1&& A, Op2&& P, double relativeAccuracy = 1e-15, double eps = 1e-15, bool verbose = false)
  {
    auto solver = CGSolver(std::forward<Op1>(A), std::forward<Op2>(P), "RCG");
    solver.setRelativeAccuracy(relativeAccuracy);
    solver.setEps(eps);
    solver.setVerbosity(verbose);
    return solver;
  }

  template <class Op1, class Op2,
            class = std::enable_if_t<std::is_base_of<Operator,std::decay_t<Op1> >::value>,
            class = std::enable_if_t<std::is_base_of<Operator,std::decay_t<Op2> >::value> >
  auto makeTCGSolver(Op1&& A, Op2&& P, double relativeAccuracy = 1e-15, double eps = 1e-15, bool verbose = false)
  {
    auto solver = CGSolver(std::forward<Op1>(A), std::forward<Op2>(P), "TCG");
    solver.setRelativeAccuracy(relativeAccuracy);
    solver.setEps(eps);
    solver.setVerbosity(verbose);
    return solver;
  }


  CGSolver makeTRCGSolver(Operator A, CallableOperator P, double relativeAccuracy = 1e-15, double eps = 1e-15, bool verbose = false);
}

#endif // ALGORITHM_CONJUGATE_GRADIENTS_CG_SOLVER_HH
