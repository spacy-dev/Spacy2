#define FUSION_MAX_VECTOR_SIZE 15

#include <chrono>
#include <iostream>
#include <functional>

#include <dune/grid/config.h>
#include <dune/grid/uggrid.hh>

#include <Spacy/Adapter/Kaskade/directBlockPreconditioner.hh>
#include <Spacy/Adapter/Kaskade/indirectBlockPreconditioner.hh>
#include <Spacy/Adapter/kaskade.hh> // must be 1. of this block! (was auto-changed at times)
#include <Spacy/Algorithm/CompositeStep/affineCovariantSolver.hh> // must be 2. of this block else doesnt work
#include <Spacy/Algorithm/PPCG/linearSolver.hh>

#include "mg/additiveMultigrid.hh"
#include <fem/fetransfer.hh>
#include <fem/forEach.hh>
#include <fem/gridmanager.hh>
#include <fem/lagrangespace.hh>
#include <io/vtk.hh>
#include <utilities/gridGeneration.hh>
#include <utilities/kaskopt.hh>

#define NCOMPONENTS 1
#include "nonlinear_control.hh"

#include <fung/examples/nonlinear_heat.hh>
#include <fung/fung.hh>

#include "fem/istlinterface.hh"
#include "Spacy/Adapter/Kaskade/util.hh"
#include <Spacy/Util/cast.hh>
#include <Spacy/inducedScalarProduct.hh>
#include <Spacy/zeroVectorCreator.hh>

template < class Matrix, class T >
Matrix getBlock( const Matrix& A, unsigned int row, unsigned int col, T blockbounds )
{

    Matrix B;
    for ( int i = 0; i < A.nnz(); ++i )
    {
        if ( A.ridx[ i ] >= blockbounds[ row ] && A.ridx[ i ] < blockbounds[ row + 1 ] &&
             A.cidx[ i ] >= blockbounds[ col ] && A.cidx[ i ] < blockbounds[ col + 1 ] )
            B.addEntry( A.ridx[ i ], A.cidx[ i ], A.data[ i ] );
    }
    return B;
}

/*
    template < typename Matrix >
    void writeMatlab( const Matrix& matrix, const std::string& name = "matrix.dat" )
    {
        std::ofstream myfile;
        myfile.open( name );
        for ( int i = 0; i < matrix.ridx.size(); i++ )
        {
            myfile << matrix.ridx[ i ] + 1;
            myfile << " ";
            myfile << matrix.cidx[ i ] + 1;
            myfile << " ";
            myfile << matrix.data[ i ];
            myfile << std::endl;
        }
        myfile.close();
    }

    template < typename Vector >
    void writeVec( const Vector& vector, const std::string& name = "vector.dat" )
    {
        std::ofstream myfile;
        myfile.open( name );
        for ( int i = 0; i < vector.size(); i++ ) // what is going on here ? size implementation
   wrong ?
        {
            myfile << vector[ i ] << "\n";
        }
        myfile << std::endl;
        myfile.close();
    }
*/
int main( int argc, char* argv[] )
{
    using namespace Kaskade;

    static constexpr int stateId = 0;
    static constexpr int controlId = 1;
    static constexpr int adjointId = 2;

    constexpr int dim = 2;
    int silence = 0;
    std::unique_ptr< boost::property_tree::ptree > pt =
        getKaskadeOptions( argc, argv, silence, false );

    double desiredAccuracy = getParameter( pt, "desiredAccuracy", 1e-6 );
    double eps = getParameter( pt, "eps", 1e-12 );
    double alpha = getParameter( pt, "alpha", 1e-6 );
    int maxSteps = getParameter( pt, "maxSteps", 500 );
    int initialRefinements = getParameter( pt, "initialRefinements", 2 );
    int iterativeRefinements = getParameter( pt, "iterativeRefinements", 0 );
    int FEorder = getParameter( pt, "FEorder", 1 );
    int verbose = getParameter( pt, "verbose", 1 );
    double c = getParameter( pt, "cPara", 1e0 );
    double d = getParameter( pt, "dPara", 1e0 );
    double e = getParameter( pt, "ePara", 0.0 );
    double desContr = getParameter( pt, "desiredContraction", 0.5 );
    double relDesContr = getParameter( pt, "relaxedContraction", desContr + 0.1 );
    double maxContr = getParameter( pt, "maxContraction", 0.75 );

    using std::cout;
    using std::endl;

    cout << "cPara: " << c << " dPara: " << d << " ePara: " << e << " alpha:" << alpha << endl;
    cout << "dContr: " << desContr << " rdContr: " << relDesContr << " mContr: " << maxContr
         << endl;

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     * * * * */
    /* * * * * * * * * * * * * * * * * * * * * * grid generation * * * * * * * * * * * * * * * * * *
     * * * * */
    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     * * * * */
    typedef Dune::UGGrid< dim > Grid;
    typedef Grid::LeafGridView GridView;

    GridManager< Grid > gm( createUnitSquare< Grid >( 0.25, false ) );
    gm.enforceConcurrentReads( true );
    gm.globalRefine( initialRefinements );

    std::cout << "vertices: " << gm.grid().size( dim ) << std::endl;

    std::cout << std::setprecision( 10 );
    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     * * * * */
    /* * * * * * * * * * * * * * * * * * * * * * function spaces * * * * * * * * * * * * * * * * * *
     * * * * */
    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     * * * * */
    typedef FEFunctionSpace< ContinuousLagrangeMapper< double, GridView > > H1Space;

    typedef boost::fusion::vector< H1Space const* > Spaces;
    using VU = VariableDescription< 0, 1, controlId >;
    using VY = VariableDescription< 0, 1, stateId >;
    using VP = VariableDescription< 0, 1, adjointId >;
    typedef boost::fusion::vector< VY, VU, VP > VariableDescriptions;
    typedef VariableSetDescription< Spaces, VariableDescriptions > Descriptions;

    typedef Descriptions::VariableSet VarSet;

    // construct variable list.
    std::string names[] = {"y", "u", "p"};

    auto const& leafView = gm.grid().leafGridView();
    // construct involved spaces.
    H1Space h1Space( gm, leafView, FEorder );
    Spaces spaces( &h1Space );
    Descriptions desc( spaces, names );

    // Reference solution
    cout << "interpolate" << endl;
    VarSet x_ref( desc );
    interpolateGloballyWeak< PlainAverage >(
        boost::fusion::at_c< stateId >( x_ref.data ),
        makeWeakFunctionView(
            []( auto const& cell, auto const& xLocal ) -> Dune::FieldVector< double, 1 > {
                auto x = cell.geometry().global( xLocal );
                return Dune::FieldVector< double, 1 >( 12 * ( 1 - x[ 1 ] ) * x[ 1 ] *
                                                       ( 1 - x[ 0 ] ) * x[ 0 ] );
            } ) );

    cout << "create domain" << endl;
    auto domain = Spacy::Kaskade::makeHilbertSpace< Descriptions >( spaces, {0u, 1u}, {2u} );
    // Normal step functional with cg solver
    // auto fn = Spacy::Kaskade::makeLagrangeCGFunctional<stateId,controlId,adjointId>(
    // NormalStepFunctional<stateId,controlId,adjointId,double,Descriptions>(alpha,x_ref,c,d) ,
    // domain );

    cout << "create functional" << endl;
    Dune::FieldVector< double, 1 > y0{0}, u0{0}, y_ref{1};
    Dune::FieldMatrix< double, 1, dim > dy0{0};
    // auto constraint = FunG::heatModel(c, d, y0, dy0);
    // auto costFunctional = tracking_type_cost_functional(alpha, y0, y_ref, u0);

    // std::function<void(const Dune::FieldVector<double,1>&)> update_reference =
    //      [&y_ref](const Dune::FieldVector<double,1>& ref){ y_ref = ref; };

    // Normal step functional with direct solver
    /*auto normalStepFunctional =
       NormalStepFunctional<Ids,decltype(constraint),decltype(costFunctional),Descriptions>
            (constraint, costFunctional, x_ref, update_reference);
            auto fn = Spacy::Kaskade::makeC2Functional( std::move(normalStepFunctional) , domain
       );*/

    auto fn = Spacy::Kaskade::makeC2Functional(
        NormalStepFunctional< stateId, controlId, adjointId, double, Descriptions >( alpha, x_ref,
                                                                                     c, d, e ),
        domain );
    // auto ft = Spacy::Kaskade::makeC2Functional(
    // TangentialStepFunctional<stateId,controlId,adjointId,double,Descriptions>(alpha,x_ref,c,d,e)
    // , domain ); => the
    // NSF_type should be the same as TSF_type

    //    using NSF_type = NormalStepFunctional<Ids,double,Descriptions>;
    using NSF_type = NormalStepFunctional< stateId, controlId, adjointId, double, Descriptions >;
    // using NSF_type =
    // NormalStepFunctional<Ids,decltype(constraint),decltype(costFunctional),Descriptions>;
    using FN_type = Spacy::Kaskade::C2Functional< NSF_type >;
    using FNLin = FN_type::Linearization;
    domain.setScalarProduct( Spacy::InducedScalarProduct( fn.hessian( zero( domain ) ) ) );

    std::function< Spacy::LinearSolver( const FNLin& ) > normalStepSolverCreator =
        [&desc, &gm]( const FNLin& f ) {
            std::vector< int > blockbounds = {};

            blockbounds.push_back( 0 );
            for ( int i = 0; i < desc.noOfVariables; i++ )
            {
                blockbounds.push_back( desc.degreesOfFreedom( i, i + 1 ) + blockbounds[ i ] );
            }

            using Numa_Mat = NumaBCRSMatrix< Dune::FieldMatrix< double, 1, 1 > >;
            auto& ma = f.get();

            auto A = getBlock( ma, 2, 0, blockbounds );
            A.setStartToZero();
            Numa_Mat A_M( A, true );
            auto bpx_A = makeBPX( A_M, gm );
            using bpx_A_type = decltype( bpx_A );

            auto AT = getBlock( ma, 0, 2, blockbounds );
            AT.setStartToZero();
            Numa_Mat AT_M( AT, true );
            auto bpx_AT = makeBPX( AT_M, gm );
            using bpx_AT_type = decltype( bpx_AT );

            typedef Spacy::Kaskade::IndirectBlockPreconditioner< NSF_type, bpx_A_type, bpx_AT_type >
                PreconditionerType;
            PreconditionerType P( f.A(), std::move( bpx_A ), std::move( bpx_AT ), desc, f.domain(),
                                  f.range() );

            auto P_shptr = std::make_shared< PreconditionerType >( P );

            using SV = ::Spacy::Vector;
            std::function< SV( const SV&, const SV& ) > the_r_operator =
                [P_shptr]( const SV& r_loc, const SV& g_loc ) -> SV {
                return P_shptr->update_r(); // does not need arguments anymore
            };

            std::function< SV( const SV&, const SV& ) > the_projection_operator =
                [P_shptr]( const SV& x_loc, const SV& dx_loc ) -> SV {
                return P_shptr->proj_to_kernel( x_loc, dx_loc );
            };

            std::function< SV( const SV& ) > the_precond_application =
                [P_shptr]( const SV& x_loc ) -> SV { return P_shptr->operator()( x_loc ); };

            std::function< SV( const SV&, const SV& ) > the_lagrange_update =
                [P_shptr]( const SV& x_loc, const SV& b_loc ) -> SV {
                return P_shptr->lagrange_update( x_loc, b_loc );
            };

            return Spacy::makePPCGSolver( f, the_precond_application, the_projection_operator,
                                          the_r_operator, the_lagrange_update, 1e-9, 1e-9, true );
        };

    cout << "make tangential functional " << endl;
    auto ft = Spacy::Kaskade::makeC2Functional(
        TangentialStepFunctional< stateId, controlId, adjointId, double, Descriptions >(
            alpha, x_ref, c, d, e ),
        domain );

    /*
        std::function< Spacy::LinearSolver( const FNLin& ) > myPCG2SolverCreator = [&desc]( const
       FNLin& f )
        {
            std::cout

                << "\n\n\n " << std::endl;
            Spacy::Kaskade::DirectBlockPreconditioner< NSF_type > P( f.A(), desc, f.domain(),
       f.range() );
            return Spacy::makeTCGSolver( f, P, 1e-9, 1e-9, true );
        };
    */

    fn.setSolverCreator( normalStepSolverCreator );
    ft.setSolverCreator( normalStepSolverCreator );

    /*
        auto convert_to_nice_vec = [&] (::Spacy::Vector vector)
        {
                using SpacyFunctional = typename ::Spacy::Kaskade::template C2Functional< NSF_type
       >;
                using AVD = typename SpacyFunctional::VariableSetDescription;
                AVD::template CoefficientVectorRepresentation<>::type x_( AVD::template
       CoefficientVectorRepresentation<>::init(
                                                        ::Spacy::Kaskade::extractSpaces< AVD >(
       domain )
                                                                                                ) );
                Spacy::Kaskade::copyToCoefficientVector< AVD >( vector, x_ );
                Dune::DynamicVector< double > vec( x_.dim(), 0 );
                IstlInterfaceDetail::toVector( x_, vec );

                return vec;
        };


            // Manual signle calculation testing
            Spacy::Vector pkt = ft.d1( Spacy::zero( domain ) );
            Spacy::PPCG::LinearSolver InvHesse =
            Spacy::cast_ref< Spacy::PPCG::LinearSolver >( ft.hessian( Spacy::zero( domain ) ) ^ -1
       );
            Spacy::Vector lin_in_pkt = InvHesse( pkt );


            writeMatlab( ft.hessian( Spacy::zero( domain ) ).get(), "mat_indBP.dat" );
            writeVec( convert_to_nice_vec(lin_in_pkt), "vec_indBP.dat" );


            ft.setSolverCreator( myPCG2SolverCreator );

            Spacy::Vector pkt_exct = ft.d1( Spacy::zero( domain ) );
            Spacy::CG::LinearSolver InvHesse_exct =
            Spacy::cast_ref< Spacy::CG::LinearSolver >( ft.hessian( Spacy::zero( domain ) ) ^ -1 );
            Spacy::Vector lin_in_pkt_exct = InvHesse_exct( pkt_exct );

            writeMatlab( ft.hessian( Spacy::zero( domain ) ).get(), "mat_dBP.dat" );
            writeVec( convert_to_nice_vec(lin_in_pkt_exct), "vec_dBP.dat" );

            std::cout
                            << "\n\n\nnorm ergebnis = \t " << norm( lin_in_pkt )
                            << "\nnorm ergebnis ex = \t " << norm( lin_in_pkt_exct )
                            << std::endl;
    */

    // /*
    cout << "set up solver" << endl;
    // algorithm and parameters
    auto cs = Spacy::CompositeStep::AffineCovariantSolver( fn, ft, domain );
    cs.tangentialSolverWasSet();
    cs.setRelativeAccuracy( desiredAccuracy );
    // cs.setEps(eps);
    cs.setVerbosityLevel( 2 );
    //     cs.setMaxSteps(maxSteps);
    cs.setMaxSteps( 2 );
    cs.setIterativeRefinements( iterativeRefinements );
    cs.setDesiredContraction( desContr );
    cs.setRelaxedDesiredContraction( relDesContr );
    cs.setMaximalContraction( maxContr );

    cout << "start solver" << endl;
    using namespace std::chrono;
    auto startTime = high_resolution_clock::now();
    auto result = cs();
    std::cout << "computation time: "
              << duration_cast< seconds >( high_resolution_clock::now() - startTime ).count()
              << "s." << std::endl;

    VarSet x( desc );
    Spacy::Kaskade::copy( result, x );

    IoOptions options;
    options.outputType = IoOptions::ascii;
    std::string outfilename( "nonlinear_control" );
    writeVTKFile( gm.grid().leafGridView(), x, outfilename, options, FEorder );
    writeVTKFile( gm.grid().leafGridView(), x_ref, "reference", options, FEorder );
    // */
}
