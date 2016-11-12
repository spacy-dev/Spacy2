#pragma once

#include <dune/grid/config.h>
#include <dune/grid/uggrid.hh>

#include <fem/assemble.hh>
#include <fem/istlinterface.hh>
#include <fem/gridmanager.hh>
#include <fem/functionspace.hh>
#include <fem/lagrangespace.hh>
#include <fem/variables.hh>
#include <utilities/gridGeneration.hh> //  createUnitSquare
#include "laplace.hh"
#include "test_functional.hh"
#include "costFunctional.hh"

inline double testValue() { return 2.; }

constexpr int testIndex() { return 2; }

#define KASKADE_SINGLE_SPACE_SETUP \
  constexpr int dim = 2, order = 1; \
  using Grid = Dune::UGGrid<dim>; \
  using LeafView = Grid::LeafGridView; \
  using H1Space = FEFunctionSpace<ContinuousLagrangeMapper<double,LeafView> >; \
  using Spaces = boost::fusion::vector<H1Space const*>; \
  using VariableDescriptions = boost::fusion::vector<Variable<SpaceIndex<0>,Components<1>,VariableId<0> > >; \
  using Descriptions = VariableSetDescription<Spaces,VariableDescriptions>; \
  using Functional = HeatFunctional<double,Descriptions>; \
  using CoefficientVectors = Descriptions::CoefficientVectorRepresentation<0,1>::type; \
  \
  GridManager<Grid> gridManager( createUnitSquare<Grid>() ); \
  \
  H1Space temperatureSpace(gridManager,gridManager.grid().leafGridView(),order); \
  Spaces spaces(&temperatureSpace); \
  Descriptions descriptions(spaces,{ "u" }); \
  \
  Descriptions::VariableSet u(descriptions); \
  CoefficientVectors x(Descriptions::CoefficientVectorRepresentation<>::init(spaces));

#define KASKADE_SINGLE_SPACE_SETUP_TEST_FUNCTIONAL \
  constexpr int dim = 2, order = 1; \
  using Grid = Dune::UGGrid<dim>; \
  using LeafView = Grid::LeafGridView; \
  using H1Space = FEFunctionSpace<ContinuousLagrangeMapper<double,LeafView> >; \
  using Spaces = boost::fusion::vector<H1Space const*>; \
  using VariableDescriptions = boost::fusion::vector<Variable<SpaceIndex<0>,Components<1>,VariableId<0> > >; \
  using Descriptions = VariableSetDescription<Spaces,VariableDescriptions>; \
  using Functional = TestFunctional<double,Descriptions>; \
  using CoefficientVectors = Descriptions::CoefficientVectorRepresentation<0,1>::type; \
  \
  GridManager<Grid> gridManager( createUnitSquare<Grid>() ); \
  \
  H1Space temperatureSpace(gridManager,gridManager.grid().leafGridView(),order); \
  Spaces spaces(&temperatureSpace); \
  Descriptions descriptions(spaces,{ "u" }); \
  \
  Descriptions::VariableSet u(descriptions); \
  CoefficientVectors x(Descriptions::CoefficientVectorRepresentation<>::init(spaces));

#define KASKADE_SINGLE_SPACE_OPERATOR Functional F;

#define KASKADE_COST_FUNCTIONAL CostFunctional<Descriptions> F;

#define KASKADE_TWO_SPACE_SETUP \
  constexpr int dim = 2, order = 1, stateId = 0, controlId = 1; \
  using Grid = Dune::UGGrid<dim>; \
  using GridView = Grid::LeafGridView; \
  using H1Space = FEFunctionSpace<ContinuousLagrangeMapper<double,GridView> >; \
  using Spaces = boost::fusion::vector<const H1Space*>; \
  using VState = VariableDescription<0,1,stateId>; \
  using VControl = VariableDescription<0,1,controlId>; \
  using VariableDescriptions = boost::fusion::vector< VState, VControl>; \
  using Descriptions = VariableSetDescription<Spaces,VariableDescriptions>; \
  using VarSet = Descriptions::VariableSet; \
  using CoefficientVectors = Descriptions::CoefficientVectorRepresentation<>::type; \
  \
  GridManager<Grid> gm( createUnitSquare<Grid>(1.,false)); \
  \
  const auto& gridView = gm.grid().leafGridView(); \
  H1Space space(gm,gridView,order); \
  Spaces spaces(&space); \
  std::vector<std::string> names = {"y","u"}; \
  Descriptions descriptions(spaces,names); \
  \
  VarSet u(descriptions); \
  CoefficientVectors x(Descriptions::CoefficientVectorRepresentation<>::init(spaces));

#define KASKADE_PRODUCT_SPACE_SETUP \
  constexpr int dim = 2, order = 1, stateId = 0, controlId = 1, adjointId = 2; \
  using Grid = Dune::UGGrid<dim>; \
  using GridView = Grid::LeafGridView; \
  using H1Space = FEFunctionSpace<ContinuousLagrangeMapper<double,GridView> >; \
  using Spaces = boost::fusion::vector<const H1Space*>; \
  using VState = VariableDescription<0,1,stateId>; \
  using VControl = VariableDescription<0,1,controlId>; \
  using VAdjoint = VariableDescription<0,1,adjointId>; \
  using VariableDescriptions = boost::fusion::vector< VState, VControl, VAdjoint>; \
  using Descriptions = VariableSetDescription<Spaces,VariableDescriptions>; \
  using VarSet = Descriptions::VariableSet; \
  using CoefficientVectors = Descriptions::CoefficientVectorRepresentation<>::type; \
  \
  GridManager<Grid> gm( createUnitSquare<Grid>(1.,false)); \
  \
  const auto& gridView = gm.grid().leafGridView(); \
  H1Space space(gm,gridView,order); \
  Spaces spaces(&space); \
  std::vector<std::string> names = {"y","u","p"}; \
  Descriptions descriptions(spaces,names); \
  \
  VarSet u(descriptions); \
  CoefficientVectors x(Descriptions::CoefficientVectorRepresentation<>::init(spaces));


#define KASKADE_PRODUCT_SPACE_SETUP_PERM_VARIABLE_ORDER \
  constexpr int dim = 2, order = 1, stateId = 1, controlId = 0, adjointId = 2; \
  using Grid = Dune::UGGrid<dim>; \
  using GridView = Grid::LeafGridView; \
  using H1Space = FEFunctionSpace<ContinuousLagrangeMapper<double,GridView> >; \
  using Spaces = boost::fusion::vector<const H1Space*>; \
  using VState = VariableDescription<0,1,stateId>; \
  using VControl = VariableDescription<0,1,controlId>; \
  using VAdjoint = VariableDescription<0,1,adjointId>; \
  using VariableDescriptions = boost::fusion::vector< VControl, VState, VAdjoint>; \
  using Descriptions = VariableSetDescription<Spaces,VariableDescriptions>; \
  using VarSet = Descriptions::VariableSet; \
  using CoefficientVectors = Descriptions::CoefficientVectorRepresentation<>::type; \
  \
  GridManager<Grid> gm( createUnitSquare<Grid>(1.,false)); \
  \
  const auto& gridView = gm.grid().leafGridView(); \
  H1Space space(gm,gridView,order); \
  Spaces spaces(&space); \
  std::vector<std::string> names = {"u","y","p"}; \
  Descriptions descriptions(spaces,names); \
  \
  VarSet u(descriptions); \
  CoefficientVectors x(Descriptions::CoefficientVectorRepresentation<>::init(spaces));
//inline auto testDim()
//{
//  return 2;
//}

//inline auto testXValue()
//{
//  return 1;
//}

//inline auto testYValue()
//{
//  return 2;
//}

//inline auto testVector()
//{
//  Eigen::VectorXd vec(testDim());
//  vec << testXValue() , testYValue();
//  return vec;
//}

//inline auto testMatrix()
//{
//  Eigen::MatrixXd mat(testDim(),testDim());
//  mat << 1, 0, 1, 2;
//  return mat;
//}

//inline auto testOperator(const Spacy::VectorSpace& domain, const Spacy::VectorSpace& range)
//{
//  return Spacy::Rn::C1Operator( [](const auto& x){ return 1.1*x; } ,
//                                [](const auto& x){ return 1.1*::Eigen::MatrixXd::Identity(x.size(),x.size()); },
//                                domain, range);
//}
