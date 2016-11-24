#pragma once

#include <Spacy/Spaces/ScalarSpace/real.hh>

namespace Spacy
{
  namespace Mixin
  {
    /// %Mixin class for accepting local models \f$m\f$ of nonlinear optimization problems \f$\min f(x)\f$.
    class DecreaseCondition
    {
    public:
      /**
       * @brief Constructor.
       * @param minimalDecrease minimal required decrease
       * @param relaxedMinimalDecrease relaxed required decrease
       */
      explicit DecreaseCondition(Real minimalDecrease = 0.05, Real relaxedMinimalDecrease = 0.01) noexcept;

      /**
       * @brief Set required minimal decrease.
       * @param decrease minimal required decrease
       */
      void setMinimalDecrease(Real decrease) noexcept;

      /**
       * @brief Access minimal decrease.
       * @return minimal decrease
       */
      Real minimalDecrease() const noexcept;

      /**
       * @brief Set relaxed minimal decrease.
       * @param decrease relaxed required decrease
       *
       * This is used for deciding about rejecting tangential steps in CompositeSteps::AffineCovariantSolver.
       */
      void setRelaxedMinimalDecrease(Real decrease) noexcept;

      /**
       * @brief Decide if measure relative decrease is acceptable.
       * @param decrease measured relative decrease \f$\delta m/\delta f\f$.
       */
      bool acceptableDecrease(Real decrease) const noexcept;

      /**
       * @brief Decide if measure relative decrease is acceptable with respect to the relaxed decrease condition.
       * @param decrease measured relative decrease \f$\delta m/\delta f\f$.
       */
      bool acceptableRelaxedDecrease(Real decrease) const noexcept;

    private:
      Real minimalDecrease_, relaxedMinimalDecrease_;
    };
  }
}
