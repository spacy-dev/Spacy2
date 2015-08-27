#include "vectorSpace.hh"

#include <cmath>
#include <stdexcept>
#include <utility>

namespace Algorithm
{
  namespace
  {
    class HilbertSpaceNorm
    {
    public:
      explicit HilbertSpaceNorm(ScalarProduct sp)
        : sp_(sp)
      {}

      double operator()(const boost::type_erasure::any<Concepts::VectorConcept>& x) const
      {
        return sqrt(sp_(x,x));
      }

    private:
      ScalarProduct sp_;
    };
  }


  VectorSpace::VectorSpace(VectorCreator impl, Norm norm)
    : Mixin::Impl<VectorCreator>(std::move(impl)),
      norm_(norm)
  {}

  VectorSpace::VectorSpace(VectorSpace&& other)
    : Mixin::Impl<VectorCreator>(other.impl()) ,
      norm_(std::move(other.norm_)) ,
      sp_(std::move(other.sp_)) ,
      index_(std::move(other.index_)) ,
      primalSpaces_(std::move(other.primalSpaces_)) ,
      dualSpaces_(std::move(other.dualSpaces_))
  {
    if( &other == other.dualSpace_)
      setDualSpace(this);
    else
      setDualSpace(other.dualSpace_);
  }

  void VectorSpace::setNorm(Algorithm::Norm norm)
  {
    norm_ = std::move(norm);
  }

  const Norm& VectorSpace::norm() const
  {
    return norm_;
  }

  boost::type_erasure::any<Concepts::VectorConcept> VectorSpace::element() const
  {
    return impl()(this);
  }

  unsigned VectorSpace::index() const
  {
    return index_;
  }

  void VectorSpace::setScalarProduct(ScalarProduct sp)
  {
    sp_ = std::make_shared<ScalarProduct>(std::move(sp));
    setNorm( HilbertSpaceNorm(*sp_) );
  }

  const ScalarProduct& VectorSpace::scalarProduct() const
  {
    if( sp_ == nullptr ) throw std::runtime_error("No scalar product defined!");
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

  const VectorSpace& VectorSpace::dualSpace() const
  {
    assert( dualSpace_ != nullptr );
    return *dualSpace_;
  }

  void VectorSpace::setDualSpace(const VectorSpace* Y)
  {
    dualSpace_ = Y;
  }

  bool VectorSpace::isHilbertSpace() const
  {
    return sp_ != nullptr;
  }

  bool VectorSpace::isAdmissible(const boost::type_erasure::any<Concepts::VectorConcept>& x) const
  {
    return restriction_(x);
  }

  void VectorSpace::setRestriction(std::function<bool(const boost::type_erasure::any<Concepts::VectorConcept>&)> f)
  {
    restriction_ = std::move(f);
  }


  VectorSpace makeBanachSpace(VectorCreator impl, Norm norm)
  {
    return VectorSpace{std::move(impl),std::move(norm)};
  }

  VectorSpace makeHilbertSpace(VectorCreator impl, ScalarProduct scalarProduct)
  {
    auto V = VectorSpace{std::move(impl),HilbertSpaceNorm{scalarProduct}};
    V.setScalarProduct(std::move(scalarProduct));
    V.setDualSpace(&V);
    V.addDualSpace(V);
    V.addPrimalSpace(V);
    return std::move(V);
  }

  void connectPrimalDual(VectorSpace& primalSpace, VectorSpace& dualSpace)
  {
    primalSpace.addDualSpace( dualSpace );
    dualSpace.addPrimalSpace( primalSpace );
  }
}
