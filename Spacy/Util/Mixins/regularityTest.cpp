#include "regularityTest.hh"

namespace Spacy
{
  Mixin::RegularityTest::RegularityTest(double lowerBound) noexcept
    : lowerBound_(lowerBound)
  {}

  void Mixin::RegularityTest::setLowerBound(double lowerBound)
  {
    lowerBound_ = lowerBound;
    notify();
  }

  double Mixin::RegularityTest::getLowerBound() const noexcept
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

  void Mixin::RegularityTest::update(RegularityTest* changedSubject)
  {
    setLowerBound( changedSubject->getLowerBound() );
  }
}
