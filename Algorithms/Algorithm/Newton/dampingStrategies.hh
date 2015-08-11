#ifndef ALGORITHM_NEWTON_DAMPINGSTRATEGIES_HH
#define ALGORITHM_NEWTON_DAMPINGSTRATEGIES_HH

#include "vector.hh"
#include "Algorithm/dampingFactor.hh"
#include "Util/Mixins/eps.hh"
#include "Util/Mixins/regularityTest.hh"

namespace Algorithm
{
  /// \cond
  class Operator;
  class LinearSolver;
  class Norm;
  /// \endcond

  namespace Newton
  {
    namespace DampingStrategy
    {
      class Base : public Mixin::Eps, public Mixin::RegularityTest
      {
      public:
        virtual ~Base() = default;

        virtual DampingFactor compute(const LinearSolver&, const Vector&, const Vector&) = 0;
      };

      /**
       * @ingroup NewtonGroup
       * @brief Affine covariant damping strategy as described in Deuflhard: Newton Methods for Nonlinear Problems, Sec. 3.3.
       */
      class AffineCovariant : public Base
      {
      public:
        AffineCovariant(const Operator& F);

        DampingFactor compute(const LinearSolver& DFInv_, const Vector& x, const Vector& dx) final override;

      private:
        const Operator& F_;

        DampingFactor oldNu = -1;
        double normOldDx = -1, normOldDs = -1;
        Vector oldDs;
      };

      /**
       * @ingroup NewtonGroup
       * @brief Affine contravariant damping strategy as described in Deuflhard: Newton Methods for Nonlinear Problems, Sec. 3.2.
       */
      class AffineContravariant : public Base
      {
      public:
        AffineContravariant(const Operator& F);

        DampingFactor compute(const LinearSolver&, const Vector& x, const Vector& dx) final override;

      private:
        const Operator& F_;

        double muPrime = -1.;
        double norm_F_x_old = -1;
      };

      /**
       * @ingroup NewtonGroup
       * @brief No damping.
       */
      class None : public Base
      {
      public:
        None(const Operator& F);

        DampingFactor compute(const LinearSolver&, const Vector&, const Vector&);
      };
    }
  }
}

#endif // ALGORITHM_NEWTON_DAMPINGSTRATEGIES_HH
