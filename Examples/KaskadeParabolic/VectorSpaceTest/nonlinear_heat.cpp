#include <iostream>

#include <dune/grid/config.h>
#include <dune/grid/uggrid.hh>

#define SPACY_ENABLE_LOGGING
#include <Spacy/zeroVectorCreator.hh>
#include <Spacy/Adapter/kaskadeParabolic.hh>
#include <Spacy/Algorithm/Newton/newton.hh>
#include <Spacy/Util/cast.hh>
#include <Spacy/Util/log.hh>
#include <Spacy/inducedScalarProduct.hh>

#include <fem/gridmanager.hh>
#include <fem/lagrangespace.hh>
#include <fem/variables.hh>
#include <io/vtk.hh>
#include <utilities/gridGeneration.hh> //  createUnitSquare

#include "../fung_operator.hh"

#include <fung/fung.hh>
#include <fung/examples/nonlinear_heat.hh>

#include "nonlinear_heat.hh"

using namespace Kaskade;

int main()
{
    constexpr int dim = 2;
    int refinements = 6;
    int order       = 1;
    unsigned no_time_steps = 11;
    double dt_size = 0.1;

    using Grid = Dune::UGGrid<dim>;
    using H1Space = FEFunctionSpace<ContinuousLagrangeMapper<double,Grid::LeafGridView> >;
    using Spaces = boost::fusion::vector<H1Space const*>;
    using VariableDescriptions =boost::fusion::vector<::Kaskade::VariableDescription<0,1,0> >;
    using VariableSetDesc = VariableSetDescription<Spaces,VariableDescriptions>;


    double c = 0., d = 1.;

//    // With FunG (seems to be buggy though)
//    Dune::FieldVector<double,1> u0{0};
//    Dune::FieldMatrix<double,1,dim> du0{0};
//    auto integrand = FunG::heatModel(c, d, u0, du0);
//    auto F = FungOperator<decltype(integrand),VariableSetDesc>(integrand);
//    std::vector< FungOperator<decltype(integrand),VariableSetDesc> > FVec(no_time_steps, FungOperator<decltype(integrand),VariableSetDesc>(integrand));

    // WIthout FunG
    using Functional = HeatFunctional<double,VariableSetDesc>;
    // first argument nonlinearity, second singularity
    Functional F(1.,1.);



    ::Spacy::KaskadeParabolic::GridManager<Spaces> gm (no_time_steps,dt_size,refinements,order);
    auto domain = Spacy::KaskadeParabolic::makeHilbertSpace(gm);

    auto A = Spacy::KaskadeParabolic::makeC1Operator( F, gm , domain , domain.dualSpace() );

    A.setVerbosity(false);


    auto p = Spacy::Newton::Parameter{};
    p.setRelativeAccuracy(1e-12);

    domain.setScalarProduct( Spacy::InducedScalarProduct( A.linearization(zero(domain))) );

    std::cout<<"Starting Newton "<<std::endl;
    auto x = covariantNewton(A,p);

    auto& vc = ::Spacy::creator<::Spacy::KaskadeParabolic::VectorCreator<VariableSetDesc> >(domain);
    vc.refine(8);
    A.informAboutRefinement(8);
    vc.refine(5);
    A.informAboutRefinement(5);
    vc.refine(6);
    A.informAboutRefinement(6);
    vc.refine(1);
    A.informAboutRefinement(1);
    ::Spacy::KaskadeParabolic::PDE::writeVTK(Spacy::cast_ref< ::Spacy::KaskadeParabolic::Vector<VariableSetDesc> >(x), "refnonopt");

    std::cout<<"setting scalar product"<<std::endl;
    domain.setScalarProduct( Spacy::InducedScalarProduct( A.linearization(zero(domain))) );

    std::cout<<"Starting Newton again"<<std::endl;
    auto x3 = covariantNewton(A,x,p);

    ::Spacy::KaskadeParabolic::PDE::writeVTK(Spacy::cast_ref< ::Spacy::KaskadeParabolic::Vector<VariableSetDesc> >(x3), "refopt");
    std::cout<<"returning from main"<<std::endl;
}
