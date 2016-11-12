#pragma once

#include <Spacy/vectorSpace.hh>

#include <algorithm>

namespace Spacy
{
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
      static_cast<Derived*>(this)->get() += y.get();
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
      static_cast<Derived*>(this)->get() -= y.get();
      return static_cast<Derived&>(*this);
    }

    /**
     * @brief In-place multiplication \f$ x*=a\f$.
     * @param a scaling factor
     * @return \f$ x*=a\f$.
     */
    Derived& operator*=(double a)
    {
      static_cast<Derived*>(this)->get() *= a;
      return static_cast<Derived&>(*this);
    }

    /**
     * @brief Negation \f$ -x\f$.
     * @return \f$ -x \f$.
     */
    Derived operator-() const
    {
      Derived y = static_cast<const Derived&>(*this);
      y.get() *= -1;
      return y;
    }

    /**
     * @brief Comparison operator \f$ x==y\f$.
     * @param y vector to compare with this vector
     * @return \f$ x==y\f$.
     */
    bool operator==(const Derived& y) const
    {
      const auto& this_ = static_cast<const Derived&>(*this);
      checkSpaceCompatibility(this_.space(),y.space());
      auto max = std::max(get(this_(this_)), get(y(y)));
      if( max == 0 )
        max = 1;
      auto dx = y;
      dx -= this_;
      return get(dx(dx)) < max*y.space().eps()*y.space().eps();
    }
  };
}
