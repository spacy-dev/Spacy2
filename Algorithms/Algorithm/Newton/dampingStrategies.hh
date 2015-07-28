#ifndef ALGORITHM_NEWTON_DAMPINGSTRATEGIES_HH
#define ALGORITHM_NEWTON_DAMPINGSTRATEGIES_HH

#include "functionSpaceElement.hh"
#include "Algorithm/dampingFactor.hh"
#include "Util/Mixins/eps.hh"
#include "Util/Mixins/regularityTest.hh"

namespace Algorithm
{
  /// \cond
  class C1Operator;
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

        virtual DampingFactor compute(const LinearSolver&, const FunctionSpaceElement&, const FunctionSpaceElement&) = 0;
      };

      /**
       * @ingroup NewtonGroup
       * @brief Affine covariant damping strategy as described in Deuflhard: Newton Methods for Nonlinear Problems, Sec. 3.3.
       */
      class AffineCovariant : public Base
      {
      public:
        AffineCovariant(const C1Operator& F);

        DampingFactor compute(const LinearSolver& DFInv_, const FunctionSpaceElement& x, const FunctionSpaceElement& dx) final override;

      private:
        const C1Operator& F_;

        DampingFactor oldNu = -1;
        double normOldDx = -1, normOldDs = -1;
        FunctionSpaceElement oldDs;
      };

      /**
       * @ingroup NewtonGroup
       * @brief Affine contravariant damping strategy as described in Deuflhard: Newton Methods for Nonlinear Problems, Sec. 3.2.
       */
      class AffineContravariant : public Base
      {
      public:
        AffineContravariant(const C1Operator& F);

        DampingFactor compute(const LinearSolver&, const FunctionSpaceElement& x, const FunctionSpaceElement& dx) final override;

      private:
        const C1Operator& F_;

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
        None(const C1Operator& F);

        DampingFactor compute(const LinearSolver&, const FunctionSpaceElement&, const FunctionSpaceElement&);
      };
    }
  }
}

#endif // ALGORITHM_NEWTON_DAMPINGSTRATEGIES_HH
