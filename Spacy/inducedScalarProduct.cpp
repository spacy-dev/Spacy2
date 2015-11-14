#include "inducedScalarProduct.hh"

#include "Util/cast.hh"
#include "Spacy/Spaces/PrimalDualProductSpace/vector.hh"
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
    const auto& x_ = cast_ref<PrimalDualProductSpace::Vector>(x);
    const auto& y_ = cast_ref<PrimalDualProductSpace::Vector>(y);

    return M_(y_)(x_);
  }
}
