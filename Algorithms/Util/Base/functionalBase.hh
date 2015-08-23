#ifndef ALGORITHM_FUNCTIONAL_BASE_HH
#define ALGORITHM_FUNCTIONAL_BASE_HH

#include "vector.hh"

namespace Algorithm
{
  /// \cond
  class VectorSpace;
  /// \endcond

  /**
   * @brief Base class for functionals \f$ f:\ X\rightarrow \mathbb{R}\f$.
   */
  template <class Impl>
  class FunctionalBase
  {
  public:
    /**
     * @brief Constructor.
     * @param domain domain space \f$X\f$.
     */
    explicit FunctionalBase(VectorSpace* domain)
      : domain_(domain)
    {}

    /**
     * @brief Constructor.
     * @param domain domain space \f$X\f$.
     */
    explicit FunctionalBase(VectorSpace& domain)
      : FunctionalBase(&domain)
    {}

    /**
     * @brief Compute first directional derivative \f$f'(x) \in X^* \f$.
     */
    Vector d1(const Vector& x) const
    {
      return static_cast<const Impl*>(this)->d1_(x);
    }

    /**
     * @brief Compute first directional derivative \f$f'(x):\ X \rightarrow \mathbb{R} \f$.
     */
    double d1(const Vector &x, const Vector& dx) const
    {
      return d1(x)(dx);
    }

    /**
     * @brief Compute second directional derivative \f$f''(x):\ X \rightarrow X^* \f$.
     */
    Vector d2(const Vector& x, const Vector& dx) const
    {
      return static_cast<const Impl*>(this)->d2_(x,dx);
    }

    /**
     * @brief Compute second directional derivative \f$f''(x):\ X\times\X \rightarrow \mathbb{R} \f$.
     */
    double d2(const Vector &x, const Vector &dx, const Vector& dy) const
    {
      return d2(x,dx)(dy);
    }

    /// Access domain space \f$X\f$.
    VectorSpace& domain()
    {
      return *domain_;
    }

    /// Access domain space \f$X\f$.
    const VectorSpace& domain() const
    {
      return *domain_;
    }

    /// Access pointer to domain space \f$X\f$.
    VectorSpace* domain_ptr() const
    {
      return domain_;
    }

  private:
    VectorSpace *domain_;
  };
}

#endif // ALGORITHM_FUNCTIONAL_BASE_HH
