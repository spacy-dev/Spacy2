#include "maxSteps.hh"

namespace Spacy
{
  namespace Mixin
  {
    MaxSteps::MaxSteps(unsigned maxSteps) noexcept
      : maxSteps_(maxSteps)
    {}

    void MaxSteps::setMaxSteps(unsigned maxSteps)
    {
      maxSteps_ = maxSteps;
      notify();
    }

    unsigned MaxSteps::maxSteps() const noexcept
    {
      return maxSteps_;
    }


    void MaxSteps::update(MaxSteps* changedSubject)
    {
      setMaxSteps( changedSubject->maxSteps() );
    }

    void MaxSteps::attachMaxSteps(MaxSteps& other)
    {
      attach(other);
    }

    void MaxSteps::detachMaxSteps(MaxSteps& other)
    {
      detach(other);
    }
  }
}
