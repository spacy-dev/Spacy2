#include "abstractC2Functional.hh"

#include "Interface/abstractFunctionSpace.hh"
#include "Interface/abstractLinearSolver.hh"
#include "hessian.hh"
#include "Util/Exceptions/callOfUndefinedFunctionException.hh"

namespace Algorithm
{
  namespace Interface
  {
    AbstractC2Functional::AbstractC2Functional(std::shared_ptr<AbstractFunctionSpace> domain)
      : AbstractC1Functional(domain)
    {}

    std::unique_ptr<Hessian> AbstractC2Functional::hessian(const AbstractFunctionSpaceElement& x) const
    {
      return makeHessian(x);
    }

    std::unique_ptr<Hessian> AbstractC2Functional::makeHessian(const AbstractFunctionSpaceElement& x) const
    {
      return std::make_unique<Hessian>(clone(this),x);
    }

    std::unique_ptr<AbstractLinearSolver> AbstractC2Functional::makeSolver() const
    {
      throw CallOfUndefinedFunctionException("AbstractC2Functional::makeSolver");
    }

    void AbstractC2Functional::setOrigin(const AbstractFunctionSpaceElement& x) const
    {
      //throw CallOfUndefinedFunctionException("AbstractC2Functional::setOrigin");
    }
  }
}
