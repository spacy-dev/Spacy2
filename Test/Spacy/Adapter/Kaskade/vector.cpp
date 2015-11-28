// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include <gtest/gtest.h>

#include "dune/grid/config.h"
#include "dune/grid/uggrid.hh"

#include "fem/assemble.hh"
#include "fem/gridmanager.hh"
#include "fem/lagrangespace.hh"
#include "fem/variables.hh"
#include "utilities/gridGeneration.hh" //  createUnitSquare

//#include "Spacy/Adapter/Kaskade/vector.hh"
#include "Spacy/Adapter/Kaskade/vectorSpace.hh"
//#include "Spacy/Adapter/Eigen/vectorCreator.hh"

#include "Test/mockSetup.hh"
#include "setup.hh"

using namespace Kaskade;

TEST(Kaskade,CreateVectorCreator)
{
  constexpr int dim = 2, order = 1;

  using Grid = Dune::UGGrid<dim>;
  using LeafView = Grid::LeafGridView;
  using H1Space = FEFunctionSpace<ContinuousLagrangeMapper<double,LeafView> >;
  using Spaces = boost::fusion::vector<H1Space const*>;
  using VariableDescriptions = boost::fusion::vector<Variable<SpaceIndex<0>,Components<1>,VariableId<0> > >;
  using VariableSetDesc = VariableSetDescription<Spaces,VariableDescriptions>;
//  using CoefficientVectors = VariableSetDesc::CoefficientVectorRepresentation<0,1>::type;


  GridManager<Grid> gridManager( createUnitSquare<Grid>() );

  // construction of finite element space for the scalar solution u.
  H1Space temperatureSpace(gridManager,gridManager.grid().leafGridView(),order);
//  Spaces spaces(&temperatureSpace);
//  VariableSetDesc variableSetDesc(spaces,{ "u" });

  //construct Galerkin representation
//  VariableSetDesc::VariableSet u(variableSetDesc);
//  CoefficientVectors x(VariableSetDesc::CoefficientVectorRepresentation<>::init(spaces));

  Spacy::Kaskade::VectorCreator<VariableSetDesc> creator(temperatureSpace);

//  auto vec = testVector();

//  auto space = createMockBanachSpace();
//  Spacy::Rn::Vector spacyVector(vec,space);

//  ASSERT_EQ( spacyVector.impl()[0] , 1.);
//  ASSERT_EQ( spacyVector.impl()[1] , 2.);
}

//TEST(Rn,AssignFromEigen_VectorXd)
//{
//  auto vec = testVector();

//  auto space = createMockBanachSpace();
//  Spacy::Rn::Vector spacy_v(0*vec,space);

//  ASSERT_EQ( spacy_v.impl()[0] , 0.);
//  ASSERT_EQ( spacy_v.impl()[1] , 0.);

//  spacy_v = vec;
//  ASSERT_EQ( spacy_v.impl()[0] , 1.);
//  ASSERT_EQ( spacy_v.impl()[1] , 2.);
//}

//TEST(Rn,AddVectors)
//{
//  auto vec = testVector();

//  auto space = createMockBanachSpace();
//  Spacy::Rn::Vector spacy_v(vec,space);

//  spacy_v += spacy_v;
//  ASSERT_EQ( spacy_v.impl()[0] , 2.);
//  ASSERT_EQ( spacy_v.impl()[1] , 4.);
//}

//TEST(Rn,SubtractVectors)
//{
//  auto vec = testVector();

//  auto space = createMockBanachSpace();
//  Spacy::Rn::Vector spacy_v(vec,space);

//  spacy_v -= spacy_v;
//  ASSERT_EQ( spacy_v.impl()[0] , 0.);
//  ASSERT_EQ( spacy_v.impl()[1] , 0.);
//}

//TEST(Rn,MultiplyVectorWithScalar)
//{
//  auto vec = testVector();

//  auto space = createMockBanachSpace();
//  Spacy::Rn::Vector spacy_v(vec,space);

//  spacy_v *= 3;
//  ASSERT_EQ( spacy_v.impl()[0] , 3.);
//  ASSERT_EQ( spacy_v.impl()[1] , 6.);
//}

//TEST(Rn,ApplyAsDual)
//{
//  auto vec = testVector();

//  auto space = createMockBanachSpace();
//  Spacy::Rn::Vector spacy_v(vec,space);

//  auto dualPairing = spacy_v(spacy_v);
//  ASSERT_EQ( dualPairing , 5.);
//}

//TEST(Rn,Negation)
//{
//  auto vec = testVector();

//  auto space = createMockBanachSpace();
//  Spacy::Rn::Vector spacy_v(vec,space);

//  auto spacy_w = -spacy_v;
//  ASSERT_EQ( spacy_w.impl()[0] , -1.);
//  ASSERT_EQ( spacy_w.impl()[1] , -2.);
//}

//TEST(Rn,Comparison)
//{
//  auto vec = testVector();
//  vec[1] = 0;

//  auto space = Spacy::Rn::makeHilbertSpace(testDim());
//  Spacy::Rn::Vector spacy_v(vec,space), spacy_w(vec,space);

//  ASSERT_TRUE( spacy_v == spacy_w );

//  space.setEps(1e-5);
//  spacy_w.impl()[0] = 1 - 5e-6;

//  ASSERT_TRUE( spacy_v == spacy_w );
//  spacy_w.impl()[0] = 1 - 1.1e-5;
//  ASSERT_FALSE( spacy_v == spacy_w );
//}
