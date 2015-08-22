#include "cgSolver.hh"

namespace Algorithm
{
  CGSolver::CGSolver(const CGSolver& other) :
    AbstractLinearSolver(other.domain_ptr(),other.range_ptr()),
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
    return cg.solve(y);
  }

  CGMethod& CGSolver::impl()
  {
    return cg;
  }

  bool CGSolver::systemIsPositiveDefinite() const
  {
    return !cg.encounteredNonConvexity();
  }

  const Operator& CGSolver::P() const
  {
    return cg.P();
  }

  const Operator& CGSolver::A() const
  {
    return cg.A();
  }

  CGSolver* CGSolver::cloneImpl() const
  {
    return new CGSolver(*this);
  }
}
