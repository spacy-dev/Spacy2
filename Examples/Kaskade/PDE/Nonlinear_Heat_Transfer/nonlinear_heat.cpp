#include <iostream>

#include <dune/grid/config.h>
#include <dune/grid/uggrid.hh>

#include <Spacy/Adapter/kaskade.hh>
#include <Spacy/Algorithm/Newton/newton.hh>
#include <Spacy/inducedScalarProduct.hh>

#include "fem/gridmanager.hh"
#include "fem/lagrangespace.hh"
#include "fem/variables.hh"
#include "io/vtk.hh"
#include "utilities/gridGeneration.hh" //  createUnitSquare

#include "nonlinear_heat.hh"

using namespace Kaskade;

int main()
{
  constexpr int dim = 2;
  int refinements = 8,
      order       = 1;

  using Grid = Dune::UGGrid<dim>;
  using H1Space = FEFunctionSpace<ContinuousLagrangeMapper<double,Grid::LeafGridView> >;
  using Spaces = boost::fusion::vector<H1Space const*>;
  using VariableDescriptions = boost::fusion::vector< Variable<SpaceIndex<0>,Components<1>,VariableId<0> > >;
  using VariableSetDesc = VariableSetDescription<Spaces,VariableDescriptions>;
  using Functional = HeatFunctional<double,VariableSetDesc>;


  GridManager<Grid> gridManager( createUnitSquare<Grid>(1.,false) );
  gridManager.globalRefine(refinements);

  H1Space temperatureSpace(gridManager,gridManager.grid().leafGridView(),order);

  Spaces spaces(&temperatureSpace);
  VariableSetDesc variableSetDesc(spaces,{ "u" });

  Functional F;
  
  // compute solution
  auto domain = Spacy::Kaskade::makeHilbertSpace<VariableSetDesc>( temperatureSpace );
  auto range = Spacy::Kaskade::makeHilbertSpace<VariableSetDesc>( temperatureSpace );
  Spacy::connect(domain,range);

  Spacy::C1Operator A = Spacy::Kaskade::makeC1Operator( F , domain , range );
  domain.setScalarProduct( Spacy::InducedScalarProduct( A.linearization(domain.zeroVector())) );
  
  auto p = Spacy::Newton::Parameter{};
  p.setVerbosity(true);
  p.setRelativeAccuracy(1e-12);

  auto x = Spacy::covariantNewton(A,p);
//  auto sol = Spacy::contravariantNewton(f,p);
//  auto sol = Spacy::localNewton(f,p);

  //construct Galerkin representation
  VariableSetDesc::VariableSet u(variableSetDesc);
  Spacy::Kaskade::copy(x,u);
  
  writeVTKFile(gridManager.grid().leafGridView(), u ,"temperature",IoOptions(),order);
}
