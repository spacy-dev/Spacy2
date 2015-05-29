#include "abstractBanachSpace.hh"
#include "abstractDualPairing.hh"
#include "abstractHilbertSpace.hh"

#include "../Util/invalidargumentexception.hh"

namespace Algorithm
{
  void AbstractBanachSpace::setNorm(std::shared_ptr<AbstractNorm> norm)
  {
    setNormImpl(norm);
  }

  std::shared_ptr<AbstractNorm> AbstractBanachSpace::getNorm() const
  {
    return getNormImpl();
  }

  void AbstractBanachSpace::setDualPairing(std::shared_ptr<AbstractDualPairing> dp)
  {
    setDualPairingImpl(dp);
  }

  std::shared_ptr<AbstractDualPairing> AbstractBanachSpace::getDualPairing() const
  {
    return getDualPairingImpl();
  }

  std::unique_ptr<AbstractFunctionSpaceElement> AbstractBanachSpace::element() const
  {
    return elementImpl();
  }

  unsigned AbstractBanachSpace::index() const
  {
    return index_;
  }

  void AbstractBanachSpace::setDualSpace(AbstractBanachSpace& dualSpace)
  {
    dualSpace_ = &dualSpace;
  }

  AbstractBanachSpace* AbstractBanachSpace::getDualSpace() const
  {
    return dualSpace_;
  }


  double operator* (const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y)
  {
//    if( dynamic_cast<const AbstractHilbertSpace*>(&x.getSpace()) == nullptr ) throw InvalidArgumentException("operator*(const AbstractFunctionSpaceElement&,const AbstractFunctionSpaceElement)");
    //if( x.getSpace().index() != y.getSpace().index()) throw IncompatibleSpaceException("operator*(scalar product)",x.getSpace().index(),y.getSpace().index());

    if( isHilbertSpace(x.getSpace()) && isHilbertSpace(y.getSpace()) && x.spaceIndex() == y.spaceIndex() )
    {
      auto sp = dynamic_cast<const AbstractHilbertSpace&>(x.getSpace()).getScalarProduct();
      return sp->operator ()(x,y);
    }
    if( x.getSpace().getDualSpace() != nullptr && x.getSpace().getDualSpace()->index() == y.spaceIndex() )
      return x.getSpace().getDualPairing()->operator ()(x,y);
    if( y.getSpace().getDualPairing() != nullptr && y.getSpace().getDualSpace()->index() == x.spaceIndex() )
      return y.getSpace().getDualPairing()->operator ()(y,x);

    throw InvalidArgumentException("operator*(const AbstractFunctionSpaceElement&,const AbstractFunctionSpaceElement&)");
    auto dp = dynamic_cast<const AbstractBanachSpace&>(x.getSpace()).getDualPairing();
    return dp->operator()(x,y);
  }
}
