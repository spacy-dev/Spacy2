#include "regularityTest.hh"

namespace VSA
{
  Mixin::RegularityTest::RegularityTest(double lowerBound) noexcept
    : lowerBound_(lowerBound)
  {}

  void Mixin::RegularityTest::setLowerBound(double lowerBound)
  {
    lowerBound_ = lowerBound;
    notify();
  }

  double Mixin::RegularityTest::lowerBound() const noexcept
  {
    return lowerBound_;
  }

  bool Mixin::RegularityTest::regularityTestPassed(double nu) const noexcept
  {
    return nu > lowerBound_;
  }

  bool Mixin::RegularityTest::regularityTestFailed(double nu) const noexcept
  {
    return !regularityTestPassed(nu);
  }

  void Mixin::RegularityTest::update(MixinConnection<RegularityTest>* changedSubject)
  {
    setLowerBound( static_cast<RegularityTest*>(changedSubject)->lowerBound() );
  }

  void Mixin::RegularityTest::attachRegularityTest(RegularityTest& other)
  {
    attach(other);
  }

  void Mixin::RegularityTest::detachRegularityTest(RegularityTest& other)
  {
    detach(other);
  }
}

