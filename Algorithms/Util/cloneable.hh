#ifndef ALGORITHM_UTIL_CLONEABLE_HH
#define ALGORITHM_UTIL_CLONEABLE_HH

#include <memory>

namespace Algorithm
{
  template <class Type>
  struct Cloneable
  {
    using Base = Type;

    virtual ~Cloneable() = default;

  protected:
    virtual Type* cloneImpl() const = 0;

    template <class T>
    friend std::unique_ptr<T> Algorithm::clone(const T&);
  };

  template <class T>
  std::unique_ptr<T> clone(const T& t)
  {
    auto rawPtr = static_cast<const typename T::Base&>(t).cloneImpl();
    return std::unique_ptr<T>(static_cast<T*>(rawPtr));
  }

  template <class T>
  auto clone(T* t)
  {
    return clone(*t);
  }

  template <class T>
  auto clone(const std::unique_ptr<T>& t)
  {
    return clone(*t);
  }
}

#endif // ALGORITHM_UTIL_CLONEABLE_HH

