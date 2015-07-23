#ifndef AFFINECOVARIANTTERMINATIONCRITERION_HH
#define AFFINECOVARIANTTERMINATIONCRITERION_HH

#include "Algorithm/parameter.hh"
#include "Util/mixins.hh"

namespace Algorithm
{
  class FunctionSpaceElement;
  class Norm;
  class Operator;

  namespace Newton
  {
    namespace TerminationCriterion
    {
      class AffineCovariant
          : public Mixin::RelativeAccuracy, public Mixin::Verbosity, public Mixin::Eps
      {
      public:
        AffineCovariant(const Operator&, const Norm& norm, double relativeAccuracy, bool verbose = false);

        bool operator()(double nu, const FunctionSpaceElement& x, const FunctionSpaceElement& dx) const;

      private:
        const Norm& norm_;
      };

      class AffineContravariant
          : public Mixin::RelativeAccuracy, public Mixin::Verbosity, public Mixin::Eps
      {
      public:
        AffineContravariant(const Operator& F, const Norm& norm, double relativeAccuracy, bool verbose = false);

        bool operator()(double nu, const FunctionSpaceElement& x, const FunctionSpaceElement&) const;

      private:
        const Operator& F_;
        const Norm& norm_;
        mutable double initialResidual = -1;
      };
    }
  }
}

#endif // AFFINECOVARIANTTERMINATIONCRITERION_HH
