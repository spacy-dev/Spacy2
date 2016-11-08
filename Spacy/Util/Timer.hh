#pragma once

#include <chrono>
#include <tuple>
#include <vector>

#ifndef SPACY_ENABLE_TIMERS
#define SPACY_START_TIMER(unit)
#define SPACY_PAUSE_TIMER
#define SPACY_END_TIMER(message)
#else
#define SPACY_START_TIMER(unit) Timer<unit> timer;
#define SPACY_PAUSE_TIMER timer.stop();
#define SPACY_END_TIMER(message) timer.stop(); timer.report(message);

namespace Spacy
{
    template <class Unit>
    class Timer
    {
    public:
        /**
         * @brief Start timer.
         *
         * Starts new or stopped timers, resumes paused timers.
         */
        void start();
        void stop();
        void reset();
        /**
         * @brief Print out timing result.
         * @param message message to be associated with the timing result.
         *
         * For printing std::printf is used. Thus the place where the timing result appears in the
         * message must be marked by '%i'.
         */
        void report(const char* message);

    private:
        using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;
        using TimeInterval = std::tuple<TimePoint,TimePoint>;
        std::vector<TimeInterval> intervals_;
    };
}

#endif
