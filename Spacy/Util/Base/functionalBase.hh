#ifndef ALGORITHM_FUNCTIONAL_BASE_HH
#define ALGORITHM_FUNCTIONAL_BASE_HH

#include "Spacy/vector.hh"

namespace Spacy
{
  /// \cond
  class VectorSpace;
  /// \endcond

  /**
   * @brief Base class for twice differentiable functionals \f$ f:\ X\rightarrow \mathbb{R}\f$.
   */
  template <class Functional>
  class C2FunctionalBase
  {
  public:
    /**
     * @brief Constructor.
     * @param domain domain space \f$X\f$.
     */
    explicit C2FunctionalBase(const VectorSpace& domain)
      : domain_(domain)
    {}

    /**
     * @brief Compute first directional derivative \f$f'(x) \in X^* \f$.
     *
     * Calls Functional::d1_(x) via CRTP.
     *
     * @param x current iterate
     * @return \f$f'(x)\f$
     */
    ::Spacy::Vector d1(const ::Spacy::Vector& x) const
    {
      return static_cast<const Functional*>(this)->d1_(x);
    }

    /**
     * @brief Compute first directional derivative \f$f'(x)dx \in \mathbb{R} \f$.
     *
     * @param x current iterate
     * @param dx perturbation
     * @return \f$f'(x)dx\f$
     */
    double d1(const ::Spacy::Vector &x, const ::Spacy::Vector& dx) const
    {
      return d1(x)(dx);
    }

    /**
     * @brief Compute second directional derivative \f$f''(x)dx\in X^* \f$.
     *
     * Calls Functionals::d2_(x) via CRTP.
     *
     * @param x current iterate
     * @param dx perturbation
     * @return \f$f''(x)dx\f$
     */
    ::Spacy::Vector d2(const ::Spacy::Vector& x, const ::Spacy::Vector& dx) const
    {
      return static_cast<const Functional*>(this)->d2_(x,dx);
    }

    /**
     * @brief Compute second directional derivative \f$f''(x)(dx,dy) \in \mathbb{R} \f$.
     * @param x current iterate
     * @param dx perturbation
     * @param dy perturbation
     * @return \f$f''(x)(dx,dy)\f$
     */
    double d2(const ::Spacy::Vector &x, const ::Spacy::Vector &dx, const ::Spacy::Vector& dy) const
    {
      return d2(x,dx)(dy);
    }

    /**
     * @brief Access domain space \f$X\f$.
     * @return domain space \f$X\f$.
     */
    const VectorSpace& domain() const
    {
      return domain_;
    }

  private:
    const VectorSpace& domain_;
  };
}

#endif // ALGORITHM_FUNCTIONAL_BASE_HH
