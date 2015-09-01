#include "solver.hh"

#include "VSA/Util/Exceptions/invalidArgumentException.hh"
#include "VSA/Util/cast.hh"
#include "VSA/Spaces/RealSpace/vector.hh"

namespace VSA
{
  namespace Real
  {
    Solver::Solver(double z)
       : z_(z)
    {}

    ::VSA::Vector Solver::operator ()(const ::VSA::Vector& y) const
    {
      if( !is<Real::Vector>(y) ) throw InvalidArgumentException("RealSolver::operator()(const Vector&)");

      auto x = y;
      x *= 1./z_;
      return x;
    }
  }
}
