// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "inducedScalarProduct.hh"

#include "Util/cast.hh"
#include "Spacy/Spaces/ProductSpace/vector.hh"
#include "Spacy/Spaces/RealSpace/real.hh"
#include "Spacy/vector.hh"

#include <utility>
#include <iostream>

namespace Spacy
{
  InducedScalarProduct::InducedScalarProduct(LinearOperator M)
    : M_(std::move(M))
  {}

  Real InducedScalarProduct::operator()(const Vector& x, const Vector& y) const
  {
    return M_(y)(x);
  }


  PrimalInducedScalarProduct::PrimalInducedScalarProduct(LinearOperator M)
    : M_(std::move(M))
  {}

  Real PrimalInducedScalarProduct::operator()(Vector x, Vector y) const
  {
//    std::cout << "induced scalar product" << std::endl;
//    auto& x_ = cast_ref<ProductSpace::Vector>(x);
//    auto& y_ = cast_ref<ProductSpace::Vector>(y);
//    x_.component(DUAL) *= 0;
//    y_.component(DUAL) *= 0;

    /* TODO PROJECTION */

    return M_(y)(x);
  }
}
