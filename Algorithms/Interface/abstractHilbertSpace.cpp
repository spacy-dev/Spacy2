#include "abstractHilbertSpace.hh"

#include "abstractFunctionSpaceElement.hh"
#include "abstractNorm.hh"
#include "hilbertSpaceNorm.hh"

#include "Util/Exceptions/invalidArgumentException.hh"

namespace Algorithm
{
  namespace Interface
  {
    AbstractHilbertSpace::AbstractHilbertSpace(std::shared_ptr<AbstractScalarProduct> sp)
      : AbstractBanachSpace(std::make_shared<HilbertSpaceNorm>(sp)), sp_(sp)
    {
      addDualSpace(*this);
      addPrimalSpace(*this);
    }

    void AbstractHilbertSpace::setScalarProduct(std::shared_ptr<AbstractScalarProduct> sp)
    {
      sp_ = sp;
      setNorm(std::make_shared<HilbertSpaceNorm>(sp));
    }

    std::shared_ptr<AbstractScalarProduct> AbstractHilbertSpace::getScalarProduct() const
    {
      return sp_;
    }

    bool isHilbertSpace(const AbstractBanachSpace& space)
    {
      return dynamic_cast<const AbstractHilbertSpace*>(&space) != nullptr;
    }

    double operator* (const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y)
    {
      if( isHilbertSpace(x.getSpace()) && isHilbertSpace(y.getSpace()) && x.spaceIndex() == y.spaceIndex() )
      {
        auto sp = dynamic_cast<const AbstractHilbertSpace&>(x.getSpace()).getScalarProduct();
        return sp->operator ()(x,y);
      }

      throw InvalidArgumentException("operator*(const AbstractFunctionSpaceElement&,const AbstractFunctionSpaceElement&)");
      return 0.;
    }
  }
}
