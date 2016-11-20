#pragma once

namespace Spacy
{
    /// @cond
    class DynamicC1Operator;
    class Vector;
    /// @endcond

    namespace Rothe
    {
        Vector genericMethod(const DynamicC1Operator& A, double t0=0, double t1=1);
    }
}
