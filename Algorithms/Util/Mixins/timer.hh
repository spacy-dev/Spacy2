#ifndef ALGORITHM_UTIL_MIXIN_TIMER_HH
#define ALGORITHM_UTIL_MIXIN_TIMER_HH

#include <chrono>

namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @ingroup MixinGroup
     * @brief Mixin class providing a simple timer.
     * @tparam Unit time unit (std::chrono::seconds, std::chrono::milliseconds, ...)
     * @see std::chrono
     */
    template <class Unit = std::chrono::milliseconds>
    class Timer
    {
    public:
      /// Start new measurement.
      void startTimer() const;

      /// Get elapsed time
      typename std::chrono::high_resolution_clock::rep elapsedTime() const;

      /// Print elapsed time.
      void printElapsedTime() const;

    private:
      mutable std::chrono::time_point<std::chrono::high_resolution_clock> startTime = {};
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_TIMER_HH
