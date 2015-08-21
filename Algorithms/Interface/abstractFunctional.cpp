#include "abstractFunctional.hh"

#include "abstractLinearSolver.hh"
#include "hessian.hh"

#include "Util/Exceptions/invalidArgumentException.hh"
#include "Util/Exceptions/callOfUndefinedFunctionException.hh"

#include "Interface/abstractVector.hh"
#include "vectorSpace.hh"

namespace Algorithm
{
  namespace Interface
  {
    AbstractFunctional::AbstractFunctional(VectorSpace* domain)
      : domain_(domain)
    {}

    AbstractFunctional::~AbstractFunctional(){}

    double AbstractFunctional::operator ()(const AbstractVector& x) const
    {
      if( domain().index() != x.space().index() ) throw InvalidArgumentException("AbstractFunctional::operator() (incompatible space ids)");

      return d0(x);
    }

    std::unique_ptr<AbstractVector> AbstractFunctional::d1(const AbstractVector& x) const
    {
      throw CallOfUndefinedFunctionException("AbstractFunctional::d1");
    }

    std::unique_ptr<AbstractVector> AbstractFunctional::d2(const AbstractVector& x, const AbstractVector& dx) const
    {
      throw CallOfUndefinedFunctionException("AbstractFunctional::d2");
    }

    std::unique_ptr<Hessian> AbstractFunctional::hessian(const AbstractVector& x) const
    {
      return makeHessian(x);
    }

    std::unique_ptr<Hessian> AbstractFunctional::makeHessian(const AbstractVector& x) const
    {
      return std::make_unique<Hessian>(clone(this),x);
    }

    std::unique_ptr<AbstractLinearSolver> AbstractFunctional::makeSolver() const
    {
      throw CallOfUndefinedFunctionException("AbstractC2Functional::makeSolver");
    }

    const VectorSpace& AbstractFunctional::domain() const
    {
      return *domain_;
    }

    VectorSpace* AbstractFunctional::domain_ptr() const
    {
      return domain_;
    }
  }
}
