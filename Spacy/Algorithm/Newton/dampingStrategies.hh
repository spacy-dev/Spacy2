#pragma once

#include <Spacy/vector.hh>
#include <Spacy/Spaces/ScalarSpace/real.hh>
#include <Spacy/Algorithm/dampingFactor.hh>
#include <Spacy/Util/Mixins/eps.hh>
#include <Spacy/Util/Mixins/regularityTest.hh>

#include <functional>

namespace Spacy
{
  /// @cond
  class C1Operator;
  /// @endcond

  namespace Newton
  {
    namespace Damping
    {
      /**
       * @ingroup NewtonGroup
       * @brief Affine covariant damping strategy as described in @cite Deuflhard2004 [Sec. 3.3].
       */
      class AffineCovariant : public Mixin::Eps, public Mixin::RegularityTest
      {
      public:
        /// Constructor.
        AffineCovariant(const C1Operator& F);

        /// Compute damping factor.
        DampingFactor operator()(const std::function<Vector(const Vector&)>& DFInv_, const Vector& x, const Vector& dx) const;

      private:
        const C1Operator& F_;

        mutable DampingFactor oldNu{-1};
        mutable Real normOldDx = Real{-1}, normOldDs = Real{-1};
        mutable Vector oldDs;
      };

      /**
       * @ingroup NewtonGroup
       * @brief Affine contravariant damping strategy as described in @cite Deuflhard2004 [Sec. 3.2].
       */
      class AffineContravariant : public Mixin::Eps, public Mixin::RegularityTest
      {
      public:
        /// Constructor.
        AffineContravariant(const C1Operator& F);

        /// @copydoc AffineCovariant::operator()()
        DampingFactor operator()(const std::function<Vector(const Vector&)>&, const Vector& x, const Vector& dx) const;

      private:
        const C1Operator& F_;
        mutable Real muPrime = Real{-1}, norm_F_x_old = Real{-1};
      };

      /**
       * @ingroup NewtonGroup
       * @brief No damping, yields local newton method.
       */
      class None
      {
      public:
        /// Constructor.
        None(const C1Operator&);

        /**
         * @brief Compute damping factor. Always returns 1.
         * @return DampingFactor(1)
         */
        DampingFactor operator()(const std::function<Vector(const Vector&)>&, const Vector&, const Vector&) const;
      };
    }
  }
}
