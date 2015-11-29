// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "inducedScalarProduct.hh"

#include "Util/cast.hh"
#include "Spacy/Spaces/ProductSpace/vector.hh"
#include "Spacy/Spaces/RealSpace/real.hh"
#include "Spacy/vector.hh"

#include <utility>

namespace Spacy
{
  InducedScalarProduct::InducedScalarProduct(CallableOperator M)
    : M_(std::move(M))
  {}

  Real InducedScalarProduct::operator()(const Vector& x, const Vector& y) const
  {
    return M_(y)(x);
  }


  PrimalInducedScalarProduct::PrimalInducedScalarProduct(CallableOperator M)
    : M_(std::move(M))
  {}

  Real PrimalInducedScalarProduct::operator()(const Vector& x, const Vector& y) const
  {
    const auto& x_ = cast_ref<ProductSpace::Vector>(x);
    const auto& y_ = cast_ref<ProductSpace::Vector>(y);

    /* TODO PROJECTION */

    return M_(y_)(x_);
  }
}
