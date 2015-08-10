#include "abstractFunctionSpace.hh"

#include "abstractScalarProduct.hh"
#include "hilbertSpaceNorm.hh"

#include "Util/Exceptions/invalidArgumentException.hh"

#include <cassert>
#include <stdexcept>

namespace Algorithm
{
  namespace Interface
  {
    AbstractFunctionSpace::AbstractFunctionSpace(std::shared_ptr<AbstractNorm> norm)
      : norm_(norm)
    {
      std::cout << "Creating space number " << index_ << "." << std::endl;
    }

    AbstractFunctionSpace::AbstractFunctionSpace(std::shared_ptr<AbstractScalarProduct> sp)
      : AbstractFunctionSpace(std::make_shared<HilbertSpaceNorm>(sp))
    {
      sp_ = sp;
      addDualSpace(*this);
      addPrimalSpace(*this);
    }

    void AbstractFunctionSpace::setNorm(std::shared_ptr<AbstractNorm> norm)
    {
      norm_ = norm;
    }

    const AbstractNorm& AbstractFunctionSpace::norm() const
    {
      assert(norm_ != nullptr);
      return *norm_;
    }

    std::shared_ptr<AbstractNorm> AbstractFunctionSpace::sharedNorm() const
    {
      return norm_;
    }

    std::unique_ptr<AbstractFunctionSpaceElement> AbstractFunctionSpace::element() const
    {
      return elementImpl();
    }

    unsigned AbstractFunctionSpace::index() const noexcept
    {
      return index_;
    }

//    const AbstractFunctionSpace& AbstractFunctionSpace::dualSpace() const
//    {
//      if( dualSpace_.expired() ) throw std::runtime_error("Dual space not defined.");
//      return *dualSpace_;
//    }

    std::shared_ptr<AbstractFunctionSpace> AbstractFunctionSpace::dualSpacePtr() const
    {
      if( dualSpace_.expired() ) throw std::runtime_error("Dual space not defined.");
      return dualSpace_.lock();
    }

    void AbstractFunctionSpace::setDualSpace(std::shared_ptr<AbstractFunctionSpace> dualSpace)
    {
      dualSpace_ = dualSpace;
    }


    void AbstractFunctionSpace::addPrimalSpace(const AbstractFunctionSpace& primalSpace)
    {
      primalSpaces_.push_back(primalSpace.index());
    }

    void AbstractFunctionSpace::addDualSpace(const AbstractFunctionSpace& dualSpace)
    {
      dualSpaces_.push_back(dualSpace.index());
    }

    bool AbstractFunctionSpace::isPrimalWRT(const AbstractFunctionSpace& dualSpace) const
    {
      for( auto index : dualSpaces_ )
        if( index == dualSpace.index() )
          return true;

      return false;
    }

    bool AbstractFunctionSpace::isDualWRT(const AbstractFunctionSpace& primalSpace) const
    {
      for( auto index : primalSpaces_ )
        if( index == primalSpace.index() )
          return true;

      return false;
    }

    void AbstractFunctionSpace::setScalarProduct(std::shared_ptr<AbstractScalarProduct> sp)
    {
      sp_ = sp;
      setNorm(std::make_shared<HilbertSpaceNorm>(sp));
    }

    std::shared_ptr<AbstractScalarProduct> AbstractFunctionSpace::scalarProduct() const
    {
      return sp_;
    }

    bool AbstractFunctionSpace::isHilbertSpace() const
    {
      return sp_ != nullptr;
    }

    double operator* (const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y)
    {
      if( x.space().isHilbertSpace() && x.spaceIndex() == y.spaceIndex() )
      {
        auto sp = x.space().scalarProduct();
        return sp->operator ()(x,y);
      }

      throw InvalidArgumentException("operator*(const AbstractFunctionSpaceElement&,const AbstractFunctionSpaceElement&)");
      return 0.;
    }


    void makePrimalDual(AbstractFunctionSpace& primalSpace, AbstractFunctionSpace& dualSpace)
    {
      dualSpace.addPrimalSpace(primalSpace);
      primalSpace.addDualSpace(dualSpace);
    }

  }
}
