#ifndef ALGORITHMS_UTIL_Impl_HH
#define ALGORITHMS_UTIL_Impl_HH

#include <memory>
#include <utility>

namespace Algorithm
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
  class Impl< std::unique_ptr<Type> >
  {
  public:
    explicit Impl(std::unique_ptr<Type>&& impl)
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
      return *impl_;
    }

    /**
     * @brief Access implementation.
     */
    const Type& impl() const
    {
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
  using UniqueImpl = Impl< std::unique_ptr<Type> >;

  template <class Type>
  class Impl< std::shared_ptr<Type> >
  {
  public:
    Impl() = default;

    explicit Impl(std::shared_ptr<Type> impl)
      : impl_(impl)
    {}


    /**
     * @brief Access implementation.
     */
    Type& impl()
    {
      return *impl_;
    }


    /**
     * @brief Access implementation.
     */
    const Type& impl() const
    {
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
  using SharedImpl = Impl< std::shared_ptr<Type> >;
}

#endif // ALGORITHMS_UTIL_Impl_HH

