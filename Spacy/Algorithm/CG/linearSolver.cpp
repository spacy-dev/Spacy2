#include "linearSolver.hh"

#include <Spacy/operator.hh>
#include <Spacy/vectorSpace.hh>
#include <Spacy/zeroVectorCreator.hh>

#include <utility>

namespace Spacy
{
    namespace CG
    {
        LinearSolver::LinearSolver( Operator A_, CallableOperator P_, const std::string& type )
            : OperatorBase( A_.range(), A_.domain() ), cg( std::move( A_ ), std::move( P_ ), type )

        {
            using namespace Mixin;
            cast_and_attach< Eps >( *this, cg );
            cast_and_attach< AbsoluteAccuracy >( *this, cg );
            cast_and_attach< RelativeAccuracy >( *this, cg );
            cast_and_attach< Verbosity >( *this, cg );
            cast_and_attach< IterativeRefinements >( *this, cg );
            cast_and_attach< MaxSteps >( *this, cg );
        }

        LinearSolver::LinearSolver( Operator A_, CallableOperator P_, MyOperator r_update,
                                    const std::string& type )
            : OperatorBase( A_.range(), A_.domain() ),
              cg( std::move( A_ ), std::move( P_ ), std::move( r_update ), type )
        {
            using namespace Mixin;
            cast_and_attach< Eps >( *this, cg );
            cast_and_attach< AbsoluteAccuracy >( *this, cg );
            cast_and_attach< RelativeAccuracy >( *this, cg );
            cast_and_attach< Verbosity >( *this, cg );
            cast_and_attach< IterativeRefinements >( *this, cg );
            cast_and_attach< MaxSteps >( *this, cg );
        }

        LinearSolver::LinearSolver( const LinearSolver& other )
            : OperatorBase( other ), Mixin::AbsoluteAccuracy( other.getAbsoluteAccuracy() ),
              Mixin::RelativeAccuracy( other.getRelativeAccuracy() ), Mixin::Eps( other.eps() ),
              Mixin::Verbosity( other.getVerbosityLevel() ),
              Mixin::IterativeRefinements( other.getIterativeRefinements() ),
              Mixin::MaxSteps( other.getMaxSteps() ), cg( other.cg )
        {
            using namespace Mixin;
            cast_and_attach< Eps >( *this, cg );
            cast_and_attach< AbsoluteAccuracy >( *this, cg );
            cast_and_attach< RelativeAccuracy >( *this, cg );
            cast_and_attach< Verbosity >( *this, cg );
            cast_and_attach< IterativeRefinements >( *this, cg );
            cast_and_attach< MaxSteps >( *this, cg );
        }

        Vector LinearSolver::operator()( const Vector& y ) const
        {
            return cg.solve( zero( range() ), y );
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

        const MyOperator& LinearSolver::update_r_operator() const
        {
            return cg.update_r_operator();
        }
    }

    CG::LinearSolver makeCGSolver( Operator A, CallableOperator P, Real relativeAccuracy, Real eps,
                                   bool verbose )
    {
        auto solver = CG::LinearSolver( std::move( A ), std::move( P ), "CG" );
        solver.setRelativeAccuracy( relativeAccuracy );
        solver.set_eps( eps );
        solver.setVerbosity( verbose );
        return solver;
    }

    CG::LinearSolver makeRCGSolver( Operator A, CallableOperator P, Real relativeAccuracy, Real eps,
                                    bool verbose )
    {
        auto solver = CG::LinearSolver( std::move( A ), std::move( P ), "RCG" );
        solver.setRelativeAccuracy( relativeAccuracy );
        solver.set_eps( eps );
        solver.setVerbosity( verbose );
        return solver;
    }

    CG::LinearSolver makeTCGSolver( Operator A, CallableOperator P, Real relativeAccuracy, Real eps,
                                    bool verbose )
    {
        auto solver = CG::LinearSolver( std::move( A ), std::move( P ), "TCG" );
        solver.setRelativeAccuracy( relativeAccuracy );
        solver.set_eps( eps );
        solver.setVerbosity( verbose );
        return solver;
    }

    CG::LinearSolver makeTRCGSolver( Operator A, CallableOperator P, Real relativeAccuracy,
                                     Real eps, bool verbose )
    {
        auto solver = CG::LinearSolver( std::move( A ), std::move( P ), "TRCG" );
        solver.setRelativeAccuracy( relativeAccuracy );
        solver.set_eps( eps );
        solver.setVerbosity( verbose );
        return solver;
    }

    CG::LinearSolver makeICGSolver( Operator A, CallableOperator P, MyOperator r_update,
                                    Real relativeAccuracy, Real eps, bool verbose )
    {
        auto solver =
            CG::LinearSolver( std::move( A ), std::move( P ), std::move( r_update ), "ICG" );
        solver.setRelativeAccuracy( relativeAccuracy );
        solver.set_eps( eps );
        solver.setVerbosity( verbose );
        return solver;
    }
}
