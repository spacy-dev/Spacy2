#pragma once

#ifndef SPACY_ENABLE_LOGGING
#define SET_LOG_PRINTER(printer)
#define ADD_LOG_PRINTER(printer)
#define DEFINE_LOG_TAG(tag)
#define ENABLE_LOG_TAG(tag)
#define DISABLE_LOG_TAG(tag)
#define LOG_INFO(tag, text)
#define LOG(tag, ...)
#define LOG_SEPARATOR
#else
#define SET_LOG_PRINTER(printer) ::Spacy::Log::Logger::get().setPrinter(printer);
#define ADD_LOG_PRINTER(printer) ::Spacy::Log::Logger::get().addPrinter(printer);
#define DEFINE_LOG_TAG(tag) tag;
#define ENABLE_LOG_TAG(tag) ::Spacy::Log::Logger::get().enable(tag);
#define DISABLE_LOG_TAG(tag) ::Spacy::Log::Logger::get().disable(tag);
#define LOG_INFO(tag, text) ::Spacy::Log::log(tag, text, "");
#define LOG(tag, ...) ::Spacy::Log::log(tag, __VA_ARGS__);
#define LOG_SEPARATOR(tag) ::Spacy::Log::log(tag, "", ""); \
    ::Spacy::Log::log(tag, "--------------------------------------------------", ""); \
    ::Spacy::Log::log(tag, "", "")

#include <functional>
#include <fstream>
#include <ostream>
#include <unordered_map>
#include <utility>
#include <vector>

#include "printer.hh"

namespace Spacy
{
    namespace Log
    {
        /// Wraps printing to stream.
        using Printable = std::function<void(std::ostream&)>;

        /**
         * @brief Writes t to std::ostream using the stream-operator.
         *
         * Default implementation for Logger.
         *
         * In case you want to log types for which there is no operator<<,
         * you can provide overloads to this function for implementing/customizing
         * writing to streams.
         */
        template <class T>
        Printable make_printable(const T& t)
        {
            return [&t](std::ostream& os) { os << t; };
        }

        /// Simple printer for std::ostream.
        class StreamPrinter
        {
        public:
            /// Construct printer for std::cout.
            StreamPrinter();

            /// Construct printer for os.
            explicit StreamPrinter(std::ostream& os);

            /// @copydoc ::Spacy::Log::Printer::operator()
            void operator()(const char* tag, const char* name, const Printable& printable);

        private:
            std::ostream& os_;
        };

        /// Simple file printer
        class FilePrinter
        {
        public:
            /// Construct printer that writes to file 'filename'.
            explicit FilePrinter(const char* filename);

            FilePrinter(FilePrinter&&) = default;

            FilePrinter& operator=(FilePrinter&&) = default;

            /// Closes file stream.
            ~FilePrinter();

            /// @copydoc ::Spacy::Log::Printer::operator()
            void operator()(const char* tag, const char* name, const Printable& printable);

        private:
            std::ofstream file_stream_;
        };

        /**
         * @brief Logger for %Spacy.
         *
         *
         */
        class Logger
        {
        public:
            Logger(const Logger&) = delete;
            Logger& operator=(const Logger&) = delete;

            /// Access logger. Logs to std::cout by default.
            static Logger& get();


            /**
             * @brief Log value.
             * @param level level of logged value
             * @param name name of the logged value
             * @param value logged value
             */
            template <class Arg>
            void log(const char* tag, const char* name, const Arg& value)
            {
                if( !is_disabled(tag) )
                    for(auto& printer : printer_)
                        printer(tag, name, make_printable(value));
            }

            /// Enable all logs with given tag.
            void enable(const char* tag);

            /// Disable all logs with given tag.
            void disable(const char* tag);

            /// Disable all logs with given tags.
            void disable(std::vector<std::string> tags);

            /// Sets printer, deletes all previously added/set printers.
            void setPrinter(Printer&& printer);

            /// Add printer.
            void addPrinter(Printer&& printer);

            /// Remove all printers.
            void clear();

        private:
            /// @brief Default constructor, logs to std::cout.
            Logger();

            bool is_disabled(const char* tag) const;

            std::vector< Printer > printer_{};
            std::unordered_map<std::string,bool> disabled_;
        };

        template <class Arg>
        void log(const char* tag, const char* name, const Arg& value)
        {
            Log::Logger::get().log(tag, name, value);
        }


        /// @cond
        template <typename...>
        struct CombineAndLog;

        template <class StringArg, class Arg>
        struct CombineAndLog<StringArg,Arg>
        {
            static void apply(const char* tag, const StringArg& name, const Arg& arg)
            {
                log(tag, name, arg);
            }
        };

        template <class StringArg, class Arg, typename... Args_>
        struct CombineAndLog<StringArg,Arg,Args_...>
        {
            template <typename... Args>
            static void apply(const char* tag, const StringArg& name, const Arg& arg, const Args&... args)
            {
                log(tag, name, arg);
                CombineAndLog<Args...>::apply(tag, args...);
            }
        };
        /// @endcond

        /**
         * @brief Log an arbitrary number of arguments with its names on a fixed level.
         * @brief level level for all arguments
         * @brief arguments (name1, value1, name2, value2, ...)
         */
        template <typename... Args>
        void log(const char* tag, const Args&... args)
        {
            CombineAndLog<Args...>::apply(tag, args...);
        }
    }
}
#endif
