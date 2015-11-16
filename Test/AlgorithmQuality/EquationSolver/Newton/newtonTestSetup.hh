#ifndef TEST_ALGORITHM_QUALITY_EQUATION_SOLVER_NEWTON_TEST_SETUP_HH
#define TEST_ALGORITHM_QUALITY_EQUATION_SOLVER_NEWTON_TEST_SETUP_HH

#include <functional>
#include "Spacy/Algorithm/Newton/newton.hh"

using namespace Spacy;

auto createLocalNewton(const C1Operator& F, double relativeAccuracy, double eps, unsigned maxSteps)
{
  auto p = Newton::Parameter{};
  p.setRelativeAccuracy(relativeAccuracy);
  p.setEps(eps);
  p.setMaxSteps(maxSteps);

  return [F,p](const Vector& x0)
  {
    return localNewton(F,x0,p);
  };
}

auto createCovariantNewton(const C1Operator& F, double relativeAccuracy, double eps, unsigned maxSteps)
{
  auto p = Newton::Parameter{};
  p.setRelativeAccuracy(relativeAccuracy);
  p.setEps(eps);
  p.setMaxSteps(maxSteps);

  return [F,p](const Vector& x0)
  {
    return covariantNewton(F,x0,p);
  };
}

auto createContravariantNewton(const C1Operator& F, double relativeAccuracy, double eps, unsigned maxSteps)
{
  auto p = Newton::Parameter{};
  p.setRelativeAccuracy(relativeAccuracy);
  p.setEps(eps);
  p.setMaxSteps(maxSteps);

  return [F,p](const Vector& x0)
  {
    return contravariantNewton(F,x0,p);
  };
}

#endif // TEST_ALGORITHM_QUALITY_EQUATION_SOLVER_NEWTON_TEST_SETUP_HH
