#include "regularityTest.hh"

namespace Algorithm
{
  Mixin::RegularityTest::RegularityTest(double lowerBound) noexcept
    : lowerBound_(lowerBound)
  {}

  void Mixin::RegularityTest::setLowerBound(double lowerBound) noexcept
  {
    lowerBound_ = lowerBound;
  }

  bool Mixin::RegularityTest::regularityTestPassed(double nu) const noexcept
  {
    return nu > lowerBound_;
  }

  bool Mixin::RegularityTest::regularityTestFailed(double nu) const noexcept
  {
    return !regularityTestPassed(nu);
  }
}

