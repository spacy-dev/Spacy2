#ifndef ALGORITHM_VECTOR_BASE_HH
#define ALGORITHM_VECTOR_BASE_HH

#include "Algorithms/Util/Mixins/eps.hh"
#include "Algorithms/Util/Mixins/impl.hh"
#include "Algorithms/Util/Exceptions/incompatibleSpaceException.hh"
#include "Algorithms/vectorSpace.hh"

namespace Algorithm
{
  /// @cond
  template <class V>
  void checkSameSpaces(const V& x, const V& y);
  /// @endcond

  /**
   * @brief Base class for vector implementations.
   *
   * Provides access to the underlying vector space and related operations.
   * Stores an object of type Value. This object holds the employed vector implementation.
   * @see Real::Vector
   */
  template <class Derived, class Value = void>
  class VectorBase :
      public VectorBase<Derived> , public Mixin::Impl<Value>
  {
  public:
    /**
     * @brief Constructor.
     * @param space underlying vector space
     * @param value to be stored in Mixin::Impl<Value>
     */
    VectorBase( const VectorSpace& space , Value value )
      : VectorBase<Derived>(space) , Mixin::Impl<Value>(std::move(value))
    {}
  };


  /**
   * @brief Base class for vector implementations.
   * @tparam Derived derived vector implementation, provided for CRTP
   *
   * Provides access to the underlying vector space and related operations.
   * @see Kaskade::Vector, Fenics::Vector
   */
  template <class Derived>
  class VectorBase<Derived,void>
  {
  public:
    /**
     * @brief Constructor.
     * @param space underlying vector space
     */
    VectorBase( const VectorSpace& space )
      : space_(space)
    {}

    /**
     * @brief Copy constructor.
     * @param y object to copy from
     */
    VectorBase(const VectorBase& y)
      : space_(y.space_)
    {}

    /**
     * @brief Move constructor.
     * @param y object to move from
     */
    VectorBase(VectorBase&& y) noexcept
      : space_(y.space_)
    {}

    /**
     * @brief Copy assignment.
     * @param y object to copy from
     */
    VectorBase& operator=(const VectorBase& y)
    {
      checkSameSpaces(*this,y);
      return *this;
    }

    /**
     * @brief Move assignment.
     * @param y object to move from
     */
    VectorBase& operator=(VectorBase&& y) noexcept
    {
      checkSameSpaces(*this,y);
      return *this;
    }

    /**
     * @brief Access underlying vector space.
     * @return underlying vector space
     */
    const VectorSpace* space() const
    {
      return &space_;
    }

  private:
    const VectorSpace& space_;
  };

  /**
   * @brief Base class providing some operations for vectors via CRTP.
   * @tparam Derived derived vector implementation, provided for CRTP
   *
   * Provides operator+=, operator-=, operator*=, operator-, operator==.
   *
   * @see Real::Vector, Fenics::Vector, Kaskade::Vector
   */
  template <class Derived>
  class SupportedOperatorBase : public Mixin::Eps
  {
  public:
    /**
     * @brief In-place summation \f$ x+=y\f$.
     * @param y vector to add to this vector
     * @return \f$ x+=y\f$.
     */
    Derived& operator+=(const Derived& y)
    {
      checkSameSpaces(static_cast<const Derived&>(*this),y);
      static_cast<Derived*>(this)->impl() += y.impl();
      return static_cast<Derived&>(*this);
    }

    /**
     * @brief In-place subtraction \f$ x-=y\f$.
     * @param y vector to subtract from this vector
     * @return \f$ x-=y\f$.
     */
    Derived& operator-=(const Derived& y)
    {
      checkSameSpaces(static_cast<const Derived&>(*this),y);
      static_cast<Derived*>(this)->impl() -= y.impl();
      return static_cast<Derived&>(*this);
    }

    /**
     * @brief In-place multiplication \f$ x*=a\f$.
     * @param a scaling factor
     * @return \f$ x*=a\f$.
     */
    Derived& operator*=(double a)
    {
      static_cast<Derived*>(this)->impl() *= a;
      return static_cast<Derived&>(*this);
    }

    /**
     * @brief Negation \f$ -x\f$.
     * @return \f$ -x \f$.
     */
    Derived operator-() const
    {
      Derived y = static_cast<const Derived&>(*this);
      y.impl() *= -1;
      return y;
    }

    /**
     * @brief Comparison operator \f$ x==y\f$.
     * @param y vector to compare with this vector
     * @return \f$ x==y\f$.
     */    bool operator==(const Derived& y) const
    {
      checkSameSpaces(static_cast<const Derived&>(*this),y);
      auto dx = y;
      dx -= static_cast<const Derived&>(*this);
      return dx(dx) < eps()*eps();
    }
  };
}

#endif // ALGORITHM_VECTOR_BASE_HH
