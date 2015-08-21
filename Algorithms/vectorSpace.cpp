#include "vectorSpace.hh"

#include "hilbertSpaceNorm.hh"

#include <utility>

namespace Algorithm
{
//  VectorSpace::VectorSpace(VectorSpaceImpl impl, Norm norm)
//    : Mixin::Impl<VectorSpaceImpl>(std::move(impl)),
//      norm_(norm)
//  {
////    if( impl().isHilbertSpace() )
////      impl().setDualSpace(sharedImpl());
//  }

  VectorSpace::VectorSpace(const VectorSpaceImpl& impl, std::shared_ptr<Interface::AbstractScalarProduct> sp)
    : Mixin::Impl<VectorSpaceImpl>(std::move(impl)) ,
      norm_(HilbertSpaceNorm(sp)),
      sp_(std::make_shared<ScalarProduct>(sp))
  {
    setDualSpace(this);
    addDualSpace(*this);
    addPrimalSpace(*this);
    std::cout << "create space: " << index_ << std::endl;
  }

  void VectorSpace::setNorm(Algorithm::Norm norm)
  {
    norm_ = std::move(norm);
  }

  const Norm& VectorSpace::norm() const
  {
    return norm_;
  }

  Vector VectorSpace::element() const
  {
    return impl().element(this);
  }

  unsigned VectorSpace::index() const
  {
    return index_;
  }

  void VectorSpace::setScalarProduct(Algorithm::ScalarProduct sp)
  {
    sp_ = std::make_shared<ScalarProduct>(sp);
    setNorm( HilbertSpaceNorm(sp_->sharedImpl()) );
  }

  const ScalarProduct& VectorSpace::scalarProduct() const
  {
    return *sp_;
  }


  void VectorSpace::addPrimalSpace(const VectorSpace& Y)
  {
    primalSpaces_.push_back(Y.index());
  }

  void VectorSpace::addDualSpace(const VectorSpace& Y)
  {
    dualSpaces_.push_back(Y.index());
  }

  bool VectorSpace::isPrimalWRT(const VectorSpace& Y) const
  {
    for( auto index : dualSpaces_ )
      if( index == Y.index() )
        return true;

    return false;
  }

  bool VectorSpace::isDualWRT(const VectorSpace& Y) const
  {
    for( auto index : primalSpaces_ )
      if( index == Y.index() )
        return true;

    return false;
  }

  VectorSpace* VectorSpace::dualSpace_ptr() const
  {
    assert( dualSpace_ != nullptr );
    return dualSpace_;
  }

  void VectorSpace::setDualSpace(VectorSpace* Y)
  {
    dualSpace_ = Y;
  }

  bool VectorSpace::isHilbertSpace() const
  {
    return this == dualSpace_ptr();
  }

  void connectPrimalDual(VectorSpace& primalSpace, VectorSpace& dualSpace)
  {
    primalSpace.addDualSpace( dualSpace );
    dualSpace.addPrimalSpace( primalSpace );
  }
}
