#include "decreaseCondition.hh"

namespace VSA
{
  namespace Mixin
  {
    DecreaseCondition::DecreaseCondition(double minimalDecrease, double relaxedMinimalDecrease) noexcept
      : minimalDecrease_(minimalDecrease), relaxedMinimalDecrease_(relaxedMinimalDecrease)
    {}

    void DecreaseCondition::setMinimalDecrease(double decrease) noexcept
    {
      minimalDecrease_ = decrease;
    }

    double DecreaseCondition::minimalDecrease() const noexcept
    {
      return minimalDecrease_;
    }

    void DecreaseCondition::setRelaxedMinimalDecrease(double decrease) noexcept
    {
      relaxedMinimalDecrease_ = decrease;
    }

    bool DecreaseCondition::acceptableDecrease(double decrease) const noexcept
    {
      return decrease > minimalDecrease_;
    }

    bool DecreaseCondition::acceptableRelaxedDecrease(double decrease) const noexcept
    {
      return decrease > relaxedMinimalDecrease_;
    }
  }
}
