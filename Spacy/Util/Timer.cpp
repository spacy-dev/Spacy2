#include "Timer.hh"

#ifdef SPACY_ENABLE_TIMERS
#include <algorithm>
#include <cstdio>

namespace
{
    template <class Unit> struct Name;

    template <>
    struct Name<std::chrono::hours>
    {
        static const char* get() { return "hours"; }
    };

    template <>
    struct Name<std::chrono::minutes>
    {
        static const char* get() { return "minutes"; }
    };

    template <>
    struct Name<std::chrono::seconds>
    {
        static const char* get() { return "seconds"; }
    };

    template <>
    struct Name<std::chrono::milliseconds>
    {
        static const char* get() { return "milliseconds"; }
    };

    template <>
    struct Name<std::chrono::microseconds>
    {
        static const char* get() { return "microseconds"; }
    };

    template <>
    struct Name<std::chrono::nanoseconds>
    {
        static const char* get() { return "nanoseconds"; }
    };
}

namespace Spacy
{
    template <class Unit>
    void Timer<Unit>::start()
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        intervals_.emplace_back( start_time, start_time ) ;
    }

    template <class Unit>
    void Timer<Unit>::stop()
    {
        std::get<1>( intervals_.back() ) = std::chrono::high_resolution_clock::now();
    }

    template <class Unit>
    void Timer<Unit>::reset()
    {
        intervals_.clear();
    }

    template <class Unit>
    void Timer<Unit>::report(const char* message)
    {
        std::printf(message, std::accumulate(begin(intervals_), end(intervals_), 0,
                                             [](typename Unit::rep current, const Timer<Unit>::TimeInterval& interval)
        {
            return current +=  std::chrono::duration_cast<Unit>( std::get<1>(interval) - std::get<0>(interval) ).count();
        }));
    }

    template class Timer<std::chrono::hours>;
    template class Timer<std::chrono::minutes>;
    template class Timer<std::chrono::seconds>;
    template class Timer<std::chrono::milliseconds>;
    template class Timer<std::chrono::microseconds>;
    template class Timer<std::chrono::nanoseconds>;
}
#endif
