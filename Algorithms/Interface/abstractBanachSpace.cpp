#include "abstractBanachSpace.hh"
#include "abstractHilbertSpace.hh"

namespace Algorithm
{
  namespace Interface
  {
    AbstractBanachSpace::AbstractBanachSpace(std::shared_ptr<AbstractNorm> norm)
      : norm_(norm)
    {
      std::cout << "Creating space number " << index_ << "." << std::endl;
    }

    void AbstractBanachSpace::setNorm(std::shared_ptr<AbstractNorm> norm)
    {
      norm_ = norm;
    }

    const AbstractNorm& AbstractBanachSpace::getNorm() const
    {
      return *norm_;
    }

    std::shared_ptr<AbstractNorm> AbstractBanachSpace::getSharedNorm() const
    {
      return norm_;
    }

    std::unique_ptr<AbstractFunctionSpaceElement> AbstractBanachSpace::element() const
    {
      return elementImpl();
    }

    unsigned AbstractBanachSpace::index() const noexcept
    {
      return index_;
    }

    void AbstractBanachSpace::addPrimalSpace(const AbstractBanachSpace& primalSpace)
    {
      primalSpaces_.push_back(primalSpace.index());
    }

    void AbstractBanachSpace::addDualSpace(const AbstractBanachSpace& dualSpace)
    {
      dualSpaces_.push_back(dualSpace.index());
    }

    bool AbstractBanachSpace::isPrimalWRT(const AbstractBanachSpace& dualSpace) const
    {
      for( auto index : dualSpaces_ )
        if( index == dualSpace.index() )
          return true;

      return false;
    }

    bool AbstractBanachSpace::isDualWRT(const AbstractBanachSpace& primalSpace) const
    {
      for( auto index : primalSpaces_ )
        if( index == primalSpace.index() )
          return true;

      return false;
    }
  }
}
