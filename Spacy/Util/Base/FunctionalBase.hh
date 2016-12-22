#pragma once

namespace Spacy
{
    /// @cond
    class VectorSpace;
    /// @endcond

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
