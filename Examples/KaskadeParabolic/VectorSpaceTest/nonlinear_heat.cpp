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

using namespace Kaskade;

int main()
{
    constexpr int dim = 2;
    int refinements = 6;
    int order       = 1;
    unsigned no_time_steps = 11;
    ::Spacy::Real dt_size = 0.1;

    using Grid = Dune::UGGrid<dim>;
    using H1Space = FEFunctionSpace<ContinuousLagrangeMapper<double,Grid::LeafGridView> >;
    using Spaces = boost::fusion::vector<H1Space const*>;
    using VariableDescriptions = boost::fusion::vector< Variable<SpaceIndex<0>,Components<1>,VariableId<0> > >;
    using VariableSetDesc = VariableSetDescription<Spaces,VariableDescriptions>;

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    /* * * * * * * * * * * * * * * * * * * * * * grid generation * * * * * * * * * * * * * * * * * * * * * */
    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    std::vector<::Spacy::Real> dt(no_time_steps, ::Spacy::Real(dt_size));
    dt.at(0) = 0;

    std::vector<std::unique_ptr<Grid> > grids;
    std::vector<std::unique_ptr<GridManager < Grid> > > gm;

    for (auto i = 0; i < no_time_steps; i++) {
        gm.push_back(std::move(std::make_unique<GridManager < Grid> > (createUnitSquare<Grid>(1., false))));
        gm.at(i)->globalRefine(refinements);
        gm.at(i)->enforceConcurrentReads(true);
    }

//    for (auto i = 0; i < gm.size(); i++)
//        std::cout << "Size of grid at timestep " << i << ": " << gm.at(i)->grid().leafGridView().size(dim) << std::endl;

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    /* * * * * * * * * * * * * * * * * * * * * * function spaces * * * * * * * * * * * * * * * * * * * * * */
    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

    std::vector<std::shared_ptr<H1Space> > spacesVecHelper_{};
    spacesVecHelper_.reserve(no_time_steps);

    std::vector<std::shared_ptr<Spaces> > spacesVec_{};
    spacesVec_.reserve(no_time_steps);

    std::vector<VariableSetDesc> descriptionsVec_{};
    descriptionsVec_.reserve(no_time_steps);


    for (unsigned k = 0; k < no_time_steps; k++) {
      spacesVecHelper_.emplace_back(std::make_shared<H1Space> (H1Space(*gm.at(k), gm.at(k)->grid().leafGridView(), order)));
      spacesVec_.emplace_back(std::make_shared<Spaces>(Spaces(&(*(spacesVecHelper_.back())))));
      descriptionsVec_.emplace_back(VariableSetDesc(*spacesVec_.back(), {"u"}));
    }

    auto domain = ::Spacy::KaskadeParabolic::makeHilbertSpace<VariableSetDesc>( descriptionsVec_ , spacesVec_);

    std::cout<<"Creating zero"<<std::endl;
        auto y = zero(domain);

        {
          auto z = y;
        }
     auto& vc = ::Spacy::creator<::Spacy::KaskadeParabolic::VectorCreator<VariableSetDesc> >(domain);
     vc.refine(5);
    auto z = zero(domain);
    std::cout<<"adding"<<std::endl;

    y += z;
    y -= z;
    y*=1;
//    std::cout<<std::boolalpha<<(y==z)<<std::endl;
    std::cout<<"Norm is "<<std::endl;
    std::cout<<y(y)<<norm(y)<<std::endl;
    vc.refine(3);



//    double c = 1e-1, d = 1e2;
//    Dune::FieldVector<double,1> u0{0};
//    Dune::FieldMatrix<double,1,dim> du0{0};
//    auto integrand = FunG::heatModel(c, d, u0, du0);
//    auto F = FungOperator<decltype(integrand),VariableSetDesc>(integrand);


//    // compute solution
//    auto domain = Spacy::Kaskade::makeHilbertSpace<VariableSetDesc>( variableSetDesc );

//    Spacy::C1Operator A = Spacy::Kaskade::makeC1Operator( F , domain , domain.dualSpace() );
//    domain.setScalarProduct( Spacy::InducedScalarProduct( A.linearization(zero(domain))) );

//    auto p = Spacy::Newton::Parameter{};
//    p.setRelativeAccuracy(1e-12);

//    auto x = covariantNewton(A,p);
//    //  auto x = Spacy::contravariantNewton(A,p);
//    //  auto x = Spacy::localNewton(A,p);

//    //construct Galerkin representation
//    writeVTK(Spacy::cast_ref< Spacy::Kaskade::Vector<VariableSetDesc> >(x), "temperature");
    std::cout<<"returning from main"<<std::endl;
}
