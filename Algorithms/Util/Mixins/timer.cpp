#include "timer.hh"

namespace Algorithm
{
  namespace Mixin
  {
    void Timer::startTimer() const
    {
      startTime = std::chrono::high_resolution_clock::now();
    }

    double Timer::elapsedTime() const
    {
      return std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - startTime).count();
    }
  }
}
