#ifndef ALGORITHM_REALNUMBERS_REALSOLVER_HH
#define ALGORITHM_REALNUMBERS_REALSOLVER_HH

#include "Interface/abstractLinearSolver.hh"

namespace Algorithm
{
  /// \cond
  class VectorSpace;
  /// \endcond

  class RealSolver : public Interface::AbstractLinearSolver
  {
  public:
    RealSolver(double value, VectorSpace* domain, VectorSpace* range);

    std::unique_ptr<Interface::AbstractVector> operator()(const Interface::AbstractVector& y) const final override;

  private:
    RealSolver* cloneImpl() const;

    double value_;
  };
}

#endif // ALGORITHM_REALNUMBERS_REALSOLVER_HH
