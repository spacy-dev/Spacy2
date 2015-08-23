#ifndef ALGORITHM_REALNUMBERS_REALSOLVER_HH
#define ALGORITHM_REALNUMBERS_REALSOLVER_HH

#include "vector.hh"

namespace Algorithm
{
  class RealSolver
  {
  public:
    RealSolver(double value);

    Vector operator()(const Vector& y) const;

  private:
    double value_;
  };
}

#endif // ALGORITHM_REALNUMBERS_REALSOLVER_HH
