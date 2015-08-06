#include "abstractHilbertSpace.hh"

#include "abstractFunctionSpaceElement.hh"
#include "abstractNorm.hh"
#include "hilbertSpaceNorm.hh"

#include "Util/Exceptions/invalidArgumentException.hh"
#include "Util/castTo.hh"

namespace Algorithm
{
  namespace Interface
  {
    AbstractHilbertSpace::AbstractHilbertSpace(std::shared_ptr<AbstractScalarProduct> sp)
      : AbstractBanachSpace(std::make_shared<HilbertSpaceNorm>(sp)),
        sp_(sp)
    {
      addDualSpace(*this);
      addPrimalSpace(*this);
    }

    void AbstractHilbertSpace::setScalarProduct(std::shared_ptr<AbstractScalarProduct> sp)
    {
      sp_ = sp;
      setNorm(std::make_shared<HilbertSpaceNorm>(sp));
    }

    std::shared_ptr<AbstractScalarProduct> AbstractHilbertSpace::scalarProduct() const
    {
      return sp_;
    }

    double operator* (const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y)
    {
      if( is<AbstractHilbertSpace>(x.space()) && is<AbstractHilbertSpace>(y.space()) && x.spaceIndex() == y.spaceIndex() )
      {
        auto sp = castTo<AbstractHilbertSpace>(x.space()).scalarProduct();
        return sp->operator ()(x,y);
      }

      throw InvalidArgumentException("operator*(const AbstractFunctionSpaceElement&,const AbstractFunctionSpaceElement&)");
      return 0.;
    }
  }
}
