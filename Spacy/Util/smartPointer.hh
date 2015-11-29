// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_UTIL_SMART_POINTER_HH
#define SPACY_UTIL_SMART_POINTER_HH

#include <cassert>
#include <memory>
#include <type_traits>

namespace Spacy
{
  template < class Type >
  class CopyViaClonePtr
  {
  public:
    CopyViaClonePtr() = default;

    template <class T, class = std::enable_if_t< std::is_convertible<std::decay_t<T>*,Type*>::value > >
    explicit CopyViaClonePtr(T&& impl)
      : impl_(std::make_unique< std::decay_t<T> >(std::forward<T>(impl)))
    {}

    template <class T, class = std::enable_if_t< std::is_convertible<std::decay_t<T>*,Type*>::value > >
    explicit CopyViaClonePtr(std::unique_ptr<T>&& impl)
      : impl_(std::move(impl))
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

    template <class T, class = std::enable_if_t< std::is_convertible<std::decay_t<T>*,Type*>::value > >
    CopyViaClonePtr& operator=(T&& impl)
    {
      impl_ = std::make_unique< std::decay_t<T> >(std::forward<T>(impl));
      return *this;
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

    operator bool() const
    {
      return impl_ != nullptr;
    }

    bool operator==(std::nullptr_t&& t) const
    {
      return impl_ == t;
    }

  private:
    std::unique_ptr<Type> impl_ = nullptr;
  };

  template < class Type >
  class CopyingUniquePtr
  {
  public:
    CopyingUniquePtr() = default;

    template <class T, class = std::enable_if_t< std::is_convertible<std::decay_t<T>*,Type*>::value > >
    explicit CopyingUniquePtr(T&& impl)
      : impl_(std::make_unique< std::decay_t<T> >(std::forward<T>(impl)))
    {}

    template <class T, class = std::enable_if_t< std::is_convertible<std::decay_t<T>*,Type*>::value > >
    explicit CopyingUniquePtr(std::unique_ptr<T>&& impl)
      : impl_(std::move(impl))
    {}

    CopyingUniquePtr(const CopyingUniquePtr& other)
      : impl_( (other==nullptr) ? nullptr : std::make_unique<Type>(*other))
    {}

    CopyingUniquePtr(CopyingUniquePtr&&) = default;

    CopyingUniquePtr& operator=(CopyingUniquePtr&& other) = default;

    CopyingUniquePtr& operator=(const CopyingUniquePtr& other)
    {
      if(other)
        impl_ = std::make_unique<Type>(*other);
      else impl_ = nullptr;
      return *this;
    }

    template <class T, class = std::enable_if_t< std::is_convertible<std::decay_t<T>*,Type*>::value > >
    CopyingUniquePtr& operator=(T&& impl)
    {
      impl_ = std::make_unique< std::decay_t<T> >(std::forward<T>(impl));
      return *this;
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

    operator bool() const
    {
      return impl_ != nullptr;
    }

    bool operator==(std::nullptr_t&& t) const
    {
      return impl_ == t;
    }

  private:
    std::unique_ptr<Type> impl_ = nullptr;
  };
}

#endif // SPACY_UTIL_SMART_POINTER_HH
