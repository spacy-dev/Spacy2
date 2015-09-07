#include "linearOperator.hh"

#include "Spacy/Util/Exceptions/callOfUndefinedFunctionException.hh"

#include "util.hh" // copy
#include "operatorSpace.hh"
#include "Spacy/vector.hh"
#include "Spacy/vectorSpace.hh"
#include "Spacy/Spaces/RealSpace/vectorSpace.hh"
#include "Spacy/Util/cast.hh"
#include <iostream>
namespace Spacy
{
  namespace FEniCS
  {
    LinearOperator::LinearOperator(std::shared_ptr<dolfin::GenericMatrix> A, const VectorSpace& space, std::shared_ptr<const dolfin::FunctionSpace> dolfinSpace)
      : OperatorBase(cast_ref<LinearOperatorCreator>(space.impl()).domain() ,
                     cast_ref<LinearOperatorCreator>(space.impl()).range()) ,
        VectorBase(space) ,
        A_(std::move(A)) ,
        space_(dolfinSpace)
    {}

    LinearOperator::LinearOperator(std::shared_ptr<dolfin::GenericMatrix> A, const VectorSpace& space, std::shared_ptr<const dolfin::FunctionSpace> dolfinSpace,
                   std::function<LinearSolver(const LinearOperator&)> solverCreator)
      : OperatorBase(cast_ref<LinearOperatorCreator>(space.impl()).domain(),
                     cast_ref<LinearOperatorCreator>(space.impl()).range()),
        VectorBase(space),
        A_(std::move(A)),
        solverCreator_(std::move(solverCreator)),
        space_(dolfinSpace)
    {}

    ::Spacy::Vector LinearOperator::operator()(const ::Spacy::Vector& x) const
    {
      if( x.space().index() == domain().index() )
        return applyAsOperator(x);

      if( x.space().isPrimalWRT(*space()))
        return applyAsDualElement(x);

      assert(false);
    }

    LinearSolver LinearOperator::solver() const
    {
      return solverCreator_(*this);
    }

    dolfin::GenericMatrix& LinearOperator::impl()
    {
      assert(A_ != nullptr);
      return *A_;
    }

    const dolfin::GenericMatrix& LinearOperator::impl() const
    {
      assert(A_ != nullptr);
      return *A_;
    }

    bool LinearOperator::symmetric() const
    {
      return symmetric_;
    }

    std::shared_ptr<const dolfin::FunctionSpace> LinearOperator::dolfinSpace() const
    {
      return space_;
    }

    ::Spacy::Vector LinearOperator::applyAsOperator(const ::Spacy::Vector& x) const
    {
      auto x_ = dolfin::Function( space_ );
      copy(x,x_);
      auto y_ = x_.vector()->copy();
      A_->mult(*x_.vector(), *y_);

      auto y = range().vector();
      copy(*y_,y);

      return std::move(y);
    }

    ::Spacy::Vector LinearOperator::applyAsDualElement(const ::Spacy::Vector& x) const
    {
      throw CallOfUndefinedFunctionException("FEniCS::LinearOperator::applyAsDualElement");
    }
  }
}
