#include "lipschitzConstant.hh"

#include <algorithm>

namespace Spacy
{
    LipschitzConstant::LipschitzConstant(double initialOmega)
        : Mixin::Get<double>(initialOmega)
    {}

    LipschitzConstant& LipschitzConstant::operator=(double newOmega)
    {
        previousOmega_ = get();
        if( newOmega < 0 ) get() = minFactor_*previousOmega_;
        else get() = newOmega;

        get() = std::max(get(),eps());
        get() = std::min(get(),previousOmega_*maxFactor_);
        return *this;
    }

    double LipschitzConstant::previous() const
    {
        return previousOmega_;
    }

    void LipschitzConstant::setMaxFactor(double factor)
    {
        maxFactor_ = factor;
    }

    void LipschitzConstant::setMinFactor(double factor)
    {
        minFactor_ = factor;
    }

    Real operator*(const LipschitzConstant& x, Real y)
    {
        get(y) *= get(x);
        return y;
    }

    Real operator*(Real y, const LipschitzConstant& x)
    {
        get(y) *= get(x);
        return y;
    }
}
