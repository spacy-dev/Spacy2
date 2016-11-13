#include "regularityTest.hh"

namespace Spacy
{
  Mixin::RegularityTest::RegularityTest(DampingFactor lowerBound) noexcept
    : lowerBound_(lowerBound)
  {}

  void Mixin::RegularityTest::setLowerBound(DampingFactor lowerBound)
  {
    lowerBound_ = lowerBound;
    notify();
  }

  DampingFactor Mixin::RegularityTest::getLowerBound() const noexcept
  {
    return lowerBound_;
  }

  bool Mixin::RegularityTest::regularityTestPassed(DampingFactor nu) const noexcept
  {
    return nu > lowerBound_;
  }

  bool Mixin::RegularityTest::regularityTestFailed(DampingFactor nu) const noexcept
  {
    return !regularityTestPassed(nu);
  }

  void Mixin::RegularityTest::update(RegularityTest* changedSubject)
  {
    setLowerBound( changedSubject->getLowerBound() );
  }
}
