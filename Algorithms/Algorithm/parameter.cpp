#include "parameter.hh"

namespace Algorithm
{
  void Parameter::setAbsoluteAccuracy(double absoluteAccuracy) noexcept
  {
    absoluteAccuracy_ = absoluteAccuracy;
  }

  void Parameter::setRelativeAccuracy(double relativeAccuracy) noexcept
  {
    relativeAccuracy_ = relativeAccuracy;
  }

  void Parameter::setEps(double eps) noexcept
  {
    eps_ = eps;
  }
  void Parameter::setMaxSteps(unsigned maxSteps) noexcept
  {
    maxSteps_ = maxSteps;
  }

  void Parameter::setVerbosity(bool verbose) noexcept
  {
    verbose_ = verbose;
  }

  double Parameter::absoluteAccuracy() const noexcept
  {
    return absoluteAccuracy_;
  }

  double Parameter::relativeAccuracy() const noexcept
  {
    return relativeAccuracy_;
  }

  double Parameter::eps() const noexcept
  {
    return eps_;
  }

  unsigned Parameter::maxSteps() const noexcept
  {
    return maxSteps_;
  }

  bool Parameter::isVerbose() const noexcept
  {
    return verbose_;
  }
}
