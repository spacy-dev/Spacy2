#include "cgSolver.hh"

namespace Algorithm
{
  CGSolver::CGSolver(const CGSolver& other) :
    AbstractLinearSolver(other.sharedDomain(),other.sharedRange()),
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

  std::unique_ptr<Interface::AbstractVector> CGSolver::operator()(const Interface::AbstractVector& y) const
  {
    return clone( cg.solve( Vector( clone(y) ) ).impl() );
  }

  CGMethod& CGSolver::impl()
  {
    return cg;
  }

  bool CGSolver::systemIsPositiveDefinite() const
  {
    return !cg.encounteredNonConvexity();
  }

  const Operator& CGSolver::preconditioner() const
  {
    return cg.preconditioner();
  }

  CGSolver* CGSolver::cloneImpl() const
  {
    return new CGSolver(*this);
  }
}
