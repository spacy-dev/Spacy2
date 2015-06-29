#ifndef REGULARITYTEST_HH
#define REGULARITYTEST_HH

namespace Algorithm
{
  class RegularityTest
  {
  public:
    void setLowerBound(double) noexcept;

    bool regularityTestPassed(double nu) const noexcept;

  private:
      double lowerBound_ = 1e-12;
  };
}
#endif // REGULARITYTEST_HH
