// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_UTIL_TYPE_ERASED_STORAGE_HH
#define SPACY_UTIL_TYPE_ERASED_STORAGE_HH

#include <memory>
#include <type_traits>
#include <utility>

#include "smartPointer.hh"

namespace Spacy
{
  class TypeErasedStorage
  {
    struct AbstractBase
    {
      virtual ~AbstractBase(){}
      virtual std::unique_ptr<AbstractBase> clone() const = 0;
    };

    template <class Impl>
    struct Base : AbstractBase
    {
      explicit Base(const Impl& impl)
        : impl_(impl)
      {}

      explicit Base(Impl&& impl)
        : impl_(std::move(impl))
      {}

      std::unique_ptr<AbstractBase> clone() const final override
      {
        return std::make_unique< Base<Impl> >(impl_);
      }

      Impl impl_;
    };

  public:
    TypeErasedStorage() = default;

    template <class Impl>
    explicit TypeErasedStorage(Impl&& impl)
      : base_( Base< std::decay_t<Impl> >( std::forward<Impl>(impl) ) )
    {}

    template <class Impl>
    TypeErasedStorage& operator=(Impl&& impl)
    {
      base_ = Base< std::decay_t<Impl> >( std::forward<Impl>(impl) );
      return *this;
    }

    template <class Impl>
    const Impl* target() const
    {
      if( !base_ )
        return nullptr;
      return &dynamic_cast<const Base< std::decay_t<Impl> >&>(*base_).impl_;
    }

    template <class Impl>
    Impl* target()
    {
      if( !base_ )
        return nullptr;
      return &dynamic_cast<Base< std::decay_t<Impl> >&>(*base_).impl_;
    }

  private:
    friend bool is_empty(const TypeErasedStorage &storage);
    CopyViaClonePtr<AbstractBase> base_ = {};
  };

  inline bool is_empty(const TypeErasedStorage& storage)
  {
    return !storage.base_;
  }

  template <class T>
  std::decay_t<T>* target(TypeErasedStorage& storage)
  {
    return storage.template target< std::decay_t<T> >();
  }

  template <class T>
  const std::decay_t<T>* target(const TypeErasedStorage& storage)
  {
    return storage.template target< std::decay_t<T> >();
  }

  template <class T, class Return, class... Args>
  std::decay_t<T>* target(std::function<Return(Args...)>& storage)
  {
    return storage.template target< std::decay_t<T> >();
  }

  template <class T, class Return, class... Args>
  const std::decay_t<T>* target(const std::function<Return(Args...)>& storage)
  {
    return storage.template target< std::decay_t<T> >();
  }
}

#endif // SPACY_UTIL_TYPE_ERASED_STORAGE_HH
