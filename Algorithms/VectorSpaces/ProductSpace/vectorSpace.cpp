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


    VectorCreator::VectorCreator(const std::vector<std::shared_ptr<VectorSpace> >& spaces,
                               const std::vector<unsigned>& primalSubSpaceIds,
                               const std::vector<unsigned>& dualSubSpaceIds)
      : primalSubSpaceIds_(primalSubSpaceIds),
        dualSubSpaceIds_(dualSubSpaceIds),
        isPrimalDual_(!dualSubSpaceIds_.empty())
    {
      for( auto i = 0u; i< primalSubSpaceIds_.size(); ++i )
        primalMap_[primalSubSpaceIds[i]] = i;
      for( auto i = 0u; i< dualSubSpaceIds_.size(); ++i )
        dualMap_[dualSubSpaceIds[i]] = i;

      spaces_ = std::vector<std::shared_ptr<VectorSpace> >(2,nullptr);
      spaces_[0] = std::make_shared<VectorSpace>( makeHilbertSpace( extractSubSpaces(spaces,primalSubSpaceIds_) ) );
      spaces_[1] = std::make_shared<VectorSpace>( makeHilbertSpace( extractSubSpaces(spaces,dualSubSpaceIds_) ) );
    }

    VectorCreator::VectorCreator(const std::vector<std::shared_ptr<VectorSpace> >& spaces)
      : spaces_(spaces),
        primalSubSpaceIds_(extractSpaceIds(spaces))
    {
      for(auto i=0u; i<primalSubSpaceIds_.size(); ++i)
        primalMap_[i] = i;
    }


    const std::vector<std::shared_ptr<VectorSpace> >& VectorCreator::subSpaces() const
    {
      return spaces_;
    }

    const VectorSpace& VectorCreator::subSpace(unsigned i) const
    {
      if( isPrimalDual() )
      {
        if( isPrimalSubSpaceId(i) ) return cast_ref<ProductSpace::VectorCreator>(spaces_[0]->impl()).subSpace(primalMap_.find(i)->second);
        if( isDualSubSpaceId(i) ) return cast_ref<ProductSpace::VectorCreator>(spaces_[1]->impl()).subSpace(dualMap_.find(i)->second);
      }
      return *spaces_[i];
    }

    VectorSpace* VectorCreator::subSpace_ptr(unsigned i) const
    {
      return sharedSubSpace(i).get();
    }

    std::shared_ptr<VectorSpace> VectorCreator::sharedSubSpace(unsigned i) const
    {
      if( isPrimalDual() )
      {
        if( isPrimalSubSpaceId(i) ) return cast_ref<ProductSpace::VectorCreator>(spaces_[0]->impl()).sharedSubSpace(primalMap_.find(i)->second);
        if( isDualSubSpaceId(i) ) return cast_ref<ProductSpace::VectorCreator>(spaces_[1]->impl()).sharedSubSpace(dualMap_.find(i)->second);
      }
      return spaces_[i];
    }


    ::Algorithm::Vector VectorCreator::operator()(const VectorSpace* space) const
    {
      return Vector{*space};
    }

    VectorSpace& VectorCreator::primalSubSpace()
    {
      assert( isPrimalDual() );
      return *spaces_[0];
    }

    const VectorSpace& VectorCreator::primalSubSpace() const
    {
      assert( isPrimalDual() );
      return *spaces_[0];
    }

    VectorSpace* VectorCreator::primalSubSpace_ptr() const
    {
      return sharedPrimalSubSpace().get();
    }

    VectorSpace& VectorCreator::dualSubSpace()
    {
      assert( isPrimalDual() );
      return *spaces_[1];
    }

    const VectorSpace& VectorCreator::dualSubSpace() const
    {
      assert( isPrimalDual() );
      return *spaces_[1];
    }

    VectorSpace* VectorCreator::dualSubSpace_ptr() const
    {
      return sharedDualSubSpace().get();
    }

    std::shared_ptr<VectorSpace> VectorCreator::sharedPrimalSubSpace() const
    {
      assert( isPrimalDual() );
      return spaces_[0];
    }

    std::shared_ptr<VectorSpace> VectorCreator::sharedDualSubSpace() const
    {
      assert( isPrimalDual() );
      return spaces_[1];
    }


    const std::vector<unsigned>& VectorCreator::primalSubSpaceIds() const
    {
      return primalSubSpaceIds_;
    }

    const std::vector<unsigned>& VectorCreator::dualSubSpaceIds() const
    {
      return dualSubSpaceIds_;
    }

    bool VectorCreator::isPrimalSubSpaceId(unsigned i) const
    {
      return primalMap_.find(i) != end(primalMap_);
    }

    bool VectorCreator::isDualSubSpaceId(unsigned i) const
    {
      return dualMap_.find(i) != end(dualMap_);
    }

    unsigned VectorCreator::primalIdMap(unsigned i) const
    {
      return primalMap_.find(i)->second;
    }

    unsigned VectorCreator::dualIdMap(unsigned i) const
    {
      return dualMap_.find(i)->second;
    }

    bool VectorCreator::isPrimalDual() const
    {
      return isPrimalDual_;
    }


    VectorSpace makeHilbertSpace(const std::vector<std::shared_ptr<VectorSpace> >& spaces)
    {
      return ::Algorithm::makeHilbertSpace( VectorCreator{ spaces } , ScalarProduct{} );
    }

    VectorSpace makeHilbertSpace(const std::vector<std::shared_ptr<VectorSpace> >& spaces,
                                  const std::vector<unsigned>& primalSubSpaceIds,
                                  const std::vector<unsigned>& dualSubSpaceIds)
    {
      return ::Algorithm::makeHilbertSpace( VectorCreator{ spaces , primalSubSpaceIds , dualSubSpaceIds } , ScalarProduct{} );
    }
  }
}
