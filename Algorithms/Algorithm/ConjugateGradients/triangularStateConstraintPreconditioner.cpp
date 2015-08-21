#include "triangularStateConstraintPreconditioner.hh"

#include "FunctionSpaces/ProductSpace/productSpaceElement.hh"
#include "vectorSpace.hh"

#include <utility>
#include <iostream>
namespace Algorithm
{
  TriangularStateConstraintPreconditioner::TriangularStateConstraintPreconditioner(std::shared_ptr<Interface::AbstractLinearSolver> stateSolver,
                                                                                   std::shared_ptr<Interface::AbstractLinearSolver> controlSolver,
                                                                                   std::shared_ptr<Interface::AbstractLinearSolver> adjointSolver,
                                                                                   std::unique_ptr<AbstractOperator>&& B,
                                                                                   std::unique_ptr<AbstractOperator>&& BT,
                                                                                   VectorSpace* domain,
                                                                                   VectorSpace* range)
    : AbstractOperator(domain,range),
      stateSolver_(stateSolver), controlSolver_(controlSolver), adjointSolver_(adjointSolver),
      B_(std::move(B)), BT_(std::move(BT))
  {}

  std::unique_ptr<Interface::AbstractVector> TriangularStateConstraintPreconditioner::operator()(const Interface::AbstractVector& x) const
  {
    auto x_ = clone( castTo<ProductSpaceElement>(x) );
    auto y = clone(x);
    auto& y_ = castTo<ProductSpaceElement>(*y);

    y_.variable(adjointIndex()) = *(*adjointSolver_)( x_->variable(stateIndex()) );
    x_->variable(controlIndex()) -= *(*BT_)( y_.variable(adjointIndex()) );

    y_.variable(controlIndex()) = *(*controlSolver_)( x_->variable(controlIndex()) );

    x_->variable(adjointIndex()) -= *(*B_)( y_.variable(controlIndex()) );

    y_.variable(stateIndex()) = *(*stateSolver_)( x_->variable(adjointIndex()) );

    return y;
  }

  std::unique_ptr<Interface::AbstractVector> TriangularStateConstraintPreconditioner::kernelOffset(const Interface::AbstractVector& rhs) const
  {
    auto y = range().element();

    castTo<ProductSpaceElement>(y.impl()).variable(stateIndex()) = *(*stateSolver_)( castTo<ProductSpaceElement>(rhs).variable(adjointIndex()) );
    return clone(y.impl());
  }


  TriangularStateConstraintPreconditioner* TriangularStateConstraintPreconditioner::cloneImpl() const
  {
    auto result = new TriangularStateConstraintPreconditioner( stateSolver_ , controlSolver_ , adjointSolver_ , clone(B_) , clone(BT_) , domain_ptr() , range_ptr() );
    result->setStateIndex(stateIndex());
    result->setControlIndex(controlIndex());
    result->setAdjointIndex(adjointIndex());
    return result;
  }
}

