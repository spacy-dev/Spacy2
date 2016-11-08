#include <iostream>

#include <dune/grid/config.h>
#include <dune/grid/uggrid.hh>

#define SPACY_ENABLE_LOGGING
#include <Spacy/zeroVectorCreator.hh>
#include <Spacy/Adapter/kaskade.hh>
#include <Spacy/Algorithm/Newton/newton.hh>
#include <Spacy/Util/cast.hh>
#include <Spacy/inducedScalarProduct.hh>

#include <fem/gridmanager.hh>
#include <fem/lagrangespace.hh>
#include <fem/variables.hh>
#include <io/vtk.hh>
#include <utilities/gridGeneration.hh> //  createUnitSquare

#include "../fung_functional.hh"

#include <fung/fung.hh>
#include <fung/examples/biomechanics/adipose_tissue_sommer_holzapfel.hh>
#include <fung/examples/rubber/neo_hooke.hh>


using namespace Kaskade;

int main()
{
    SET_LOG_PRINTER(Spacy::Log::StreamPrinter());

    constexpr int dim = 3;
    int refinements = 4;
    int order       = 1;

    using Grid = Dune::UGGrid<dim>;
    using H1Space = FEFunctionSpace<ContinuousLagrangeMapper<double,Grid::LeafGridView> >;
    using Spaces = boost::fusion::vector<H1Space const*>;
    using VariableDescriptions = boost::fusion::vector< Variable<SpaceIndex<0>,Components<dim>,VariableId<0> > >;
    using VariableSetDesc = VariableSetDescription<Spaces,VariableDescriptions>;


    GridManager<Grid> gridManager( createUnitCube<Grid>(1.,false) );
    gridManager.globalRefine(refinements);

    H1Space temperatureSpace(gridManager,gridManager.grid().leafGridView(),order);

    Spaces spaces(&temperatureSpace);
    VariableSetDesc variableSetDesc(spaces,{ "u" });


    using Matrix = Dune::FieldMatrix<double,dim,dim>;
    // fiber tensor for 'fibers' aligned with x-axes
    auto fiberTensor = Matrix(0);
    fiberTensor[0][0] = 1;
    auto y0 = FunG::LinearAlgebra::unitMatrix<Matrix>();
//    auto integrand = FunG::incompressibleNeoHooke(1.0,y0);
    auto integrand = FunG::incompressibleAdiposeTissue_SommerHolzapfel(fiberTensor,y0);
    auto F = FungFunctional<decltype(integrand),VariableSetDesc>(integrand);


    // compute solution
    auto domain = Spacy::Kaskade::makeHilbertSpace<VariableSetDesc>( variableSetDesc );
    auto range = Spacy::Kaskade::makeHilbertSpace<VariableSetDesc>( variableSetDesc );
    connectAsPrimalDualPair(domain,range);

    Spacy::C1Operator A = Spacy::Kaskade::makeC1Operator( F , domain , range );
    domain.setScalarProduct( Spacy::InducedScalarProduct( A.linearization(zero(domain))) );

    auto p = Spacy::Newton::Parameter{};
    p.setVerbosity(true);
    p.setRelativeAccuracy(1e-6);

    auto x = covariantNewton(A,p);
    //  auto x = Spacy::contravariantNewton(A,p);
    //  auto x = Spacy::localNewton(A,p);

    //construct Galerkin representation
    writeVTK(Spacy::cast_ref< Spacy::Kaskade::Vector<VariableSetDesc> >(x), "displacement");
}
