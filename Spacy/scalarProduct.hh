#ifndef SPACY_SCALAR_PRODUCT_HH
#define SPACY_SCALAR_PRODUCT_HH

#include <functional>

namespace Spacy
{
  /// @cond
  class Real;
  class Vector;
  /// @endcond

  /**
   * @brief Type-erased scalar product.
   *
   * The minimal signature (besides copy and/or move constructor/assignment) of a scalar product is:
   * @code
   * // My scalar product.
   * class MyScalarProduct
   * {
   * public:
   *   // Compute (x,y).
   *   Spacy::Real operator()(const Spacy::Vector& x, const Spacy::Vector& y) const;
   * };
   * @endcode
   */
  using ScalarProduct = std::function<Real(const Vector&, const Vector&)>;
}
#endif // SPACYS_SCALAR_PRODUCT_HH
