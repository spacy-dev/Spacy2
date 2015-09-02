#ifndef ALGORITHM_UTIL_MIXIN_TIMER_HH
#define ALGORITHM_UTIL_MIXIN_TIMER_HH

#include <chrono>

namespace Spacy
{
  namespace Mixin
  {
    /**
     * @ingroup MixinGroup
     * @brief %Mixin class providing a simple timer.
     * @tparam Unit time unit (std::chrono::seconds, std::chrono::milliseconds, ...)
     * @see std::chrono
     */
    template <class Unit = std::chrono::milliseconds>
    class Timer
    {
    public:
      /**
       * @brief Start timer.
       */
      void startTimer() const;

      /**
       * @brief Get elapsed time.
       * @return elapsed in time in specified time unit
       */
      typename std::chrono::high_resolution_clock::rep elapsedTime() const;

      /**
       * @brief Print elapsed time to std::cout.
       */
      void printElapsedTime() const;

    private:
      mutable std::chrono::time_point<std::chrono::high_resolution_clock> startTime = {};
    };
  }
}

#endif // ALGORITHM_UTIL_MIXIN_TIMER_HH
