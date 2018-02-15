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

#include "Spacy/Adapter/KaskadeParabolic/Constraints/linearModelConstraint.hh"


using namespace Kaskade;

int main()
{
    constexpr int dim = 2;
    int refinements = 6;
    int order       = 1;
    unsigned no_time_steps = 11;
    double T_end = 1.;

    using Grid = Dune::UGGrid<dim>;
    using H1Space = FEFunctionSpace<ContinuousLagrangeMapper<double,Grid::LeafGridView> >;
    using Spaces = boost::fusion::vector<H1Space const*>;
    using VariableDescriptions =boost::fusion::vector<::Kaskade::VariableDescription<0,1,0> >;
    using VariableSetDesc = VariableSetDescription<Spaces,VariableDescriptions>;


    auto d = 1.;
    auto mu = 0.;

    using HeatFunctionalDefinition = LinearModelPDE<double,VariableSetDesc, VariableSetDesc>;
    std::function<HeatFunctionalDefinition(const VariableSetDesc::VariableSet )> forwardFunctionalGenerator
        = [&d,&mu](const VariableSetDesc::VariableSet control){
      return HeatFunctionalDefinition(d,mu,control);
    };


    ::Spacy::KaskadeParabolic::GridManager<Spaces> gm (no_time_steps,T_end,refinements,order);
    gm.getTempGrid().print();
    ::Spacy::VectorSpace domain = Spacy::KaskadeParabolic::makeHilbertSpace(gm);


    // Source constant in time and space
    auto source = zero(domain);
    auto& source_impl = ::Spacy::cast_ref<::Spacy::KaskadeParabolic::Vector<VariableSetDesc> >(source);
    for(auto i = 0u ; i < no_time_steps ; i++)
      source_impl.get_nonconst(i) = 1.;

    auto A = Spacy::KaskadeParabolic::makeC1Operator( forwardFunctionalGenerator, gm , domain , domain.dualSpace(), source);

    A.setVerbosity(false);

    auto p = Spacy::Newton::Parameter{};
    p.setRelativeAccuracy(1e-12);

    domain.setScalarProduct( Spacy::InducedScalarProduct( A.linearization(zero(domain))) );

    std::cout<<"Starting Newton "<<std::endl;
    auto x = ::Spacy::covariantNewton(A,p);

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

    ::Spacy::KaskadeParabolic::PDE::printNormSolution(x3,A.linearization(x3),gm,"sol");
    std::cout<<"returning from main"<<std::endl;



}
