// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

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
      verbosityLevel_ = 1;
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
