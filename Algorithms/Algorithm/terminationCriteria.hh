#ifndef AFFINECOVARIANTTERMINATIONCRITERION_HH
#define AFFINECOVARIANTTERMINATIONCRITERION_HH

#include "parameter.hh"
#include "Util/Mixins/mixins.hh"

namespace Algorithm
{
  class FunctionSpaceElement;
  class Norm;
  class Operator;

  class AffineCovariantRelativeTerminationCriterion
      : public Mixin::RelativeAccuracy, public Mixin::Verbosity, public Mixin::Eps
  {
  public:
    AffineCovariantRelativeTerminationCriterion(const Operator&, const Norm& norm, double relativeAccuracy, bool verbose = false);

    bool operator()(double nu, const FunctionSpaceElement& x, const FunctionSpaceElement& dx) const;

  private:
    const Norm& norm_;
  };

  class AffineContravariantRelativeTerminationCriterion
      : public Mixin::RelativeAccuracy, public Mixin::Verbosity, public Mixin::Eps
  {
  public:
    AffineContravariantRelativeTerminationCriterion(const Operator& F, const Norm& norm, double relativeAccuracy, bool verbose = false);

    bool operator()(double nu, const FunctionSpaceElement& x, const FunctionSpaceElement&) const;

  private:
    const Operator& F_;
    const Norm& norm_;
    mutable double initialResidual = -1;
  };
}

#endif // AFFINECOVARIANTTERMINATIONCRITERION_HH
