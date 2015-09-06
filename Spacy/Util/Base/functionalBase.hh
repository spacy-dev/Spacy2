#ifndef SPACY_FUNCTIONAL_BASE_HH
#define SPACY_FUNCTIONAL_BASE_HH

#include "Spacy/vector.hh"

namespace Spacy
{
  /// \cond
  class VectorSpace;
  /// \endcond

  /**
   * @brief Base class for twice differentiable functionals \f$ f:\ X\rightarrow \mathbb{R}\f$.
   */
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

#endif // SPACY_FUNCTIONAL_BASE_HH
