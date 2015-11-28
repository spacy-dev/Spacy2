// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPAYC_UTIL_BASE_ADD_ARITHMETIC_OPERATORS_HH
#define SPAYC_UTIL_BASE_ADD_ARITHMETIC_OPERATORS_HH

#include <algorithm>
#include "Spacy/vectorSpace.hh"

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
      auto l2NormY = toDouble(y(y)), l2NormX = toDouble(static_cast<const Derived&>(*this)( static_cast<const Derived&>(*this) ));
      auto max = std::max(l2NormX,l2NormY);
      if( max == 0 )
        max = 1;
      auto dx = y;
      dx -= static_cast<const Derived&>(*this);
      return Real(dx(dx)) < max*y.space()->eps()*y.space()->eps();
    }
  };
}

#endif // SPAYC_UTIL_BASE_ADD_ARITHMETIC_OPERATORS_HH
