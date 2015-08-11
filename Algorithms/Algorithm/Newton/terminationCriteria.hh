#ifndef AFFINECOVARIANTTERMINATIONCRITERION_HH
#define AFFINECOVARIANTTERMINATIONCRITERION_HH

#include "Util/mixins.hh"

namespace Algorithm
{
  /// \cond
  class Vector;
  class Norm;
  class Operator;
  /// \endcond

  namespace Newton
  {
    namespace TerminationCriterion
    {
      class Base
          : public Mixin::RelativeAccuracy, public Mixin::Verbosity, public Mixin::Eps
      {
      public:
        Base(double relativeAccuracy, bool verbose);

        virtual ~Base() = default;

        virtual bool passed(double nu, const Vector& x, const Vector& dx) const = 0;
      };

      /**
       * @ingroup NewtonGroup
       * @brief Affine covariant relative error criterion.
       */
      class AffineCovariant : public Base
      {
      public:
        AffineCovariant(const Operator&, double relativeAccuracy, bool verbose = false);

        bool passed(double nu, const Vector& x, const Vector& dx) const final override;
      };

      /**
       * @ingroup NewtonGroup
       * @brief Affine contravariant relative error criterion.
       */
      class AffineContravariant : public Base
      {
      public:
        AffineContravariant(const Operator& F, double relativeAccuracy, bool verbose = false);

        bool passed(double nu, const Vector& x, const Vector&) const final override;

      private:
        const Operator& F_;
        mutable double initialResidual = -1;
      };
    }
  }
}

#endif // AFFINECOVARIANTTERMINATIONCRITERION_HH
