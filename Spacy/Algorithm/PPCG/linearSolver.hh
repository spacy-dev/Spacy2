/**********************************************************
 *  Author: Jan Zetzmann
 *
 *********************************************************/

#ifndef SPACY_PROJECTED_CONJUGATE_GRADIENTS_CG_SOLVER_HH
#define SPACY_PROJECTED_CONJUGATE_GRADIENTS_CG_SOLVER_HH

#include "linearSolver.hh"
#include "ppcg.hh"

#include "Spacy/Util/mixins.hh"
#include "Spacy/Util/Base/OperatorBase.hh"

namespace Spacy
{

    // "Preconditioner for solving the preconditioner problem" - in ppcg.hh
    // namespace
    // {
    //     auto Identity_Preconditioner = []( const Vector& x_loc ) -> Vector { return x_loc; };
    // }
    /** @addtogroup PPCGGroup @{ */
    namespace PPCG
    {
        /// Projected preconditioned conjugate gradient solver
        class LinearSolver :
            public OperatorBase,
            public Mixin::AbsoluteAccuracy,
            public Mixin::RelativeAccuracy,
            public Mixin::Eps,
            public Mixin::IterativeRefinements,
            public Mixin::MaxSteps,
            public Mixin::Verbosity
        {
        public:
            /**
            * @brief Set up projected preconditioned conjugate gradient solver
            * @param A_ operator
            * @param P_ preconditioner
            * @param type solver type, i.e. "PPCG", "TCG", "RCG" or "TRCG"
            */
            LinearSolver(Operator A_, CallableOperator P_,
                MyOperator proj, MyOperator r_update, MyOperator lagr_update, 
                const std::string& type="PPCG" );
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

            /// Access operator \f$H\f
            const MyOperator& r_update();

        private:
            mutable Solver ppcg;
        };
    }

    PPCG::LinearSolver makePPCGSolver( Operator A, CallableOperator P, MyOperator proj_op, 
                                        MyOperator update_r_op, MyOperator lagr_update,
                                        Real relativeAccuracy = 1e-15, Real eps = 1e-15, bool verbose = false );
}

#endif // SPACY_PROJECTED_CONJUGATE_GRADIENTS_CG_SOLVER_HH