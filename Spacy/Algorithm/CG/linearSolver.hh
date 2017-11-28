#pragma once

#include "cg.hh"

#include "Spacy/Util/Base/OperatorBase.hh"
#include "Spacy/Util/mixins.hh"
#include <Spacy/Algorithm/CG/Regularization.hh>

namespace Spacy
{
    /** @addtogroup CGGroup @{ */
    namespace CG
    {
        /// Conjugate gradient solver.
        class LinearSolver : public OperatorBase,
                             public Mixin::AbsoluteAccuracy,
                             public Mixin::RelativeAccuracy,
                             public Mixin::Eps,
                             public Mixin::IterativeRefinements,
                             public Mixin::MaxSteps,
                             public Mixin::Verbosity
        {
        public:
            /**
             * @brief Set up conjugate gradient solver
             * @param A_ operator
             * @param P_ preconditioner
             * @param regularization regularization for the treatment of nonconvexities
             * @param truncated set to true if the algorithm should terminate on nonconvexities
             */
            LinearSolver( Operator A_, CallableOperator P_, bool truncated,
                          Regularization regularization );

            LinearSolver( const LinearSolver& other );

            /// Apply conjugate gradient solver.
            Vector operator()( const Vector& y ) const;

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
     * @return CGSolver(A,P)
     */
    CG::LinearSolver makeCGSolver( Operator A, CallableOperator P, Real relativeAccuracy = 1e-15,
                                   Real eps = 1e-15, bool verbose = false );

    /**
     * @brief Construct regularized conjugate gradient method.
     * @param A operator
     * @param P preconditioner
     * @param relativeAccuracy relative accuracy
     * @param eps maximal attainable accuracy
     * @param verbose verbosity
     * @return CGSolver(A,P,false,RegularizeViaPreconditioner())
     */
    CG::LinearSolver makeRCGSolver( Operator A, CallableOperator P, Real relativeAccuracy = 1e-15,
                                    Real eps = 1e-15, bool verbose = false );

    /**
     * @brief Construct truncated conjugate gradient method.
     * @param A operator
     * @param P preconditioner
     * @param relativeAccuracy relative accuracy
     * @param eps maximal attainable accuracy
     * @param verbose verbosity
     * @return CGSolver(A,P,true)
     */
    CG::LinearSolver makeTCGSolver( Operator A, CallableOperator P, Real relativeAccuracy = 1e-15,
                                    Real eps = 1e-15, bool verbose = false );

    /**
     * @brief Construct truncated regularized conjugate gradient method.
     * @param A operator
     * @param P preconditioner
     * @param relativeAccuracy relative accuracy
     * @param eps maximal attainable accuracy
     * @param verbose verbosity
     * @return CGSolver(A,P,true,RegularizeViaPreconditioner())
     */
    CG::LinearSolver makeTRCGSolver( Operator A, CallableOperator P, Real relativeAccuracy = 1e-15,
                                     Real eps = 1e-15, bool verbose = false );

    /** @} */
}
