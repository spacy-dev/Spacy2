#ifndef ALGORITHM_CONJUGATE_GRADIENTS_CG_SOLVER_HH
#define ALGORITHM_CONJUGATE_GRADIENTS_CG_SOLVER_HH

#include "conjugateGradients.hh"
#include "operator.hh"

#include "Util/mixins.hh"
#include "Util/Exceptions/callOfUndefinedFunctionException.hh"
#include "Util/Base/operatorBase.hh"

namespace Algorithm
{
  /**
   * @ingroup CGGroup
   * @brief Conjugate gradient solver satisfying the GeneralLinearOperatorConcept
   * @see GeneralLinearOperatorConcept, CGMethod
   */
  class CGSolver :
      public OperatorBase ,
      public Mixin::AbsoluteAccuracy, public Mixin::RelativeAccuracy,
      public Mixin::Eps, public Mixin::Verbosity,
      public Mixin::IterativeRefinements , public Mixin::MaxSteps
  {
  public:
    /**
     * @brief Set up conjugate gradient solver
     * @param A_ operator
     * @param P_ preconditioner
     * @param type solver type, i.e. "CG", "TCG", "RCG" or "TRCG"
     */
    CGSolver(Operator A_, CallableOperator P_, const std::string& type );

    /// Copy constructor.
    CGSolver(const CGSolver& other);

    /// Apply conjugate gradient solver with right hand side \f$y\f$.
    Vector operator()(const Vector& y) const;

    /// Access conjugate gradient implementation.
    CGMethod& impl();

    /**
     * @brief Checks positive definiteness of \f$A\f$.
     * @return true if \f$A\f$ is positive definite, else false
     */
    bool isPositiveDefinite() const;

    /// Access preconditioner.
    const CallableOperator& P() const;

    /// Access operator.
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

  /**
   * @ingroup CGGroup
   * @brief makeTRCGSolver
   * @param A operator
   * @param P preconditioner
   * @param relativeAccuracy relative accuracy
   * @param eps maximal attainable accuracy
   * @param verbose verbosity
   * @return CGSolver(A,P,"TRCG")
   */
  CGSolver makeTRCGSolver(Operator A, CallableOperator P, double relativeAccuracy = 1e-15, double eps = 1e-15, bool verbose = false);
}

#endif // ALGORITHM_CONJUGATE_GRADIENTS_CG_SOLVER_HH
