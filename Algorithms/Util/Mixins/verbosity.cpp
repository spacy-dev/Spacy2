#include "verbosity.hh"

namespace Algorithm
{
  namespace Mixin
  {
    Verbosity::Verbosity(bool verbose) noexcept
      : verbose_(verbose)
    {}

    void Verbosity::setVerbosity(bool verbose)
    {
      verbose_ = verbose;
      notify();
    }

    void Verbosity::setDetailedVerbosity(bool verbose)
    {
      verbose_detailed_ = verbose;
      notify();
    }

    bool Verbosity::verbose() const noexcept
    {
      return verbose_;
    }

    bool Verbosity::verbose_detailed() const noexcept
    {
      return verbose_detailed_;
    }

    void Verbosity::update(Verbosity* changedSubject)
    {
      setVerbosity( changedSubject->verbose() );
      setDetailedVerbosity( changedSubject->verbose_detailed() );
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
