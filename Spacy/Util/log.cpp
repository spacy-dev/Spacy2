#include "log.hh"

#ifdef SPACY_ENABLE_LOGGING
#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>

namespace Spacy
{
    namespace Log
    {
        namespace
        {
            std::string adjust(std::string name)
            {
                auto max_size = 15u;
                auto offset = (name.size() > max_size) ? std::string() : std::string(max_size - name.size(), ' ');
                return name += offset;
            }
        }


        StreamPrinter::StreamPrinter() :
            os_(std::cout)
        {}

        StreamPrinter::StreamPrinter(std::ostream& os) :
            os_(os)
        {}

        void StreamPrinter::operator()(const char* tag, const char* name, const Printable& printable)
        {
            printable(os_ << '[' << tag << "] " << adjust(name));
            os_ << '\n';
        }


        FilePrinter::FilePrinter(const char* filename)
        {
            file_stream_.open(filename);
        }

        FilePrinter::~FilePrinter()
        {
            file_stream_.close();
        }

        void FilePrinter::operator()(const char* tag, const char* name, const Printable& printable)
        {
            StreamPrinter(file_stream_).operator()(tag, name, printable);
        }


        Logger::Logger()
        {
            setPrinter(StreamPrinter{});
        }

        Logger& Logger::get()
        {
            static Logger logger{};
            return logger;
        }

        void Logger::enable(const char* tag)
        {
            disabled_.erase(std::find_if(begin(disabled_), end(disabled_),
                                         [&tag](const typename decltype(disabled_)::value_type& entry)
                                         { return entry.first == tag; })
                            );
        }

        void Logger::disable(const char* tag)
        {
            if( !is_disabled(tag) )
                disabled_[tag] = true;
        }

        void Logger::disable(std::vector<std::string> tags)
        {
            // remove duplicates
            tags.erase(std::remove_if(begin(tags), end(tags),
                                      [this](const std::string& tag)
                                      { return is_disabled(tag.c_str()); }),
                       end(tags));

            disabled_.reserve(disabled_.size() + tags.size());
            for(const auto& tag : tags)
                disabled_.emplace(std::make_pair(tag,true));
        }

        void Logger::setPrinter(Printer&& printer)
        {
            printer_.clear();
            addPrinter(std::move(printer));
        }

        void Logger::addPrinter(Printer&& printer)
        {
            printer_.emplace_back(std::move(printer));
        }

        void Logger::clear()
        {
            printer_.clear();
        }

        bool Logger::is_disabled(const char* tag) const
        {
            return std::find(begin(disabled_), end(disabled_),
                             std::pair<const std::string,bool>(tag,true)) != end(disabled_);
        }
    }
}
#endif
