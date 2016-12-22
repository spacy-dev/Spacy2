#include <Test/gtest.hh>

#include <Spacy/linearSolver.hh>
#include <Spacy/Spaces/ScalarSpace/Real.hh>
#include <Test/Mock/linearSolver.hh>
#include <Test/Mock/vector.hh>

using namespace Spacy;

TEST(IndefiniteLinearSolver,Construction)
{
  auto solver = Mock::IndefiniteLinearSolver{};
  IndefiniteLinearSolver typeErasedSolver( solver );
  ASSERT_FALSE( !typeErasedSolver );
}

TEST(IndefiniteLinearSolver,Assignment)
{
  auto solver = Mock::IndefiniteLinearSolver{};
  IndefiniteLinearSolver typeErasedSolver;
  ASSERT_TRUE( !typeErasedSolver );
  typeErasedSolver = solver;
  ASSERT_FALSE( !typeErasedSolver );
}

TEST(IndefiniteLinearSolver,Apply)
{
  auto solver = Mock::IndefiniteLinearSolver{};
  IndefiniteLinearSolver typeErasedSolver( solver );
  Mock::Vector vector;
  ASSERT_NO_THROW( typeErasedSolver(vector) );
}

TEST(IndefiniteLinearSolver,IsPositiveDefinite)
{
  auto solver = Mock::IndefiniteLinearSolver{};
  IndefiniteLinearSolver typeErasedSolver( solver );
  ASSERT_NO_THROW( typeErasedSolver.isPositiveDefinite() );
  ASSERT_TRUE( typeErasedSolver.isPositiveDefinite() );
}
