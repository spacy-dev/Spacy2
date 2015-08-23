#include "vectorSpace.hh"

#include "Util/Exceptions/invalidArgumentException.hh"
#include "Util/cast.hh"

#include "vector.hh"
#include "scalarProduct.hh"

#include "../../vectorSpace.hh"

#include <algorithm>

namespace Algorithm
{
  namespace ProductSpace
  {
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


    SpaceCreator::SpaceCreator(const std::vector<std::shared_ptr<VectorSpace> >& spaces,
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
      spaces_[0] = std::make_shared<VectorSpace>( makeHilbertSpace( extractSubSpaces(spaces,primalSubSpaceIds_) ) );
      spaces_[1] = std::make_shared<VectorSpace>( makeHilbertSpace( extractSubSpaces(spaces,dualSubSpaceIds_) ) );
    }

    SpaceCreator::SpaceCreator(const std::vector<std::shared_ptr<VectorSpace> >& spaces)
      : spaces_(spaces),
        primalSubSpaceIds_(extractSpaceIds(spaces))
    {
      for(auto i=0u; i<primalSubSpaceIds_.size(); ++i)
        primalMap_[i] = i;
    }


    const std::vector<std::shared_ptr<VectorSpace> >& SpaceCreator::subSpaces() const
    {
      return spaces_;
    }

    const VectorSpace& SpaceCreator::subSpace(unsigned i) const
    {
      if( isPrimalDualProductSpace() )
      {
        if( isPrimalSubSpaceId(i) ) return cast_ref<ProductSpace::SpaceCreator>(spaces_[0]->impl()).subSpace(primalMap_.find(i)->second);
        if( isDualSubSpaceId(i) ) return cast_ref<ProductSpace::SpaceCreator>(spaces_[1]->impl()).subSpace(dualMap_.find(i)->second);
      }
      return *spaces_[i];
    }

    VectorSpace* SpaceCreator::subSpace_ptr(unsigned i) const
    {
      return sharedSubSpace(i).get();
    }

    std::shared_ptr<VectorSpace> SpaceCreator::sharedSubSpace(unsigned i) const
    {
      if( isPrimalDualProductSpace() )
      {
        if( isPrimalSubSpaceId(i) ) return cast_ref<ProductSpace::SpaceCreator>(spaces_[0]->impl()).sharedSubSpace(primalMap_.find(i)->second);
        if( isDualSubSpaceId(i) ) return cast_ref<ProductSpace::SpaceCreator>(spaces_[1]->impl()).sharedSubSpace(dualMap_.find(i)->second);
      }
      return spaces_[i];
    }


    ::Algorithm::Vector SpaceCreator::operator()(const VectorSpace* space) const
    {
      return Vector{*space};
    }

    VectorSpace& SpaceCreator::primalSubSpace()
    {
      assert( isPrimalDualProductSpace() );
      return *spaces_[0];
    }

    const VectorSpace& SpaceCreator::primalSubSpace() const
    {
      assert( isPrimalDualProductSpace() );
      return *spaces_[0];
    }

    VectorSpace* SpaceCreator::primalSubSpace_ptr() const
    {
      return sharedPrimalSubSpace().get();
    }

    VectorSpace& SpaceCreator::dualSubSpace()
    {
      assert( isPrimalDualProductSpace() );
      return *spaces_[1];
    }

    const VectorSpace& SpaceCreator::dualSubSpace() const
    {
      assert( isPrimalDualProductSpace() );
      return *spaces_[1];
    }

    VectorSpace* SpaceCreator::dualSubSpace_ptr() const
    {
      return sharedDualSubSpace().get();
    }

    std::shared_ptr<VectorSpace> SpaceCreator::sharedPrimalSubSpace() const
    {
      assert( isPrimalDualProductSpace() );
      return spaces_[0];
    }

    std::shared_ptr<VectorSpace> SpaceCreator::sharedDualSubSpace() const
    {
      assert( isPrimalDualProductSpace() );
      return spaces_[1];
    }


    const std::vector<unsigned>& SpaceCreator::primalSubSpaceIds() const
    {
      return primalSubSpaceIds_;
    }

    const std::vector<unsigned>& SpaceCreator::dualSubSpaceIds() const
    {
      return dualSubSpaceIds_;
    }

    bool SpaceCreator::isPrimalSubSpaceId(unsigned i) const
    {
      return primalMap_.find(i) != end(primalMap_);
    }

    bool SpaceCreator::isDualSubSpaceId(unsigned i) const
    {
      return dualMap_.find(i) != end(dualMap_);
    }

    unsigned SpaceCreator::primalIdMap(unsigned i) const
    {
      return primalMap_.find(i)->second;
    }

    unsigned SpaceCreator::dualIdMap(unsigned i) const
    {
      return dualMap_.find(i)->second;
    }

    bool SpaceCreator::isPrimalDualProductSpace() const
    {
      return isPrimalDualProductSpace_;
    }


    VectorSpace makeHilbertSpace(const std::vector<std::shared_ptr<VectorSpace> >& spaces)
    {
      return ::Algorithm::makeHilbertSpace( SpaceCreator{ spaces } , ScalarProduct{} );
    }

    VectorSpace makeHilbertSpace(const std::vector<std::shared_ptr<VectorSpace> >& spaces,
                                  const std::vector<unsigned>& primalSubSpaceIds,
                                  const std::vector<unsigned>& dualSubSpaceIds)
    {
      return ::Algorithm::makeHilbertSpace( SpaceCreator{ spaces , primalSubSpaceIds , dualSubSpaceIds } , ScalarProduct{} );
    }
  }
}
