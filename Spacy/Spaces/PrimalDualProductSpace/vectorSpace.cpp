#include "vectorSpace.hh"

#include "Spacy/Util/Exceptions/invalidArgumentException.hh"
#include "Spacy/Util/cast.hh"

#include "vector.hh"
#include "scalarProduct.hh"
#include "Spacy/Spaces/ProductSpace/vectorSpace.hh"

#include "Spacy/vectorSpace.hh"

#include <algorithm>

namespace Spacy
{
  namespace PrimalDualProductSpace
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
        dualSubSpaceIds_(dualSubSpaceIds)
    {
      for( auto i = 0u; i< primalSubSpaceIds_.size(); ++i )
        primalMap_[primalSubSpaceIds[i]] = i;
      for( auto i = 0u; i< dualSubSpaceIds_.size(); ++i )
        dualMap_[dualSubSpaceIds[i]] = i;

      spaces_ = std::vector<std::shared_ptr<VectorSpace> >(2,nullptr);
      spaces_[0] = std::make_shared<VectorSpace>( ProductSpace::makeHilbertSpace( extractSubSpaces(spaces,primalSubSpaceIds_) ) );
      spaces_[1] = std::make_shared<VectorSpace>( ProductSpace::makeHilbertSpace( extractSubSpaces(spaces,dualSubSpaceIds_) ) );
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

    const VectorSpace& VectorCreator::subSpace(unsigned k) const
    {
      if( isPrimalSubSpaceId(k) ) return cast_ref<ProductSpace::VectorCreator>(spaces_[0]->impl()).subSpace(primalMap_.find(k)->second);
      if( isDualSubSpaceId(k) ) return cast_ref<ProductSpace::VectorCreator>(spaces_[1]->impl()).subSpace(dualMap_.find(k)->second);
      return *spaces_[k];
    }

    VectorSpace& VectorCreator::subSpace(unsigned k)
    {
      if( isPrimalSubSpaceId(k) ) return cast_ref<ProductSpace::VectorCreator>(spaces_[0]->impl()).subSpace(primalMap_.find(k)->second);
      if( isDualSubSpaceId(k) ) return cast_ref<ProductSpace::VectorCreator>(spaces_[1]->impl()).subSpace(dualMap_.find(k)->second);
      return *spaces_[k];
    }

    Vector VectorCreator::operator()(const VectorSpace* space) const
    {
      return Vector{*space};
    }

    VectorSpace& VectorCreator::primalSubSpace()
    {
      return *spaces_[0];
    }

    const VectorSpace& VectorCreator::primalSubSpace() const
    {
      return *spaces_[0];
    }

    VectorSpace& VectorCreator::dualSubSpace()
    {
      return *spaces_[1];
    }

    const VectorSpace& VectorCreator::dualSubSpace() const
    {
      return *spaces_[1];
    }

    const std::vector<unsigned>& VectorCreator::primalSubSpaceIds() const
    {
      return primalSubSpaceIds_;
    }

    const std::vector<unsigned>& VectorCreator::dualSubSpaceIds() const
    {
      return dualSubSpaceIds_;
    }

    bool VectorCreator::isPrimalSubSpaceId(unsigned k) const
    {
      return primalMap_.find(k) != end(primalMap_);
    }

    bool VectorCreator::isDualSubSpaceId(unsigned k) const
    {
      return dualMap_.find(k) != end(dualMap_);
    }

    unsigned VectorCreator::primalIdMap(unsigned k) const
    {
      return primalMap_.find(k)->second;
    }

    unsigned VectorCreator::dualIdMap(unsigned k) const
    {
      return dualMap_.find(k)->second;
    }


    VectorSpace makeHilbertSpace(const std::vector<std::shared_ptr<VectorSpace> >& spaces)
    {
      return ::Spacy::makeHilbertSpace( VectorCreator{ spaces } , ScalarProduct{} );
    }

    VectorSpace makeHilbertSpace(const std::vector<std::shared_ptr<VectorSpace> >& spaces,
                                  const std::vector<unsigned>& primalSubSpaceIds,
                                  const std::vector<unsigned>& dualSubSpaceIds)
    {
      return ::Spacy::makeHilbertSpace( VectorCreator{ spaces , primalSubSpaceIds , dualSubSpaceIds } , ScalarProduct{} );
    }
  }
}
