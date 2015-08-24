#include "cgSolver.hh"

#include <utility>

namespace Algorithm
{
  CGSolver::CGSolver(Operator A_, CallableOperator P_, const std::string& type )
    : OperatorBase(A_.range_ptr(),A_.domain_ptr()),
      cg(std::move(A_),std::move(P_),type)
  {
    attachEps(&cg);
    attachAbsoluteAccuracy(&cg.terminationCriterion());
    attachRelativeAccuracy(&cg.terminationCriterion());
    attachVerbosity(&cg);
    attachIterativeRefinements(&cg);
    attachMaxSteps(&cg);
  }

  CGSolver::CGSolver(const CGSolver& other) :
    OperatorBase(other.domain_ptr(),other.range_ptr()),
    Mixin::AbsoluteAccuracy(other.absoluteAccuracy()),
    Mixin::RelativeAccuracy(other.relativeAccuracy()),
    Mixin::Eps(other.eps()),
    Mixin::Verbosity(other.verbose()),
    cg(other.cg)
  {
    attachEps(&cg);
    attachAbsoluteAccuracy(&cg.terminationCriterion());
    attachRelativeAccuracy(&cg.terminationCriterion());
    attachVerbosity(&cg);
    attachIterativeRefinements(&cg);
    setDetailedVerbosity(other.verbose_detailed());
  }

  Vector CGSolver::operator()(const Vector& y) const
  {
    return cg.solve(range().element(),y);
  }

  CGMethod& CGSolver::impl()
  {
    return cg;
  }

  bool CGSolver::isPositiveDefinite() const
  {
    return !cg.encounteredNonConvexity();
  }

  const CallableOperator& CGSolver::P() const
  {
    return cg.P();
  }

  const CallableOperator& CGSolver::A() const
  {
    return cg.A();
  }

  CGSolver makeTRCGSolver(Operator A, CallableOperator P, double relativeAccuracy, double eps, bool verbose)
  {
    auto solver = CGSolver(std::move(A), std::move(P), "TRCG");
    solver.setRelativeAccuracy(relativeAccuracy);
    solver.setEps(eps);
    solver.setVerbosity(verbose);
    return solver;
  }

}
