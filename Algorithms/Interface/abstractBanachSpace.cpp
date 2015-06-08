#include "abstractBanachSpace.hh"
#include "abstractDualPairing.hh"
#include "abstractHilbertSpace.hh"

#include "../Util/invalidargumentexception.hh"

namespace Algorithm
{
  AbstractBanachSpace::AbstractBanachSpace(const AbstractBanachSpace* dualSpace, std::shared_ptr<AbstractNorm> norm)
    : dualSpace_(dualSpace), norm_(norm)
  {
    std::cout << "Creating space number " << index_ << "." << std::endl;
  }

  void AbstractBanachSpace::setNorm(std::shared_ptr<AbstractNorm> norm)
  {
    norm_ = norm;
  }

  std::shared_ptr<AbstractNorm> AbstractBanachSpace::getNorm() const
  {
    return norm_;
  }

  void AbstractBanachSpace::setDualPairing(std::shared_ptr<AbstractDualPairing> dp)
  {
    dp_ = dp;
  }

  std::shared_ptr<AbstractDualPairing> AbstractBanachSpace::getDualPairing() const
  {
    return dp_;
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

  const AbstractBanachSpace& AbstractBanachSpace::getDualSpace() const
  {
    if( dualSpace_ == nullptr ) throw InvalidArgumentException("AbstractBanachSpace::getDualSpace");
    return *dualSpace_;
  }

  bool AbstractBanachSpace::hasDualSpace() const
  {
    return dualSpace_ != nullptr;
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
    if( x.getSpace().hasDualSpace() && x.getSpace().getDualSpace().index() == y.spaceIndex() )
      return x.getSpace().getDualPairing()->operator ()(x,y);
    if( y.getSpace().hasDualSpace() && y.getSpace().getDualSpace().index() == x.spaceIndex() )
      return y.getSpace().getDualPairing()->operator ()(y,x);

    throw InvalidArgumentException("operator*(const AbstractFunctionSpaceElement&,const AbstractFunctionSpaceElement&)");
    auto dp = dynamic_cast<const AbstractBanachSpace&>(x.getSpace()).getDualPairing();
    return dp->operator()(x,y);
  }
}
