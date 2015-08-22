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

  Vector TriangularStateConstraintPreconditioner::operator()(const Vector& x) const
  {
    auto x_ = castAny<ProductSpaceElement>(x);
    auto y = range().element();
    auto& y_ = castAny<ProductSpaceElement>(y);

    y_.variable(adjointIndex()) = (*adjointSolver_)( x_.variable(stateIndex()) );
    x_.variable(controlIndex()) -= (*BT_)( y_.variable(adjointIndex()) );

    y_.variable(controlIndex()) = (*controlSolver_)( x_.variable(controlIndex()) );

    x_.variable(adjointIndex()) -= (*B_)( y_.variable(controlIndex()) );

    y_.variable(stateIndex()) = (*stateSolver_)( x_.variable(adjointIndex()) );

    return y;
  }

  Vector TriangularStateConstraintPreconditioner::kernelOffset(const Vector& rhs) const
  {
    auto y = range().element();

    castAny<ProductSpaceElement>(y).variable(stateIndex()) = (*stateSolver_)( castAny<ProductSpaceElement>(rhs).variable(adjointIndex()) );
    return y;
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

