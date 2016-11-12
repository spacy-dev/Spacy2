#include <Test/gtest.hh>

#include "setup.hh"

#include <Spacy/c2Functional.hh>
#include <Spacy/zeroVectorCreator.hh>
#include <Spacy/Adapter/Kaskade/c2Functional.hh>
#include <Spacy/Util/cast.hh>

TEST(Kaskade,C2Functional_CostFunctional_CreateAndApply)
{
    KASKADE_TWO_SPACE_SETUP
    KASKADE_COST_FUNCTIONAL

    using boost::fusion::at_c;
    using std::cout;
    using std::endl;
    auto V = Spacy::Kaskade::makeHilbertSpace(descriptions, {stateId,controlId} );
    Spacy::C2Functional f = Spacy::Kaskade::makeC2Functional(F,V);

    auto r = f(zero(V));
    EXPECT_DOUBLE_EQ( get(norm(r)) , 0. );

    auto w = zero(V);
    ASSERT_TRUE(Spacy::is<Spacy::ProductSpace::Vector>(w));
    auto& w_ = Spacy::cast_ref<Spacy::ProductSpace::Vector>(w);
    ASSERT_EQ( w_.numberOfVariables(), 2 );
    using StateVector = Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,stateId> >;
    ASSERT_TRUE(Spacy::is<StateVector>(w_.component(stateId)));
    auto& w_state = Spacy::cast_ref<StateVector>(w_.component(stateId));
    for(auto i=0u; i< at_c<0>(w_state.get().data).size(); ++i)
        at_c<0>(w_state.get().data)[i] = 1;

    EXPECT_DOUBLE_EQ( get(f(w)), .5 );

    auto& w2_ = Spacy::cast_ref<Spacy::ProductSpace::Vector>(w);
    using ControlVector = Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,controlId> >;
    ASSERT_TRUE(Spacy::is<ControlVector>(w_.component(controlId)));
    auto& w_control = Spacy::cast_ref<Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,controlId> > >(w2_.component(controlId));
    for(auto i=0u; i< at_c<0>(w_control.get().data).size(); ++i)
        at_c<0>(w_control.get().data)[i] = 1;

    EXPECT_DOUBLE_EQ( get(f(w)), 1. );
}

TEST(Kaskade,C2Functional_CostFunctional_PrimalDual_CreateAndApply)
{
    KASKADE_TWO_SPACE_SETUP
    KASKADE_COST_FUNCTIONAL

    using boost::fusion::at_c;
    auto V = Spacy::Kaskade::makeHilbertSpace(descriptions, {stateId} , {controlId} );
    Spacy::C2Functional f = Spacy::Kaskade::makeC2Functional(F,V);

    auto r = f(zero(V));
    EXPECT_DOUBLE_EQ( get(norm(r)) , 0. );

    auto w = zero(V);
    auto& w_ = Spacy::cast_ref<Spacy::ProductSpace::Vector>(w);
    EXPECT_EQ( w_.numberOfVariables(), 2 );

    ASSERT_TRUE( Spacy::is<Spacy::ProductSpace::Vector>(w_.component(Spacy::PRIMAL)) );
    auto& w_primal = Spacy::cast_ref<Spacy::ProductSpace::Vector>(w_.component(Spacy::PRIMAL));

    using StateVector = Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,stateId> >;
    ASSERT_EQ( w_primal.numberOfVariables() , 1 );
    ASSERT_TRUE( Spacy::is<StateVector>(w_primal.component(0)) );
    auto& w_state = Spacy::cast_ref<StateVector>(w_primal.component(0));
    for(auto i=0u; i< at_c<0>(w_state.get().data).size(); ++i)
        at_c<0>(w_state.get().data)[i] = 1;

    EXPECT_DOUBLE_EQ( get(f(w)), .5 );

    using ControlVector = Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,controlId> >;
    auto& w2_ = Spacy::cast_ref<Spacy::ProductSpace::Vector>(w);
    ASSERT_TRUE( Spacy::is<Spacy::ProductSpace::Vector>(w2_.component(Spacy::DUAL)) );
    auto& w_dual = Spacy::cast_ref<Spacy::ProductSpace::Vector>(w2_.component(Spacy::DUAL));

    ASSERT_EQ( w_dual.numberOfVariables() , 1 );
    ASSERT_TRUE( Spacy::is<ControlVector>(w_dual.component(0)) );
    auto& w_control = Spacy::cast_ref<ControlVector>(w_dual.component(0));
    for(auto i=0u; i< at_c<0>(w_control.get().data).size(); ++i)
        at_c<0>(w_control.get().data)[i] = 1;

    EXPECT_DOUBLE_EQ( get(f(w)), 1. );
}

TEST(Kaskade,C2Functional_CostFunctional_D1)
{
    KASKADE_TWO_SPACE_SETUP
    KASKADE_COST_FUNCTIONAL

    using boost::fusion::at_c;
    auto V = Spacy::Kaskade::makeHilbertSpace(descriptions, {stateId,controlId} );
    Spacy::C2Functional f = Spacy::Kaskade::makeC2Functional(F,V);

    auto w = zero(V);
    ASSERT_TRUE(Spacy::is<Spacy::ProductSpace::Vector>(w));
    auto& w_ = Spacy::cast_ref<Spacy::ProductSpace::Vector>(w);
    ASSERT_EQ( w_.numberOfVariables(), 2u );

    using StateVector = Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,stateId> >;
    ASSERT_TRUE(Spacy::is<StateVector>(w_.component(stateId)));
    auto& ws_ = Spacy::cast_ref<StateVector>(w_.component(stateId));
    for(auto i=0u; i< at_c<0>(ws_.get().data).size(); ++i)
        at_c<0>(ws_.get().data)[i] = 1;

    EXPECT_DOUBLE_EQ( get( f.d1(w)(w) ) , 1. );
    EXPECT_DOUBLE_EQ( get( d1(f,w)(w) ) , 1. );

    using ControlVector = Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,controlId> >;
    ASSERT_TRUE(Spacy::is<ControlVector>(w_.component(controlId)));
    auto& wd_ = Spacy::cast_ref<ControlVector>(w_.component(controlId));
    for(auto i=0u; i< at_c<0>(wd_.get().data).size(); ++i)
        at_c<0>(wd_.get().data)[i] = 1;

    EXPECT_DOUBLE_EQ( get(f.d1(w)(w)) , 2. );
    EXPECT_DOUBLE_EQ( get(d1(f,w)(w)) , 2. );
}

TEST(Kaskade,C2Functional_CostFunctional_PrimalDual_D1)
{
    KASKADE_TWO_SPACE_SETUP
    KASKADE_COST_FUNCTIONAL

            using boost::fusion::at_c;
    auto V = Spacy::Kaskade::makeHilbertSpace(descriptions, {stateId} , {controlId} );
    Spacy::C2Functional f = Spacy::Kaskade::makeC2Functional(F,V);

    auto w = zero(V);
    ASSERT_TRUE(Spacy::is<Spacy::ProductSpace::Vector>(w));
    auto& w_ = Spacy::cast_ref<Spacy::ProductSpace::Vector>(w);
    ASSERT_EQ( w_.numberOfVariables(), 2u );

    ASSERT_TRUE(Spacy::is<Spacy::ProductSpace::Vector>(w_.component(Spacy::PRIMAL)));
    auto& wp = Spacy::cast_ref<Spacy::ProductSpace::Vector>(w_.component(Spacy::PRIMAL));

    using StateVector = Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,stateId> >;
    ASSERT_TRUE(Spacy::is<StateVector>(wp.component(0)));
    auto& ws_ = Spacy::cast_ref<StateVector>(wp.component(0));

    for(auto i=0u; i< at_c<0>(ws_.get().data).size(); ++i)
        at_c<0>(ws_.get().data)[i] = 1;

    EXPECT_DOUBLE_EQ( get( f.d1(w)(w) ) , 1. );
    EXPECT_DOUBLE_EQ( get( d1(f,w)(w) ) , 1. );

    ASSERT_TRUE(Spacy::is<Spacy::ProductSpace::Vector>(w_.component(Spacy::DUAL)));
    auto& wd = Spacy::cast_ref<Spacy::ProductSpace::Vector>(w_.component(Spacy::DUAL));

    using ControlVector = Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,controlId> >;
    ASSERT_TRUE(Spacy::is<ControlVector>(wd.component(0)));
    auto& wd_ = Spacy::cast_ref<ControlVector>(wd.component(0));

    for(auto i=0u; i< at_c<0>(wd_.get().data).size(); ++i)
        at_c<0>(wd_.get().data)[i] = 1;

    EXPECT_DOUBLE_EQ( get(f.d1(w)(w)) , 2. );
    EXPECT_DOUBLE_EQ( get(d1(f,w)(w)) , 2. );
}

TEST(Kaskade,C2Functional_CostFunctional_D2)
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

    EXPECT_DOUBLE_EQ( get( f.d2(w,w)(w) ) , 1. );
    EXPECT_DOUBLE_EQ( get( d2(f,w)(w)(w) ) , 1. );

    for(auto i=0u; i< at_c<0>(wd_.get().data).size(); ++i)
        at_c<0>(wd_.get().data)[i] = 1;

    EXPECT_DOUBLE_EQ( get(f.d2(w,w)(w)) , 2. );
    EXPECT_DOUBLE_EQ( get( d2(f,w)(w)(w) ) , 2. );
}

TEST(Kaskade,C2Functional_CostFunctional_Hessian)
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
    EXPECT_DOUBLE_EQ( get( hessian(w)(w) ) , 1. );

    for(auto i=0u; i< at_c<0>(wd_.get().data).size(); ++i)
        at_c<0>(wd_.get().data)[i] = 1;

    EXPECT_DOUBLE_EQ( get( hessian(w)(w) ) , 2. );
}
