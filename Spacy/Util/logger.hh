#pragma once

#include <fstream>
#include <string>

#include <Spacy/Util/voider.hh>

namespace Spacy
{
  /// @cond
  template <class T>
  using TryInStreamOperator = decltype( std::declval<std::ofstream>() << std::declval<T>() );
  /// @endcond

  /// Logger for some quantity of type T.
  template <class T,
            class = voider< TryInStreamOperator<T> > >
  struct Logger
  {
    explicit Logger(const std::string& fileName)
      : logFile_(fileName)
    {}

    void operator()(const T& toLog)
    {
      logFile_ << toLog << separator_;
      logFile_.flush();
    }

  private:
    std::ofstream logFile_;
    char separator_ = '\n';
  };
}
