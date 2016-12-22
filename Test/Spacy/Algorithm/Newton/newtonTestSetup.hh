#pragma once

#include <functional>
#include <Spacy/c1Operator.hh>
#include <Spacy/zeroVectorCreator.hh>
#include <Spacy/Algorithm/Newton/newton.hh>
#include <Spacy/inducedScalarProduct.hh>
#include <Spacy/Spaces/realSpace.hh>

using namespace Spacy;

auto createLocalNewton(C1Operator F, double relativeAccuracy, double eps, unsigned maxSteps)
{
  auto p = Newton::Parameter{};
  p.setRelativeAccuracy(relativeAccuracy);
  p.set_eps(eps);
  p.setMaxSteps(maxSteps);

  return [F,p](const Vector& x0)
  {
    return localNewton(F,x0,p);
  };
}

auto createCovariantNewton(C1Operator F, double relativeAccuracy, double eps, unsigned maxSteps)
{
  auto p = Newton::Parameter{};
  p.setRelativeAccuracy(relativeAccuracy);
  p.set_eps(eps);
  p.setMaxSteps(maxSteps);
  Space::R.setScalarProduct( InducedScalarProduct(F.linearization( zero(F.domain()) )) );

  return [F,p](const Vector& x0)
  {
    return covariantNewton(F,x0,p);
  };
}

auto createContravariantNewton(C1Operator F, double relativeAccuracy, double eps, unsigned maxSteps)
{
  auto p = Newton::Parameter{};
  p.setRelativeAccuracy(relativeAccuracy);
  p.set_eps(eps);
  p.setMaxSteps(maxSteps);

  return [F,p](const Vector& x0)
  {
    return contravariantNewton(F,x0,p);
  };
}
