/**********************************************************
 *  Author: Jan Zetzmann
 *
 *********************************************************/

 #include "Spacy/Algorithm/PPCG/linearSolver.hh"
 
 #include <Spacy/operator.hh>
 #include <Spacy/vectorSpace.hh>
 #include <Spacy/zeroVectorCreator.hh>
 
 #include <utility>
 
 namespace Spacy
 {
   namespace PPCG
   {
        LinearSolver::LinearSolver(Operator A_, CallableOperator P_, 
                                    MyOperator proj, MyOperator r_update, MyOperator lagr_update, 
                                    const std::string& type) :
            OperatorBase(A_.range(),A_.domain()),
            // ppcg(std::move(A_),std::move(P_), std::move(proj), std::move(update))
            ppcg(A_,P_,proj,r_update, lagr_update)
            // ppcg(A_,P_,type)
        {
            using namespace Mixin;
            cast_and_attach<Eps>(*this,ppcg);
            cast_and_attach<AbsoluteAccuracy>(*this,ppcg);
            cast_and_attach<RelativeAccuracy>(*this,ppcg);
            cast_and_attach<Verbosity>(*this,ppcg);
            cast_and_attach<IterativeRefinements>(*this,ppcg);
            cast_and_attach<MaxSteps>(*this,ppcg);
        }

        LinearSolver::LinearSolver(const LinearSolver& other) :
            OperatorBase(other),
            Mixin::AbsoluteAccuracy(other.getAbsoluteAccuracy()),
            Mixin::RelativeAccuracy(other.getRelativeAccuracy()),
            Mixin::Eps(other.eps()),
            Mixin::Verbosity(other.getVerbosityLevel()),
            Mixin::IterativeRefinements (other.getIterativeRefinements()),
            Mixin::MaxSteps (other.getMaxSteps()),
            ppcg(other.ppcg)
        {
            using namespace Mixin;
            cast_and_attach<Eps>(*this,ppcg);
            cast_and_attach<AbsoluteAccuracy>(*this,ppcg);
            cast_and_attach<RelativeAccuracy>(*this,ppcg);
            cast_and_attach<Verbosity>(*this,ppcg);
            cast_and_attach<IterativeRefinements>(*this,ppcg);
            cast_and_attach<MaxSteps>(*this,ppcg);
        }

        /// Apply projected conjugate gradient solver.
        Vector LinearSolver::operator()( const Vector& y ) const
        {
            return ppcg.solve( zero( range() ), y );
        }
        /// Access projected conjugate gradient implementation.
        Solver& LinearSolver::impl()
        {
            return ppcg;
        }

        /// Checks positive definiteness of \f$A\f$.
        bool LinearSolver::isPositiveDefinite() const
        {
            return !ppcg.indefiniteOperator();
        }

        /// Access preconditioner \f$P\f$.
        const CallableOperator& LinearSolver::P() const
        {
            return ppcg.P();
        }

        /// Access operator \f$A\f$.
        const CallableOperator& LinearSolver::A() const
        {
            return ppcg.A();
        }
    }
        
    PPCG::LinearSolver makePPCGSolver( Operator A, CallableOperator P, MyOperator proj_op, 
                                       MyOperator update_r_op, MyOperator lagr_update, 
                                       Real relativeAccuracy, Real eps, bool verbose )
    {
        auto solver = PPCG::LinearSolver( std::move( A ), std::move( P ), std::move(proj_op), 
                                            std::move(update_r_op), std::move(lagr_update) );
        // solver.setRelativeAccuracy( relativeAccuracy );
        // solver.setEps( eps );
        // solver.set_eps( eps );
        // solver.setVerbosity( verbose );
        return solver;
    }
}




