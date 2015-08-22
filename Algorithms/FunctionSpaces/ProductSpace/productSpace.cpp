#include "productSpace.hh"

#include "Util/Exceptions/invalidArgumentException.hh"

#include "productSpaceElement.hh"
#include "productSpaceProduct.hh"

#include "../../vector.hh"

#include <algorithm>
#include <boost/type_erasure/any_cast.hpp>

namespace Algorithm
{
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


    std::vector<std::shared_ptr<VectorSpace> > extractSubSpaces(const std::vector<std::shared_ptr<VectorSpace> >& spaces,
                                                                        const std::vector<unsigned>& subSpaceIds)
    {
      std::vector< std::shared_ptr<VectorSpace> > subSpaces;
      for( unsigned i : subSpaceIds )
        subSpaces.push_back( spaces[i] );
      return subSpaces;
    }
  }


  ProductSpace::ProductSpace(const std::vector<std::shared_ptr<VectorSpace> >& spaces,
                             const std::vector<unsigned>& primalSubSpaceIds,
                             const std::vector<unsigned>& dualSubSpaceIds)
    : primalSubSpaceIds_(primalSubSpaceIds),
      dualSubSpaceIds_(dualSubSpaceIds),
      isPrimalDualProductSpace_(true)
  {
    for( auto i = 0u; i< primalSubSpaceIds_.size(); ++i )
      primalMap_[primalSubSpaceIds[i]] = i;
    for( auto i = 0u; i< dualSubSpaceIds_.size(); ++i )
      dualMap_[dualSubSpaceIds[i]] = i;

    spaces_ = std::vector<std::shared_ptr<VectorSpace> >(2,nullptr);
    spaces_[0] = std::make_shared<VectorSpace>( ProductSpace( extractSubSpaces(spaces,primalSubSpaceIds_) ) , ProductSpaceProduct() );
    spaces_[1] = std::make_shared<VectorSpace>( ProductSpace( extractSubSpaces(spaces,dualSubSpaceIds_) ) , ProductSpaceProduct() );
  }

  ProductSpace::ProductSpace(const std::vector<std::shared_ptr<VectorSpace> >& spaces)
    : spaces_(spaces),
      primalSubSpaceIds_(extractSpaceIds(spaces))
  {
    for(auto i=0u; i<primalSubSpaceIds_.size(); ++i)
      primalMap_[i] = i;
  }


  std::vector<std::shared_ptr<VectorSpace> > ProductSpace::subSpaces() const
  {
    return spaces_;
  }

  const VectorSpace& ProductSpace::subSpace(unsigned i) const
  {
    if( isPrimalDualProductSpace() )
    {
      if( isPrimalSubSpaceId(i) ) return boost::type_erasure::any_cast<const ProductSpace&>(spaces_[0]->impl()).subSpace(primalMap_.find(i)->second);
      if( isDualSubSpaceId(i) ) return boost::type_erasure::any_cast<const ProductSpace&>(spaces_[1]->impl()).subSpace(dualMap_.find(i)->second);
    }
    return *spaces_[i];
  }

  std::shared_ptr<VectorSpace> ProductSpace::sharedSubSpace(unsigned i) const
  {
    if( isPrimalDualProductSpace() )
    {
      if( isPrimalSubSpaceId(i) ) return boost::type_erasure::any_cast<const ProductSpace&>(spaces_[0]->impl()).sharedSubSpace(primalMap_.find(i)->second);
      if( isDualSubSpaceId(i) ) return boost::type_erasure::any_cast<const ProductSpace&>(spaces_[1]->impl()).sharedSubSpace(dualMap_.find(i)->second);
    }
    return spaces_[i];
  }


  Vector ProductSpace::element(const VectorSpace* space) const
  {
    return ProductSpaceElement{*space};
  }

  VectorSpace& ProductSpace::primalSubSpace()
  {
    assert( isPrimalDualProductSpace() );
    return *spaces_[0];
  }

  const VectorSpace& ProductSpace::primalSubSpace() const
  {
    assert( isPrimalDualProductSpace() );
    return *spaces_[0];
  }

  VectorSpace& ProductSpace::dualSubSpace()
  {
    assert( isPrimalDualProductSpace() );
    return *spaces_[1];
  }

  const VectorSpace& ProductSpace::dualSubSpace() const
  {
    assert( isPrimalDualProductSpace() );
    return *spaces_[1];
  }

  std::shared_ptr<VectorSpace> ProductSpace::sharedPrimalSubSpace() const
  {
    assert( isPrimalDualProductSpace() );
    return spaces_[0];
  }

  std::shared_ptr<VectorSpace> ProductSpace::sharedDualSubSpace() const
  {
    assert( isPrimalDualProductSpace() );
    return spaces_[1];
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


  VectorSpace makeProductSpace(const std::vector<std::shared_ptr<VectorSpace> >& spaces)
  {
    return makeProductSpace( spaces , extractSpaceIds(spaces) );
  }

  VectorSpace makeProductSpace(const std::vector<std::shared_ptr<VectorSpace> >& spaces,
                                const std::vector<unsigned>& primalSubSpaceIds,
                                const std::vector<unsigned>& dualSubSpaceIds)
  {
    return VectorSpace( ProductSpace( spaces , primalSubSpaceIds , dualSubSpaceIds ) , ProductSpaceProduct() );
//    return createFromSharedImpl< ::Algorithm::VectorSpace , ::Algorithm::ProductSpace >( spaces , primalSubSpaceIds , dualSubSpaceIds );
  }

}
