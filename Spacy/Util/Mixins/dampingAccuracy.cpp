#include "dampingAccuracy.hh"

namespace Spacy
{
  namespace Mixin
  {
    DampingAccuracy::DampingAccuracy(double accuracy) noexcept
      : dampingAccuracy_(accuracy)
    {}

    void DampingAccuracy::setDampingAccuracy(double accuracy) noexcept
    {
      dampingAccuracy_ = accuracy;
    }

    double DampingAccuracy::dampingAccuracy() const noexcept
    {
      return dampingAccuracy_;
    }
  }
}
