#pragma once

#include <functional>
#include <memory>

#include <Spacy/Util/Base/functionalBase.hh>

namespace Spacy
{
    /// @cond
    class LinearOperator;
    class Real;
    class Vector;
    /// @endcond

    /** @addtogroup ScalarGroup @{ */
    namespace Scalar
    {
        /// @cond
        class LinearOperator;
        /// @endcond

        class C2Functional : public Spacy::FunctionalBase
        {
        public:
            C2Functional(std::function<double(double)> value,
                         std::function<double(double)> derivative,
                         std::function<double(double)> secDerivative);

            // Compute f(x).
            Spacy::Real operator()(const ::Spacy::Vector& x) const;

            // Compute f'(x) as element of X*.
            Spacy::Vector d1(const ::Spacy::Vector& x) const;

            // Compute f''(x)dx as element of X*.
            Spacy::Vector d2(const ::Spacy::Vector& x, const ::Spacy::Vector& dx) const;

            // Access f''(x) as mapping f''(x): X->X*.
            Spacy::Scalar::LinearOperator hessian(const ::Spacy::Vector& x) const;

        private:
            std::shared_ptr<VectorSpace> operatorSpace_ = nullptr;
            std::function<double(const double&)> value_ ;
            std::function<double(const double&)> derivative_;
            std::function<double(const double&)> secDerivative_;
        };
    }
    /** @} */
}
