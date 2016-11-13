#include "dampingFactor.hh"

namespace Spacy
{
    namespace {
        Real set(Real nu, Real eps) noexcept {
            return ( abs(1-nu) < eps ) ? 1. : nu;
        }
    }

    DampingFactor::DampingFactor(Real nu, Real eps) noexcept
        : Mixin::Get<Real>(set(nu, eps)),
          Mixin::Eps(eps)
    {}

    DampingFactor& DampingFactor::operator=(Real nu) noexcept
    {
        get() = set(nu, eps());
        return *this;
    }

    DampingFactor& DampingFactor::operator*=(Real value)
    {
        get() *= value;
        return *this;
    }


    DampingFactor DampingFactor::operator-() const noexcept
    {
        return DampingFactor(-get(),eps());
    }
}
