#ifndef ALGORITHMS_UTIL_Impl_HH
#define ALGORITHMS_UTIL_Impl_HH

#include <memory>
#include <utility>
#include <stdexcept>

#include "Util/voider.hh"

namespace Algorithm
{
  namespace Mixin
  {
    template <class Type>
    class Impl
    {
    public:
      Impl() = default;

      explicit Impl(const Type& impl)
        : impl_(impl)
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

    template <class Type>
    class Impl<const Type&>
    {
    public:
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

    template <class Type>
    using CRefImpl = Impl<const Type&>;

    template <class Type>
    class MutableImpl
    {
    public:
      MutableImpl() = default;

      explicit MutableImpl(const Type& impl)
        : impl_(impl)
      {}

      /**
     * @brief Access implementation.
     */
      Type& impl() const
      {
        return impl_;
      }

    private:
      mutable Type impl_;
    };

    template < class Type >
    class UniqueImpl
    {
    public:
      explicit UniqueImpl(std::unique_ptr<Type>&& impl)
        : impl_(std::move(impl))
      {}

      void setImpl(std::unique_ptr<Type>&& impl)
      {
        impl_ = std::move(impl);
      }

      /**
     * @brief Access implementation.
     */
      Type& impl()
      {
        if( impl_ == nullptr ) throw std::runtime_error("impl_ undefined in Impl<std::unique_ptr<...> >.");
        return *impl_;
      }

      /**
     * @brief Access implementation.
     */
      const Type& impl() const
      {
        if( impl_ == nullptr ) throw std::runtime_error("impl_ undefined in Impl<std::unique_ptr<...> >.");
        return *impl_;
      }

      bool implIsNullPtr() const
      {
        return impl_ == nullptr;
      }

    private:
      std::unique_ptr<Type> impl_;
    };


    template <class Type>
    class SharedImpl
    {
    public:
      SharedImpl() = default;

      explicit SharedImpl(std::shared_ptr<Type> impl)
        : impl_(impl)
      {}


      /**
     * @brief Access implementation.
     */
      Type& impl()
      {
        if( impl_ == nullptr ) throw std::runtime_error("impl_ undefined in Impl<std::shared_ptr<...> >.");
        return *impl_;
      }


      /**
     * @brief Access implementation.
     */
      const Type& impl() const
      {
        if( impl_ == nullptr ) throw std::runtime_error("impl_ undefined in Impl<std::shared_ptr<...> >.");
        return *impl_;
      }

      std::shared_ptr<Type> sharedImpl() const
      {
        return impl_;
      }

    private:
      std::shared_ptr<Type> impl_;
    };

    template <class Type>
    class NonOwningPImpl
    {
    public:
      NonOwningPImpl() = default;

      explicit NonOwningPImpl(Type& impl)
        : impl_(&impl)
      {}


//      /**
//     * @brief Access implementation.
//     */
//      Type& impl()
//      {
//        if( impl_ == nullptr ) throw std::runtime_error("impl_ undefined in Impl<std::shared_ptr<...> >.");
//        return *impl_;
//      }


      /**
     * @brief Access implementation.
     */
      const Type& impl() const
      {
        if( impl_ == nullptr ) throw std::runtime_error("impl_ undefined in Impl<std::shared_ptr<...> >.");
        return *impl_;
      }

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

