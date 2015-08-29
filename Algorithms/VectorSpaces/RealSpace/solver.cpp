#include "solver.hh"

#include "Algorithms/Util/Exceptions/invalidArgumentException.hh"
#include "Algorithms/Util/cast.hh"
#include "Algorithms/VectorSpaces/RealSpace/vector.hh"

namespace Algorithm
{
  namespace Real
  {
    Solver::Solver(double z)
       : z_(z)
    {}

    ::Algorithm::Vector Solver::operator ()(const ::Algorithm::Vector& y) const
    {
      if( !is<Real::Vector>(y) ) throw InvalidArgumentException("RealSolver::operator()(const Vector&)");

      auto x = y;
      x *= 1./z_;
      return x;
    }
  }
}
