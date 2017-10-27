#include "findGlobalMinimizer.hh"

#include <cassert>
#include <iostream>

namespace Spacy
{
    namespace Scalar
    {
        Real findGlobalMinimizer(const std::function<Real(const Real)>& f, Real a, Real b, Real eps)
        {
            assert(a<b);
            eps *= b-a;
            Real tmin = a;
            Real fmin = f(a);

            while( (a+=eps) <= b )
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
            while( (b*=(1-eps)) >= a )
                if (f(b) < fmin)
                {
                    fmin = f(b);
                    tmin = b;
                }

            return tmin;
        }

        Real findLogGlobalMinimizeTangentialDamping(const std::function<Real(const Real)>& f, Real a, Real b, const Real normdn2,
                                                    const Real two_sp_dn_Dt, const Real normDt2, Real eps)
        {
            assert(a<b);
            assert(a>0);
            Real tmin = b;
            Real fmin = f(b);
            eps = min(eps,0.5);

            Real normbefore = 1.e16;
            Real norm = 0.;

            while( (b*=(1-eps)) >= a)
            {
//                std::cout<<b<<std::endl;
                if( f(b) < fmin )
                {
                    fmin = f(b);
                    tmin = b;
                }
                norm = normdn2 + two_sp_dn_Dt*b + b*b*normDt2;

                // stops the decrease of the damping parameter, if the norm of dn+tmin*Dt increases
                // This is possible, if dn and Dt are not orthogonal.
                if(norm > normbefore)
                {
                    std::cout<<"norm increased: stopping tangential damping"<<std::endl;
                    break;
                }
                normbefore = norm;
            }
            return tmin;
        }
    }
}
