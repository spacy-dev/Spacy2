#include "productSpace.hh"

#include "Util/Exceptions/invalidArgumentException.hh"

#include "productSpaceElement.hh"
#include "productSpaceProduct.hh"

#include <algorithm>

namespace Algorithm
{
  using Interface::AbstractVectorSpace;
  using Interface::AbstractVector;

  namespace
  {
    template <class T>
    std::vector<unsigned> extractSpaceIds(const std::vector<T>& v)
    {
      std::vector<unsigned> result(v.size());

      for(auto i=0u; i<result.size(); ++i) result[i] = i;
      return result;
    }


    std::vector<std::shared_ptr<AbstractVectorSpace> > extractSubSpaces(const std::vector<std::shared_ptr<AbstractVectorSpace> >& spaces,
                                                                        const std::vector<unsigned>& subSpaceIds)
    {
      std::vector< std::shared_ptr<AbstractVectorSpace> > subSpaces;
      for( unsigned i : subSpaceIds )
        subSpaces.push_back( spaces[i] );
      return subSpaces;
    }
  }


  ProductSpace::ProductSpace(const std::vector<std::shared_ptr<AbstractVectorSpace> >& spaces,
                             const std::vector<unsigned>& primalSubSpaceIds,
                             const std::vector<unsigned>& dualSubSpaceIds)
    : AbstractVectorSpace(std::make_shared<ProductSpaceProduct>()),
      primalSubSpaceIds_(primalSubSpaceIds),
      dualSubSpaceIds_(dualSubSpaceIds),
      isPrimalDualProductSpace_(true)
  {
    for( auto i = 0u; i< primalSubSpaceIds_.size(); ++i )
      primalMap_[primalSubSpaceIds[i]] = i;
    for( auto i = 0u; i< dualSubSpaceIds_.size(); ++i )
      dualMap_[dualSubSpaceIds[i]] = i;

    spaces_ = std::vector<std::shared_ptr<AbstractVectorSpace> >(2,nullptr);
    spaces_[0] = std::make_shared<ProductSpace>(extractSubSpaces(spaces,primalSubSpaceIds_));
    spaces_[1] = std::make_shared<ProductSpace>(extractSubSpaces(spaces,dualSubSpaceIds_));
  }

  ProductSpace::ProductSpace(const std::vector<std::shared_ptr<AbstractVectorSpace> >& spaces)
    : AbstractVectorSpace(std::make_shared<ProductSpaceProduct>()),
      spaces_(spaces),
      primalSubSpaceIds_(extractSpaceIds(spaces))
  {
    for(auto i=0u; i<primalSubSpaceIds_.size(); ++i)
      primalMap_[i] = i;
  }


  std::vector<std::shared_ptr<Interface::AbstractVectorSpace> > ProductSpace::subSpaces() const
  {
    return spaces_;
  }

  const Interface::AbstractVectorSpace& ProductSpace::subSpace(unsigned i) const
  {
    if( isPrimalDualProductSpace() )
    {
      if( isPrimalSubSpaceId(i) ) return castTo<ProductSpace>(*spaces_[0]).subSpace(primalMap_.find(i)->second);
      if( isDualSubSpaceId(i) ) return castTo<ProductSpace>(*spaces_[0]).subSpace(dualMap_.find(i)->second);
    }
    return *spaces_[i];
  }

  std::shared_ptr<Interface::AbstractVectorSpace> ProductSpace::sharedSubSpace(unsigned i) const
  {
    if( isPrimalDualProductSpace() )
    {
      if( isPrimalSubSpaceId(i) ) return castTo<ProductSpace>(*spaces_[0]).sharedSubSpace(primalMap_.find(i)->second);
      if( isDualSubSpaceId(i) ) return castTo<ProductSpace>(*spaces_[0]).sharedSubSpace(dualMap_.find(i)->second);
    }
    return spaces_[i];
  }


  std::unique_ptr<AbstractVector> ProductSpace::elementImpl() const
  {
    return std::make_unique<ProductSpaceElement>(*this);
  }

  ProductSpace& ProductSpace::primalSubSpace()
  {
    assert( isPrimalDualProductSpace() );
    return castTo<ProductSpace>(*spaces_[0]);
  }

  const ProductSpace& ProductSpace::primalSubSpace() const
  {
    assert( isPrimalDualProductSpace() );
    return castTo<ProductSpace>(*spaces_[0]);
  }

  ProductSpace& ProductSpace::dualSubSpace()
  {
    assert( isPrimalDualProductSpace() );
    return castTo<ProductSpace>(*spaces_[1]);
  }

  const ProductSpace& ProductSpace::dualSubSpace() const
  {
    assert( isPrimalDualProductSpace() );
    return castTo<ProductSpace>(*spaces_[1]);
  }

  std::shared_ptr<ProductSpace> ProductSpace::sharedPrimalSubSpace() const
  {
    assert( isPrimalDualProductSpace() );
    return std::static_pointer_cast<ProductSpace>(spaces_[0]);
  }

  std::shared_ptr<ProductSpace> ProductSpace::sharedDualSubSpace() const
  {
    assert( isPrimalDualProductSpace() );
    return std::static_pointer_cast<ProductSpace>(spaces_[1]);
  }


  const std::vector<unsigned>& ProductSpace::primalSubSpaceIds() const
  {
    return primalSubSpaceIds_;
  }

  const std::vector<unsigned>& ProductSpace::dualSubSpaceIds() const
  {
    return dualSubSpaceIds_;
  }

  bool ProductSpace::isPrimalSubSpaceId(unsigned i) const
  {
    return primalMap_.find(i) != end(primalMap_);
  }

  bool ProductSpace::isDualSubSpaceId(unsigned i) const
  {
    return dualMap_.find(i) != end(dualMap_);
  }

  unsigned ProductSpace::primalIdMap(unsigned i) const
  {
    return primalMap_.find(i)->second;
  }

  unsigned ProductSpace::dualIdMap(unsigned i) const
  {
    return dualMap_.find(i)->second;
  }

  bool ProductSpace::isPrimalDualProductSpace() const
  {
    return isPrimalDualProductSpace_;
  }


  VectorSpace makeProductSpace(const std::vector<std::shared_ptr<Interface::AbstractVectorSpace> >& spaces)
  {
    return makeProductSpace( spaces , extractSpaceIds(spaces) );
  }

  VectorSpace makeProductSpace(const std::vector<std::shared_ptr<Interface::AbstractVectorSpace> >& spaces,
                                const std::vector<unsigned>& primalSubSpaceIds,
                                const std::vector<unsigned>& dualSubSpaceIds)
  {
    return createFromSharedImpl< ::Algorithm::VectorSpace , ::Algorithm::ProductSpace >( spaces , primalSubSpaceIds , dualSubSpaceIds );
  }

}
