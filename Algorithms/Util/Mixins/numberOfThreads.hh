#ifndef ALGORITHM_MIXINS_NUMBER_OF_THREADS_
#define ALGORITHM_MIXINS_NUMBER_OF_THREADS_

#include "mixinConnection.hh"

namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @ingroup MixinGroup
     * @brief Mixin class for number of threads
     */
    class NumberOfThreads : public MixinConnection<NumberOfThreads>
    {
    public:
      /**
       * @brief Set number of allowed threads.
       */
      void setNumberOfThreads(unsigned nThreads);

      /**
       * @brief Access number of allowed threads.
       */
      unsigned nThreads() const noexcept;

      /**
       * @brief update function for observer pattern.
       */
      void update(NumberOfThreads* changed);

    private:
      unsigned nThreads_ = 1;
    };
  }
}

#endif // ALGORITHM_MIXINS_NUMBER_OF_THREADS_
