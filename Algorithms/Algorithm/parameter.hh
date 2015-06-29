#ifndef ALGORITHM_ALGORITHM_PARAMETERS_HH
#define ALGORITHM_ALGORITHM_PARAMETERS_HH

namespace Algorithm
{
  class Parameter
  {
  public:
    void setAbsoluteAccuracy(double) noexcept;
    void setRelativeAccuracy(double) noexcept;
    void setMaxSteps(unsigned) noexcept;
    void setVerbosity(bool) noexcept;

    double absoluteAccuracy() const noexcept;
    double relativeAccuracy() const noexcept;
    unsigned maxSteps() const noexcept;
    bool isVerbose() const noexcept;

  private:
    double absoluteAccuracy_ = 1e-12;
    double relativeAccuracy_ = 1e-6;
    unsigned maxSteps_ = 100;
    bool verbose_ = false;
  };
}

#endif // ALGORITHM_ALGORITHM_PARAMETERS_HH
