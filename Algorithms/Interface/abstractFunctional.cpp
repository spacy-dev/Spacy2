#include "abstractFunctional.hh"

#include "abstractFunctionSpace.hh"
#include "abstractLinearSolver.hh"
#include "hessian.hh"

#include "Util/Exceptions/invalidArgumentException.hh"
#include "Util/Exceptions/callOfUndefinedFunctionException.hh"

namespace Algorithm
{
  namespace Interface
  {
    AbstractFunctional::AbstractFunctional(std::shared_ptr<AbstractFunctionSpace> domain)
      : domain_(domain)
    {}

    AbstractFunctional::~AbstractFunctional(){}

    double AbstractFunctional::operator ()(const AbstractFunctionSpaceElement& x) const
    {
      if( domain().index() != x.space().index() ) throw InvalidArgumentException("AbstractFunctional::operator() (incompatible space ids)");

      return d0(x);
    }

    std::unique_ptr<AbstractFunctionSpaceElement> AbstractFunctional::d1(const AbstractFunctionSpaceElement& x) const
    {
      throw CallOfUndefinedFunctionException("AbstractFunctional::d1");
    }

    std::unique_ptr<AbstractFunctionSpaceElement> AbstractFunctional::d2(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& dx) const
    {
      throw CallOfUndefinedFunctionException("AbstractFunctional::d2");
    }

    std::unique_ptr<Hessian> AbstractFunctional::hessian(const AbstractFunctionSpaceElement& x) const
    {
      return makeHessian(x);
    }

    std::unique_ptr<Hessian> AbstractFunctional::makeHessian(const AbstractFunctionSpaceElement& x) const
    {
      return std::make_unique<Hessian>(clone(this),x);
    }

    std::unique_ptr<AbstractLinearSolver> AbstractFunctional::makeSolver() const
    {
      throw CallOfUndefinedFunctionException("AbstractC2Functional::makeSolver");
    }

    const AbstractFunctionSpace& AbstractFunctional::domain() const
    {
      return *domain_;
    }

    std::shared_ptr<AbstractFunctionSpace> AbstractFunctional::sharedDomain() const
    {
      return domain_;
    }
  }
}
