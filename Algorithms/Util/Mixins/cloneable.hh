#ifndef ALGORITHM_UTIL_MIXINS_CLONEABLE_HH
#define ALGORITHM_UTIL_MIXINS_CLONEABLE_HH

#include <memory>

namespace Algorithm
{
  namespace Mixin
  {
    template <class Type>
    struct Cloneable
    {
      using Base = Type;

      virtual ~Cloneable() = default;

    protected:
      virtual Type* cloneImpl() const = 0;

      template <class T>
      friend std::unique_ptr<T> cloneInMixin(const T&);
    };


    template <class T>
    std::unique_ptr<T> cloneInMixin(const T& t)
    {
      auto rawPtr = static_cast<const typename T::Base&>(t).cloneImpl();
      return std::unique_ptr<T>(static_cast<T*>(rawPtr));
    }

    template <class T>
    auto cloneInMixin(T* t)
    {
      return cloneInMixin(*t);
    }

    template <class T>
    auto cloneInMixin(const std::unique_ptr<T>& t)
    {
      return cloneInMixin(*t);
    }
  }

  template <class T>
  auto clone(T&& t)
  {
    return Mixin::cloneInMixin(std::forward<T>(t));
  }
}

#endif // ALGORITHM_UTIL_MIXINS_CLONEABLE_HH

