//#include <gtest/gtest.h>

//#include "Spacy/Adapter/Eigen/vector.hh"
//#include "Spacy/Adapter/Eigen/vectorCreator.hh"
//#include "Spacy/Adapter/Eigen/linearOperator.hh"
//#include "setup.hh"

//using namespace Spacy;

//TEST(Rn,LinearOperator_Apply)
//{
//  auto space = Spacy::Rn::makeHilbertSpace(testDim());
//  auto A = Spacy::Rn::LinearOperator(testMatrix(),space,space,space);
//  auto v = Spacy::Rn::Vector( testVector() , space );

//  auto w = A(v);
//  ASSERT_EQ( cast_ref<Spacy::Rn::Vector>(w).impl()[0] ,  1. );
//  ASSERT_EQ( cast_ref<Spacy::Rn::Vector>(w).impl()[1] ,  5. );
//}


//TEST(Rn,LinearOperator_ApplySolver)
//{
//  auto space = Spacy::Rn::makeHilbertSpace(testDim());
//  auto A = Spacy::Rn::LinearOperator(testMatrix(),space,space,space);
//  auto v = space.zeroVector();
//  cast_ref<Spacy::Rn::Vector>(v) = testVector();

//  auto w = A.solver()(v);
//  ASSERT_EQ( cast_ref<Spacy::Rn::Vector>(w).impl()[0] ,  1. );
//  ASSERT_EQ( cast_ref<Spacy::Rn::Vector>(w).impl()[1] ,  .5 );
//}
