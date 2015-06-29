#include "regularityTest.hh"

namespace Algorithm
{
  void RegularityTest::setLowerBound(double lowerBound) noexcept
  {
    lowerBound_ = lowerBound;
  }

  bool RegularityTest::regularityTestPassed(double nu) const noexcept
  {
    return nu > lowerBound_;
  }
}

