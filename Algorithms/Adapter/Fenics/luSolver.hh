#ifndef ALGORITHM_ADAPTER_FENICS_LUSOLVER_HH
#define ALGORITHM_ADAPTER_FENICS_LUSOLVER_HH

#include <memory>
#include <vector>

#include <dolfin.h>

#include "Interface/abstractLinearSolver.hh"
#include "Util/Exceptions/callOfUndefinedFunctionException.hh"

namespace Algorithm
{
  static int lusolverid = 0;

  namespace Fenics
  {
    class LUSolver : public Interface::AbstractLinearSolver
    {
    public:
      LUSolver(std::shared_ptr<dolfin::GenericMatrix> A, const dolfin::FunctionSpace& productSpace,
               ::Algorithm::VectorSpace* domain , ::Algorithm::VectorSpace* range);


      std::unique_ptr<Interface::AbstractVector> operator()(const Interface::AbstractVector& x) const final override;

    private:
      LUSolver* cloneImpl() const;

      mutable dolfin::LUSolver solver_;
      const dolfin::FunctionSpace& productSpace_;
    };

//    std::shared_ptr<Interface::AbstractLinearSolver> createLUSolver(const dolfin::GenericMatrix& A);

//    template <class... Args>
//    std::shared_ptr<Interface::AbstractLinearSolver> createLUSolver(const Args&...);


//    class TransposedLUSolver : public Interface::AbstractLinearSolver
//    {
//    public:
//      explicit TransposedLUSolver(const dolfin::GenericMatrix& A);

//      std::unique_ptr<Interface::AbstractVector> operator()(const Interface::AbstractVector& x) const final override;

//    private:
//      std::unique_ptr<dolfin::GenericMatrix> A_;
//      mutable dolfin::LUSolver solver;
//    };

//    std::shared_ptr<Interface::AbstractLinearSolver> createTransposedLUSolver(const dolfin::GenericMatrix& A);

//    template <class... Args>
//    std::shared_ptr<Interface::AbstractLinearSolver> createTransposedLUSolver(const Args&...);
  }
}

#endif // ALGORITHM_ADAPTER_FENICS_LUSOLVER_HH
