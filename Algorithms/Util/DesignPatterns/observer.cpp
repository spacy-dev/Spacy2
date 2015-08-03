#include "observer.hh"

namespace Algorithm
{
  namespace DesignPattern
  {
    namespace Observer
    {
      void Subject::attach(Observer* observer)
      {
        observers_.push_back(observer);
      }

      void Subject::detach(Observer* observer)
      {
        auto iend = end(observers_);
        for(auto iter = begin(observers_); iter!= iend; ++iter)
          if( *iter == observer )
          {
            observers_.erase(iter);
            return;
          }
      }

      void Subject::notify()
      {
        for( Observer* observer : observers_ )
          observer->update(this);
      }
    }
  }
}
