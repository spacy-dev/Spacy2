#ifndef SPACY_VECTOR_BASE_HH
#define SPACY_VECTOR_BASE_HH

#include <algorithm>
#include "Spacy/vectorSpace.hh"

namespace Spacy
{
  /// @cond
  void checkSpaceCompatibility(const VectorSpace* x, const VectorSpace* y);
  /// @endcond


  /**
   * @brief Base class for vector implementations.
   * @tparam Derived derived vector implementation, provided for CRTP
   *
   * Provides access to the underlying vector space and related operations.
   * @see Kaskade::Vector, Fenics::Vector
   */
  class VectorBase
  {
  public:
    /**
     * @brief Constructor.
     * @param space underlying vector space
     */
    VectorBase( const VectorSpace& space );

    /**
     * @brief Copy constructor.
     * @param y object to copy from
     */
    VectorBase(const VectorBase& y);

    /**
     * @brief Move constructor.
     * @param y object to move from
     */
    VectorBase(VectorBase&& y) noexcept;

    /**
     * @brief Copy assignment.
     * @param y object to copy from
     */
    VectorBase& operator=(const VectorBase& y);

    /**
     * @brief Move assignment.
     * @param y object to move from
     */
    VectorBase& operator=(VectorBase&& y) noexcept;

    /**
     * @brief Access underlying vector space.
     * @return underlying vector space
     */
    const VectorSpace* space() const;

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
  class AddArithmeticOperators
  {
  public:
    /**
     * @brief In-place summation \f$ x+=y\f$.
     * @param y vector to add to this vector
     * @return \f$ x+=y\f$.
     */
    Derived& operator+=(const Derived& y)
    {
      checkSpaceCompatibility(static_cast<const Derived*>(this)->space(),y.space());
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
      checkSpaceCompatibility(static_cast<const Derived*>(this)->space(),y.space());
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
     */
    bool operator==(const Derived& y) const
    {
      checkSpaceCompatibility(static_cast<const Derived*>(this)->space(),y.space());
      auto l2NormY = y(y), l2NormX = static_cast<const Derived&>(*this)( static_cast<const Derived&>(*this) );
      auto dx = y;
      dx -= static_cast<const Derived&>(*this);
      return dx(dx) < std::max(l2NormX,l2NormY)*y.space()->eps()*y.space()->eps();
    }
  };
}

#endif // SPACY_VECTOR_BASE_HH
