#include "verbosity.hh"

namespace Spacy
{
  namespace Mixin
  {
    Verbosity::Verbosity(unsigned verbosityLevel) noexcept
      : verbosityLevel_(verbosityLevel)
    {}

    void Verbosity::setVerbosity(bool verbose)
    {
      verbosityLevel_ = verbose ? 1u : 0u;
      notify();
    }

    bool Verbosity::verbose() const noexcept
    {
      return verbosityLevel_ > 0;
    }

    void Verbosity::setVerbosityLevel(unsigned level) noexcept
    {
      verbosityLevel_ = level;
      notify();
    }

    unsigned Verbosity::getVerbosityLevel() const noexcept
    {
      return verbosityLevel_;
    }

    void Verbosity::update(Verbosity* changedSubject)
    {
      setVerbosityLevel( changedSubject->getVerbosityLevel() );
    }
  }
}
