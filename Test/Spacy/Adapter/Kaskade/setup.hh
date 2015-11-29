// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_TEST_ADAPTER_KASKADE_SETUP_HH
#define SPACY_TEST_ADAPTER_KASKADE_SETUP_HH

#include "dune/grid/config.h"
#include "dune/grid/uggrid.hh"

#include "fem/assemble.hh"
#include "fem/istlinterface.hh"
#include "fem/gridmanager.hh"
#include "fem/lagrangespace.hh"
#include "fem/variables.hh"
#include "utilities/gridGeneration.hh" //  createUnitSquare
#include "laplace.hh"

#define KASKADE_SINGLE_SPACE_SETUP \
  constexpr int dim = 2, order = 1; \
  using Grid = Dune::UGGrid<dim>; \
  using LeafView = Grid::LeafGridView; \
  using H1Space = FEFunctionSpace<ContinuousLagrangeMapper<double,LeafView> >; \
  using Spaces = boost::fusion::vector<H1Space const*>; \
  using VariableDescriptions = boost::fusion::vector<Variable<SpaceIndex<0>,Components<1>,VariableId<0> > >; \
  using VariableSetDesc = VariableSetDescription<Spaces,VariableDescriptions>; \
  using Functional = HeatFunctional<double,VariableSetDesc>; \
  using CoefficientVectors = VariableSetDesc::CoefficientVectorRepresentation<0,1>::type; \
  \
  GridManager<Grid> gridManager( createUnitSquare<Grid>() ); \
  \
  H1Space temperatureSpace(gridManager,gridManager.grid().leafGridView(),order); \
  Spaces spaces(&temperatureSpace); \
  VariableSetDesc variableSetDesc(spaces,{ "u" }); \
  \
  VariableSetDesc::VariableSet u(variableSetDesc); \
  CoefficientVectors x(VariableSetDesc::CoefficientVectorRepresentation<>::init(spaces));

#define KASKADE_SINGLE_SPACE_OPERATOR \
  Functional F; \


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


#endif // SPACY_TEST_ADAPTER_EIGEN_SETUP_HH

