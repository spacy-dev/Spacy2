#pragma once

#include <functional>
#include <ostream>

namespace Spacy
{
    namespace Log
    {
        /// Interface for logger's printers.
        class Printer
        {
        public:
            /**
             * @brief Log printable according to its level and name.
             * @param tag specifies the algorithm/function this log originates from
             * @param name name of the logged value
             * @param printable wraps writing of the logged value to some stream
             */
            void operator()(const char* tag, const char* name, const std::function<void(std::ostream&)>& printable);
        };
    }
}
