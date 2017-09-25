/**
 *  Author: Jan Zetzmann
 *  modified from 'directBlockPreconditioner.hh'
 */

#ifndef SPACY_KASKADE_INDIRECTBLOCKPRECONDITIONER_HH
#define SPACY_KASKADE_INDIRECTBLOCKPRECONDITIONER_HH

// Kaskade
#include "dune/istl/bcrsmatrix.hh"
#include "dune/istl/operators.hh"
#include "fem/variables.hh"
#include "linalg/symmetricOperators.hh"
#include "linalg/threadedMatrix.hh"
#include "linalg/triplet.hh"
#include "mg/additiveMultigrid.hh"

// Spacy
#include "Spacy/Adapter/Kaskade/util.hh"
#include "Spacy/Spaces/ProductSpace/vector.hh"
#include "Spacy/Util/Base/OperatorBase.hh"
#include "Spacy/c2Functional.hh"
#include "Spacy/linearSolver.hh"
#include "Spacy/operator.hh"
#include "Spacy/vector.hh"
#include <Spacy/zeroVectorCreator.hh>

#include "Spacy/Algorithm/CG/cg.hh"

#include <iostream>
// #include <functional>
#include <memory>

namespace Spacy
{
    /// \cond
    class VectorSpace;
    /// \endcond

    namespace Kaskade
    {
        /**
         * @brief A Preconditioner that uses give solvers for A and AT but still direct solvers for
         * solving M_u, projection and lagrange update
         *
         * FunctionalDefinition     the functional that will be solved in general
         * solA_type                this is the type of a preconditioner that solves the A-block
         *                          (methods take boost::fusion::vectors => use "component<>()" )
         */
        template < class FunctionalDefinition, class solA_type, class solAT_type >
        class IndirectBlockPreconditioner : public OperatorBase //, public CallableOperator
        {
        public:
            using Description = typename FunctionalDefinition::AnsatzVars;
            using SpacyFunctional =
                typename ::Spacy::Kaskade::template C2Functional< FunctionalDefinition >;
            using AVD = typename SpacyFunctional::VariableSetDescription;
            using VariableSet = typename Description::VariableSet;
            using Spaces = typename AVD::Spaces;

            using KaskadeOperator = typename SpacyFunctional::KaskadeOperator;
            using Domain = typename AVD::template CoefficientVectorRepresentation<>::type;
            using Range = typename AVD::template CoefficientVectorRepresentation<>::type;
            template < class X, class Y >
            using M = ::Kaskade::MatrixRepresentedOperator< MatrixAsTriplet< double >, X, Y >;
            using VYSetDescription = Detail::ExtractDescription_t< AVD, 0 >;
            using VUSetDescription = Detail::ExtractDescription_t< AVD, 1 >;
            using VPSetDescription = Detail::ExtractDescription_t< AVD, 2 >;
            using DomainY =
                typename VYSetDescription::template CoefficientVectorRepresentation<>::type;
            using DomainU =
                typename VUSetDescription::template CoefficientVectorRepresentation<>::type;
            using DomainP =
                typename VPSetDescription::template CoefficientVectorRepresentation<>::type;
            using Atype = M< DomainY, DomainP >;
            using ATtype = M< DomainP, DomainY >;
            using Btype = M< DomainU, DomainP >;
            // using BTtype = M< DomainP, DomainU >;
            using Mutype = M< DomainU, DomainU >;
            using Mytype = M< DomainY, DomainY >;

            /**
             * @brief Constructor.
             * @param domain domain space
             * @param range range space
             */
            IndirectBlockPreconditioner( const KaskadeOperator& K, solA_type&& some_solA,
                                         solAT_type&& some_solAT,
                                         /*  const ::Spacy::Vector& origin, */
                                         const Description& desc, const VectorSpace& domain,
                                         const VectorSpace& range )
                : OperatorBase( domain, range ), desc_( desc ),
                  spaces_(::Spacy::Kaskade::extractSpaces< AVD >( domain ) ),
                  solA( new solA_type( std::move( some_solA ) ) ),
                  solAT( new solAT_type( std::move( some_solAT ) ) ), r_updated( zero( domain ) )
            {
                blockbounds.push_back( 0 );
                {
                    typename Description::VariableSet originK( desc );
                    // 								copy(origin_,originK);
                    for ( int i = 0; i < originK.descriptions.noOfVariables; i++ )
                    {
                        blockbounds.push_back( originK.descriptions.degreesOfFreedom( i, i + 1 ) +
                                               blockbounds[ i ] );
                    }
                }

                // proj_solA and lagrange_solAT should be changed to precond. cg solvers
                // setting up the solvers for block-operators A and A^T
                std::cout << "  Factorization for projection only.." << std::flush;
                Atype A;
                A = getBlock< Atype >( K, 2, 0 );
                A.get_non_const().setStartToZero();
                proj_solA = std::make_shared<::Kaskade::InverseLinearOperator<
                    ::Kaskade::DirectSolver< DomainY, DomainP > > >(
                    ::Kaskade::directInverseOperator( A, DirectType::UMFPACK3264,
                                                      MatrixProperties::GENERAL ) );

                ATtype AT;
                AT = getBlock< Atype >( K, 0, 2 );
                AT.get_non_const().setStartToZero();
                lagrange_solAT = std::make_shared<::Kaskade::InverseLinearOperator<
                    ::Kaskade::DirectSolver< DomainY, DomainP > > >(
                    ::Kaskade::directInverseOperator( AT, DirectType::UMFPACK3264,
                                                      MatrixProperties::GENERAL ) );

                std::cout << "." << std::endl;

                Mutype Mu;
                Mu = getBlockDiag< Mutype >( K, 1, 1 );
                Mu.get_non_const().setStartToZero();
                solMu = std::make_shared<::Kaskade::InverseLinearOperator<
                    ::Kaskade::DirectSolver< DomainU, DomainU > > >(
                    ::Kaskade::directInverseOperator( Mu, DirectType::UMFPACK3264,
                                                      MatrixProperties::GENERAL ) );

                // for lagrange update
                My = getBlockDiag< Mytype >( K, 0, 0 );
                My.get_non_const().setStartToZero();

                B = getBlock< Btype >( K, 2, 1 );
                B.get_non_const().setStartToZero();
            }

            /**
             * @brief Apply preconditioner \f$P\f$.
             * @param x argument
             * @return \f$P(x)\f$
             */
            ::Spacy::Vector operator()( const ::Spacy::Vector& b ) const
            {
                using namespace boost::fusion;

                Domain b_( AVD::template CoefficientVectorRepresentation<>::init( spaces_ ) );
                Domain x_( AVD::template CoefficientVectorRepresentation<>::init( spaces_ ) );

                copyToCoefficientVector< AVD >( b, b_ );

                DomainY by_( component< 0 >( b_ ) );
                DomainU bu_( component< 1 >( b_ ) );
                DomainP bp_( component< 2 >( b_ ) );

                DomainY xy_( component< 0 >( x_ ) );
                DomainU xu_( component< 1 >( x_ ) );
                DomainP xp_( component< 2 >( x_ ) );

                // Solve A^T p = b0
                solAT->apply( component< 0 >( xp_ ), component< 0 >( by_ ) );

                B.applyscaleaddTransposed( -1.0, xp_, bu_ ); // bu_ := bu_ +(-1.0)*(B^T xp_)

                // Solve M_u xu_ = bu_
                solMu->apply( bu_, xu_ );

                // bp_ := bp_ + B^T xp_
                B.applyscaleadd( -1.0, xu_, bp_ ); // bp_ -> bp_ +(-1.0)*(B xu_)

                // Solve A xy_ = bp_
                solA->apply( component< 0 >( xy_ ), component< 0 >( bp_ ) );

                component< 0 >( x_ ) = component< 0 >( xy_ );
                component< 1 >( x_ ) = component< 0 >( xu_ );
                component< 2 >( x_ ) = component< 0 >( xp_ );

                auto x = zero( domain() );
                copyFromCoefficientVector< AVD >( x_, x );

                {
                    // update_r
                    // to later be retrieved by update_r() method
                    // in icg
                    // r = b as P(r) = P(b)
                    // g = x at the end of operator()
                    // $r_u = B^T g_\lambda$
                    Domain b_dummy( AVD::template CoefficientVectorRepresentation<>::init(
                        spaces_ ) ); // r_dummy=0;
                    DomainU bu_dummy(
                        component< 1 >( b_dummy ) ); // empty_dummy_for_workaround  =>  bu_dummy = 0
                    B.applyscaleaddTransposed(
                        -1.0, xp_,
                        bu_dummy ); // bu_dummy := bu_dummy +(-1.0)*(B^T xp_)  =>  same as above

                    // place back into b_ = r_
                    component< 1 >( b_ ) = component< 0 >( bu_dummy );

                    // save in member r_updated
                    copyFromCoefficientVector< AVD >( b_, r_updated );

                    // end of update_r code
                }

                return x;
            }

            /**
            * @brief returns updated r
            *
            * w_k = - r + \beta_k * w_{k-1} = -(r_{yk} & B^T g_{lk} & r_{lk} )^T + \beta_k * w_{k-1}
            * w necessary for creation of \tilde{H} in ppcg as described in inexact step computation
            * (see @cite citation needed: L. Lubkoll, A. Schiela, M. Weiser)
            */
            ::Spacy::Vector update_r() const
            {
                return r_updated;
            }

            /**
            *   \delta_u = u'-u_k
            *   \delta_y = A^{-1}*B*\delta_u
            */
            ::Spacy::Vector proj_to_kernel( const ::Spacy::Vector& x, ::Spacy::Vector dx ) const
            {
                Domain x_( AVD::template CoefficientVectorRepresentation<>::init( spaces_ ) );
                Domain dx_( AVD::template CoefficientVectorRepresentation<>::init( spaces_ ) );

                copyToCoefficientVector< AVD >( x, x_ );
                copyToCoefficientVector< AVD >( dx, dx_ );

                // extract
                DomainU xu( component< 1 >( x_ ) );

                DomainY dxy( component< 0 >( dx_ ) );
                DomainU dxu( component< 1 >( dx_ ) );

                dxu -= xu;

                // $x_u = B x_u$ apply matrix B on xu
                Domain dx_dummy( AVD::template CoefficientVectorRepresentation<>::init(
                    spaces_ ) ); // dx_dummy=0;
                DomainU dxu_dummy(
                    component< 1 >( dx_dummy ) ); // empty_dummy_for_workaround  =>  dxu_dummy = 0
                B.applyscaleadd( -1.0, dxu, dxu_dummy );

                // Solve A*dxy = dxu_dummy
                proj_solA->apply( dxu_dummy, dxy );

                // put back together
                component< 0 >( dx_ ) = component< 0 >( dxy );
                component< 1 >( dx_ ) = component< 0 >( dxu );

                // auto ret_dx_ = zero( domain() );
                copyFromCoefficientVector< AVD >( dx_, dx );

                return dx;
            }

            ::Spacy::Vector lagrange_update(::Spacy::Vector x, const ::Spacy::Vector& b )
            {
                Domain x_( AVD::template CoefficientVectorRepresentation<>::init( spaces_ ) );
                Domain b_( AVD::template CoefficientVectorRepresentation<>::init( spaces_ ) );

                copyToCoefficientVector< AVD >( x, x_ );
                copyToCoefficientVector< AVD >( b, b_ );

                // extract
                DomainY xy( component< 0 >( x_ ) );
                DomainP xp( component< 2 >( x_ ) );

                DomainP by( component< 0 >( b_ ) );
                by *= -1.0;
                My.applyscaleadd( -1.0, xy, by ); // -by -> -by +(-1.0)*(My xy) = -(My xy + by)

                lagrange_solAT->apply( by, xp );

                component< 2 >( x_ ) = component< 0 >( xp );
                copyFromCoefficientVector< AVD >( x_, x );

                return x;
            }

        private:
            const Description& desc_;
            Spaces spaces_;
            std::vector< int > blockbounds = {};

            Btype B;

            // for projection
            mutable std::shared_ptr<
                ::Kaskade::InverseLinearOperator<::Kaskade::DirectSolver< DomainY, DomainP > > >
                proj_solA = nullptr;

            // for lagrange update
            mutable std::shared_ptr<
                ::Kaskade::InverseLinearOperator<::Kaskade::DirectSolver< DomainY, DomainP > > >
                lagrange_solAT = nullptr;
            Mytype My;

            std::shared_ptr< solA_type > solA = nullptr;
            std::shared_ptr< solAT_type > solAT = nullptr;

            mutable ::Spacy::Vector r_updated;

            mutable std::shared_ptr<
                ::Kaskade::InverseLinearOperator<::Kaskade::DirectSolver< DomainU, DomainU > > >
                solMu = nullptr;

            template < class Result >
            Result getBlock( const KaskadeOperator& K, unsigned int row, unsigned int col )
            {
                const MatrixAsTriplet< double >& A( K.get() );
                Result B;
                for ( int i = 0; i < A.nnz(); ++i )
                {
                    if ( A.ridx[ i ] >= blockbounds[ row ] &&
                         A.ridx[ i ] < blockbounds[ row + 1 ] &&
                         A.cidx[ i ] >= blockbounds[ col ] && A.cidx[ i ] < blockbounds[ col + 1 ] )
                        B.get_non_const().addEntry( A.ridx[ i ], A.cidx[ i ], A.data[ i ] );
                }
                return B;
            }
            template < class Result >
            Result getBlockDiag( const KaskadeOperator& K, unsigned int row, unsigned int col )
            {
                const MatrixAsTriplet< double >& A( K.get() );
                Result B;
                for ( int i = 0; i < A.nnz(); ++i )
                {
                    if ( A.ridx[ i ] >= blockbounds[ row ] &&
                         A.ridx[ i ] < blockbounds[ row + 1 ] &&
                         A.cidx[ i ] >= blockbounds[ col ] &&
                         A.cidx[ i ] < blockbounds[ col + 1 ] && A.ridx[ i ] == A.cidx[ i ] )
                        B.get_non_const().addEntry( A.ridx[ i ], A.cidx[ i ], A.data[ i ] );
                }
                return B;
            }
        };
    }
}

#endif // SPACY_KASKADE_INDIRECTBLOCKPRECONDITIONER_HH
