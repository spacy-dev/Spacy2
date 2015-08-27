#include "triangularStateConstraintPreconditioner.hh"

#include "VectorSpaces/ProductSpace/vector.hh"
#include "vectorSpace.hh"

#include <utility>

namespace Algorithm
{
  TriangularStateConstraintPreconditioner::TriangularStateConstraintPreconditioner(LinearSolver stateSolver,
                                                                                   LinearSolver controlSolver,
                                                                                   LinearSolver adjointSolver,
                                                                                   CallableOperator B,
                                                                                   CallableOperator BT,
                                                                                   const VectorSpace& domain,
                                                                                   const VectorSpace& range)
    : OperatorBase(domain,range),
      stateSolver_(std::move(stateSolver)), controlSolver_(std::move(controlSolver)), adjointSolver_(std::move(adjointSolver)),
      B_(std::move(B)), BT_(std::move(BT))
  {}

  Vector TriangularStateConstraintPreconditioner::operator()(const Vector& x) const
  {
    auto x_ = cast_ref<ProductSpace::Vector>(x);
    auto y = range().element();
    auto& y_ = cast_ref<ProductSpace::Vector>(y);

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
    cast_ref<ProductSpace::Vector>(y).variable(stateIndex()) = stateSolver_( cast_ref<ProductSpace::Vector>(rhs).variable(adjointIndex()) );
    return y;
  }
}

