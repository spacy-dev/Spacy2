// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_CONJUGATE_GRADIENTS_CG_SOLVER_HH
#define SPACY_CONJUGATE_GRADIENTS_CG_SOLVER_HH

#include "cg.hh"

#include "Spacy/Util/mixins.hh"
#include "Spacy/Util/Base/operatorBase.hh"

namespace Spacy
{
  /** @addtogroup CGGroup @{ */
  namespace CG
  {
    /// Conjugate gradient solver satisfying the \ref IndefiniteLinearSolverConceptAnchor "IndefiniteLinearSolverConcept"
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

      LinearSolver(const LinearSolver& other);

      /// Apply conjugate gradient solver.
      Vector operator()(const Vector& y) const;

      /// Access conjugate gradient implementation.
      Solver& impl();

      /// Checks positive definiteness of \f$A\f$.
      bool isPositiveDefinite() const;

      /// Access preconditioner \f$P\f$.
      const CallableOperator& P() const;

      /// Access operator \f$A\f$.
      const CallableOperator& A() const;

    private:
      mutable Solver cg;
    };
  }

  /**
   * @brief Construct conjugate gradient method.
   * @param A operator
   * @param P preconditioner
   * @param relativeAccuracy relative accuracy
   * @param eps maximal attainable accuracy
   * @param verbose verbosity
   * @return CGSolver(A,P,"CG")
   */
  CG::LinearSolver makeCGSolver(Operator A, CallableOperator P, Real relativeAccuracy = 1e-15, Real eps = 1e-15, bool verbose = false);

  /**
   * @brief Construct regularized conjugate gradient method.
   * @param A operator
   * @param P preconditioner
   * @param relativeAccuracy relative accuracy
   * @param eps maximal attainable accuracy
   * @param verbose verbosity
   * @return CGSolver(A,P,"RCG")
   */
  CG::LinearSolver makeRCGSolver(Operator A, CallableOperator P, Real relativeAccuracy = 1e-15, Real eps = 1e-15, bool verbose = false);

  /**
   * @brief Construct truncated conjugate gradient method.
   * @param A operator
   * @param P preconditioner
   * @param relativeAccuracy relative accuracy
   * @param eps maximal attainable accuracy
   * @param verbose verbosity
   * @return CGSolver(A,P,"TCG")
   */
  CG::LinearSolver makeTCGSolver(Operator A, CallableOperator P, Real relativeAccuracy = 1e-15, Real eps = 1e-15, bool verbose = false);

  /**
   * @brief Construct truncated regularized conjugate gradient method.
   * @param A operator
   * @param P preconditioner
   * @param relativeAccuracy relative accuracy
   * @param eps maximal attainable accuracy
   * @param verbose verbosity
   * @return CGSolver(A,P,"TRCG")
   */
  CG::LinearSolver makeTRCGSolver(Operator A, CallableOperator P, Real relativeAccuracy = 1e-15, Real eps = 1e-15, bool verbose = false);

  /** @} */
}

#endif // SPACY_CONJUGATE_GRADIENTS_CG_SOLVER_HH
