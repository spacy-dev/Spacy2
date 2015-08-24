#ifndef ALGORITHM_ADAPTER_FENICS_LUSOLVER_HH
#define ALGORITHM_ADAPTER_FENICS_LUSOLVER_HH

#include <memory>
#include <string>

#include <dolfin.h>

#include "../../vector.hh"
#include "Util/Base/operatorBase.hh"
#include "Util/Exceptions/callOfUndefinedFunctionException.hh"

namespace Algorithm
{
  namespace Fenics
  {
    class LUSolver : public OperatorBase
    {
    public:
      LUSolver(std::shared_ptr<dolfin::GenericMatrix> A, const dolfin::FunctionSpace& space,
               ::Algorithm::VectorSpace* domain , ::Algorithm::VectorSpace* range,
               bool symmetric = false, const std::string& solverName = "mumps");


      ::Algorithm::Vector operator()(const ::Algorithm::Vector& x) const;

    private:
      mutable dolfin::LUSolver solver_;
      const dolfin::FunctionSpace& space_;
    };


    class TransposedLUSolver : public OperatorBase
    {
    public:
      TransposedLUSolver(std::shared_ptr<dolfin::GenericMatrix> A, const dolfin::FunctionSpace& productSpace,
               ::Algorithm::VectorSpace* domain , ::Algorithm::VectorSpace* range,
               bool symmetric = false, const std::string& solverName = "mumps");

      ::Algorithm::Vector operator()(const ::Algorithm::Vector& x) const;

    private:
      mutable dolfin::LUSolver solver_;
      const dolfin::FunctionSpace& space_;
    };
  }
}

#endif // ALGORITHM_ADAPTER_FENICS_LUSOLVER_HH
