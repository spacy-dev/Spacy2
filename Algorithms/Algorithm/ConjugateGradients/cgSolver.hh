#ifndef ALGORITHM_CONJUGATE_GRADIENTS_CG_SOLVER_HH
#define ALGORITHM_CONJUGATE_GRADIENTS_CG_SOLVER_HH

#include <memory>
#include <utility>

#include "conjugateGradients.hh"
#include "operator.hh"

#include "Interface/abstractVector.hh"
#include "Interface/abstractLinearSolver.hh"
#include "Util/mixins.hh"
#include "Util/Exceptions/callOfUndefinedFunctionException.hh"

namespace Algorithm
{
  class Operator;

  class CGSolver :
      public Interface::AbstractLinearSolver,
      public Mixin::AbsoluteAccuracy, public Mixin::RelativeAccuracy,
      public Mixin::Eps, public Mixin::Verbosity,
      public Mixin::IterativeRefinements
  {
  public:
    template <class Op1, class Op2,
              class = std::enable_if_t<std::is_base_of<Operator,std::decay_t<Op1> >::value>,
              class = std::enable_if_t<std::is_base_of<Operator,std::decay_t<Op2> >::value> >
    CGSolver(Op1&& A, Op2&& P, const std::string& type )
      : AbstractLinearSolver(A.impl().sharedRange(),A.impl().sharedDomain()),
        cg(std::forward<Op1>(A),std::forward<Op2>(P),type)
    {
      attachEps(&cg);
      attachAbsoluteAccuracy(&cg.terminationCriterion());
      attachRelativeAccuracy(&cg.terminationCriterion());
      attachVerbosity(&cg);
      attachIterativeRefinements(&cg);
    }

    CGSolver(const CGSolver& other);

    std::unique_ptr<Interface::AbstractVector> operator()(const Interface::AbstractVector& y) const override;

    CGMethod& impl();

    bool systemIsPositiveDefinite() const override;

    const Operator& preconditioner() const;

  private:
    CGSolver* cloneImpl() const;

    mutable CGMethod cg;
  };

  template <class Op1, class Op2,
            class = std::enable_if_t<std::is_base_of<Operator,std::decay_t<Op1> >::value>,
            class = std::enable_if_t<std::is_base_of<Operator,std::decay_t<Op2> >::value> >
  auto makeCGSolver(Op1&& A, Op2&& P, double relativeAccuracy = 1e-15, double eps = 1e-15, bool verbose = false)
  {
    auto solver = std::make_unique<CGSolver>(std::forward<Op1>(A), std::forward<Op2>(P), "cg" );
    solver->setRelativeAccuracy(relativeAccuracy);
    solver->setEps(eps);
    solver->setVerbosity(verbose);
    return std::move(solver);
  }

  template <class Op1, class Op2,
            class = std::enable_if_t<std::is_base_of<Operator,std::decay_t<Op1> >::value>,
            class = std::enable_if_t<std::is_base_of<Operator,std::decay_t<Op2> >::value> >
  auto makeRCGSolver(Op1&& A, Op2&& P, double relativeAccuracy = 1e-15, double eps = 1e-15, bool verbose = false)
  {
    auto solver = std::make_unique<CGSolver>(std::forward<Op1>(A), std::forward<Op2>(P), "rcg");
    solver->setRelativeAccuracy(relativeAccuracy);
    solver->setEps(eps);
    solver->setVerbosity(verbose);
    return std::move(solver);
  }

  template <class Op1, class Op2,
            class = std::enable_if_t<std::is_base_of<Operator,std::decay_t<Op1> >::value>,
            class = std::enable_if_t<std::is_base_of<Operator,std::decay_t<Op2> >::value> >
  auto makeTCGSolver(Op1&& A, Op2&& P, double relativeAccuracy = 1e-15, double eps = 1e-15, bool verbose = false)
  {
    auto solver = std::make_unique<CGSolver>(std::forward<Op1>(A), std::forward<Op2>(P), "tcg");
    solver->setRelativeAccuracy(relativeAccuracy);
    solver->setEps(eps);
    solver->setVerbosity(verbose);
    return std::move(solver);
  }

  template <class Op1, class Op2,
            class = std::enable_if_t<std::is_base_of<Operator,std::decay_t<Op1> >::value>,
            class = std::enable_if_t<std::is_base_of<Operator,std::decay_t<Op2> >::value> >
  auto makeTRCGSolver(Op1&& A, Op2&& P, double relativeAccuracy = 1e-15, double eps = 1e-15, bool verbose = false)
  {
    auto solver = std::make_unique<CGSolver>(std::forward<Op1>(A), std::forward<Op2>(P), "trcg");
    solver->setRelativeAccuracy(relativeAccuracy);
    solver->setEps(eps);
    solver->setVerbosity(verbose);
    return std::move(solver);
  }
}

#endif // ALGORITHM_CONJUGATE_GRADIENTS_CG_SOLVER_HH
