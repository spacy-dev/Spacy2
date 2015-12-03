// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include <gtest/gtest.h>

#include "setup.hh"
//#include "Spacy/Adapter/Kaskade/vector.hh"
#include "Spacy/Adapter/Kaskade/vectorSpace.hh"
#include "Spacy/Adapter/Kaskade/util.hh"
#include "Spacy/vectorSpace.hh"

#include "Test/mockSetup.hh"


using namespace Kaskade;

TEST(Kaskade,VectorCreator_Create)
{
  KASKADE_SINGLE_SPACE_SETUP

  Spacy::Kaskade::VectorCreator<Descriptions> creator(descriptions);
  ASSERT_EQ( creator.get().degreesOfFreedom() , temperatureSpace.degreesOfFreedom() );

  Spacy::VectorCreator spacyCreator(creator);
  ASSERT_EQ( spacyCreator.target<Spacy::Kaskade::VectorCreator<Descriptions>>()->get().degreesOfFreedom() , temperatureSpace.degreesOfFreedom() );
}


TEST(Kaskade,VectorCreator_CreateVector)
{
  KASKADE_SINGLE_SPACE_SETUP

  auto V = Spacy::Kaskade::makeHilbertSpace<Descriptions>(descriptions);

  auto v = V.zeroVector();

  ASSERT_EQ( norm(v) , 0. );

  auto& kv = *v.target< Spacy::Kaskade::Vector<Descriptions> >();

  boost::fusion::at_c<0>(kv.get().data)[testIndex()] = testValue();
  ASSERT_EQ( norm(v) , testValue() );
}

//TEST(Kaskade, ProductSpace_VectorCreator_Create)
//{
//  KASKADE_PRODUCT_SPACE_SETUP
//}

TEST(Kaskade,SingleSpace_CopyVector)
{
  KASKADE_SINGLE_SPACE_SETUP

  using boost::fusion::at_c;
  auto V = Spacy::Kaskade::makeHilbertSpace<Descriptions>(descriptions);
  auto v = V.zeroVector();
  auto& kv = *v.target< Spacy::Kaskade::Vector<Descriptions> >();
  at_c<0>(kv.get().data)[testIndex()] = testValue();
  Spacy::Kaskade::copyToCoefficientVector<Descriptions>(v,x);
  auto w = V.zeroVector();
  Spacy::Kaskade::copyFromCoefficientVector<Descriptions>(x,w);
  Spacy::Kaskade::copy(w,u);

  ASSERT_EQ( at_c<0>(Spacy::target<Spacy::Kaskade::Vector<Descriptions> >(w)->get().data)[testIndex()] , testValue() );
  ASSERT_EQ( at_c<0>(u.data).coefficients()[testIndex()][0] , testValue() );
}

TEST(Kaskade,ProductSpace_CopyVector_StateVariable)
{
  KASKADE_PRODUCT_SPACE_SETUP

  using boost::fusion::at_c;
  auto V = Spacy::Kaskade::makeHilbertSpace(descriptions, {stateId,controlId,adjointId});
  auto v = V.zeroVector();

  ASSERT_TRUE( Spacy::is<Spacy::ProductSpace::Vector>(v) );
  auto& v_ = *v.target<Spacy::ProductSpace::Vector>();
  ASSERT_EQ( v_.numberOfVariables() , 3u );
  using StateVector = Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,stateId> >;
  ASSERT_TRUE( Spacy::is<StateVector>(v_.component(stateId)) );
  auto& stateVector = *Spacy::target<StateVector>(v_.component(stateId));
  at_c<0>(stateVector.get().data)[testIndex()] = testValue();
  Spacy::Kaskade::copyToCoefficientVector<Descriptions>(v,x);

  ASSERT_EQ( norm(v) , testValue() );
  ASSERT_EQ( (x*x) , testValue()*testValue() );
  ASSERT_EQ( at_c<stateId>(x.data)[testIndex()][0] , testValue() );

  auto w = V.zeroVector();
  Spacy::Kaskade::copyFromCoefficientVector<Descriptions>(x,w);
  Spacy::Kaskade::copy(w,u);

  ASSERT_EQ( at_c<stateId>(u.data).coefficients()[testIndex()][0] , testValue() );
}

TEST(Kaskade,ProductSpace_CopyVector_ControlVariable)
{
  KASKADE_PRODUCT_SPACE_SETUP

  using boost::fusion::at_c;
  auto V = Spacy::Kaskade::makeHilbertSpace(descriptions, {stateId,controlId,adjointId});
  auto v = V.zeroVector();

  ASSERT_TRUE( Spacy::is<Spacy::ProductSpace::Vector>(v) );
  auto& v_ = *v.target<Spacy::ProductSpace::Vector>();
  ASSERT_EQ( v_.numberOfVariables() , 3u );
  using ControlVector = Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,controlId> >;
  ASSERT_TRUE( Spacy::is<ControlVector>(v_.component(controlId)) );
  auto& stateVector = *Spacy::target<ControlVector>(v_.component(controlId));
  at_c<0>(stateVector.get().data)[testIndex()] = testValue();
  Spacy::Kaskade::copyToCoefficientVector<Descriptions>(v,x);

  ASSERT_EQ( norm(v) , testValue() );
  ASSERT_EQ( (x*x) , testValue()*testValue() );
  ASSERT_EQ( at_c<controlId>(x.data)[testIndex()][0] , testValue() );

  auto w = V.zeroVector();
  Spacy::Kaskade::copyFromCoefficientVector<Descriptions>(x,w);
  Spacy::Kaskade::copy(w,u);

  ASSERT_EQ( at_c<controlId>(u.data).coefficients()[testIndex()][0] , testValue() );
}

TEST(Kaskade,ProductSpace_CopyVector_StateVariable_PermutedVariableOrder)
{
  KASKADE_PRODUCT_SPACE_SETUP_PERM_VARIABLE_ORDER

  using boost::fusion::at_c;
  auto V = Spacy::Kaskade::makeHilbertSpace(descriptions, {stateId,controlId,adjointId});
  auto v = V.zeroVector();

  ASSERT_TRUE( Spacy::is<Spacy::ProductSpace::Vector>(v) );
  auto& v_ = *v.target<Spacy::ProductSpace::Vector>();
  ASSERT_EQ( v_.numberOfVariables() , 3u );
  using StateVector = Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,stateId> >;
  ASSERT_TRUE( Spacy::is<StateVector>(v_.component(stateId)) );
  auto& stateVector = *Spacy::target<StateVector>(v_.component(stateId));
  at_c<0>(stateVector.get().data)[testIndex()] = testValue();
  Spacy::Kaskade::copyToCoefficientVector<Descriptions>(v,x);

  ASSERT_EQ( norm(v) , testValue() );
  ASSERT_EQ( (x*x) , testValue()*testValue() );
  ASSERT_EQ( at_c<stateId>(x.data)[testIndex()][0] , testValue() );

  auto w = V.zeroVector();
  Spacy::Kaskade::copyFromCoefficientVector<Descriptions>(x,w);
  Spacy::Kaskade::copy(w,u);

  ASSERT_EQ( at_c<stateId>(u.data).coefficients()[testIndex()][0] , testValue() );
}

TEST(Kaskade,PrimalDualProductSpace_CopyVector_StateVariable)
{
  KASKADE_PRODUCT_SPACE_SETUP

  using boost::fusion::at_c;
  auto V = Spacy::Kaskade::makeHilbertSpace(descriptions, {stateId,controlId}, {adjointId});
  auto v = V.zeroVector();

  ASSERT_TRUE( Spacy::is<Spacy::ProductSpace::Vector>(v) );
  auto& v_ = *v.target<Spacy::ProductSpace::Vector>();
  ASSERT_EQ( v_.numberOfVariables() , 2u );
  ASSERT_TRUE( Spacy::is<Spacy::ProductSpace::Vector>(v_.component(Spacy::PRIMAL)) );
  ASSERT_TRUE( Spacy::is<Spacy::ProductSpace::Vector>(v_.component(Spacy::DUAL)) );
  auto& primal = *Spacy::target<Spacy::ProductSpace::Vector>(v_.component(Spacy::PRIMAL));
  using StateVector = Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,stateId> >;
  ASSERT_TRUE( Spacy::is<StateVector>(primal.component(0)) );
  auto& stateVector = *Spacy::target<StateVector>(primal.component(0));
  at_c<0>(stateVector.get().data)[testIndex()] = testValue();
  Spacy::Kaskade::copyToCoefficientVector<Descriptions>(v,x);

  ASSERT_EQ( norm(v) , testValue() );
  ASSERT_EQ( (x*x) , testValue()*testValue() );
  ASSERT_EQ( at_c<stateId>(x.data)[testIndex()][0] , testValue() );

  auto w = V.zeroVector();
  Spacy::Kaskade::copyFromCoefficientVector<Descriptions>(x,w);
  Spacy::Kaskade::copy(w,u);

  ASSERT_EQ( at_c<stateId>(u.data).coefficients()[testIndex()][0] , testValue() );
}


TEST(Kaskade,PrimalDualProductSpace_CopyVector_StateVariable_PermutedVariableOrder)
{
  KASKADE_PRODUCT_SPACE_SETUP_PERM_VARIABLE_ORDER

  using boost::fusion::at_c;
  auto V = Spacy::Kaskade::makeHilbertSpace(descriptions, {stateId,controlId}, {adjointId});
  auto v = V.zeroVector();

  ASSERT_TRUE( Spacy::is<Spacy::ProductSpace::Vector>(v) );
  auto& v_ = *v.target<Spacy::ProductSpace::Vector>();
  ASSERT_EQ( v_.numberOfVariables() , 2u );
  ASSERT_TRUE( Spacy::is<Spacy::ProductSpace::Vector>(v_.component(Spacy::PRIMAL)) );
  ASSERT_TRUE( Spacy::is<Spacy::ProductSpace::Vector>(v_.component(Spacy::DUAL)) );
  auto& primal = *Spacy::target<Spacy::ProductSpace::Vector>(v_.component(Spacy::PRIMAL));
  using StateVector = Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,stateId> >;
  ASSERT_TRUE( Spacy::is<StateVector>(primal.component(0)) );
  auto& stateVector = *Spacy::target<StateVector>(primal.component(0));
  at_c<0>(stateVector.get().data)[testIndex()] = testValue();
  Spacy::Kaskade::copyToCoefficientVector<Descriptions>(v,x);

  ASSERT_EQ( norm(v) , testValue() );
  ASSERT_EQ( (x*x) , testValue()*testValue() );
  ASSERT_EQ( at_c<stateId>(x.data)[testIndex()][0] , testValue() );

  auto w = V.zeroVector();
  Spacy::Kaskade::copyFromCoefficientVector<Descriptions>(x,w);
  Spacy::Kaskade::copy(w,u);

  ASSERT_EQ( at_c<stateId>(u.data).coefficients()[testIndex()][0] , testValue() );
}

TEST(Kaskade,PrimalDualProductSpace_CopyVector_ControlVariable)
{
  KASKADE_PRODUCT_SPACE_SETUP

  using boost::fusion::at_c;
  auto V = Spacy::Kaskade::makeHilbertSpace(descriptions, {stateId,controlId}, {adjointId});
  auto v = V.zeroVector();

  ASSERT_TRUE( Spacy::is<Spacy::ProductSpace::Vector>(v) );
  auto& v_ = *v.target<Spacy::ProductSpace::Vector>();
  ASSERT_EQ( v_.numberOfVariables() , 2u );
  ASSERT_TRUE( Spacy::is<Spacy::ProductSpace::Vector>(v_.component(Spacy::PRIMAL)) );
  ASSERT_TRUE( Spacy::is<Spacy::ProductSpace::Vector>(v_.component(Spacy::DUAL)) );
  auto& primal = *Spacy::target<Spacy::ProductSpace::Vector>(v_.component(Spacy::PRIMAL));
  using ControlVector = Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,controlId> >;
  ASSERT_TRUE( Spacy::is<ControlVector>(primal.component(1)) );
  auto& stateVector = *Spacy::target<ControlVector>(primal.component(1));
  at_c<0>(stateVector.get().data)[testIndex()] = testValue();
  Spacy::Kaskade::copyToCoefficientVector<Descriptions>(v,x);

  ASSERT_EQ( norm(v) , testValue() );
  ASSERT_EQ( (x*x) , testValue()*testValue() );
  ASSERT_EQ( at_c<controlId>(x.data)[testIndex()][0] , testValue() );

  auto w = V.zeroVector();
  Spacy::Kaskade::copyFromCoefficientVector<Descriptions>(x,w);
  Spacy::Kaskade::copy(w,u);

  ASSERT_EQ( at_c<controlId>(u.data).coefficients()[testIndex()][0] , testValue() );
}

//TEST(Rn,AssignFromEigen_VectorXd)
//{
//  auto vec = testVector();

//  auto space = createMockBanachSpace();
//  Spacy::Rn::Vector spacy_v(0*vec,space);

//  ASSERT_EQ( spacy_v.get()[0] , 0.);
//  ASSERT_EQ( spacy_v.get()[1] , 0.);

//  spacy_v = vec;
//  ASSERT_EQ( spacy_v.get()[0] , 1.);
//  ASSERT_EQ( spacy_v.get()[1] , 2.);
//}

//TEST(Rn,AddVectors)
//{
//  auto vec = testVector();

//  auto space = createMockBanachSpace();
//  Spacy::Rn::Vector spacy_v(vec,space);

//  spacy_v += spacy_v;
//  ASSERT_EQ( spacy_v.get()[0] , 2.);
//  ASSERT_EQ( spacy_v.get()[1] , 4.);
//}

//TEST(Rn,SubtractVectors)
//{
//  auto vec = testVector();

//  auto space = createMockBanachSpace();
//  Spacy::Rn::Vector spacy_v(vec,space);

//  spacy_v -= spacy_v;
//  ASSERT_EQ( spacy_v.get()[0] , 0.);
//  ASSERT_EQ( spacy_v.get()[1] , 0.);
//}

//TEST(Rn,MultiplyVectorWithScalar)
//{
//  auto vec = testVector();

//  auto space = createMockBanachSpace();
//  Spacy::Rn::Vector spacy_v(vec,space);

//  spacy_v *= 3;
//  ASSERT_EQ( spacy_v.get()[0] , 3.);
//  ASSERT_EQ( spacy_v.get()[1] , 6.);
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
//  ASSERT_EQ( spacy_w.get()[0] , -1.);
//  ASSERT_EQ( spacy_w.get()[1] , -2.);
//}

//TEST(Rn,Comparison)
//{
//  auto vec = testVector();
//  vec[1] = 0;

//  auto space = Spacy::Rn::makeHilbertSpace(testDim());
//  Spacy::Rn::Vector spacy_v(vec,space), spacy_w(vec,space);

//  ASSERT_TRUE( spacy_v == spacy_w );

//  space.setEps(1e-5);
//  spacy_w.get()[0] = 1 - 5e-6;

//  ASSERT_TRUE( spacy_v == spacy_w );
//  spacy_w.get()[0] = 1 - 1.1e-5;
//  ASSERT_FALSE( spacy_v == spacy_w );
//}
