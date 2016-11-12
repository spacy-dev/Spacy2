#include "timer.hh"

#include <cassert>
#include <iostream>
#include <type_traits>

namespace Spacy
{
  namespace Mixin
  {
    namespace
    {
      template <class Unit>
      auto ending()
      {
        if( std::is_same<Unit,std::chrono::hours>() ) return "h.";
        if( std::is_same<Unit,std::chrono::minutes>() ) return "m.";
        if( std::is_same<Unit,std::chrono::seconds>() ) return "s.";
        if( std::is_same<Unit,std::chrono::milliseconds>() ) return "ms.";
        if( std::is_same<Unit,std::chrono::microseconds>() ) return "mus.";
        if( std::is_same<Unit,std::chrono::nanoseconds>() ) return "ns.";

        assert(false);
        return "should never get here.";
      }
    }

    template <class Unit>
    void Timer<Unit>::startTimer() const
    {
      startTime = std::chrono::high_resolution_clock::now();
    }

    template <class Unit>
    std::chrono::high_resolution_clock::rep Timer<Unit>::elapsedTime() const
    {
      return std::chrono::duration_cast<Unit>(std::chrono::high_resolution_clock::now() - startTime).count();
    }

    template <class Unit>
    void Timer<Unit>::printElapsedTime() const
    {
      std::cout << "Elapsed time: " << elapsedTime() << ending<Unit>() << std::endl;
    }

    template class Timer<std::chrono::hours>;
    template class Timer<std::chrono::minutes>;
    template class Timer<std::chrono::seconds>;
    template class Timer<std::chrono::milliseconds>;
    template class Timer<std::chrono::microseconds>;
    template class Timer<std::chrono::nanoseconds>;
  }
}
