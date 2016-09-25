#include <Test/gtest.hh>

#include "Test/mockSetup.hh"

using namespace Spacy;

TEST(ProductSpaceVector,Add)
{
  auto V = makeProductHilbertSpace();
  auto v = createFirstTestVector(std::get<0>(V));
  auto w = createSecondTestVector(std::get<0>(V));

  auto sumOfFirstComponent  = valueOfComponent(v,0) + valueOfComponent(w,0);
  auto sumOfSecondComponent = valueOfComponent(v,1) + valueOfComponent(w,1);
  auto z = v + w;
  ASSERT_EQ( valueOfComponent(z,0) , sumOfFirstComponent );
  ASSERT_EQ( valueOfComponent(z,1) , sumOfSecondComponent );
  z = w + v;
  ASSERT_EQ( valueOfComponent(z,0) , sumOfFirstComponent );
  ASSERT_EQ( valueOfComponent(z,1) , sumOfSecondComponent );
}

TEST(ProductSpaceVector, AddThrow)
{
  auto V = makeProductHilbertSpace();
  auto W = makeProductHilbertSpace();

  auto v = zero(std::get<0>(V));
  auto w = zero(std::get<0>(W));

  ASSERT_THROW( v + w , IncompatibleSpaceException );
}

TEST(ProductSpaceVector,Subtract)
{
  auto V = makeProductHilbertSpace();
  auto v = createFirstTestVector(std::get<0>(V));
  auto w = createSecondTestVector(std::get<0>(V));

  auto subtractionOfFirstComponent  = valueOfComponent(v,0) - valueOfComponent(w,0);
  auto subtractionOfSecondComponent = valueOfComponent(v,1) - valueOfComponent(w,1);
  auto z = v - w;
  ASSERT_EQ( valueOfComponent(z,0) , subtractionOfFirstComponent );
  ASSERT_EQ( valueOfComponent(z,1) , subtractionOfSecondComponent );
  z = w - v;
  ASSERT_EQ( valueOfComponent(z,0) , - subtractionOfFirstComponent );
  ASSERT_EQ( valueOfComponent(z,1) , - subtractionOfSecondComponent );
}

TEST(ProductSpaceVector, SubtractThrow)
{
  auto V = makeProductHilbertSpace();
  auto W = makeProductHilbertSpace();

  auto v = zero(std::get<0>(V));
  auto w = zero(std::get<0>(W));

  ASSERT_THROW( v - w , IncompatibleSpaceException );
}

TEST(ProductSpaceVector,MultiplyWithDouble)
{
  auto V = makeProductHilbertSpace();
  auto v = createFirstTestVector(std::get<0>(V));
  double a = 2;

  auto z = a*v;
  ASSERT_EQ( valueOfComponent(z,0) , a * valueOfComponent(v,0) );
  ASSERT_EQ( valueOfComponent(z,1) , a * valueOfComponent(v,1) );
  z = v*a;
  ASSERT_EQ( valueOfComponent(z,0) , a * valueOfComponent(v,0) );
  ASSERT_EQ( valueOfComponent(z,1) , a * valueOfComponent(v,1) );
}

TEST(ProductSpaceVector,Negation)
{
  auto V = makeProductHilbertSpace();
  auto v = createFirstTestVector(std::get<0>(V));

  auto z = -v;
  ASSERT_EQ( valueOfComponent(z,0) , - valueOfComponent(v,0) );
  ASSERT_EQ( valueOfComponent(z,1) , - valueOfComponent(v,1) );
}

TEST(ProductSpaceVector,Comparison)
{
  auto V = makeProductHilbertSpace();
  auto v = createFirstTestVector(std::get<0>(V));
  auto w = createFirstTestVector(std::get<0>(V));
  auto x = createSecondTestVector(std::get<0>(V));

  ASSERT_TRUE( v == w );
  ASSERT_FALSE( v == x );
}

TEST(ProductSpaceVector,ComparisonThrow)
{
  auto V = makeProductHilbertSpace();
  auto W = makeProductHilbertSpace();
  auto v = createFirstTestVector(std::get<0>(V));
  auto w = createFirstTestVector(std::get<0>(W));

  ASSERT_THROW( v == w , IncompatibleSpaceException );
}

TEST(ProductSpaceVector,NumberOfVariables)
{
  auto V = makeProductHilbertSpace();
  auto v = zero(std::get<0>(V));

  ASSERT_EQ( cast_ref<ProductSpace::Vector>(v).numberOfVariables() , numberOfVariables() );
}

TEST(ProductSpaceVector,VariableAccess)
{
  auto V = makeProductHilbertSpace();
  auto v = createFirstTestVector(std::get<0>(V));

  ASSERT_EQ( valueOfComponent(v,0) , 1. );
  ASSERT_EQ( valueOfComponent(v,1) , 2. );
}

TEST(ProductSpaceVector,CreatorAccess)
{
  auto V = makeProductHilbertSpace();
  auto v = zero(std::get<0>(V));

  auto consistentType = std::is_same< std::decay_t<decltype( cast_ref<ProductSpace::Vector>(v).creator() )> , ProductSpace::VectorCreator >::value;
  ASSERT_TRUE( consistentType );
}

TEST(ProductSpaceVector, ApplyAsDualToSelf)
{
  auto V = makeProductHilbertSpace();
  auto v = createFirstTestVector(std::get<0>(V));

  ASSERT_EQ( toDouble(v(v)) , 5. );
}

TEST(ProductSpaceVector, ApplyAsDual)
{
  auto Vi = makeProductHilbertSpace();
  auto& V = std::get<0>(Vi);
  auto Wi = makeProductHilbertSpace();
  auto& W = std::get<0>(Wi);
  connectAsPrimalDualPair(V,W);

  auto primal = createFirstTestVector(V);
  auto dual = createFirstTestVector(W);

  ASSERT_EQ( toDouble(dual(primal)) , 5. );
  ASSERT_THROW( primal(dual) , IncompatibleSpaceException );
}

TEST(ExtractComponentView, OneLayer)
{
  std::vector< std::shared_ptr<VectorSpace> > spaces;
  for(auto i=0u; i<3; ++i)
    spaces.push_back( std::make_shared<VectorSpace>( createMockHilbertSpace() ) );
  auto X = ProductSpace::makeHilbertSpace(spaces);
  auto x = zero(X);
  const auto y = zero(X);

  auto singleVectorView = ProductSpace::extractSingleComponentView<Mock::Vector>(x);
  auto singleConstVectorView = ProductSpace::extractSingleComponentView<Mock::Vector>(y);

  for(auto i=0u; i<spaces.size(); ++i)
  {
    ASSERT_EQ( spaces[i]->index() , singleVectorView[i].space().index() );
    ASSERT_EQ( spaces[i]->index() , singleConstVectorView[i].space().index() );
  }
}

TEST(ExtractComponentView, TwoLayers)
{
  std::vector< std::shared_ptr<VectorSpace> > spaces;
  for(auto i=0u; i<3; ++i)
    spaces.push_back( std::make_shared<VectorSpace>( createMockHilbertSpace() ) );
  auto X = ProductSpace::makeHilbertSpace(spaces,{0,1},{2});
  auto x = zero(X);
  const auto y = zero(X);

  auto singleVectorView = ProductSpace::extractSingleComponentView<Mock::Vector>(x);
  auto singleConstVectorView = ProductSpace::extractSingleComponentView<Mock::Vector>(y);

  for(auto i=0u; i<spaces.size(); ++i)
  {
    ASSERT_EQ( spaces[i]->index() , singleVectorView[i].space().index() );
    ASSERT_EQ( spaces[i]->index() , singleConstVectorView[i].space().index() );
  }
}

TEST(ExtractComponentView, ThreeLayers)
{
  std::vector< std::shared_ptr<VectorSpace> > spaces;
  for(auto i=0u; i<3; ++i)
    spaces.push_back( std::make_shared<VectorSpace>( createMockHilbertSpace() ) );

  auto X = std::make_shared<VectorSpace>(ProductSpace::makeHilbertSpace(spaces,{0,1},{2}));
  auto Y = std::make_shared<VectorSpace>(ProductSpace::makeHilbertSpace(spaces,{0,1},{2}));
  auto Z = ProductSpace::makeHilbertSpace({X,Y});
  auto x = zero(Z);
  const auto y = zero(Z);

  auto singleVectorView = ProductSpace::extractSingleComponentView<Mock::Vector>(x);
  auto singleConstVectorView = ProductSpace::extractSingleComponentView<Mock::Vector>(y);

  for(auto i=0u; i<spaces.size(); ++i)
  {
    ASSERT_EQ( spaces[i]->index() , singleVectorView[i].space().index() );
    ASSERT_EQ( spaces[i]->index() , singleVectorView[i+spaces.size()].space().index() );
    ASSERT_EQ( spaces[i]->index() , singleConstVectorView[i].space().index() );
    ASSERT_EQ( spaces[i]->index() , singleConstVectorView[i+spaces.size()].space().index() );
  }
}

TEST(ExtractComponentView,Throw)
{
  std::vector< std::shared_ptr<VectorSpace> > spaces;
  for(auto i=0u; i<3; ++i)
    spaces.push_back( std::make_shared<VectorSpace>( createMockHilbertSpace() ) );
  auto X = ProductSpace::makeHilbertSpace(spaces);
  auto x = zero(X);
  const auto y = zero(X);

  ASSERT_THROW( ProductSpace::extractSingleComponentView<Real>(x) , Exception::InvalidArgument );
  ASSERT_THROW( ProductSpace::extractSingleComponentView<Real>(y) , Exception::InvalidArgument );
}
