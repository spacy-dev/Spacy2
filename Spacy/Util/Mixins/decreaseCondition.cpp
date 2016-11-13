#include "decreaseCondition.hh"

namespace Spacy
{
  namespace Mixin
  {
    DecreaseCondition::DecreaseCondition(Real minimalDecrease, Real relaxedMinimalDecrease) noexcept
      : minimalDecrease_(minimalDecrease), relaxedMinimalDecrease_(relaxedMinimalDecrease)
    {}

    void DecreaseCondition::setMinimalDecrease(Real decrease) noexcept
    {
      minimalDecrease_ = decrease;
    }

    Real DecreaseCondition::minimalDecrease() const noexcept
    {
      return minimalDecrease_;
    }

    void DecreaseCondition::setRelaxedMinimalDecrease(Real decrease) noexcept
    {
      relaxedMinimalDecrease_ = decrease;
    }

    bool DecreaseCondition::acceptableDecrease(Real decrease) const noexcept
    {
      return decrease > minimalDecrease_;
    }

    bool DecreaseCondition::acceptableRelaxedDecrease(Real decrease) const noexcept
    {
      return decrease > relaxedMinimalDecrease_;
    }
  }
}
