#ifndef ALGORITHM_REAL_SPACE_SOLVER_HH
#define ALGORITHM_REAL_SPACE_SOLVER_HH

#include "../../vector.hh"

namespace Algorithm
{
  namespace Real
  {
    class Solver
    {
    public:
      Solver(double value);

      ::Algorithm::Vector operator()(const ::Algorithm::Vector& y) const;

    private:
      double value_;
    };
  }
}

#endif // ALGORITHM_REAL_SPACE_SOLVER_HH
