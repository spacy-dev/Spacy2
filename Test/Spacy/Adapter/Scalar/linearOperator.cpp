#include <Test/gtest.hh>

#include <Spacy/Adapter/Scalar/linearOperator.hh>
#include <Test/mockSetup.hh>

using namespace Spacy;

TEST(ScalarAdapter_LinearOperator,Apply)
{
  auto space = createMockBanachSpace();
  auto A = Scalar::LinearOperator(space,2);

  ASSERT_EQ( toDouble(A(Real(3))) ,  6. );
}


TEST(ScalarAdapter_LinearOperator,ApplySolver)
{
  auto space = createMockBanachSpace();
  auto A = Scalar::LinearOperator(space,2);

  ASSERT_EQ( toDouble(A.solver()(Real(3))) ,  1.5 );
}
