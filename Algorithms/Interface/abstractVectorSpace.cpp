#include "abstractVectorSpace.hh"

#include "abstractScalarProduct.hh"
#include "hilbertSpaceNorm.hh"

#include "Util/Exceptions/invalidArgumentException.hh"

#include <cassert>
#include <stdexcept>

namespace Algorithm
{
  namespace Interface
  {
    AbstractVectorSpace::AbstractVectorSpace(std::shared_ptr<AbstractNorm> norm)
      : norm_(norm)
    {
      std::cout << "Creating space number " << index_ << "." << std::endl;
    }

    AbstractVectorSpace::AbstractVectorSpace(std::shared_ptr<AbstractScalarProduct> sp)
      : AbstractVectorSpace(std::make_shared<HilbertSpaceNorm>(sp))
    {
      sp_ = sp;
      addDualSpace(*this);
      addPrimalSpace(*this);
    }

    void AbstractVectorSpace::setNorm(std::shared_ptr<AbstractNorm> norm)
    {
      norm_ = norm;
    }

    const AbstractNorm& AbstractVectorSpace::norm() const
    {
      assert(norm_ != nullptr);
      return *norm_;
    }

    std::shared_ptr<AbstractNorm> AbstractVectorSpace::sharedNorm() const
    {
      return norm_;
    }

    std::unique_ptr<AbstractVector> AbstractVectorSpace::element() const
    {
      return elementImpl();
    }

    unsigned AbstractVectorSpace::index() const noexcept
    {
      return index_;
    }

//    const AbstractVectorSpace& AbstractVectorSpace::dualSpace() const
//    {
//      if( dualSpace_.expired() ) throw std::runtime_error("Dual space not defined.");
//      return *dualSpace_;
//    }

    std::shared_ptr<AbstractVectorSpace> AbstractVectorSpace::dualSpacePtr() const
    {
      if( dualSpace_.expired() ) throw std::runtime_error("Dual space not defined.");
      return dualSpace_.lock();
    }

    void AbstractVectorSpace::setDualSpace(std::shared_ptr<AbstractVectorSpace> dualSpace)
    {
      dualSpace_ = dualSpace;
    }


    void AbstractVectorSpace::addPrimalSpace(const AbstractVectorSpace& primalSpace)
    {
      primalSpaces_.push_back(primalSpace.index());
    }

    void AbstractVectorSpace::addDualSpace(const AbstractVectorSpace& dualSpace)
    {
      dualSpaces_.push_back(dualSpace.index());
    }

    bool AbstractVectorSpace::isPrimalWRT(const AbstractVectorSpace& dualSpace) const
    {
      for( auto index : dualSpaces_ )
        if( index == dualSpace.index() )
          return true;

      return false;
    }

    bool AbstractVectorSpace::isDualWRT(const AbstractVectorSpace& primalSpace) const
    {
      for( auto index : primalSpaces_ )
        if( index == primalSpace.index() )
          return true;

      return false;
    }

    void AbstractVectorSpace::setScalarProduct(std::shared_ptr<AbstractScalarProduct> sp)
    {
      sp_ = sp;
      setNorm(std::make_shared<HilbertSpaceNorm>(sp));
    }

    std::shared_ptr<AbstractScalarProduct> AbstractVectorSpace::scalarProduct() const
    {
      return sp_;
    }

    bool AbstractVectorSpace::isHilbertSpace() const
    {
      return sp_ != nullptr;
    }


    void makePrimalDual(AbstractVectorSpace& primalSpace, AbstractVectorSpace& dualSpace)
    {
      dualSpace.addPrimalSpace(primalSpace);
      primalSpace.addDualSpace(dualSpace);
    }

  }
}
