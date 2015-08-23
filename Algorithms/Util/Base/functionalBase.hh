#ifndef ALGORITHM_FUNCTIONAL_BASE_HH
#define ALGORITHM_FUNCTIONAL_BASE_HH

#include "vector.hh"
#include "vectorSpace.hh"

namespace Algorithm
{
  /**
   * @brief Base class for functionals \f$A:\ X\rightarrow \mathbb{R}\f$.
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

    /// Access domain space \f$X\f$.
    VectorSpace& domain()
    {
      return *domain_;
    }

    Vector d1(const Vector& x) const
    {
      return static_cast<const Impl*>(this)->d1_(x);
    }

    double d1(const Vector &x, const Vector& dx) const
    {
      return d1(x)(dx);
    }

    Vector d2(const Vector& x, const Vector& dx) const
    {
      return static_cast<const Impl*>(this)->d2_(x,dx);
    }

    double d2(const Vector &x, const Vector &dx, const Vector& dy) const
    {
      return d2(x,dx)(dy);
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
