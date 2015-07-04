#ifndef ALGORITHM_ADAPTER_FENICS_LUSOLVER_HH
#define ALGORITHM_ADAPTER_FENICS_LUSOLVER_HH

#include <memory>
#include <vector>

#include <dolfin.h>

#include "Interface/abstractLinearSolver.hh"

namespace Algorithm
{
  namespace Fenics
  {
    class LUSolver : public Interface::AbstractLinearSolver
    {
    public:
      explicit LUSolver(const dolfin::GenericMatrix& A);

      std::unique_ptr<Interface::AbstractFunctionSpaceElement> operator()(const Interface::AbstractFunctionSpaceElement& x) const final override;

    private:
      std::unique_ptr<dolfin::GenericMatrix> A_;
      mutable dolfin::LUSolver solver;
    };
  }
}

#endif // ALGORITHM_ADAPTER_FENICS_LUSOLVER_HH
