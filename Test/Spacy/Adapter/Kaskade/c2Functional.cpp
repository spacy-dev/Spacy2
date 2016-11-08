#include <Test/gtest.hh>

#include "setup.hh"

#include <Spacy/c2Functional.hh>
#include <Spacy/zeroVectorCreator.hh>
#include <Spacy/Adapter/Kaskade/c2Functional.hh>
#include <Spacy/Util/cast.hh>

TEST(Kaskade,DISABLED_C2Functional_CostFunctional_CreateAndApply)
{
  KASKADE_TWO_SPACE_SETUP
  KASKADE_COST_FUNCTIONAL

  using boost::fusion::at_c;
  auto V = Spacy::Kaskade::makeHilbertSpace(descriptions, {stateId,controlId} );
  Spacy::C2Functional f = Spacy::Kaskade::makeC2Functional(F,V);

  auto r = f(zero(V));
  EXPECT_DOUBLE_EQ( toDouble(norm(r)) , 0. );

  auto w = zero(V);
  auto& w_ = Spacy::cast_ref<Spacy::ProductSpace::Vector>(w);
  auto& w_state = Spacy::cast_ref<Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,stateId> > >(w_.component(stateId));
  auto& w_control = Spacy::cast_ref<Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,controlId> > >(w_.component(controlId));
  for(auto i=0u; i< at_c<0>(w_state.get().data).size(); ++i)
    at_c<0>(w_state.get().data)[i] = 1;

  EXPECT_DOUBLE_EQ( get(f(w)), .5 );

  for(auto i=0u; i< at_c<0>(w_control.get().data).size(); ++i)
    at_c<0>(w_control.get().data)[i] = 1;

  EXPECT_DOUBLE_EQ( get(f(w)), 1. );
}

TEST(Kaskade,DISABLED_C2Functional_CostFunctional_PrimalDual_CreateAndApply)
{
  KASKADE_TWO_SPACE_SETUP
  KASKADE_COST_FUNCTIONAL

  using boost::fusion::at_c;
  auto V = Spacy::Kaskade::makeHilbertSpace(descriptions, {stateId} , {controlId} );
  Spacy::C2Functional f = Spacy::Kaskade::makeC2Functional(F,V);

  auto r = f(zero(V));
  EXPECT_DOUBLE_EQ( toDouble(norm(r)) , 0. );

  auto w = zero(V);
  auto& w_ = Spacy::cast_ref<Spacy::ProductSpace::Vector>(w);
  auto& w_primal = Spacy::cast_ref<Spacy::ProductSpace::Vector>(w_.component(Spacy::PRIMAL));
  auto& w_dual = Spacy::cast_ref<Spacy::ProductSpace::Vector>(w_.component(Spacy::DUAL));
  auto& w_state = Spacy::cast_ref<Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,stateId> > >(w_primal.component(0));
  auto& w_control = Spacy::cast_ref<Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,controlId> > >(w_dual.component(0));
  for(auto i=0u; i< at_c<0>(w_state.get().data).size(); ++i)
    at_c<0>(w_state.get().data)[i] = 1;

  EXPECT_DOUBLE_EQ( get(f(w)), .5 );

  for(auto i=0u; i< at_c<0>(w_control.get().data).size(); ++i)
    at_c<0>(w_control.get().data)[i] = 1;

  EXPECT_DOUBLE_EQ( toDouble(f(w)), 1. );
}

TEST(Kaskade,DISABLED_C2Functional_CostFunctional_D1)
{
  KASKADE_TWO_SPACE_SETUP
  KASKADE_COST_FUNCTIONAL

  using boost::fusion::at_c;
  auto V = Spacy::Kaskade::makeHilbertSpace(descriptions, {stateId,controlId} );
  Spacy::C2Functional f = Spacy::Kaskade::makeC2Functional(F,V);

  auto w = zero(V);
  auto& w_ = Spacy::cast_ref<Spacy::ProductSpace::Vector>(w);
  auto& ws_ = Spacy::cast_ref<Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,stateId> > >(w_.component(stateId));
  auto& wd_ = Spacy::cast_ref<Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,controlId> > >(w_.component(controlId));
  for(auto i=0u; i< at_c<0>(ws_.get().data).size(); ++i)
    at_c<0>(ws_.get().data)[i] = 1;

  EXPECT_DOUBLE_EQ( get( f.d1(w)(w) ) , 1. );
  EXPECT_DOUBLE_EQ( get( d1(f,w)(w) ) , 1. );

  for(auto i=0u; i< at_c<0>(wd_.get().data).size(); ++i)
    at_c<0>(wd_.get().data)[i] = 1;

  EXPECT_DOUBLE_EQ( get(f.d1(w)(w)) , 2. );
  EXPECT_DOUBLE_EQ( get(d1(f,w)(w)) , 2. );
}

TEST(Kaskade,DISABLED_C2Functional_CostFunctional_PrimalDual_D1)
{
  KASKADE_TWO_SPACE_SETUP
  KASKADE_COST_FUNCTIONAL

  using boost::fusion::at_c;
  auto V = Spacy::Kaskade::makeHilbertSpace(descriptions, {stateId} , {controlId} );
  Spacy::C2Functional f = Spacy::Kaskade::makeC2Functional(F,V);

  auto w = zero(V);
  auto& w_ = Spacy::cast_ref<Spacy::ProductSpace::Vector>(w);
  auto& wp = Spacy::cast_ref<Spacy::ProductSpace::Vector>(w_.component(Spacy::PRIMAL));
  auto& wd = Spacy::cast_ref<Spacy::ProductSpace::Vector>(w_.component(Spacy::DUAL));
  auto& ws_ = Spacy::cast_ref<Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,stateId> > >(wp.component(0));
  auto& wd_ = Spacy::cast_ref<Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,controlId> > >(wd.component(0));
  for(auto i=0u; i< at_c<0>(ws_.get().data).size(); ++i)
    at_c<0>(ws_.get().data)[i] = 1;

  EXPECT_DOUBLE_EQ( get( f.d1(w)(w) ) , 1. );
  EXPECT_DOUBLE_EQ( get( d1(f,w)(w) ) , 1. );

  for(auto i=0u; i< at_c<0>(wd_.get().data).size(); ++i)
    at_c<0>(wd_.get().data)[i] = 1;

  EXPECT_DOUBLE_EQ( get(f.d1(w)(w)) , 2. );
  EXPECT_DOUBLE_EQ( get(d1(f,w)(w)) , 2. );
}

TEST(Kaskade,DISABLED_C2Functional_CostFunctional_D2)
{
  KASKADE_TWO_SPACE_SETUP
  KASKADE_COST_FUNCTIONAL

  using boost::fusion::at_c;
  auto V = Spacy::Kaskade::makeHilbertSpace(descriptions, {stateId,controlId} );
  Spacy::C2Functional f = Spacy::Kaskade::makeC2Functional(F,V);

  auto w = zero(V);
  auto& w_ = Spacy::cast_ref<Spacy::ProductSpace::Vector>(w);
  auto& ws_ = Spacy::cast_ref<Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,stateId> > >(w_.component(stateId));
  auto& wd_ = Spacy::cast_ref<Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,controlId> > >(w_.component(controlId));
  for(auto i=0u; i< at_c<0>(ws_.get().data).size(); ++i)
    at_c<0>(ws_.get().data)[i] = 1;

  EXPECT_DOUBLE_EQ( toDouble( f.d2(w,w)(w) ) , 1. );
  EXPECT_DOUBLE_EQ( toDouble( d2(f,w)(w)(w) ) , 1. );

  for(auto i=0u; i< at_c<0>(wd_.get().data).size(); ++i)
    at_c<0>(wd_.get().data)[i] = 1;

  EXPECT_DOUBLE_EQ( toDouble(f.d2(w,w)(w)) , 2. );
  EXPECT_DOUBLE_EQ( toDouble( d2(f,w)(w)(w) ) , 2. );
}

TEST(Kaskade,DISABLED_C2Functional_CostFunctional_Hessian)
{
  KASKADE_TWO_SPACE_SETUP
  KASKADE_COST_FUNCTIONAL

  using boost::fusion::at_c;
  auto V = Spacy::Kaskade::makeHilbertSpace(descriptions, {stateId,controlId} );
  Spacy::C2Functional f = Spacy::Kaskade::makeC2Functional(F,V);

  auto w = zero(V);
  auto& w_ = Spacy::cast_ref<Spacy::ProductSpace::Vector>(w);
  auto& ws_ = Spacy::cast_ref<Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,stateId> > >(w_.component(stateId));
  auto& wd_ = Spacy::cast_ref<Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,controlId> > >(w_.component(controlId));
  for(auto i=0u; i< at_c<0>(ws_.get().data).size(); ++i)
    at_c<0>(ws_.get().data)[i] = 1;

  auto hessian = f.hessian(w);
  EXPECT_DOUBLE_EQ( toDouble( hessian(w)(w) ) , 1. );

  for(auto i=0u; i< at_c<0>(wd_.get().data).size(); ++i)
    at_c<0>(wd_.get().data)[i] = 1;

  EXPECT_DOUBLE_EQ( toDouble( hessian(w)(w) ) , 2. );
}

