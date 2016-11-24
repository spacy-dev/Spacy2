#pragma once

#include <functional>
#include <Spacy/Spaces/ScalarSpace/real.hh>

namespace Spacy
{
    /// @cond
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
     *   Spacy::Real operator()(const Spacy::Vector& x) const;
     * };
     * @endcode
     */
    using Norm = std::function<Real(const Vector&)>;
}
