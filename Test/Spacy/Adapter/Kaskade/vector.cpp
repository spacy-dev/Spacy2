#include <Test/gtest.hh>

#include "setup.hh"
#include <Spacy/vectorSpace.hh>
#include <Spacy/zeroVectorCreator.hh>
#include <Spacy/Adapter/Kaskade/vectorSpace.hh>
#include <Spacy/Adapter/Kaskade/util.hh>
#include <Spacy/Util/cast.hh>

#include <Test/mockSetup.hh>


using namespace Kaskade;

TEST(Kaskade,VectorCreator_Create)
{
  KASKADE_SINGLE_SPACE_SETUP

  Spacy::Kaskade::VectorCreator<Descriptions> creator(descriptions);
  EXPECT_EQ( creator.get().degreesOfFreedom() , temperatureSpace.degreesOfFreedom() );

  Spacy::ZeroVectorCreator spacyCreator(creator);
  EXPECT_EQ( spacyCreator.target<Spacy::Kaskade::VectorCreator<Descriptions>>()->get().degreesOfFreedom() , temperatureSpace.degreesOfFreedom() );
}


TEST(Kaskade,VectorCreator_CreateVector)
{
  KASKADE_SINGLE_SPACE_SETUP

  auto V = Spacy::Kaskade::makeHilbertSpace<Descriptions>(descriptions);

  auto v = zero(V);

  EXPECT_DOUBLE_EQ( get(norm(v)) , 0 );

  auto& kv = Spacy::cast_ref< Spacy::Kaskade::Vector<Descriptions> >(v);

  boost::fusion::at_c<0>(kv.get().data)[testIndex()] = testValue();
  EXPECT_DOUBLE_EQ( get(norm(v)) , testValue() );
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
  auto v = zero(V);
  auto& kv = *v.target< Spacy::Kaskade::Vector<Descriptions> >();
  at_c<0>(kv.get().data)[testIndex()] = testValue();
  Spacy::Kaskade::copyToCoefficientVector<Descriptions>(v,x);
  auto w = zero(V);
  Spacy::Kaskade::copyFromCoefficientVector<Descriptions>(x,w);
  Spacy::Kaskade::copy(w,u);

  EXPECT_DOUBLE_EQ( at_c<0>(Spacy::cast_ref<Spacy::Kaskade::Vector<Descriptions> >(w).get().data)[testIndex()] , testValue() );
  EXPECT_DOUBLE_EQ( at_c<0>(u.data).coefficients()[testIndex()][0] , testValue() );
}

TEST(Kaskade,ProductSpace_CopyVector_StateVariable)
{
  KASKADE_PRODUCT_SPACE_SETUP

  using boost::fusion::at_c;
  auto V = Spacy::Kaskade::makeHilbertSpace(descriptions, {stateId,controlId,adjointId});
  auto v = zero(V);

  EXPECT_TRUE( Spacy::is<Spacy::ProductSpace::Vector>(v) );
  auto& v_ = Spacy::cast_ref<Spacy::ProductSpace::Vector>( v );
  EXPECT_EQ( v_.numberOfVariables() , 3u );
  using StateVector = Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,stateId> >;
  EXPECT_TRUE( Spacy::is<StateVector>(v_.component(stateId)) );
  auto& stateVector = Spacy::cast_ref<StateVector>(v_.component(stateId));
  at_c<0>(stateVector.get().data)[testIndex()] = testValue();
  Spacy::Kaskade::copyToCoefficientVector<Descriptions>(v,x);

  EXPECT_DOUBLE_EQ( norm(v).get() , testValue() );
  EXPECT_DOUBLE_EQ( (x*x) , testValue()*testValue() );
  EXPECT_DOUBLE_EQ( at_c<stateId>(x.data)[testIndex()][0] , testValue() );

  auto w = zero(V);
  Spacy::Kaskade::copyFromCoefficientVector<Descriptions>(x,w);
  Spacy::Kaskade::copy(w,u);

  EXPECT_DOUBLE_EQ( at_c<stateId>(u.data).coefficients()[testIndex()][0] , testValue() );
}

TEST(Kaskade,ProductSpace_CopyVector_ControlVariable)
{
  KASKADE_PRODUCT_SPACE_SETUP

  using boost::fusion::at_c;
  auto V = Spacy::Kaskade::makeHilbertSpace(descriptions, {stateId,controlId,adjointId});
  auto v = zero(V);

  EXPECT_TRUE( Spacy::is<Spacy::ProductSpace::Vector>(v) );
  auto& v_ = Spacy::cast_ref<Spacy::ProductSpace::Vector>(v);
  EXPECT_EQ( v_.numberOfVariables() , 3u );
  using ControlVector = Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,controlId> >;
  EXPECT_TRUE( Spacy::is<ControlVector>(v_.component(controlId)) );
  auto& stateVector = Spacy::cast_ref<ControlVector>(v_.component(controlId));
  at_c<0>(stateVector.get().data)[testIndex()] = testValue();
  Spacy::Kaskade::copyToCoefficientVector<Descriptions>(v,x);

  EXPECT_DOUBLE_EQ( norm(v).get() , testValue() );
  EXPECT_DOUBLE_EQ( (x*x) , testValue()*testValue() );
  EXPECT_DOUBLE_EQ( at_c<controlId>(x.data)[testIndex()][0] , testValue() );

  auto w = zero(V);
  Spacy::Kaskade::copyFromCoefficientVector<Descriptions>(x,w);
  Spacy::Kaskade::copy(w,u);

  EXPECT_DOUBLE_EQ( at_c<controlId>(u.data).coefficients()[testIndex()][0] , testValue() );
}

TEST(Kaskade,ProductSpace_CopyVector_StateVariable_PermutedVariableOrder)
{
  KASKADE_PRODUCT_SPACE_SETUP_PERM_VARIABLE_ORDER

  using boost::fusion::at_c;
  auto V = Spacy::Kaskade::makeHilbertSpace(descriptions, {stateId,controlId,adjointId});
  auto v = zero(V);

  EXPECT_TRUE( Spacy::is<Spacy::ProductSpace::Vector>(v) );
  auto& v_ = Spacy::cast_ref<Spacy::ProductSpace::Vector>(v);
  EXPECT_EQ( v_.numberOfVariables() , 3u );
  using StateVector = Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,stateId> >;
  EXPECT_TRUE( Spacy::is<StateVector>(v_.component(stateId)) );
  auto& stateVector = Spacy::cast_ref<StateVector>(v_.component(stateId));
  at_c<0>(stateVector.get().data)[testIndex()] = testValue();
  Spacy::Kaskade::copyToCoefficientVector<Descriptions>(v,x);

  EXPECT_DOUBLE_EQ( norm(v).get() , testValue() );
  EXPECT_DOUBLE_EQ( (x*x) , testValue()*testValue() );
  EXPECT_DOUBLE_EQ( at_c<stateId>(x.data)[testIndex()][0] , testValue() );

  auto w = zero(V);
  Spacy::Kaskade::copyFromCoefficientVector<Descriptions>(x,w);
  Spacy::Kaskade::copy(w,u);

  EXPECT_DOUBLE_EQ( at_c<stateId>(u.data).coefficients()[testIndex()][0] , testValue() );
}

TEST(Kaskade,PrimalDualProductSpace_CopyVector_StateVariable)
{
  KASKADE_PRODUCT_SPACE_SETUP

  using boost::fusion::at_c;
  auto V = Spacy::Kaskade::makeHilbertSpace(descriptions, {stateId,controlId}, {adjointId});
  auto v = zero(V);

  EXPECT_TRUE( Spacy::is<Spacy::ProductSpace::Vector>(v) );
  auto& v_ = Spacy::cast_ref<Spacy::ProductSpace::Vector>(v);
  EXPECT_EQ( v_.numberOfVariables() , 2u );
  EXPECT_TRUE( Spacy::is<Spacy::ProductSpace::Vector>(v_.component(Spacy::PRIMAL)) );
  EXPECT_TRUE( Spacy::is<Spacy::ProductSpace::Vector>(v_.component(Spacy::DUAL)) );
  auto& primal = Spacy::cast_ref<Spacy::ProductSpace::Vector>(v_.component(Spacy::PRIMAL));
  using StateVector = Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,stateId> >;
  EXPECT_TRUE( Spacy::is<StateVector>(primal.component(0)) );
  auto& stateVector = Spacy::cast_ref<StateVector>(primal.component(0));
  at_c<0>(stateVector.get().data)[testIndex()] = testValue();
  Spacy::Kaskade::copyToCoefficientVector<Descriptions>(v,x);

  EXPECT_DOUBLE_EQ( norm(v).get() , testValue() );
  EXPECT_DOUBLE_EQ( (x*x) , testValue()*testValue() );
  EXPECT_DOUBLE_EQ( at_c<stateId>(x.data)[testIndex()][0] , testValue() );

  auto w = zero(V);
  Spacy::Kaskade::copyFromCoefficientVector<Descriptions>(x,w);
  Spacy::Kaskade::copy(w,u);

  EXPECT_DOUBLE_EQ( at_c<stateId>(u.data).coefficients()[testIndex()][0] , testValue() );
}


TEST(Kaskade,PrimalDualProductSpace_CopyVector_StateVariable_PermutedVariableOrder)
{
  KASKADE_PRODUCT_SPACE_SETUP_PERM_VARIABLE_ORDER

  using boost::fusion::at_c;
  auto V = Spacy::Kaskade::makeHilbertSpace(descriptions, {stateId,controlId}, {adjointId});
  auto v = zero(V);

  EXPECT_TRUE( Spacy::is<Spacy::ProductSpace::Vector>(v) );
  auto& v_ = Spacy::cast_ref<Spacy::ProductSpace::Vector>(v);
  EXPECT_EQ( v_.numberOfVariables() , 2u );
  EXPECT_TRUE( Spacy::is<Spacy::ProductSpace::Vector>(v_.component(Spacy::PRIMAL)) );
  EXPECT_TRUE( Spacy::is<Spacy::ProductSpace::Vector>(v_.component(Spacy::DUAL)) );
  auto& primal = Spacy::cast_ref<Spacy::ProductSpace::Vector>(v_.component(Spacy::PRIMAL));
  using StateVector = Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,stateId> >;
  EXPECT_TRUE( Spacy::is<StateVector>(primal.component(0)) );
  auto& stateVector = Spacy::cast_ref<StateVector>(primal.component(0));
  at_c<0>(stateVector.get().data)[testIndex()] = testValue();
  Spacy::Kaskade::copyToCoefficientVector<Descriptions>(v,x);

  EXPECT_DOUBLE_EQ( get(norm(v)) , testValue() );
  EXPECT_DOUBLE_EQ( (x*x) , testValue()*testValue() );
  EXPECT_DOUBLE_EQ( at_c<stateId>(x.data)[testIndex()][0] , testValue() );

  auto w = zero(V);
  Spacy::Kaskade::copyFromCoefficientVector<Descriptions>(x,w);
  Spacy::Kaskade::copy(w,u);

  EXPECT_DOUBLE_EQ( at_c<stateId>(u.data).coefficients()[testIndex()][0] , testValue() );
}

TEST(Kaskade,PrimalDualProductSpace_CopyVector_ControlVariable)
{
  KASKADE_PRODUCT_SPACE_SETUP

  using boost::fusion::at_c;
  auto V = Spacy::Kaskade::makeHilbertSpace(descriptions, {stateId,controlId}, {adjointId});
  auto v = zero(V);

  EXPECT_TRUE( Spacy::is<Spacy::ProductSpace::Vector>(v) );
  auto& v_ = *v.target<Spacy::ProductSpace::Vector>();
  EXPECT_EQ( v_.numberOfVariables() , 2u );
  EXPECT_TRUE( Spacy::is<Spacy::ProductSpace::Vector>(v_.component(Spacy::PRIMAL)) );
  EXPECT_TRUE( Spacy::is<Spacy::ProductSpace::Vector>(v_.component(Spacy::DUAL)) );
  auto& primal = Spacy::cast_ref<Spacy::ProductSpace::Vector>(v_.component(Spacy::PRIMAL));
  using ControlVector = Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,controlId> >;
  EXPECT_TRUE( Spacy::is<ControlVector>(primal.component(1)) );
  auto& stateVector = Spacy::cast_ref<ControlVector>(primal.component(1));
  at_c<0>(stateVector.get().data)[testIndex()] = testValue();
  Spacy::Kaskade::copyToCoefficientVector<Descriptions>(v,x);

  EXPECT_DOUBLE_EQ( get(norm(v)) , testValue() );
  EXPECT_DOUBLE_EQ( (x*x) , testValue()*testValue() );
  EXPECT_DOUBLE_EQ( at_c<controlId>(x.data)[testIndex()][0] , testValue() );

  auto w = zero(V);
  Spacy::Kaskade::copyFromCoefficientVector<Descriptions>(x,w);
  Spacy::Kaskade::copy(w,u);

  EXPECT_DOUBLE_EQ( at_c<controlId>(u.data).coefficients()[testIndex()][0] , testValue() );
}

TEST(Kaskade,ProductSpace_CastVectorComponents)
{
    KASKADE_TWO_SPACE_SETUP

    using boost::fusion::at_c;
    auto V = Spacy::Kaskade::makeHilbertSpace(descriptions, {stateId,controlId} );

    auto w = zero(V);

    EXPECT_TRUE( Spacy::is<Spacy::ProductSpace::Vector>(w) );
    auto& w_ = Spacy::cast_ref<Spacy::ProductSpace::Vector>(w);
    EXPECT_EQ( w_.numberOfVariables(), 2 );

    auto state_component = w_.component(stateId);
    using StateVector = Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,stateId> >;
    EXPECT_TRUE( Spacy::is<StateVector>(state_component));

    auto control_component = w_.component(controlId);
    using ControlVector = Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,controlId> >;
    EXPECT_TRUE( Spacy::is<ControlVector>(control_component));
}


//TEST(Rn,AssignFromEigen_VectorXd)
//{
//  auto vec = testVector();

//  auto space = createMockBanachSpace();
//  Spacy::Rn::Vector spacy_v(0*vec,space);

//  EXPECT_EQ( spacy_v.get()[0] , 0.);
//  EXPECT_EQ( spacy_v.get()[1] , 0.);

//  spacy_v = vec;
//  EXPECT_EQ( spacy_v.get()[0] , 1.);
//  EXPECT_EQ( spacy_v.get()[1] , 2.);
//}

//TEST(Rn,AddVectors)
//{
//  auto vec = testVector();

//  auto space = createMockBanachSpace();
//  Spacy::Rn::Vector spacy_v(vec,space);

//  spacy_v += spacy_v;
//  EXPECT_EQ( spacy_v.get()[0] , 2.);
//  EXPECT_EQ( spacy_v.get()[1] , 4.);
//}

//TEST(Rn,SubtractVectors)
//{
//  auto vec = testVector();

//  auto space = createMockBanachSpace();
//  Spacy::Rn::Vector spacy_v(vec,space);

//  spacy_v -= spacy_v;
//  EXPECT_EQ( spacy_v.get()[0] , 0.);
//  EXPECT_EQ( spacy_v.get()[1] , 0.);
//}

//TEST(Rn,MultiplyVectorWithScalar)
//{
//  auto vec = testVector();

//  auto space = createMockBanachSpace();
//  Spacy::Rn::Vector spacy_v(vec,space);

//  spacy_v *= 3;
//  EXPECT_EQ( spacy_v.get()[0] , 3.);
//  EXPECT_EQ( spacy_v.get()[1] , 6.);
//}

//TEST(Rn,ApplyAsDual)
//{
//  auto vec = testVector();

//  auto space = createMockBanachSpace();
//  Spacy::Rn::Vector spacy_v(vec,space);

//  auto dualPairing = spacy_v(spacy_v);
//  EXPECT_EQ( dualPairing , 5.);
//}

//TEST(Rn,Negation)
//{
//  auto vec = testVector();

//  auto space = createMockBanachSpace();
//  Spacy::Rn::Vector spacy_v(vec,space);

//  auto spacy_w = -spacy_v;
//  EXPECT_EQ( spacy_w.get()[0] , -1.);
//  EXPECT_EQ( spacy_w.get()[1] , -2.);
//}

//TEST(Rn,Comparison)
//{
//  auto vec = testVector();
//  vec[1] = 0;

//  auto space = Spacy::Rn::makeHilbertSpace(testDim());
//  Spacy::Rn::Vector spacy_v(vec,space), spacy_w(vec,space);

//  EXPECT_TRUE( spacy_v == spacy_w );

//  space.setEps(1e-5);
//  spacy_w.get()[0] = 1 - 5e-6;

//  EXPECT_TRUE( spacy_v == spacy_w );
//  spacy_w.get()[0] = 1 - 1.1e-5;
//  EXPECT_FALSE( spacy_v == spacy_w );
//}
