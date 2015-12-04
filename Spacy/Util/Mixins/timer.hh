// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_UTIL_MIXIN_TIMER_HH
#define SPACY_UTIL_MIXIN_TIMER_HH

#include <chrono>

namespace Spacy
{
  namespace Mixin
  {
    /**
     * @brief %Mixin class providing a simple timer.
     * @tparam Unit time unit (std::chrono::seconds, std::chrono::milliseconds, ...)
     */
    template <class Unit = std::chrono::milliseconds>
    class Timer
    {
    public:
      void startTimer() const;

      typename std::chrono::high_resolution_clock::rep elapsedTime() const;

      /// Print elapsed time to std::cout.
      void printElapsedTime() const;

    private:
      mutable std::chrono::time_point<std::chrono::high_resolution_clock> startTime = {};
    };
  }
}

#endif // SPACY_UTIL_MIXIN_TIMER_HH
