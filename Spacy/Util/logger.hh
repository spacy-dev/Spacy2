// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_UTIL_LOGGER_HH
#define SPACY_UTIL_LOGGER_HH

#include <fstream>
#include <string>

#include "Spacy/Util/voider.hh"

namespace Spacy
{
  /// @cond
  template <class T>
  using TryInStreamOperator = decltype( std::declval<std::ofstream>() << std::declval<T>() );
  /// @endcond

  /// Logger for some quantity of type T.
  template <class T,
            class = void_t< TryInStreamOperator<T> > >
  struct Logger
  {
    explicit Logger(const std::string& fileName)
      : logFile_(fileName)
    {}

    void operator()(const T& toLog)
    {
      logFile_ << toLog << separator_;
    }

  private:
    std::ofstream logFile_;
    char separator_ = '\n';
  };
}

#endif // SPACY_UTIL_LOGGER_HH
