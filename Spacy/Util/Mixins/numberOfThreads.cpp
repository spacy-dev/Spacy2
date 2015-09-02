#include "numberOfThreads.hh"

namespace Spacy
{
  namespace Mixin
  {
    void NumberOfThreads::setNumberOfThreads(unsigned nThreads)
    {
      nThreads_ = nThreads;
    }

    unsigned NumberOfThreads::nThreads() const noexcept
    {
      return nThreads_;
    }
  }
}
