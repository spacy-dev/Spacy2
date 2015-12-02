// Copyright (C) 2015 by Lars Lubkoll. All rights reserved.
// Released under the terms of the GNU General Public License version 3 or later.

#include "vectorSpace.hh"

#include "Spacy/Util/cast.hh"
#include "Spacy/Util/Exceptions/incompatibleSpaceException.hh"

#include "Spacy/Spaces/ProductSpace/vectorSpace.hh"
#include "Spacy/Spaces/RealSpace/real.hh"
#include "Spacy/vector.hh"
#include "Spacy/hilbertSpaceNorm.hh"

#include <boost/type_erasure/is_empty.hpp>

#include <stdexcept>
#include <utility>

namespace Spacy
{
  VectorSpace::VectorSpace(VectorCreator creator, Norm norm, bool defaultIndex)
    : creator_(creator),
      norm_{norm}
  {
    if(defaultIndex) index_ = 0;
  }

  VectorSpace::VectorSpace(VectorSpace&& V)
    : creator_{V.creator_} ,
      norm_{std::move(V.norm_)} ,
      sp_{std::move(V.sp_)} ,
      index_{std::move(V.index_)} ,
      primalSpaces_{std::move(V.primalSpaces_)} ,
      dualSpaces_{std::move(V.dualSpaces_)}
  {
    if( &V == V.dualSpace_)
      setDualSpace(this);
    else
      if( V.dualSpace_ != nullptr)
        setDualSpace(V.dualSpace_);
  }

  VectorSpace& VectorSpace::operator=(VectorSpace&& V)
  {
    creator_ = std::move(V.creator_);
    norm_ = std::move(V.norm_);
    sp_ = std::move(V.sp_);
    index_ = V.index_;
    primalSpaces_ = std::move(V.primalSpaces_);
    dualSpaces_ = std::move(V.dualSpaces_);
    if( &V == V.dualSpace_)
      setDualSpace(this);
    else
      if( V.dualSpace_ != nullptr )
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

  Vector VectorSpace::zeroVector() const
  {
    return creator_(this);
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
    if( !sp_ ) throw std::runtime_error( (std::string("No scalar product defined in space ") + std::to_string(index()) + "!").c_str());
    return sp_;
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

  const VectorSpace& VectorSpace::dualSpace() const
  {
    if( dualSpace_ == nullptr ) throw InvalidArgumentException("VectorSpace::dualSpace()");
    return *dualSpace_;
  }

  void VectorSpace::setDualSpace(const VectorSpace* Y)
  {
    dualSpace_ = Y;
    addDualSpace(*dualSpace_);
  }

  bool VectorSpace::isHilbertSpace() const
  {
    if( sp_ ) return true;
    return false;
  }

  bool VectorSpace::isAdmissible(const Vector& x) const
  {
    return restriction_(x);
  }

  void VectorSpace::setRestriction(std::function<bool(const Vector&)> f)
  {
    restriction_ = std::move(f);
  }

  VectorCreator& VectorSpace::creator()
  {
    return creator_;
  }

  const VectorCreator& VectorSpace::creator() const
  {
    return creator_;
  }



  VectorSpace makeBanachSpace(VectorCreator creator, Norm norm)
  {
    return VectorSpace{std::move(creator),std::move(norm)};
  }

  namespace
  {
    template <class Space>
    void setDualSpace(Space& subSpace)
    {
      subSpace.setDualSpace(&subSpace);
      subSpace.addDualSpace(subSpace);
    }


    template <class Creator>
    void connectSubSpacesIfConsistent(VectorCreator& creator);

    void connectSubSpaces(ProductSpace::VectorCreator& creator)
    {
      for( auto i = 0u; i < creator.subSpaces().size(); ++i)
        setDualSpace( creator.subSpace(i) );
    }

    template <class Creator>
    void connectSubSpacesIfConsistent(VectorCreator& creator)
    {
      if( creator.template target<Creator>() != nullptr )
        connectSubSpaces( *creator.template target<Creator>() );
    }

    void connectSubSpaces(VectorCreator& creator)
    {
      connectSubSpacesIfConsistent<ProductSpace::VectorCreator>(creator);
    }
  }

  VectorSpace makeHilbertSpace(VectorCreator creator, ScalarProduct scalarProduct, bool defaultIndex)
  {
    auto V = VectorSpace{std::move(creator),HilbertSpaceNorm{scalarProduct},defaultIndex};
    V.setScalarProduct(std::move(scalarProduct));
    V.setDualSpace(&V);
    V.addDualSpace(V);

    connectSubSpaces(creator);

    return V;
  }

  void connect(VectorSpace& X, VectorSpace& Y)
  {
    X.addDualSpace( Y );
  }

  void connectAsPrimalDualPair(VectorSpace& X, VectorSpace& Y)
  {
    X.addDualSpace( Y );
  }


  void checkSpaceCompatibility(const VectorSpace& V, const VectorSpace& W)
  {
    if( V.index() != W.index() )
      throw IncompatibleSpaceException(V.index(),W.index());
  }
}
