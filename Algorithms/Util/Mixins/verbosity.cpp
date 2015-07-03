#include "verbosity.hh"

namespace Algorithm
{
  namespace Mixin
  {
    Verbosity::Verbosity(bool verbose)
      : verbose_(verbose)
    {}

    void Verbosity::setVerbosity(bool verbose) noexcept
    {
      verbose_ = verbose;
    }

    bool Verbosity::verbose() const noexcept
    {
      return verbose_;
    }
  }
}
