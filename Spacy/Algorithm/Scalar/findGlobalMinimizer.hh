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

        /**
         * @brief Find log.global minimizer of \f$f\f$ in \f$[a,b]\f$ for inexact composite step methods
         * @param f Nonlinear function \f$ f: \mathbb{R}\to\mathbb{R} \f$
         * @param a lower bound of the admissible interval
         * @param b upper bound of the admissible interval
         * @param normdn2 squared norm of damped normal step
         * @param two_sp_dn_Dt twice the scalar product of dn and Dt
         * @param normDt2 squared norm of undamped tangential step
         * @param eps relative accuracy
         * @return \f$ x \in \mathrm{argmin}_{[a,b]} \f$ such that it dampens \f$ \|dn+x*Dt\| \f$
         */

        Real findLogGlobalMinimizerForDamping(const std::function<Real(const Real)>& f, Real a, Real b, const Real normdn2,
                                          const Real two_sp_dn_Dt, const Real normDt2, Real eps = 1e-2);

    }
}
