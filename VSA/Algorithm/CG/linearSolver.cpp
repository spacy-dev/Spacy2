#include "linearSolver.hh"

#include <utility>

namespace VSA
{
  namespace CG
  {
    LinearSolver::LinearSolver(Operator A_, CallableOperator P_, const std::string& type )
      : OperatorBase(A_.range(),A_.domain()),
        cg(std::move(A_),std::move(P_),type)
    {
      attachEps(cg);
      attachAbsoluteAccuracy(cg);
      attachRelativeAccuracy(cg);
      attachVerbosity(cg);
      attachIterativeRefinements(cg);
      attachMaxSteps(cg);
    }

    LinearSolver::LinearSolver(const LinearSolver& other) :
      OperatorBase(other),
      Mixin::AbsoluteAccuracy(other.absoluteAccuracy()),
      Mixin::RelativeAccuracy(other.relativeAccuracy()),
      Mixin::Eps(other.eps()),
      Mixin::Verbosity(other.verbose()),
      cg(other.cg)
    {
      attachEps(cg);
      attachAbsoluteAccuracy(cg);
      attachRelativeAccuracy(cg);
      attachVerbosity(cg);
      attachIterativeRefinements(cg);
      setVerbosityLevel(other.verbosityLevel());
    }

    Vector LinearSolver::operator()(const Vector& y) const
    {
      return cg.solve(range().vector(),y);
    }

    Solver& LinearSolver::impl()
    {
      return cg;
    }

    bool LinearSolver::isPositiveDefinite() const
    {
      return !cg.indefiniteOperator();
    }

    const CallableOperator& LinearSolver::P() const
    {
      return cg.P();
    }

    const CallableOperator& LinearSolver::A() const
    {
      return cg.A();
    }
  }


  CG::LinearSolver makeCGSolver(Operator A, CallableOperator P, double relativeAccuracy, double eps, bool verbose)
  {
    auto solver = CG::LinearSolver(std::move(A), std::move(P), "CG" );
    solver.setRelativeAccuracy(relativeAccuracy);
    solver.setEps(eps);
    solver.setVerbosity(verbose);
    return std::move(solver);
  }

  CG::LinearSolver makeRCGSolver(Operator A, CallableOperator P, double relativeAccuracy, double eps, bool verbose)
  {
    auto solver = CG::LinearSolver(std::move(A), std::move(P), "RCG");
    solver.setRelativeAccuracy(relativeAccuracy);
    solver.setEps(eps);
    solver.setVerbosity(verbose);
    return std::move(solver);
  }

  CG::LinearSolver makeTCGSolver(Operator A, CallableOperator P, double relativeAccuracy, double eps, bool verbose)
  {
    auto solver = CG::LinearSolver(std::move(A), std::move(P), "TCG");
    solver.setRelativeAccuracy(relativeAccuracy);
    solver.setEps(eps);
    solver.setVerbosity(verbose);
    return std::move(solver);
  }

  CG::LinearSolver makeTRCGSolver(Operator A, CallableOperator P, double relativeAccuracy, double eps, bool verbose)
  {
    auto solver = CG::LinearSolver(std::move(A), std::move(P), "TRCG");
    solver.setRelativeAccuracy(relativeAccuracy);
    solver.setEps(eps);
    solver.setVerbosity(verbose);
    return std::move(solver);
  }

}
