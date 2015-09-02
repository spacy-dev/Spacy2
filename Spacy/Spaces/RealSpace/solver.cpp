#include "solver.hh"

#include "Spacy/Util/Exceptions/invalidArgumentException.hh"
#include "Spacy/Util/cast.hh"
#include "Spacy/Spaces/RealSpace/vector.hh"

namespace Spacy
{
  namespace Real
  {
    Solver::Solver(double z)
       : z_(z)
    {}

    ::Spacy::Vector Solver::operator ()(const ::Spacy::Vector& y) const
    {
      if( !is<Real::Vector>(y) ) throw InvalidArgumentException("RealSolver::operator()(const Vector&)");

      auto x = y;
      x *= 1./z_;
      return x;
    }
  }
}
