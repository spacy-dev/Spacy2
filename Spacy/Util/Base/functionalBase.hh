// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#ifndef SPACY_FUNCTIONAL_BASE_HH
#define SPACY_FUNCTIONAL_BASE_HH

namespace Spacy
{
  /// \cond
  class VectorSpace;
  /// \endcond

  /// Base class for twice differentiable functionals \f$ f:\ X\rightarrow \mathbb{R}\f$.
  class FunctionalBase
  {
  public:
    /**
     * @brief Constructor.
     * @param domain domain space \f$X\f$.
     */
    explicit FunctionalBase(const VectorSpace& domain);

    /// Access domain space \f$X\f$.
    const VectorSpace& domain() const;

  private:
    const VectorSpace& domain_;
  };
}

#endif // SPACY_FUNCTIONAL_BASE_HH
