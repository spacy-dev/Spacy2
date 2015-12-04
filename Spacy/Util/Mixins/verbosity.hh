// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_UTIL_MIXIN_VERBOSITY_HH
#define SPACY_UTIL_MIXIN_VERBOSITY_HH

#include "mixinConnection.hh"

namespace Spacy
{
  namespace Mixin
  {
    /// %Mixin class for verbosity.
    class Verbosity : public MixinConnection<Verbosity>
    {
    public:
      /**
       * @brief Constructor.
       * @param verbosityLevel verbosity level (0 = silent, 1 = print information, 2 = print detailed information)
       */
      explicit Verbosity(unsigned verbosityLevel = 0) noexcept;

      /**
       * @brief Enable/disable verbosity.
       * @param verbose true: if verbosityLevel = 0, set verbosityLevel = 1; false: if set verbosityLevel = 0
       */
      void setVerbosity(bool verbose);

      /// Check if verbosityLevel > 0
      bool verbose() const noexcept;

      void setVerbosityLevel(unsigned level) noexcept;

      /// Access verbosity level.
      unsigned verbosityLevel() const noexcept;

      /// update function for observer pattern.
      void update(Verbosity* changedSubject);

    private:
      unsigned verbosityLevel_;
    };
  }
}

#endif // SPACY_UTIL_MIXIN_VERBOSITY_HH
