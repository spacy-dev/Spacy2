#ifndef SPACY_NORM_HH
#define SPACY_NORM_HH

#include <functional>

namespace Spacy
{
  /// @cond
  class Real;
  class Vector;
  /// @endcond

  /**
   * @brief Type-erased norm.
   *
   * The minimal signature (besides copy and/or move constructor/assignment) of a norm is:
   * @code
   * // My norm.
   * class MyNorm
   * {
   * public:
   *   // Compute ||x||.
   *   Real operator()(const ::Spacy::Vector& x) const;
   * };
   * @endcode
   */
  using Norm = std::function<Real(const Vector&)>;
}


#endif // SPACY_NORM_HH
