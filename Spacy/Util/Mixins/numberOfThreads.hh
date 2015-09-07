#ifndef SPACY_MIXINS_NUMBER_OF_THREADS_
#define SPACY_MIXINS_NUMBER_OF_THREADS_

namespace Spacy
{
  namespace Mixin
  {
    /**
     * @ingroup MixinGroup
     * @brief %Mixin class for number of threads
     */
    class NumberOfThreads
    {
    public:
      /**
       * @brief Set number of allowed threads.
       * @param nThreads number of threads
       */
      void setNumberOfThreads(unsigned nThreads = 1);

      /**
       * @brief Access number of allowed threads.
       * @return number of allowed threads
       */
      unsigned nThreads() const noexcept;

    private:
      unsigned nThreads_ = 1;
    };
  }
}

#endif // SPACY_MIXINS_NUMBER_OF_THREADS_