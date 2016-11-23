#include "genericRothe.hh"

#include <Spacy/dynamicOperator.hh>
#include <Spacy/vector.hh>
#include <Spacy/zeroVectorCreator.hh>
#include <Spacy/linearOperator.hh>

#include <utility>

namespace Spacy
{
  namespace Rothe
  {
    class TimeDependentAxpy
    {
    public:
      TimeDependentAxpy(LinearOperator M, LinearOperator J)
        : M_(std::move(M)), J_(std::move(J))
      {}

      LinearOperator operator()(Real dt) const
      {
        auto Axpy = J_;
        Axpy *= -get(dt);
        Axpy += M_;
        return Axpy;
//        return axpy(M_,-dt,J_);
      }

      LinearOperator M_, J_;
    };

    Vector genericMethod(const DynamicC1Operator& A, Real t0, Real t1)
    {
      unsigned maxSteps = 100;

      auto t = t0,
          dt = (t1-t0)/maxSteps;

      auto integrator = [&A,&dt](Real t, const Vector& x) -> Vector
      {
        return ( ( TimeDependentAxpy(A.linearization(get(t),x),A.M())(get(dt)) )^-1 )( -A(get(t),x) );
      };

      auto x = zero(A.domain());
      for( auto i=0u ; i<maxSteps ; ++i )
        x += integrator(t += dt, x);

      return x;
    }
  }
}
