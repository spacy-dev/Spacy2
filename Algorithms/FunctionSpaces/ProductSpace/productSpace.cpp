#include "productSpace.hh"

#include "Util/Exceptions/invalidArgumentException.hh"

#include "productSpaceElement.hh"
#include "productSpaceProduct.hh"

#include <iostream>

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
      for( unsigned i : subSpaceIds ) subSpaces.push_back( spaces[i] );
      return subSpaces;
    }
  }


  ProductSpace::ProductSpace(const std::vector<std::shared_ptr<AbstractVectorSpace> >& spaces,
                             const std::vector<unsigned>& primalSubSpaceIds,
                             const std::vector<unsigned>& dualSubSpaceIds)
    : AbstractVectorSpace(std::make_shared<ProductSpaceProduct>()),
      spaces_(spaces),
      primalSubSpaceIds_(primalSubSpaceIds),
      dualSubSpaceIds_(dualSubSpaceIds),
      primalSpace_(std::make_shared<ProductSpace>(extractSubSpaces(spaces_,primalSubSpaceIds_))),
      dualSpace_(std::make_shared<ProductSpace>(extractSubSpaces(spaces_,dualSubSpaceIds_)))
  {
    std::cout << "creating product space, p: " << primalSubSpaceIds.size() << ", d: " << dualSubSpaceIds.size() << std::endl;
  }

  ProductSpace::ProductSpace(const std::vector<std::shared_ptr<AbstractVectorSpace> >& spaces)
    : AbstractVectorSpace(std::make_shared<ProductSpaceProduct>()),
      spaces_(spaces),
      primalSubSpaceIds_(extractSpaceIds(spaces))
  {
    std::cout << "creating primal product space, p: " << extractSpaceIds(spaces).size() << std::endl;
  }


  const std::vector<std::shared_ptr<AbstractVectorSpace> >& ProductSpace::subSpaces() const
  {
    return spaces_;
  }

  const Interface::AbstractVectorSpace& ProductSpace::subSpace(unsigned i) const
  {
    return *spaces_[i];
  }

  std::shared_ptr<Interface::AbstractVectorSpace> ProductSpace::sharedSubSpace(unsigned i) const
  {
    return spaces_[i];
  }


  std::unique_ptr<AbstractVector> ProductSpace::elementImpl() const
  {
    return std::make_unique<ProductSpaceElement>(*this);
  }

  const ProductSpace& ProductSpace::primalSubSpace() const
  {
    return *primalSpace_;
  }

  const ProductSpace& ProductSpace::dualSubSpace() const
  {
    return *dualSpace_;
  }

  std::shared_ptr<ProductSpace> ProductSpace::sharedPrimalSubSpace() const
  {
    return primalSpace_;
  }

  std::shared_ptr<ProductSpace> ProductSpace::sharedDualSubSpace() const
  {
    return dualSpace_;
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
    for( auto j : primalSubSpaceIds_ ) if( i==j ) return true;
    return false;
  }

  bool ProductSpace::isDualSubSpaceId(unsigned i) const
  {
    for( auto j : dualSubSpaceIds_ ) if( i==j ) return true;
    return false;
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
