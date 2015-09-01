#ifndef ALGORITHM_MIXINS_EQUALITY_POINTER_COMPARISON_HH
#define ALGORITHM_MIXINS_EQUALITY_POINTER_COMPARISON_HH

#include <vector>

namespace VSA
{
  namespace Mixin
  {
    /**
     * @brief Connect objects of type Impl that provides a suitable method void update(Impl*) to another object of same type.
     */
    template <class Impl>
    class MixinConnection
    {
    protected:
      /// Attach observer.
      void attach(Impl& observer)
      {
        observers_.push_back(&observer);
      }

      /// Detach observer.
      void detach(Impl& observer)
      {
        auto iend = end(observers_);
        for(auto iter = begin(observers_); iter!= iend; ++iter)
          if( *iter == &observer )
          {
            observers_.erase(iter);
            return;
          }
      }

      /// Notify observers about changes.
      void notify()
      {
        for( Impl* observer : observers_ )
          observer->update(static_cast<Impl*>(this));
      }

    private:
      std::vector<Impl*> observers_ = {};
    };
  }
}

#endif // ALGORITHM_MIXINS_EQUALITY_POINTER_COMPARISON_HH
