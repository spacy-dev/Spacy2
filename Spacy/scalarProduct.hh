#pragma once

#include <functional>
#include <Spacy/Spaces/ScalarSpace/real.hh>

namespace Spacy
{
    /// @cond
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
