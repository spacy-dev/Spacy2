// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_UTIL_MIXINS_TARGET_HH
#define SPACY_UTIL_MIXINS_TARGET_HH

namespace Spacy
{
  namespace Mixin
  {
    /// Mixin class that provides a member function target() to access type-erased content (same behaviour as @see std::function::target()).
    template <class Derived>
    struct ToTarget
    {
      /// Cast to T* if possible, else returns nullptr.
      template <class T>
      T* target()
      {
        if( !static_cast<Derived*>(this)->base_ )
          return nullptr;

        if( dynamic_cast<typename Derived::template Base<T>*>(static_cast<Derived*>(this)->base_.get()) == nullptr )
          return nullptr;

        return &dynamic_cast<typename Derived::template Base<T>*>(static_cast<Derived*>(this)->base_.get())->impl();
      }

      /// Cast to const T* if possible, else returns nullptr.
      template <class T>
      const T* target() const
      {
        if( !static_cast<const Derived*>(this)->base_ )
          return nullptr;

        if( dynamic_cast<const typename Derived::template Base<T>*>(static_cast<const Derived*>(this)->base_.get()) == nullptr )
          return nullptr;

        return &dynamic_cast<const typename Derived::template Base<T>*>(static_cast<const Derived*>(this)->base_.get())->impl();
      }
    };
  }

  /// Convenient access to t.template target<T>().
  template <class T, class Derived,
            class = std::enable_if_t< std::is_base_of<Mixin::ToTarget<std::decay_t<Derived> >,std::decay_t<Derived>>::value> >
  auto target(Derived&& t)
  {
    return t.template target<T>();
  }

  /// Convenient access to f.template target<T>().
  template <class T, class Signature>
  T* target(std::function<Signature>& f)
  {
    return f.template target<T>();
  }

  /// Convenient access to f.template target<T>().
  template <class T, class Signature>
  const T* target(const std::function<Signature>& f)
  {
    return f.template target<T>();
  }
}

#endif // SPACY_UTIL_MIXINS_TARGET_HH
