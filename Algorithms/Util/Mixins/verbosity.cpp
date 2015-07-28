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
      forward(verbose_);
    }

    void Verbosity::setDetailedVerbosity(bool verbose) noexcept
    {
      verbose_detailed_ = verbose;
    }

    bool Verbosity::verbose() const noexcept
    {
      return verbose_;
    }

    bool Verbosity::verbose_detailed() const noexcept
    {
      return verbose_detailed_;
    }
  }
}
