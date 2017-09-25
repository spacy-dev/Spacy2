/**
 *  Author: Jan Zetzmann
 *  modified from 'directBlockPreconditioner.hh'
 */

#ifndef SPACY_KASKADE_INDIRECTBLOCKPRECONDITIONER_HH
#define SPACY_KASKADE_INDIRECTBLOCKPRECONDITIONER_HH


// Kaskade
#include "fem/variables.hh"
#include "dune/istl/operators.hh"
#include "dune/istl/bcrsmatrix.hh"
#include "linalg/triplet.hh"
#include "linalg/threadedMatrix.hh"
// #include "linalg/chebyshev.hh"
#include "linalg/symmetricOperators.hh"
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

#include "utilities/linalg/scalarproducts.hh"
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
        
        // template <typename T>
        // void ausgabe(const T& v)
        // {
        //     LinAlg::EuclideanScalarProduct sp;
        //     std::cout << " ||v_y||_K^2: " << sp(component< 0 >(v), component< 0 >(v)) << std::endl;
        //     std::cout << " ||v_u||_K^2: " << sp(component< 1 >(v), component< 1 >(v)) << std::endl;
        //     std::cout << " ||v_p||_K^2: " << sp(component< 2 >(v), component< 2 >(v)) << std::endl << std::endl;

        //     return;
        // }

        // template <typename T>
        // void ausgabe2(const T& v)
        // {
        //     // Domain x_( AVD::template CoefficientVectorRepresentation<>::init( spaces_ ) );
        //     // copyFromCoefficientVector< AVD >( x_, x );

        //     // component< 0 >( x_ ) = component< 0 >( xy_ );
        //     // component< 1 >( x_ ) = component< 0 >( xu_ );
        //     // component< 2 >( x_ ) = component< 0 >( xp_ );
        //     auto v_ = component<0>(v);
        //     LinAlg::EuclideanScalarProduct sp;
        //     std::cout << " ||v||_K^2: " << sp(v_,v_) << std::endl;

        //     return;
        // }
     
        /**
         * @brief A Preconditioner that uses a direct solver
         * using CallableOperator = std::function< Vector( const Vector& ) >;
         * is derived to be able to use IndirectBlockPreconditioner in cg
         * 
         * FunctionalDefinition     the functional that will be solved in general
         * solA_type                this is the type of a preconditioner that solves the A-block *                          (methods take boost::fusion::vectors => use "component<>()" )
         */
        template < class FunctionalDefinition, class solA_type, class solAT_type >
        class IndirectBlockPreconditioner : public OperatorBase//, public CallableOperator
        {
        public:
            using Description = typename FunctionalDefinition::AnsatzVars;
            using SpacyFunctional = typename ::Spacy::Kaskade::template C2Functional< FunctionalDefinition >;
            using AVD = typename SpacyFunctional::VariableSetDescription;
            using VariableSet = typename Description::VariableSet;
            using Spaces = typename AVD::Spaces;

            using KaskadeOperator = typename SpacyFunctional::KaskadeOperator;
            using Domain = typename AVD::template CoefficientVectorRepresentation<>::type;
            using Range = typename AVD::template CoefficientVectorRepresentation<>::type;
            template < class X, class Y >
            using M = ::Kaskade::MatrixRepresentedOperator< MatrixAsTriplet< double >, X, Y >;
            //start new_unnecessary
            // using dune_stuff = Dune::FieldMatrix<double, 1, 1>;
            // using NumaMat = NumaBCRSMatrix<dune_stuff>;
            // using BCRSMat = Dune::BCRSMatrix<dune_stuff>;
            // template < class X, class Y >
            // using MatRepOpwithNumaMat = ::Kaskade::MatrixRepresentedOperator< NumaMat, X, Y >;
            // template < class X, class Y >
            // using MatRepOpwithBCRSMat = ::Kaskade::MatrixRepresentedOperator< BCRSMat, X, Y >;
            //end new_unnecessary
            using VYSetDescription = Detail::ExtractDescription_t< AVD, 0 >;
            using VUSetDescription = Detail::ExtractDescription_t< AVD, 1 >;
            using VPSetDescription = Detail::ExtractDescription_t< AVD, 2 >;
            using DomainY = typename VYSetDescription::template CoefficientVectorRepresentation<>::type;
            using DomainU = typename VUSetDescription::template CoefficientVectorRepresentation<>::type;
            using DomainP = typename VPSetDescription::template CoefficientVectorRepresentation<>::type;
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
            IndirectBlockPreconditioner( const KaskadeOperator& K, solA_type&& some_solA, solAT_type&& some_solAT,
                                       /*  const ::Spacy::Vector& origin, */
                                       const Description& desc, const VectorSpace& domain, const VectorSpace& range )
                : OperatorBase( domain, range ),
                  desc_( desc ),
                  spaces_(::Spacy::Kaskade::extractSpaces< AVD >( domain ) ),
                  solA(new solA_type(std::move(some_solA))),
                  solAT(new solAT_type(std::move(some_solAT)))
            {
                blockbounds.push_back( 0 );
                {
                    typename Description::VariableSet originK( desc );
                    // 								copy(origin_,originK);
                    for ( int i = 0; i < originK.descriptions.noOfVariables; i++ )
                    {
                        blockbounds.push_back( originK.descriptions.degreesOfFreedom( i, i + 1 ) + blockbounds[ i ] );
                    }
                }

                // this does also work
                // solA = std::make_shared<solA_type>(std::move(some_solA));
                // solAT = std::make_shared<solAT_type>(std::move(some_solAT));

                { // useless braces
                    // std::cout << "  Factorization for projection only.." << std::flush;
                    Atype A;
                    A = getBlock< Atype >( K, 2, 0 );
                    A.get_non_const().setStartToZero();
                    proj_solA = 
                        std::make_shared<
                                        ::Kaskade::InverseLinearOperator<
                                                                        ::Kaskade::DirectSolver< DomainY, DomainP > 
                                                                        > 
                                        >(
                                            ::Kaskade::directInverseOperator(   A,
                                                                                DirectType::UMFPACK3264,
                                                                                MatrixProperties::GENERAL 
                                                                            )
                                            );

                    ATtype AT;
                    AT = getBlock< Atype >( K, 0, 2 );
                    AT.get_non_const().setStartToZero();
                    lagrange_solAT = 
                        std::make_shared<
                                        ::Kaskade::InverseLinearOperator<
                                                                        ::Kaskade::DirectSolver< DomainY, DomainP > 
                                                                        > 
                                        >(
                                            ::Kaskade::directInverseOperator(   AT,
                                                                                DirectType::UMFPACK3264,
                                                                                MatrixProperties::GENERAL 
                                                                            )
                                            );
                    
                    // std::cout << "." << std::endl;
                
                    
                    // // compare Atype = M< DomainY, DomainP > written as an operator
                    // std::function<DomainP(DomainY)> A_mat_op = [this, &A] (DomainY v_y_loc) -> DomainP
                    // {
                    //     Domain res_dummy(AVD::template CoefficientVectorRepresentation<>::init( spaces_ ));
                    //     DomainP res_p_dummy( component< 2 >( res_dummy ) );
                    //     A.apply(v_y_loc, res_p_dummy); // at this point we could use applytransposed in order to save extraction of AT for lagrange_solAT
                    //     return res_p_dummy;
                    // };
                    
                    // //projection cg-solver
                    // std::function<DomainY(DomainY)> bpx_precond = [this] (DomainY b_y_loc) -> DomainY
                    // {
                    //     Domain x_dummy(AVD::template CoefficientVectorRepresentation<>::init( spaces_ ));
                    //     DomainY x_y_dummy( component< 0 >( x_dummy ) ); //empty_dummy  =>  x_y_dummy = 0
                    //     solA->apply(component<0>(x_y_dummy), component<0>(b_y_loc) );
                    //     return x_y_dummy;
                    // };
                    // proj_solA = std::make_shared<::Spacy::CG::Solver>(::Spacy::CG::Solver(A_mat_op, bpx_precond));

                }


                Mutype Mu;
                Mu = getBlockDiag< Mutype >( K, 1, 1 );
                Mu.get_non_const().setStartToZero();
                solMu =
                std::make_shared<
                                    ::Kaskade::InverseLinearOperator<::Kaskade::DirectSolver< DomainU, DomainU > >
                                >(
                                    ::Kaskade::directInverseOperator( Mu, DirectType::UMFPACK3264, MatrixProperties::GENERAL )
                                );

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
                // std::cout << "\t\t\t this is indBP.operator() /solver for preconditioner" << std::endl;
                using namespace boost::fusion;

                Domain b_( AVD::template CoefficientVectorRepresentation<>::init( spaces_ ) );
                Domain x_( AVD::template CoefficientVectorRepresentation<>::init( spaces_ ) );

                copyToCoefficientVector< AVD >( b, b_ );

                DomainY b0_( component< 0 >( b_ ) );
                DomainU b1_( component< 1 >( b_ ) );
                DomainP b2_( component< 2 >( b_ ) );
                
                DomainY xy_( component< 0 >( x_ ) );
                DomainU xu_( component< 1 >( x_ ) );
                DomainP xp_( component< 2 >( x_ ) );
                
                // solAT->apply(b0_,xp_);
                
                // Solve A^T p=b0
                // std::cout << "_____________ Solve A^T p=b0 _____________" << std::endl;
                solAT->apply( component<0>(xp_), component<0>(b0_) );
                
                B.applyscaleaddTransposed( -1.0, xp_, b1_ ); // b1_ -> b1_ +(-1.0)*(B^T xp_)
                // ausgabe2(b1_);
                
                // std::cout << std::endl;
                
                // Solve M_u xu_ = b1_
                // std::cout << "_____________ Solve M_u xu_ = b1_ _____________" << std::endl;
                solMu->apply( b1_, xu_ );
                
                B.applyscaleadd( -1.0, xu_, b2_ ); // b2_ -> b2_ +(-1.0)*(B xu_)
                // ausgabe2(b2_);
                // std::cout << std::endl;
                
                // Solve Axy_ = b2_
                solA->apply(component<0>(xy_),component<0>(b2_));
                component< 0 >( x_ ) = component< 0 >( xy_ );
                component< 1 >( x_ ) = component< 0 >( xu_ );
                component< 2 >( x_ ) = component< 0 >( xp_ );
                
                // std::cout << "_____________ Solve M_u xu_ = b1_ _____________" << std::endl;
                // ausgabe(x_);
                
                auto x = zero( domain() );
                copyFromCoefficientVector< AVD >( x_, x );

                // std::cout << "components of P_() return value:\n" << std::endl;
                // std::cout << "y" << std::endl;
                // for(int i=0; i < component<0>(xy_).size(); ++i)
                //     std::cout << component<0>(xy_)[i] << " ";
                // std::cout << std::endl << "u" << std::endl;
                // for(int i=0; i < component<0>(xy_).size(); ++i)
                //     std::cout << component<0>(xu_)[i] << " ";
                // std::cout << std::endl << "l" << std::endl;
                // for(int i=0; i < component<0>(xy_).size(); ++i)
                //     std::cout << component<0>(xp_)[i] << " ";
                // std::cout << std::endl;

                // std::cout << "enter to continue" << std::endl;
                // std::cin.get();

                return x;
            };
            
            /**
            * @brief returns updated r
            *
            * w_k = -(r_{yk} & B^T g_{lk} & r_{lk} )^T + \beta_k * w_{k-1}
            * w necessary for creation of \tilde{H} in ppcg as described in inexact step computation 
            * (see @cite citation needed: L. Lubkoll, A. Schiela, M. Weiser)
            */
            ::Spacy::Vector update_r(const ::Spacy::Vector& r, const ::Spacy::Vector& g) const
            {
                // std::cout << "\t\t\t Hello there! This is update_r" << std::endl;
                using namespace boost::fusion;

                Domain r_( AVD::template CoefficientVectorRepresentation<>::init( spaces_ ) );
                Domain g_( AVD::template CoefficientVectorRepresentation<>::init( spaces_ ) );

                copyToCoefficientVector< AVD >( r, r_ );
                copyToCoefficientVector< AVD >( g, g_ );

                // extract
                DomainU ru( component< 1 >( r_ ) );

                DomainP gl( component< 2 >( g_ ) );
                // I have called the dual component from DomainP, l after Lagrange - don't get confused
                
                // $r_u = B^T g_\lambda$
                // B.applyTransposed( gl, gl ); // gl = B^T*gl
                Domain r_dummy( AVD::template CoefficientVectorRepresentation<>::init( spaces_ ) ); // r_dummy=0;
                DomainU ru_dummy( component< 1 >( r_dummy ) ); //empty_dummy_for_workaround  =>  ru_dummy = 0
                B.applyscaleaddTransposed( -1.0, gl, ru_dummy ); // ru_dummy -> ru_dummy +(1.0)*(B^T gl)  =>  same as above

                //put back together
                component< 1 >( r_ ) = component< 0 >( ru_dummy );

                auto r_updated_spacy = zero( domain() );
                copyFromCoefficientVector< AVD >(r_, r_updated_spacy);

                return r_updated_spacy;
            }

            /**
            *   \delta_u = u'-u_k
            *   \delta_y = A^{-1}*B*\delta_u
            */
            ::Spacy::Vector proj_to_kernel(const ::Spacy::Vector& x, ::Spacy::Vector dx) const
            {
                // std::cout << "\t\t\t Hello there! This is proj_to_kernel\n";
                // x_y = A^{-1}*B*x_u

                Domain x_( AVD::template CoefficientVectorRepresentation<>::init( spaces_ ) );
                Domain dx_( AVD::template CoefficientVectorRepresentation<>::init( spaces_ ) );

                copyToCoefficientVector< AVD >( x, x_ );
                copyToCoefficientVector< AVD >( dx, dx_ );

                // extract
                DomainY xy( component< 0 >( x_ ) );
                DomainU xu( component< 1 >( x_ ) );

                DomainY dxy( component< 0 >( dx_ ) );
                DomainU dxu( component< 1 >( dx_ ) );

                dxu -= xu;

                // $x_u = B x_u$ apply matrix B on xu
                Domain dx_dummy( AVD::template CoefficientVectorRepresentation<>::init( spaces_ ) );
                DomainU dxu_dummy( component< 1 >( dx_dummy ) ); //empty_dummy_for_workaround  =>  dxu_dummy = 0
                B.applyscaleadd( -1.0, dxu, dxu_dummy );

                // $dx_y = A^{-1}*(-1)*dx_u$ solve for xy
                // Solve A*dxy = dxu_dummy
                proj_solA->apply( dxu_dummy, dxy );
                // dxy = proj_solA->solve(dxu_dummy, dxy); // cg with precond

                //put back together
                component< 0 >( dx_ ) = component< 0 >( dxy );
                component< 1 >( dx_ ) = component< 0 >( dxu );

                // auto ret_dx_ = zero( domain() );
                // copyFromCoefficientVector< AVD >(dx_, ret_dx_);
                copyFromCoefficientVector< AVD >(dx_, dx);

                return dx;
            }

            ::Spacy::Vector lagrange_update(::Spacy::Vector x, const ::Spacy::Vector& b)
            {
                Domain x_( AVD::template CoefficientVectorRepresentation<>::init( spaces_ ) );
                Domain b_( AVD::template CoefficientVectorRepresentation<>::init( spaces_ ) );

                copyToCoefficientVector< AVD >( x, x_ );
                copyToCoefficientVector< AVD >( b, b_ );

                // extract
                DomainY xy( component< 0 >( x_ ) );
                DomainP xp( component< 2 >( x_ ) );

                DomainP bp( component< 2 >( b_ ) );
                bp *= -1.0;
                My.applyscaleadd( -1.0, xy, bp ); // -bp -> -bp +(-1.0)*(My xy) = -(My xy + bp)

                lagrange_solAT->apply( bp, xp );

                component< 2 >( x_ ) = component< 0 >( xp );
                copyFromCoefficientVector< AVD >(x_, x);

                return x;
            }

        private:
            // CallableOperator A;

            const Description& desc_;
            Spaces spaces_;
            //         const ::Spacy::Vector& origin_;
            std::vector< int > blockbounds = {};
            Btype B;

            // for projection
            mutable std::shared_ptr<::Kaskade::InverseLinearOperator<::Kaskade::DirectSolver< DomainY, DomainP > > >
                proj_solA = nullptr;

            // // cg for solving the projection // doesnt work because of reasons
            // std::shared_ptr<::Spacy::CG::Solver> proj_solA = nullptr;
                
            // for lagrange update
            mutable std::shared_ptr<::Kaskade::InverseLinearOperator<::Kaskade::DirectSolver< DomainY, DomainP > > >
                lagrange_solAT = nullptr;
            Mytype My;

            std::shared_ptr< solA_type > solA = nullptr;
            std::shared_ptr< solAT_type > solAT = nullptr;



            //~ mutable std::shared_ptr< ::Kaskade::InverseLinearOperator< ::Kaskade::DirectSolver<DomainP,DomainY> > >
            //solAT = nullptr;
            mutable std::shared_ptr<::Kaskade::InverseLinearOperator<::Kaskade::DirectSolver< DomainU, DomainU > > >
                solMu = nullptr;

            template < class Result >
            Result getBlock( const KaskadeOperator& K, unsigned int row, unsigned int col )
            {
                const MatrixAsTriplet< double >& A( K.get() );
                Result B;
                for ( int i = 0; i < A.nnz(); ++i )
                {
                    if ( A.ridx[ i ] >= blockbounds[ row ] && A.ridx[ i ] < blockbounds[ row + 1 ] &&
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
                    if ( A.ridx[ i ] >= blockbounds[ row ] && A.ridx[ i ] < blockbounds[ row + 1 ] &&
                         A.cidx[ i ] >= blockbounds[ col ] && A.cidx[ i ] < blockbounds[ col + 1 ] &&
                         A.ridx[ i ] == A.cidx[ i ] )
                        B.get_non_const().addEntry( A.ridx[ i ], A.cidx[ i ], A.data[ i ] );
                }
                return B;
            }
        };
    }
}



#endif // SPACY_KASKADE_INDIRECTBLOCKPRECONDITIONER_HH