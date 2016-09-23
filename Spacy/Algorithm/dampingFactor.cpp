#include "dampingFactor.hh"

#include <cmath>

namespace Spacy
{
    namespace {
        double set(double nu, double eps) noexcept {
            return ( std::abs(1-nu) < eps ) ? 1. : nu;
        }
    }

    DampingFactor::DampingFactor(double nu, double eps) noexcept
        : Mixin::Get<double>(set(nu, eps)),
          Mixin::Eps(eps)
    {}

    DampingFactor::DampingFactor(Real nu, double eps) noexcept
        : Mixin::Get<double>(set(nu.get(), eps)),
          Mixin::Eps(eps)
    {}

    DampingFactor& DampingFactor::operator=(double nu) noexcept
    {
        get() = set(nu, eps());
        return *this;
    }

    DampingFactor& DampingFactor::operator=(Real nu) noexcept
    {
        get() = set(nu.get(), eps());
        return *this;
    }

    DampingFactor& DampingFactor::operator*=(double value)
    {
        get() *= value;
        return *this;
    }

    DampingFactor& DampingFactor::operator*=(Real value)
    {
        get() *= value.get();
        return *this;
    }


    DampingFactor DampingFactor::operator-() const noexcept
    {
        return DampingFactor(-get(),eps());
    }


    Real operator*(const DampingFactor& x, Real y)
    {
        get(y) *= get(x);
        return y;
    }

    Real operator*(Real y, const DampingFactor& x)
    {
        return x * y;
    }
}
