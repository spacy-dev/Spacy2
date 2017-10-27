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
         * @brief Find log global minimizer of \f$f\f$ in \f$[a,b]\f$.
         * @param f Nonlinear function \f$ f: \mathbb{R}\to\mathbb{R} \f$
         * @param a lower bound of the admissible interval
         * @param b upper bound of the admissible interval
         * @param eps relative accuracy
         * @return \f$ x \in \mathrm{argmin}_{[a,b]} \f$
         */
        Real findLogGlobalMinimizer(const std::function<Real(const Real)>& f, Real a, Real b, Real eps = 1e-2);

        /**
         * @brief Find log global minimizer of \f$f\f$ in \f$[a,b]\f$ for inexact composite step methods.
         * Performs a scalar linesearch on the parameter \f$ x \f$ of \f$ dn+x*Dt \f$ such that this sum minimizes the function \f$ f \f$
         * It will start from the upper bound and proceed to the lower bound logarithmically, however it will stop, as soon as the
         * norm of \f$ dn+x*Dt \f$ increases for decreasing \f$ x \f$. This is possible, if dn and Dt are not orthogonal.
         * @param f Nonlinear function \f$ f: \mathbb{R}\to\mathbb{R} \f$
         * @param a lower bound of the admissible interval
         * @param b upper bound of the admissible interval
         * @param normdn2 squared norm of damped normal step
         * @param two_sp_dn_Dt twice the scalar product of dn and Dt
         * @param normDt2 squared norm of undamped tangential step
         * @param eps relative accuracy
         * @return \f$ x \in \mathrm{argmin}_{[a,b]} \f$ such that it minimizes \f$ f(dn+x*dt) \f$ without increasing the norm of
         * the dn+x*dt
         */

        Real findLogGlobalMinimizerTangentialDamping(const std::function<Real(const Real)>& f, Real a, Real b, const Real normdn2,
                                              const Real two_sp_dn_Dt, const Real normDt2, Real eps = 1e-2);

    }
}
