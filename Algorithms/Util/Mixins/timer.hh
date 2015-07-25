#ifndef ALGORITHM_UTIL_MIXIN_TIMER_HH
#define ALGORITHM_UTIL_MIXIN_TIMER_HH

#include <chrono>

namespace Algorithm
{
  namespace Mixin
  {
    class Timer
    {
    public:
      void startTimer() const;

      double elapsedTime() const;

    private:
      mutable std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_TIMER_HH
