#include "inducedScalarProduct.hh"

#include <Spacy/Util/cast.hh>
#include <Spacy/Spaces/ProductSpace/vector.hh>

#include <utility>

namespace Spacy
{
  AdaptiveInducedScalarProduct::AdaptiveInducedScalarProduct(std::function<LinearOperator()> operatorCreator)
    : operatorCreator_(std::move(operatorCreator))
  {}

  Real AdaptiveInducedScalarProduct::operator()(const Vector& x, const Vector& y) const
  {
    return operatorCreator_()(y)(x);
  }


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

  Real PrimalInducedScalarProduct::operator()(const Vector& x, const Vector& y) const
  {
//    std::cout << "induced scalar product" << std::endl;
//    auto& x_ = cast_ref<ProductSpace::Vector>(x);
//    auto& y_ = cast_ref<ProductSpace::Vector>(y);
//    x_.component(DUAL) *= 0;
//    y_.component(DUAL) *= 0;

//    /* TODO PROJECTION */
//    std::cout << "PISP y: " << norm(y) << std::endl;
//    std::cout << "PISP My: " << norm(M_(y)) << std::endl;
//    std::cout << "PISP x: " << norm(x) << std::endl;
//    std::cout << "PISP xMy: " << M_(y)(x) << std::endl;

    return M_(y)(x);
  }
}
