#pragma once

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
