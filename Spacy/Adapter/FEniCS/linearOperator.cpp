#include "linearOperator.hh"

#include <Spacy/Spaces/realSpace.hh>
#include <Spacy/Util/Exceptions/callOfUndefinedFunctionException.hh>
#include <Spacy/vector.hh>
#include <Spacy/vectorSpace.hh>
#include <Spacy/zeroVectorCreator.hh>

#include "operatorSpace.hh"
#include "util.hh" // copy
#include "vectorSpace.hh"

#include <cassert>

namespace Spacy
{
    namespace FEniCS
    {
        LinearOperator::LinearOperator(
            std::shared_ptr< dolfin::GenericMatrix > A, const VectorSpace& space,
            std::shared_ptr< const dolfin::FunctionSpace > dolfinSpace,
            std::function< LinearSolver( const LinearOperator& ) > solverCreator )
            : OperatorBase( creator< LinearOperatorCreator >( space ).domain(),
                            creator< LinearOperatorCreator >( space ).range() ),
              VectorBase( space ), A_( A ), space_( dolfinSpace )
        {
            if ( solverCreator )
                solverCreator_ = std::move( solverCreator );
        }

        ::Spacy::Vector LinearOperator::operator()( const ::Spacy::Vector& x ) const
        {
            if ( x.space().index() == domain().index() )
                return applyAsOperator( x );

            if ( x.space().isPrimalWRT( space() ) )
                return applyAsDualElement( x );

            assert( false );
        }

        Real LinearOperator::operator()( const LinearOperator& A ) const
        {
            return applyAsDualElement( A );
        }

        LinearSolver LinearOperator::solver() const
        {
            return solverCreator_( *this );
        }

        dolfin::GenericMatrix& LinearOperator::get()
        {
            assert( A_ != nullptr );
            return *A_;
        }

        const dolfin::GenericMatrix& LinearOperator::get() const
        {
            assert( A_ != nullptr );
            return *A_;
        }

        bool LinearOperator::symmetric() const
        {
            return symmetric_;
        }

        std::shared_ptr< const dolfin::FunctionSpace > LinearOperator::dolfinSpace() const
        {
            return space_;
        }

        ::Spacy::Vector LinearOperator::applyAsOperator( const ::Spacy::Vector& x ) const
        {
            auto x_ = dolfin::Function( space_ );
            copy( x, x_ );
            auto y_ = x_.vector()->copy();
            get().mult( *x_.vector(), *y_ );

            auto y = zero( range() );
            copy( *y_, y );

            return y;
        }

        ::Spacy::Real LinearOperator::applyAsDualElement( const ::Spacy::Vector& x ) const
        {
            throw CallOfUndefinedFunctionException( "FEniCS::LinearOperator::applyAsDualElement" );
        }
    }
}
