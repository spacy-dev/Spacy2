// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "triangularStateConstraintPreconditioner.hh"

#include "Spacy/Spaces/productSpace.hh"
#include "Spacy/vectorSpace.hh"

#include <utility>

namespace Spacy
{
  namespace CG
  {
    TriangularStateConstraintPreconditioner::TriangularStateConstraintPreconditioner( ::Spacy::LinearSolver stateSolver,
                                                                                      ::Spacy::LinearSolver controlSolver,
                                                                                      ::Spacy::LinearSolver adjointSolver,
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
      auto& xp_ = cast_ref<ProductSpace::Vector>(x_.component(PRIMAL));
      auto& xd_ = cast_ref<ProductSpace::Vector>(x_.component(DUAL));
      auto y = range().zeroVector();
      auto& y_ = cast_ref<ProductSpace::Vector>(y);
      auto& yp_ = cast_ref<ProductSpace::Vector>(y_.component(PRIMAL));
      auto& yd_ = cast_ref<ProductSpace::Vector>(y_.component(DUAL));

      auto localAdjointIndex = creator<ProductSpace::VectorCreator>(yd_.space()).idMap(adjointIndex());
      auto localControlIndex = creator<ProductSpace::VectorCreator>(yp_.space()).idMap(controlIndex());
      auto localStateIndex = creator<ProductSpace::VectorCreator>(yp_.space()).idMap(stateIndex());

      yd_.component(localAdjointIndex) = adjointSolver_( xp_.component(localStateIndex) );
      xp_.component(localControlIndex) -= BT_( yd_.component(localAdjointIndex) );

      yp_.component(localControlIndex) = controlSolver_( xp_.component(localControlIndex) );

      xd_.component(localAdjointIndex) -= B_( yp_.component(localControlIndex) );

      yp_.component(localStateIndex) = stateSolver_( xd_.component(localAdjointIndex) );

      return std::move(y);
    }

    Vector TriangularStateConstraintPreconditioner::kernelOffset(const Vector& rhs) const
    {
      auto& rhs_ = cast_ref<ProductSpace::Vector>(rhs);
      auto& rhsd_ = cast_ref<ProductSpace::Vector>(rhs_.component(DUAL));

      auto y = range().zeroVector();
      auto& y_ = cast_ref<ProductSpace::Vector>(y);
      auto& yp_ = cast_ref<ProductSpace::Vector>(y_.component(PRIMAL));
      auto localStateIndex = creator<ProductSpace::VectorCreator>(yp_.space()).idMap(stateIndex());
      auto localAdjointIndex = creator<ProductSpace::VectorCreator>(rhsd_.space()).idMap(adjointIndex());

      yp_.component(localStateIndex) = stateSolver_( rhsd_.component(localAdjointIndex) );
      return std::move(y);
    }
  }
}

