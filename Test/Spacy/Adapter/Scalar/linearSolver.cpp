#include <Test/gtest.hh>

#include <Spacy/Adapter/Scalar/linearSolver.hh>
#include <Spacy/Spaces/RealSpace/real.hh>
#include <Spacy/vector.hh>
#include <Spacy/Util/cast.hh>

using namespace Spacy;

TEST(ScalarAdapter_LinearSolver,Apply)
{
  auto solver = Scalar::LinearSolver(2);
  auto real = Real{3.};
  ASSERT_EQ( get(cast_ref<Real>(solver(real))) , 1.5 );
}
