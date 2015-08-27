#include "numberOfThreads.hh"

namespace Algorithm
{
  namespace Mixin
  {
    void NumberOfThreads::setNumberOfThreads(unsigned nThreads)
    {
      nThreads_ = nThreads;
      notify();
    }

    unsigned NumberOfThreads::nThreads() const noexcept
    {
      return nThreads_;
    }

    void NumberOfThreads::update(NumberOfThreads *changed)
    {
      setNumberOfThreads( changed->nThreads() );
    }
  }
}
