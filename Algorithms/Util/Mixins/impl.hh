#ifndef ALGORITHMS_UTIL_Impl_HH
#define ALGORITHMS_UTIL_Impl_HH

#include <memory>
#include <utility>
#include <stdexcept>

#include "Algorithms/Util/voider.hh"

namespace Algorithm
{
  namespace Mixin
  {
    /**
     * @ingroup MixinGroup
     * @brief Stores an object of type Type and provides access via member function impl().
     */
    template <class Type>
    class Impl
    {
    public:
      /// Default constructor.
      Impl() = default;

      /**
       * @brief Constructor.
       * @param impl value to be stored.
       */
      explicit Impl( Type impl)
        : impl_(std::move(impl))
      {}


      /**
       * @brief Access implementation.
       */
      Type& impl()
      {
        return impl_;
      }


      /**
       * @brief Access implementation.
       */
      const Type& impl() const
      {
        return impl_;
      }

    private:
      Type impl_;
    };

    /**
     * @ingroup MixinGroup
     * @brief Stores an object of type Type by const reference and provides access via member function impl().
     */
    template <class Type>
    class Impl<const Type&>
    {
    public:
      /// Default constructor.
      Impl() = default;

      /**
       * @brief Constructor.
       * @param impl value to be stored.
       */
      explicit Impl(const Type& impl)
        : impl_(impl)
      {}

      /**
       * @brief Access implementation.
       */
      const Type& impl() const
      {
        return impl_;
      }

    private:
      const Type& impl_;
    };

    /**
     * @ingroup MixinGroup
     * @brief Stores an object of type Type by const reference and provides access via member function impl().
     */
    template <class Type>
    using CRefImpl = Impl<const Type&>;

    /**
     * @ingroup MixinGroup
     * @brief Stores an object of type Type as unique pointer and provides access via member function impl().
     */
    template < class Type >
    class UniqueImpl
    {
    public:
      /// Default constructor.
      UniqueImpl() = default;

      /**
       * @brief Constructor.
       * @param impl value to be stored.
       */
      explicit UniqueImpl(std::unique_ptr<Type>&& impl)
        : impl_(std::move(impl))
      {}

      /**
       * @brief Access implementation.
       */
      Type& impl()
      {
        if( impl_ == nullptr ) throw std::runtime_error("impl_ undefined in UniqueImpl::impl().");
        return *impl_;
      }

      /**
       * @brief Access implementation.
       */
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

   /**
    * @ingroup MixinGroup
    * @brief Stores an object of type Type as shared pointer and provides access via member function impl().
    */
    template <class Type>
    class SharedImpl
    {
    public:
      /// Default constructor.
      SharedImpl() = default;

      /**
       * @brief Constructor.
       * @param impl value to be stored.
       */
      explicit SharedImpl(std::shared_ptr<Type> impl)
        : impl_(impl)
      {}


      /**
       * @brief Access implementation.
       */
      Type& impl()
      {
        if( impl_ == nullptr ) throw std::runtime_error("impl_ undefined in SharedImpl::impl().");
        return *impl_;
      }


      /**
       * @brief Access implementation.
       */
      const Type& impl() const
      {
        if( impl_ == nullptr ) throw std::runtime_error("impl_ undefined in SharedImpl::impl() const.");
        return *impl_;
      }
      /**
       * @brief Access shared pointer to implementation.
       */
      std::shared_ptr<Type> sharedImpl() const
      {
        return impl_;
      }

    private:
      std::shared_ptr<Type> impl_;
    };

    /**
     * @ingroup MixinGroup
     * @brief Stores an object of type Type as (non-owning) raw pointer and provides access via member function impl().
     */
    template <class Type>
    class NonOwningPImpl
    {
    public:
      /// Default constructor.
      NonOwningPImpl() = default;

      /**
       * @brief Constructor.
       * @param impl value to be stored.
       */
      explicit NonOwningPImpl(Type& impl)
        : impl_(&impl)
      {}


      /**
       * @brief Access implementation.
       */
      Type& impl()
      {
        if( impl_ == nullptr ) throw std::runtime_error("impl_ undefined in NonOwningPImpl::impl().");
        return *impl_;
      }


      /**
       * @brief Access implementation.
       */
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
}

#endif // ALGORITHMS_UTIL_Impl_HH

