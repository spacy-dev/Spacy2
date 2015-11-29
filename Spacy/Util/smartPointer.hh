// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_UTIL_SMART_POINTER_HH
#define SPACY_UTIL_SMART_POINTER_HH

#include <cassert>
#include <memory>
#include <type_traits>

#include "Spacy/Util/memFnChecks.hh"

namespace Spacy
{
  template < class Type ,
             class Deleter = std::default_delete<Type> ,
             class = void_t<TryMemFn_clone<Type>> >
  class CopyViaClonePtr
  {
  public:
    CopyViaClonePtr() = default;

    template <class... Args,
              class = std::enable_if_t< std::is_constructible<Type,Args...>::value > >
    explicit CopyViaClonePtr(Args&&... args)
      : impl_(std::make_unique<Type>(std::forward<Args>(args)...))
    {}

    template <class T,
              class TDeleter,
              class = std::enable_if_t<std::is_constructible<std::unique_ptr<Type,Deleter>,std::unique_ptr<T,TDeleter>>::value> >
    CopyViaClonePtr(std::unique_ptr<T,TDeleter>&& impl)
      : impl_(std::move(impl))
    {}

    template <class T, class TDeleter,
              class = std::enable_if_t<std::is_constructible<std::unique_ptr<Type,Deleter>,std::unique_ptr<T,TDeleter>>::value> >
    CopyViaClonePtr(const CopyViaClonePtr<T,TDeleter>& other)
      : impl_( ( other == nullptr ) ? nullptr : other->clone() )
    {}

    template <class T, class TDeleter,
              class = std::enable_if_t<std::is_constructible<std::unique_ptr<Type,Deleter>,std::unique_ptr<T,TDeleter>>::value> >
    CopyViaClonePtr(CopyViaClonePtr<T,TDeleter>&& other)
      : impl_( other.release() )
    {}

    CopyViaClonePtr(const CopyViaClonePtr& other)
      : impl_( ( other == nullptr ) ? nullptr : other->clone() )
    {}

    CopyViaClonePtr(CopyViaClonePtr&&) = default;

    CopyViaClonePtr& operator=(CopyViaClonePtr&& other) = default;

    CopyViaClonePtr& operator=(const CopyViaClonePtr& other)
    {
      if( other )
        impl_ = other->clone();
      else
        impl_ = nullptr;
      return *this;
    }

    template <class T, class TDeleter,
              class = std::enable_if_t<std::is_assignable<std::unique_ptr<Type,Deleter>,std::unique_ptr<T,TDeleter>>::value> >
    CopyViaClonePtr& operator=(const CopyViaClonePtr& other)
    {
      if( other )
        impl_ = other->clone();
      else
        impl_ = nullptr;
      return *this;
    }


    template <class T, class TDeleter,
              class = std::enable_if_t<std::is_assignable<std::unique_ptr<Type,Deleter>,std::unique_ptr<T,TDeleter>>::value> >
    CopyViaClonePtr& operator=(CopyViaClonePtr&& other)
    {
      impl_.reset( other.release() );
      return *this;
    }

    template <class T,
              class TDeleter,
              class = std::enable_if_t<std::is_assignable<std::unique_ptr<Type,Deleter>,std::unique_ptr<T,TDeleter>>::value> >
    CopyViaClonePtr& operator=(std::unique_ptr<T,TDeleter>&& impl)
    {
      impl_ = std::move(impl);
    }

    /// Access implementation.
    Type& operator*()
    {
      assert( impl_ != nullptr );
      return *impl_;
    }

    /// Access implementation.
    const Type& operator*() const
    {
      assert( impl_ != nullptr );
      return *impl_;
    }

    Type* operator->() const noexcept
    {
      return get();
    }

    Type* get() const
    {
      return impl_.get();
    }

    Type* release()
    {
      return impl_.release();
    }

    operator bool() const
    {
      return impl_ != nullptr;
    }

    bool operator==(std::nullptr_t&& t) const
    {
      return impl_ == t;
    }

    bool operator==(const CopyViaClonePtr& other) const
    {
      return impl_ == other.impl_;
    }

    bool operator!=(std::nullptr_t&& t) const
    {
      return impl_ != t;
    }

    bool operator!=(const CopyViaClonePtr& other) const
    {
      return impl_ != other.impl_;
    }

  private:
    std::unique_ptr<Type,Deleter> impl_ = nullptr;
  };
}

#endif // SPACY_UTIL_SMART_POINTER_HH
