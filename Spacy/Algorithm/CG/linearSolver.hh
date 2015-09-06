#ifndef SPACY_CONJUGATE_GRADIENTS_CG_SOLVER_HH
#define SPACY_CONJUGATE_GRADIENTS_CG_SOLVER_HH

#include "Spacy/operator.hh"
#include "cg.hh"

#include "Spacy/Util/mixins.hh"
#include "Spacy/Util/Exceptions/callOfUndefinedFunctionException.hh"
#include "Spacy/Util/Base/operatorBase.hh"

namespace Spacy
{
  namespace CG
  {
    /**
     * @ingroup CGGroup
     * @brief Conjugate gradient solver satisfying the \ref IndefiniteLinearSolverConceptAnchor "IndefiniteLinearSolverConcept"
     * @see \ref IndefiniteLinearSolverConceptAnchor "IndefiniteLinearSolverConcept", Solver
     */
    class LinearSolver :
        public OperatorBase ,
        public Mixin::AbsoluteAccuracy,
        public Mixin::RelativeAccuracy,
        public Mixin::Eps,
        public Mixin::IterativeRefinements ,
        public Mixin::MaxSteps,
        public Mixin::Verbosity
    {
    public:
      /**
       * @brief Set up conjugate gradient solver
       * @param A_ operator
       * @param P_ preconditioner
       * @param type solver type, i.e. "CG", "TCG", "RCG" or "TRCG"
       */
      LinearSolver(Operator A_, CallableOperator P_, const std::string& type );

      /**
       * @brief Copy constructor.
       * @param other solver to copy from
       */
      LinearSolver(const LinearSolver& other);

      /**
       * @brief Apply conjugate gradient solver.
       * @param y right hand side \f$y\f$.
       */
      Vector operator()(const Vector& y) const;

      /**
       * @brief Access conjugate gradient implementation.
       */
      Solver& impl();

      /**
       * @brief Checks positive definiteness of \f$A\f$.
       * @return true if \f$A\f$ is positive definite, else false
       */
      bool isPositiveDefinite() const;

      /**
       * @brief Access preconditioner.
       * @return preconditioner \f$P\f$
       */
      const CallableOperator& P() const;

      /**
       * @brief Access operator.
       * @return operator \f$A\f$
       */
      const CallableOperator& A() const;

    private:
      mutable Solver cg;
    };
  }

  /**
   * @ingroup CGGroup
   * @brief Construct conjugate gradient method.
   * @param A operator
   * @param P preconditioner
   * @param relativeAccuracy relative accuracy
   * @param eps maximal attainable accuracy
   * @param verbose verbosity
   * @return CGSolver(A,P,"CG")
   */
  CG::LinearSolver makeCGSolver(Operator A, CallableOperator P, double relativeAccuracy = 1e-15, double eps = 1e-15, bool verbose = false);

  /**
   * @ingroup CGGroup
   * @brief Construct regularized conjugate gradient method.
   * @param A operator
   * @param P preconditioner
   * @param relativeAccuracy relative accuracy
   * @param eps maximal attainable accuracy
   * @param verbose verbosity
   * @return CGSolver(A,P,"RCG")
   */
  CG::LinearSolver makeRCGSolver(Operator A, CallableOperator P, double relativeAccuracy = 1e-15, double eps = 1e-15, bool verbose = false);

  /**
   * @ingroup CGGroup
   * @brief Construct truncated conjugate gradient method.
   * @param A operator
   * @param P preconditioner
   * @param relativeAccuracy relative accuracy
   * @param eps maximal attainable accuracy
   * @param verbose verbosity
   * @return CGSolver(A,P,"TCG")
   */
  CG::LinearSolver makeTCGSolver(Operator A, CallableOperator P, double relativeAccuracy = 1e-15, double eps = 1e-15, bool verbose = false);

  /**
   * @ingroup CGGroup
   * @brief Construct truncated regularized conjugate gradient method.
   * @param A operator
   * @param P preconditioner
   * @param relativeAccuracy relative accuracy
   * @param eps maximal attainable accuracy
   * @param verbose verbosity
   * @return CGSolver(A,P,"TRCG")
   */
  CG::LinearSolver makeTRCGSolver(Operator A, CallableOperator P, double relativeAccuracy = 1e-15, double eps = 1e-15, bool verbose = false);
}

#endif // SPACY_CONJUGATE_GRADIENTS_CG_SOLVER_HH
