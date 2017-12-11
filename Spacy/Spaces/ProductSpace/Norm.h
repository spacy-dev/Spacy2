#pragma once

namespace Spacy
{
    /// @cond
    class Real;
    class Vector;
    /// @endcond

    namespace ProductSpace
    {
        /**
         * @ingroup ProductSpaceGroup
         * @brief Canonical norm on product spaces.
         *
         * This is the norm induced by the subspaces. I.e. for a vector-space \f$V=V_1 \times \ldots
         * \times V_n \f$,
         * the induced norm is \f$\|\cdot\|_V=\sqrt{\|\cdot\|^2_{V_1} + \ldots +
         * \|\cdot\|^2_{V_n}}\f$.
         */
        class Norm
        {
        public:
            /// Compute norm.
            Real operator()( const ::Spacy::Vector& x ) const;
        };
    }
}
