#include "cgTerminationCriteria.hh"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>

namespace Algorithm
{
  CGTerminationCriterion::CGTerminationCriterion(unsigned maxIter) noexcept
    : maximalNumberOfIterations_(maxIter)
  {}

  CGTerminationCriterion::~CGTerminationCriterion() {}

  double CGTerminationCriterion::getMinimalTolerance() const
  {
    assert("not implemented"); return 0.;
  }

  void CGTerminationCriterion::setMinimalTolerance(double)
  {
    assert("not implemented");
  }

  void CGTerminationCriterion::setLookAhead(unsigned)
  {
    assert("not implemented");
  }

  bool CGTerminationCriterion::minimalDecreaseAchieved() const
  {
    assert("not implemented");
    return false;
  }

  double CGTerminationCriterion::getMaximalAttainableAccuracy() const
  {
    assert("not implemented");
    return 0.;
  }

  void CGTerminationCriterion::setMaximalNumberOfIterations(unsigned maxIter) noexcept
  {
    maximalNumberOfIterations_ = maxIter;
  }

  unsigned CGTerminationCriterion::getMaximalNumberOfIterations() const noexcept
  {
    return maximalNumberOfIterations_;
  }

  bool CGTerminationCriterion::reachedMaximalNumberOfIterations() const noexcept
  {
    return reachedMaximalNumberOfIterations_;
  }


  StrakosTichyEnergyErrorTerminationCriterion::StrakosTichyEnergyErrorTerminationCriterion(double tol, int maxit, double eps)
    : CGTerminationCriterion(maxit), tol2(tol*tol), eps2(eps*eps)
  {}


  StrakosTichyEnergyErrorTerminationCriterion::StrakosTichyEnergyErrorTerminationCriterion(double tol, double minTol, int maxit, double eps)
    : CGTerminationCriterion(maxit), tol2(tol*tol), minTol2(minTol*minTol), eps2(eps*eps)
  {}


  StrakosTichyEnergyErrorTerminationCriterion::operator bool() const
  {
    return scaledGamma2.size() > maximalNumberOfIterations_ || ( scaledGamma2.size() > lookAhead_ && squaredRelativeError() < std::max(tol2,eps2) );
  }


  void StrakosTichyEnergyErrorTerminationCriterion::provideAlgorithmicQuantities(double alpha, double qAq, double, double rPINVr)
  {
    scaledGamma2.push_back( alpha * rPINVr );
    energyNorm2 += alpha * rPINVr;
    stepLength2 = std::abs(qAq);
  }

  bool StrakosTichyEnergyErrorTerminationCriterion::vanishingStep() const noexcept
  {
    return stepLength2 < eps2 * energyNorm2 || stepLength2 == 0;
  }

  void StrakosTichyEnergyErrorTerminationCriterion::clear() noexcept
  {
    scaledGamma2.clear();
    energyNorm2 = 0;
  }

  void StrakosTichyEnergyErrorTerminationCriterion::setTolerance(double tol) noexcept
  {
    tol2 = tol*tol;
  }

  double StrakosTichyEnergyErrorTerminationCriterion::getTolerance() const noexcept
  {
    return sqrt(tol2);
  }

  void StrakosTichyEnergyErrorTerminationCriterion::setMinimalTolerance(double tol) noexcept
  {
    minTol2 = tol*tol;
  }


  double StrakosTichyEnergyErrorTerminationCriterion::getMinimalTolerance() const noexcept
  {
    return sqrt(minTol2);
  }

  void StrakosTichyEnergyErrorTerminationCriterion::setMaximalAttainableAccuracy(double eps) noexcept
  {
    eps2 = eps*eps;
  }

  double StrakosTichyEnergyErrorTerminationCriterion::getMaximalAttainableAccuracy() const noexcept
  {
    return sqrt(eps2);
  }

  void StrakosTichyEnergyErrorTerminationCriterion::setLookAhead(unsigned lookAhead) noexcept
  {
    lookAhead_ = lookAhead;
  }

  bool StrakosTichyEnergyErrorTerminationCriterion::minimalDecreaseAchieved() const noexcept
  {
    return squaredRelativeError() < minTol2;
  }

  double StrakosTichyEnergyErrorTerminationCriterion::squaredRelativeError() const noexcept
  {
    if( scaledGamma2.size() < lookAhead_ ) return std::numeric_limits<double>::max();
    return std::accumulate(scaledGamma2.end() - lookAhead_, scaledGamma2.end(), 0.) / energyNorm2;
  }
}
