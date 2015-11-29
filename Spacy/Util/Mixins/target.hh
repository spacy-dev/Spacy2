// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_UTIL_MIXINS_TARGET_HH
#define SPACY_UTIL_MIXINS_TARGET_HH

namespace Spacy
{
  namespace Mixin
  {
    /// Mixin class that provides a member function target() to access type-erased content (@see std::function).
    template <class Derived>
    struct ToTarget
    {
      template <class T>
      T* target()
      {
        if( !static_cast<Derived*>(this)->base_ )
          return nullptr;

        if( dynamic_cast<typename Derived::template Base<T>*>(static_cast<Derived*>(this)->base_.get()) == nullptr )
          return nullptr;

        return &dynamic_cast<typename Derived::template Base<T>*>(static_cast<Derived*>(this)->base_.get())->impl();
      }

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
}

#endif // SPACY_UTIL_MIXINS_TARGET_HH
