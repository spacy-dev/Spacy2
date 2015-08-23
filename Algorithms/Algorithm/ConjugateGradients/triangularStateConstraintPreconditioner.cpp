#include "triangularStateConstraintPreconditioner.hh"

#include "FunctionSpaces/ProductSpace/productSpaceElement.hh"
#include "vectorSpace.hh"

#include <utility>
#include <iostream>
namespace Algorithm
{
  TriangularStateConstraintPreconditioner::TriangularStateConstraintPreconditioner(LinearSolver stateSolver,
                                                                                   LinearSolver controlSolver,
                                                                                   LinearSolver adjointSolver,
                                                                                   CallableOperator B,
                                                                                   CallableOperator BT,
                                                                                   VectorSpace* domain,
                                                                                   VectorSpace* range)
    : OperatorBase(domain,range),
      stateSolver_(std::move(stateSolver)), controlSolver_(std::move(controlSolver)), adjointSolver_(std::move(adjointSolver)),
      B_(std::move(B)), BT_(std::move(BT))
  {}

  Vector TriangularStateConstraintPreconditioner::operator()(const Vector& x) const
  {
    auto x_ = cast_ref<ProductSpaceElement>(x);
    auto y = range().element();
    auto& y_ = cast_ref<ProductSpaceElement>(y);

    y_.variable(adjointIndex()) = adjointSolver_( x_.variable(stateIndex()) );
    x_.variable(controlIndex()) -= BT_( y_.variable(adjointIndex()) );

    y_.variable(controlIndex()) = controlSolver_( x_.variable(controlIndex()) );

    x_.variable(adjointIndex()) -= B_( y_.variable(controlIndex()) );

    y_.variable(stateIndex()) = stateSolver_( x_.variable(adjointIndex()) );

    return y;
  }

  Vector TriangularStateConstraintPreconditioner::kernelOffset(const Vector& rhs) const
  {
    auto y = range().element();
    cast_ref<ProductSpaceElement>(y).variable(stateIndex()) = stateSolver_( cast_ref<ProductSpaceElement>(rhs).variable(adjointIndex()) );
    return y;
  }


  TriangularStateConstraintPreconditioner* TriangularStateConstraintPreconditioner::cloneImpl() const
  {
    auto result = new TriangularStateConstraintPreconditioner( stateSolver_ , controlSolver_ , adjointSolver_ , B_ , BT_ , domain_ptr() , range_ptr() );
    result->setStateIndex(stateIndex());
    result->setControlIndex(controlIndex());
    result->setAdjointIndex(adjointIndex());
    return result;
  }
}

