#ifndef SPACY_SCALARPRODUCT_HH
#define SPACY_SCALARPRODUCT_HH

#include <functional>

namespace Spacy
{
  /// @cond
  class Real;
  class Vector;
  /// @endcond

  /**
   * \ingroup SpacyGroup
   * \brief Type erased scalar product.
   *
   * The minimal signature (besides copy and/or move constructor/assignment) of a scalar product is:
   * @code
   * // My scalar product.
   * class MyScalarProduct
   * {
   * public:
   *   // Compute (x,y).
   *   Real operator()(const ::Spacy::Vector& x, const ::Spacy::Vector& y) const;
   * };
   * @endcode
   */
  using ScalarProduct = std::function<Real(const Vector&, const Vector&)>;
}
#endif // SPACYS_SCALARPRODUCT_HH
