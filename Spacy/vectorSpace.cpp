#include "vectorSpace.hh"

#include "Spacy/Spaces/productSpace.hh"
#include "Util/cast.hh"

#include <boost/type_erasure/is_empty.hpp>

#include <cmath>
#include <stdexcept>
#include <utility>

namespace Spacy
{
  namespace
  {
    class HilbertSpaceNorm
    {
    public:
      explicit HilbertSpaceNorm(ScalarProduct sp)
        : sp_(std::move(sp))
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
    : Mixin::Impl<VectorCreator>{std::move(impl)},
      norm_{norm}
  {}

  VectorSpace::VectorSpace(VectorSpace&& V)
    : Mixin::Impl<VectorCreator>{V.impl()} ,
      norm_{std::move(V.norm_)} ,
      sp_{std::move(V.sp_)} ,
      index_{std::move(V.index_)} ,
      primalSpaces_{std::move(V.primalSpaces_)} ,
      dualSpaces_{std::move(V.dualSpaces_)}
  {
    if( &V == V.dualSpace_)
      setDualSpace(this);
    else
      setDualSpace(V.dualSpace_);
  }

  VectorSpace& VectorSpace::operator=(VectorSpace&& V)
  {
    Mixin::Impl<VectorCreator>::operator=(std::move(V));
    norm_ = std::move(V.norm_);
    sp_ = std::move(V.sp_);
    index_ = V.index_;
    primalSpaces_ = std::move(V.primalSpaces_);
    dualSpaces_ = std::move(V.dualSpaces_);
    if( &V == V.dualSpace_)
      setDualSpace(this);
    else
      setDualSpace(V.dualSpace_);
    return *this;
  }

  void VectorSpace::setNorm(Norm norm)
  {
    norm_ = std::move(norm);
  }

  const Norm& VectorSpace::norm() const
  {
    return norm_;
  }

  boost::type_erasure::any<Concepts::VectorConcept> VectorSpace::vector() const
  {
    return impl()(this);
  }

  unsigned VectorSpace::index() const
  {
    return index_;
  }

  void VectorSpace::setScalarProduct(ScalarProduct sp)
  {
    sp_ = std::move(sp);
    setNorm( HilbertSpaceNorm{sp_} );
  }

  const ScalarProduct& VectorSpace::scalarProduct() const
  {
    if( is_empty(sp_) ) throw std::runtime_error("No scalar product defined!");
    return sp_;
  }


//  void VectorSpace::addPrimalSpace(const VectorSpace& Y)
//  {
//    primalSpaces_.push_back(Y.index());
//  }

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

//  bool VectorSpace::isDualWRT(const VectorSpace& Y) const
//  {
//    for( auto index : primalSpaces_ )
//      if( index == Y.index() )
//        return true;

//    return false;
//  }

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
    return !is_empty(sp_);
  }

  bool VectorSpace::isAdmissible(const boost::type_erasure::any<Concepts::VectorConcept>& x) const
  {
    return restriction_(x);
  }

  void VectorSpace::setRestriction(std::function<bool(const boost::type_erasure::any<Concepts::VectorConcept>&)> f)
  {
    restriction_ = std::move(f);
  }


  VectorSpace makeBanachSpace(VectorCreator creator, Norm norm)
  {
    return VectorSpace{std::move(creator),std::move(norm)};
  }

  VectorSpace makeHilbertSpace(VectorCreator creator, ScalarProduct scalarProduct)
  {
    auto V = VectorSpace{std::move(creator),HilbertSpaceNorm{scalarProduct}};
    V.setScalarProduct(std::move(scalarProduct));
    V.setDualSpace(&V);
    V.addDualSpace(V);
//    V.addPrimalSpace(V);

    if( is<ProductSpace::VectorCreator>(creator) )
    {
      auto& productSpaceCreator = cast_ref<ProductSpace::VectorCreator>(creator);
      for( auto i = 0u; i < productSpaceCreator.subSpaces().size(); ++i)
      {
        auto subSpace = productSpaceCreator.sharedSubSpace(i);
        subSpace->setDualSpace(subSpace.get());
        subSpace->addDualSpace(*subSpace);
//        subSpace->addPrimalSpace(*subSpace);
      }
    }

    return std::move(V);
  }

  void connect(VectorSpace& X, VectorSpace& Y)
  {
    X.addDualSpace( Y );
//    Y.addPrimalSpace( X );
  }
}
