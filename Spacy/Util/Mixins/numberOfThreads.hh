// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_MIXINS_NUMBER_OF_THREADS_
#define SPACY_MIXINS_NUMBER_OF_THREADS_

namespace Spacy
{
  namespace Mixin
  {
    /// %Mixin class for number of threads
    class NumberOfThreads
    {
    public:
      void setNumberOfThreads(unsigned nThreads = 1);

      /// Access number of allowed threads.
      unsigned nThreads() const noexcept;

    private:
      unsigned nThreads_ = 1;
    };
  }
}

#endif // SPACY_MIXINS_NUMBER_OF_THREADS_
