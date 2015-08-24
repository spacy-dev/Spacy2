#include "solver.hh"

#include "Util/Exceptions/invalidArgumentException.hh"
#include "Util/cast.hh"
#include "vector.hh"

namespace Algorithm
{
  namespace Real
  {
    Solver::Solver(double z)
       : z_(z)
    {}

    ::Algorithm::Vector Solver::operator ()(const ::Algorithm::Vector& y) const
    {
      if( !isAny<Real::Vector>(y) ) throw InvalidArgumentException("RealSolver::operator()(const Vector&)");

      auto x = y;
      x *= 1./z_;
      return x;
    }
  }
}
