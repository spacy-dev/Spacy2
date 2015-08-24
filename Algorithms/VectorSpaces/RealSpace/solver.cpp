#include "solver.hh"

#include "Util/Exceptions/invalidArgumentException.hh"
#include "Util/cast.hh"
#include "vector.hh"

namespace Algorithm
{
  namespace Real
  {
    Solver::Solver(double value)
       : value_(value)
    {}

    ::Algorithm::Vector Solver::operator ()(const ::Algorithm::Vector& y) const
    {
      if( !isAny<Real::Vector>(y) ) throw InvalidArgumentException("RealSolver::operator()(const Vector&)");

      auto x = y;
      x *= 1./value_;
      return x;
    }
  }
}
