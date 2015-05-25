#include "abstractHilbertSpace.hh"

#include "../Util/invalidargumentexception.hh"

#include "abstractFunctionSpaceElement.hh"
#include "abstractScalarProduct.hh"
#include "hilbertspacenorm.hh"

namespace Algorithm
{
  AbstractHilbertSpace::AbstractHilbertSpace()
    : norm_(nullptr)
  {}

  void AbstractHilbertSpace::setScalarProduct(std::shared_ptr<AbstractScalarProduct> sp)
  {
    setScalarProductImpl(sp);
    setNorm(std::make_shared<HilbertSpaceNorm>(getScalarProduct()));
  }

  void AbstractHilbertSpace::setNormImpl(std::shared_ptr<AbstractNorm> norm)
  {
    norm_ = norm;
  }


  std::shared_ptr<AbstractScalarProduct> AbstractHilbertSpace::getScalarProduct() const
  {
    return getScalarProductImpl();
  }

  std::shared_ptr<AbstractNorm> AbstractHilbertSpace::getNormImpl() const
  {
    return norm_;
  }


  double operator* (const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y)
  {
    if( dynamic_cast<const AbstractHilbertSpace*>(&x.getSpace()) == nullptr ) throw InvalidArgumentException("operator*(const AbstractFunctionSpaceElement&,const AbstractFunctionSpaceElement)");

    auto sp = dynamic_cast<const AbstractHilbertSpace&>(x.getSpace()).getScalarProduct();
    return sp->operator()(x,y);
  }
}
