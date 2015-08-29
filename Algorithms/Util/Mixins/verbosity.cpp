#include "verbosity.hh"

namespace Algorithm
{
  namespace Mixin
  {
    Verbosity::Verbosity(unsigned verbosityLevel) noexcept
      : verbosityLevel_(verbosityLevel)
    {}

    void Verbosity::setVerbosity(bool verbose)
    {
      verbosityLevel_ = 1;
      notify();
    }

//    void Verbosity::setDetailedVerbosity(bool verbose)
//    {
//      verbosityLevel_ = 2;
//      notify();
//    }

    bool Verbosity::verbose() const noexcept
    {
      return verbosityLevel_ > 0;
    }

//    bool Verbosity::verbose_detailed() const noexcept
//    {
//      return verbosityLevel_ > 1;
//    }

    void Verbosity::setVerbosityLevel(unsigned level) noexcept
    {
      verbosityLevel_ = level;
      notify();
    }

    unsigned Verbosity::verbosityLevel() const noexcept
    {
      return verbosityLevel_;
    }

    void Verbosity::update(Verbosity* changedSubject)
    {
      setVerbosityLevel( changedSubject->verbosityLevel() );
    }

    void Verbosity::attachVerbosity(Verbosity& other)
    {
      attach(other);
    }

    void Verbosity::detachVerbosity(Verbosity& other)
    {
      detach(other);
    }
  }
}
