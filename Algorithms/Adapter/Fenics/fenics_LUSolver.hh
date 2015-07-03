#ifndef ALGORITHM_ADAPTER_FENICS_LUSOLVER_HH
#define ALGORITHM_ADAPTER_FENICS_LUSOLVER_HH

#include <memory>
#include <vector>

#include <dolfin.h>

#include "Interface/abstractLinearSolver.hh"

namespace Algorithm
{
  class Fenics_LUSolver : public AbstractLinearSolver
  {
  public:
    explicit Fenics_LUSolver(const dolfin::GenericMatrix& A);

    std::unique_ptr<AbstractFunctionSpaceElement> operator()(const AbstractFunctionSpaceElement& x) const final override;

  private:    
    std::unique_ptr<dolfin::GenericMatrix> A_;
    mutable dolfin::LUSolver solver;
  };
}

#endif // ALGORITHM_ADAPTER_FENICS_LUSOLVER_HH
