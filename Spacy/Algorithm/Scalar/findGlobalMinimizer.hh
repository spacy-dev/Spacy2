#pragma once

#include <functional>
#include <Spacy/Spaces/ScalarSpace/Real.hh>

namespace Spacy
{
    namespace Scalar
    {
        /**
         * @brief Find global minimizer of \f$f\f$ in \f$[a,b]\f$.
         * @param f Nonlinear function \f$ f: \mathbb{R}\to\mathbb{R} \f$
         * @param a lower bound of the admissible interval
         * @param b upper bound of the admissible interval
         * @param eps relative accuracy
         * @return \f$ x \in \mathrm{argmin}_{[a,b]} \f$
         */
        Real findGlobalMinimizer(const std::function<Real(const Real)>& f, Real a, Real b, Real eps = 1e-2);

        /**
         * @brief Find log.global minimizer of \f$f\f$ in \f$[a,b]\f$.
         * @param f Nonlinear function \f$ f: \mathbb{R}\to\mathbb{R} \f$
         * @param a lower bound of the admissible interval
         * @param b upper bound of the admissible interval
         * @param eps relative accuracy
         * @return \f$ x \in \mathrm{argmin}_{[a,b]} \f$
         */
        Real findLogGlobalMinimizer(const std::function<Real(const Real)>& f, Real a, Real b, Real eps = 1e-2);
    }
}
