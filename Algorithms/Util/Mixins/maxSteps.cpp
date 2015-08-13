#include "maxSteps.hh"

namespace Algorithm
{
  namespace Mixin
  {
    MaxSteps::MaxSteps(unsigned maxSteps) noexcept
      : maxSteps_(maxSteps)
    {}

    void MaxSteps::setMaxSteps(unsigned maxSteps) noexcept
    {
      maxSteps_ = maxSteps;
      notify();
    }

    unsigned MaxSteps::maxSteps() const noexcept
    {
      return maxSteps_;
    }


    void MaxSteps::update(DesignPattern::Observer::Subject *changedSubject)
    {
      if( changedSubject == this ) return;
      setMaxSteps( dynamic_cast<MaxSteps*>(changedSubject)->maxSteps() );
    }

    void MaxSteps::attachMaxSteps(MaxSteps* other)
    {
      attach(other);
    }

    void MaxSteps::detachMaxSteps(MaxSteps* other)
    {
      detach(other);
    }
  }
}
