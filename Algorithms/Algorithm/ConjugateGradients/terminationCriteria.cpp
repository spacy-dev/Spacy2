#include "terminationCriteria.hh"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>

#include <iostream>

namespace Algorithm
{
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

  bool CGTerminationCriterion::reachedMaximalNumberOfIterations() const noexcept
  {
    return reachedMaximalNumberOfIterations_;
  }


  StrakosTichyEnergyErrorTerminationCriterion::StrakosTichyEnergyErrorTerminationCriterion(double tol, int maxit, double eps)
  {
    setRelativeAccuracy(tol);
    setEps(eps);
    setMaxSteps(maxit);
  }


  StrakosTichyEnergyErrorTerminationCriterion::StrakosTichyEnergyErrorTerminationCriterion(double tol, double minTol, int maxit, double eps)
    : StrakosTichyEnergyErrorTerminationCriterion(tol,maxit,eps)
  {
    minTol2 = minTol*minTol;
  }


  StrakosTichyEnergyErrorTerminationCriterion::operator bool() const
  {
    auto tol = std::max( relativeAccuracy() , eps() );
    return scaledGamma2.size() > maxSteps() || ( scaledGamma2.size() > lookAhead_ && squaredRelativeError() < tol*tol );
  }


  void StrakosTichyEnergyErrorTerminationCriterion::provideAlgorithmicQuantities(double alpha, double qAq, double, double rPINVr)
  {
    scaledGamma2.push_back( alpha * rPINVr );
    energyNorm2 += alpha * rPINVr;
    stepLength2 = std::abs(qAq);
  }

  bool StrakosTichyEnergyErrorTerminationCriterion::vanishingStep() const noexcept
  {
    auto tol = absoluteAccuracy()*absoluteAccuracy();
    if( energyNorm2 > tol) tol = std::min(tol,eps()*eps()*energyNorm2);
    return stepLength2 < tol;
  }

  void StrakosTichyEnergyErrorTerminationCriterion::clear() noexcept
  {
    scaledGamma2.clear();
    energyNorm2 = 0;
  }

  void StrakosTichyEnergyErrorTerminationCriterion::setMinimalTolerance(double tol) noexcept
  {
    minTol2 = tol*tol;
  }


  double StrakosTichyEnergyErrorTerminationCriterion::getMinimalTolerance() const noexcept
  {
    return sqrt(minTol2);
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
