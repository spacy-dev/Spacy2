#include "verbosity.hh"

namespace Algorithm
{
  namespace Mixin
  {
    Verbosity::Verbosity(bool verbose) noexcept
      : verbose_(verbose)
    {}

    void Verbosity::setVerbosity(bool verbose) noexcept
    {
      verbose_ = verbose;
      notify();
    }

    void Verbosity::setDetailedVerbosity(bool verbose) noexcept
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

    void Verbosity::update(DesignPattern::Observer::Subject *changedSubject)
    {
      if( changedSubject == this ) return;
      setVerbosity( dynamic_cast<Verbosity*>(changedSubject)->verbose() );
      setDetailedVerbosity( dynamic_cast<Verbosity*>(changedSubject)->verbose_detailed() );
    }

    void Verbosity::attachVerbosity(Verbosity* other)
    {
      attach(other);
    }

    void Verbosity::detachVerbosity(Verbosity* other)
    {
      detach(other);
    }
  }
}
