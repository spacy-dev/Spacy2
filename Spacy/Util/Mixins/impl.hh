#ifndef SPACYS_UTIL_Impl_HH
#define SPACYS_UTIL_Impl_HH

#include <memory>
#include <utility>
#include <stdexcept>
#include <type_traits>

#include "Spacy/Util/voider.hh"

namespace Spacy
{
  /** @addtogroup MixinGroup
   * @{
   */
  namespace Mixin
  {
    /// Stores an object of type Type and provides access via member function impl().
    template <class Type>
    class Impl
    {
    public:
      Impl()
        : impl_{}
      {}

      /**
       * @brief Constructor.
       * @param impl value to be stored.
       */
      explicit Impl( Type impl)
        : impl_(std::move(impl))
      {}

      /// Access implementation.
      Type& impl()
      {
        return impl_;
      }

      /// Access implementation.
      const Type& impl() const
      {
        return impl_;
      }

    private:
      Type impl_;
    };

    /// Stores an object of type Type by const reference and provides access via member function impl().
    template <class Type>
    class Impl<const Type&>
    {
    public:
      /**
       * @brief Constructor.
       * @param impl value to be stored.
       */
      explicit Impl(const Type& impl)
        : impl_(impl)
      {}

      /// Access implementation.
      const Type& impl() const
      {
        return impl_;
      }

    private:
      const Type& impl_;
    };

    /// Stores an object of type Type by const reference and provides access via member function impl().
    template <class Type>
    class Impl<Type&>
    {
    public:
      /**
       * @brief Constructor.
       * @param impl value to be stored.
       */
      explicit Impl(Type& impl)
        : impl_(impl)
      {}

      /// Access implementation.
      Type& impl()
      {
        return impl_;
      }

      /// Access implementation.
      const Type& impl() const
      {
        return impl_;
      }

    private:
      Type& impl_;
    };

    /// Stores an object of type Type by const reference and provides access via member function impl().
    template <class Type>
    using CRefImpl = Impl<const Type&>;

    template <class Type>
    using RefImpl = Impl<Type&>;

    /// Stores an object of type Type as unique pointer and provides access via member function impl().
    template < class Type >
    class UniqueImpl
    {
    public:
      UniqueImpl() = default;

      /**
       * @brief Constructor.
       * @param impl value to be stored.
       */
      explicit UniqueImpl(std::unique_ptr<Type>&& impl)
        : impl_(std::move(impl))
      {}

      /// Access implementation.
      Type& impl()
      {
        if( impl_ == nullptr ) throw std::runtime_error("impl_ undefined in UniqueImpl::impl().");
        return *impl_;
      }

      /// Access implementation.
      const Type& impl() const
      {
        if( impl_ == nullptr ) throw std::runtime_error("impl_ undefined in UniqueImpl::impl() const.");
        return *impl_;
      }

      /// Check if stored object is null pointer.
      bool implIsNullPtr() const
      {
        return impl_ == nullptr;
      }

    private:
      std::unique_ptr<Type> impl_ = nullptr;
    };

    /// Stores an object of type Type as unique pointer and provides access via member function impl().
    template < class Type >
    class CopyViaCloneUniqueImpl
    {
    public:
      CopyViaCloneUniqueImpl() = default;

      virtual ~CopyViaCloneUniqueImpl() = default;

      template <class T, class = std::enable_if_t< std::is_convertible<std::decay_t<T>*,Type*>::value > >
      explicit CopyViaCloneUniqueImpl(T&& impl)
        : impl_(std::make_unique< std::decay_t<T> >(std::forward<T>(impl)))
      {}

      template <class T, class = std::enable_if_t< std::is_convertible<std::decay_t<T>*,Type*>::value > >
      explicit CopyViaCloneUniqueImpl(std::unique_ptr<T>&& impl)
        : impl_(std::move(impl))
      {}

      CopyViaCloneUniqueImpl(const CopyViaCloneUniqueImpl& other)
        : impl_(other.impl().clone())
      {}

      CopyViaCloneUniqueImpl(CopyViaCloneUniqueImpl&&) = default;

      CopyViaCloneUniqueImpl& operator=(CopyViaCloneUniqueImpl&& other) = default;

      CopyViaCloneUniqueImpl& operator=(const CopyViaCloneUniqueImpl& other)
      {
        impl_ = other.impl().clone();
        return *this;
      }

      template <class T, class = std::enable_if_t< std::is_convertible<std::decay_t<T>*,Type*>::value > >
      CopyViaCloneUniqueImpl& operator=(T&& impl)
      {
        impl_ = std::make_unique< std::decay_t<T> >(std::forward<T>(impl));
        return *this;
      }

      /// Access implementation.
      Type& impl()
      {
        if( impl_ == nullptr ) throw std::runtime_error("impl_ undefined in UniqueImpl::impl().");
        return *impl_;
      }

      /// Access implementation.
      const Type& impl() const
      {
        if( impl_ == nullptr ) throw std::runtime_error("impl_ undefined in UniqueImpl::impl() const.");
        return *impl_;
      }

      template <class T>
      friend bool is_empty(const CopyViaCloneUniqueImpl<T>& impl);

    private:
      std::unique_ptr<Type> impl_ = nullptr;
    };

    /// Stores an object of type Type as unique pointer and provides access via member function impl().
    template < class Type >
    class CopyingUniqueImpl
    {
    public:
      CopyingUniqueImpl() = default;

      virtual ~CopyingUniqueImpl() = default;

      template <class T, class = std::enable_if_t< std::is_convertible<std::decay_t<T>*,Type*>::value > >
      explicit CopyingUniqueImpl(T&& impl)
        : impl_(std::make_unique< std::decay_t<T> >(std::forward<T>(impl)))
      {}

      template <class T, class = std::enable_if_t< std::is_convertible<std::decay_t<T>*,Type*>::value > >
      explicit CopyingUniqueImpl(std::unique_ptr<T>&& impl)
        : impl_(std::move(impl))
      {}

      CopyingUniqueImpl(const CopyingUniqueImpl& other)
        : impl_(std::make_unique<Type>(other.impl()))
      {}

      CopyingUniqueImpl(CopyingUniqueImpl&&) = default;

      CopyingUniqueImpl& operator=(CopyingUniqueImpl&& other) = default;

      CopyingUniqueImpl& operator=(const CopyingUniqueImpl& other)
      {
        impl_ = std::make_unique<Type>(other.impl());
        return *this;
      }

      template <class T, class = std::enable_if_t< std::is_convertible<std::decay_t<T>*,Type*>::value > >
      CopyingUniqueImpl& operator=(T&& impl)
      {
        impl_ = std::make_unique< std::decay_t<T> >(std::forward<T>(impl));
        return *this;
      }

      /// Access implementation.
      Type& impl()
      {
        if( impl_ == nullptr ) throw std::runtime_error("impl_ undefined in UniqueImpl::impl().");
        return *impl_;
      }

      /// Access implementation.
      const Type& impl() const
      {
        if( impl_ == nullptr ) throw std::runtime_error("impl_ undefined in UniqueImpl::impl() const.");
        return *impl_;
      }

      template <class T>
      friend bool is_empty(const CopyingUniqueImpl<T>& impl);

    private:
      std::unique_ptr<Type> impl_ = nullptr;
    };

    template <class Type>
    bool is_empty(const CopyingUniqueImpl<Type>& impl)
    {
      return impl.impl_ == nullptr;
    }

    template <class Type>
    bool is_empty(const CopyViaCloneUniqueImpl<Type>& impl)
    {
      return impl.impl_ == nullptr;
    }

   /// Stores an object of type Type as shared pointer and provides access via member function impl().
    template <class Type>
    class SharedImpl
    {
    public:
      SharedImpl() = default;

      /**
       * @brief Constructor.
       * @param impl value to be stored.
       */
      explicit SharedImpl(std::shared_ptr<Type> impl)
        : impl_(impl)
      {}

      /// Access implementation.
      Type& impl()
      {
        if( impl_ == nullptr ) throw std::runtime_error("impl_ undefined in SharedImpl::impl().");
        return *impl_;
      }

      /// Access implementation.
      const Type& impl() const
      {
        if( impl_ == nullptr ) throw std::runtime_error("impl_ undefined in SharedImpl::impl() const.");
        return *impl_;
      }

      /// Access shared pointer to implementation.
      std::shared_ptr<Type> sharedImpl() const
      {
        return impl_;
      }

    private:
      std::shared_ptr<Type> impl_;
    };

    /// Stores an object of type Type as (non-owning) raw pointer and provides access via member function impl().
    template <class Type>
    class NonOwningPImpl
    {
    public:
      NonOwningPImpl() = default;

      /**
       * @brief Constructor.
       * @param impl value to be stored.
       */
      explicit NonOwningPImpl(Type& impl)
        : impl_(&impl)
      {}

      /// Access implementation.
      Type& impl()
      {
        if( impl_ == nullptr ) throw std::runtime_error("impl_ undefined in NonOwningPImpl::impl().");
        return *impl_;
      }

      /// Access implementation.
      const Type& impl() const
      {
        if( impl_ == nullptr ) throw std::runtime_error("impl_ undefined in NonOwningPImpl::impl() const.");
        return *impl_;
      }

      /**
       * @brief Access pointer to implementation.
       */
      Type* impl_ptr() const
      {
        return impl_;
      }

    private:
      Type* impl_ = nullptr;
    };
  }
  /** @} */
}

#endif // SPACYS_UTIL_Impl_HH

