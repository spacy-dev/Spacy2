#include "findGlobalMinimizer.hh"

#include <cassert>

namespace Spacy
{
    Real findGlobalMinimizer(const std::function<Real(const Real)>& f, Real a, Real b, Real eps)
    {
        assert(a<b);
        eps *= b-a;
        Real tmin = a;
        Real fmin = f(a);

        while( (a+=eps) <= b)
            if( f(a) < fmin )
            {
                fmin = f(a);
                tmin = a;
            }

        return tmin;
    }

    Real findLogGlobalMinimizer(const std::function<Real(const Real)>& f, Real a, Real b, Real eps)
    {
        assert(a<b);
        assert(a>0);
        Real tmin = b;
        Real fmin = f(b);

        eps = min(eps,0.5);
        
        while( (b*=(1-eps)) >= a)
            if( f(b) < fmin )
            {
                fmin = f(b);
                tmin = b;
            }

        return tmin;
    }

}
