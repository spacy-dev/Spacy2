#include <Test/gtest.hh>

#include <Spacy/Spaces/ScalarSpace/linearOperator.hh>
#include <Spacy/Spaces/ScalarSpace/linearOperatorCreator.hh>
#include <Test/mockSetup.hh>

using namespace Spacy;

TEST(ScalarAdapter_LinearOperatorCreator,Domain)
{
  auto A = Scalar::LinearOperatorCreator{};

  ASSERT_EQ( A.domain().index() , 0u );
}

TEST(ScalarAdapter_LinearOperatorCreator,Range)
{
  auto A = Scalar::LinearOperatorCreator{};

  ASSERT_EQ( A.range().index() , 0u );
}

TEST(ScalarAdapter_LinearOperatorCreator,ThrowingApply)
{
  auto space = createMockBanachSpace();
  auto A = Scalar::LinearOperatorCreator{};

  ASSERT_THROW( A(&space) , std::runtime_error );
}

