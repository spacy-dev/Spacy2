// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include <gtest/gtest.h>

#include "setup.hh"
#include "Spacy/c2Functional.hh"
#include "Spacy/Adapter/Kaskade/c2Functional.hh"

TEST(Kaskade,C2Functional_CostFunctional_CreateAndApply)
{
  KASKADE_TWO_SPACE_SETUP
  KASKADE_COST_FUNCTIONAL

  using boost::fusion::at_c;
  auto V = Spacy::Kaskade::makeHilbertSpace(descriptions, {stateId,controlId} );
  Spacy::C2Functional f = Spacy::Kaskade::makeC2Functional(F,V);

  auto r = f(V.zeroVector());
  ASSERT_DOUBLE_EQ( toDouble(norm(r)) , 0. );

  auto w = V.zeroVector();
  auto& w_ = *Spacy::target<Spacy::ProductSpace::Vector>(w);
  auto& ws_ = *Spacy::target<Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,stateId> > >(w_.component(stateId));
  auto& wd_ = *Spacy::target<Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,controlId> > >(w_.component(controlId));
  for(auto i=0u; i< at_c<0>(ws_.impl().data).size(); ++i)
    at_c<0>(ws_.impl().data)[i] = 1;

  ASSERT_DOUBLE_EQ( toDouble(f(w)) , .5 );

  for(auto i=0u; i< at_c<0>(wd_.impl().data).size(); ++i)
    at_c<0>(wd_.impl().data)[i] = 1;

  ASSERT_DOUBLE_EQ( toDouble(f(w)) , 1. );
}

TEST(Kaskade,C2Functional_CostFunctional_PrimalDual_CreateAndApply)
{
  KASKADE_TWO_SPACE_SETUP
  KASKADE_COST_FUNCTIONAL

  using boost::fusion::at_c;
  auto V = Spacy::Kaskade::makeHilbertSpace(descriptions, {stateId} , {controlId} );
  Spacy::C2Functional f = Spacy::Kaskade::makeC2Functional(F,V);

  auto r = f(V.zeroVector());
  ASSERT_DOUBLE_EQ( toDouble(norm(r)) , 0. );

  auto w = V.zeroVector();
  auto& w_ = *Spacy::target<Spacy::ProductSpace::Vector>(w);
  auto& wp = *Spacy::target<Spacy::ProductSpace::Vector>(w_.component(Spacy::PRIMAL));
  auto& wd = *Spacy::target<Spacy::ProductSpace::Vector>(w_.component(Spacy::DUAL));
  auto& ws_ = *Spacy::target<Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,stateId> > >(wp.component(0));
  auto& wd_ = *Spacy::target<Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,controlId> > >(wd.component(0));
  for(auto i=0u; i< at_c<0>(ws_.impl().data).size(); ++i)
    at_c<0>(ws_.impl().data)[i] = 1;

  ASSERT_DOUBLE_EQ( toDouble(f(w)) , .5 );

  for(auto i=0u; i< at_c<0>(wd_.impl().data).size(); ++i)
    at_c<0>(wd_.impl().data)[i] = 1;

  ASSERT_DOUBLE_EQ( toDouble(f(w)) , 1. );
}

TEST(Kaskade,C2Functional_CostFunctional_D1)
{
  KASKADE_TWO_SPACE_SETUP
  KASKADE_COST_FUNCTIONAL

  using boost::fusion::at_c;
  auto V = Spacy::Kaskade::makeHilbertSpace(descriptions, {stateId,controlId} );
  Spacy::C2Functional f = Spacy::Kaskade::makeC2Functional(F,V);

  auto w = V.zeroVector();
  auto& w_ = *Spacy::target<Spacy::ProductSpace::Vector>(w);
  auto& ws_ = *Spacy::target<Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,stateId> > >(w_.component(stateId));
  auto& wd_ = *Spacy::target<Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,controlId> > >(w_.component(controlId));
  for(auto i=0u; i< at_c<0>(ws_.impl().data).size(); ++i)
    at_c<0>(ws_.impl().data)[i] = 1;

  ASSERT_DOUBLE_EQ( toDouble( f.d1(w)(w) ) , 1. );

  for(auto i=0u; i< at_c<0>(wd_.impl().data).size(); ++i)
    at_c<0>(wd_.impl().data)[i] = 1;

  ASSERT_DOUBLE_EQ( toDouble(f.d1(w)(w)) , 2. );
}

TEST(Kaskade,C2Functional_CostFunctional_PrimalDual_D1)
{
  KASKADE_TWO_SPACE_SETUP
  KASKADE_COST_FUNCTIONAL

  using boost::fusion::at_c;
  auto V = Spacy::Kaskade::makeHilbertSpace(descriptions, {stateId} , {controlId} );
  Spacy::C2Functional f = Spacy::Kaskade::makeC2Functional(F,V);

  auto w = V.zeroVector();
  auto& w_ = *Spacy::target<Spacy::ProductSpace::Vector>(w);
  auto& wp = *Spacy::target<Spacy::ProductSpace::Vector>(w_.component(Spacy::PRIMAL));
  auto& wd = *Spacy::target<Spacy::ProductSpace::Vector>(w_.component(Spacy::DUAL));
  auto& ws_ = *Spacy::target<Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,stateId> > >(wp.component(0));
  auto& wd_ = *Spacy::target<Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,controlId> > >(wd.component(0));
  for(auto i=0u; i< at_c<0>(ws_.impl().data).size(); ++i)
    at_c<0>(ws_.impl().data)[i] = 1;

  ASSERT_DOUBLE_EQ( toDouble( f.d1(w)(w) ) , 1. );

  for(auto i=0u; i< at_c<0>(wd_.impl().data).size(); ++i)
    at_c<0>(wd_.impl().data)[i] = 1;

  ASSERT_DOUBLE_EQ( toDouble(f.d1(w)(w)) , 2. );
}

TEST(Kaskade,C2Functional_CostFunctional_D2)
{
  KASKADE_TWO_SPACE_SETUP
  KASKADE_COST_FUNCTIONAL

  using boost::fusion::at_c;
  auto V = Spacy::Kaskade::makeHilbertSpace(descriptions, {stateId,controlId} );
  Spacy::C2Functional f = Spacy::Kaskade::makeC2Functional(F,V);

  auto w = V.zeroVector();
  auto& w_ = *Spacy::target<Spacy::ProductSpace::Vector>(w);
  auto& ws_ = *Spacy::target<Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,stateId> > >(w_.component(stateId));
  auto& wd_ = *Spacy::target<Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,controlId> > >(w_.component(controlId));
  for(auto i=0u; i< at_c<0>(ws_.impl().data).size(); ++i)
    at_c<0>(ws_.impl().data)[i] = 1;

  ASSERT_DOUBLE_EQ( toDouble( f.d2(w,w)(w) ) , 1. );

  for(auto i=0u; i< at_c<0>(wd_.impl().data).size(); ++i)
    at_c<0>(wd_.impl().data)[i] = 1;

  ASSERT_DOUBLE_EQ( toDouble(f.d2(w,w)(w)) , 2. );
}

TEST(Kaskade,C2Functional_CostFunctional_Hessian)
{
  KASKADE_TWO_SPACE_SETUP
  KASKADE_COST_FUNCTIONAL

  using boost::fusion::at_c;
  auto V = Spacy::Kaskade::makeHilbertSpace(descriptions, {stateId,controlId} );
  Spacy::C2Functional f = Spacy::Kaskade::makeC2Functional(F,V);

  auto w = V.zeroVector();
  auto& w_ = *Spacy::target<Spacy::ProductSpace::Vector>(w);
  auto& ws_ = *Spacy::target<Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,stateId> > >(w_.component(stateId));
  auto& wd_ = *Spacy::target<Spacy::Kaskade::Vector<Spacy::Kaskade::Detail::ExtractDescription_t<Descriptions,controlId> > >(w_.component(controlId));
  for(auto i=0u; i< at_c<0>(ws_.impl().data).size(); ++i)
    at_c<0>(ws_.impl().data)[i] = 1;

  auto hessian = f.hessian(w);
  ASSERT_DOUBLE_EQ( toDouble( hessian(w)(w) ) , 1. );

  for(auto i=0u; i< at_c<0>(wd_.impl().data).size(); ++i)
    at_c<0>(wd_.impl().data)[i] = 1;

  ASSERT_DOUBLE_EQ( toDouble( hessian(w)(w) ) , 2. );
}

