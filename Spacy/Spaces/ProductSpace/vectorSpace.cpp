#include "vectorSpace.hh"

#include "vector.hh"
#include "scalarProduct.hh"

#include "Spacy/vectorSpace.hh"

namespace Spacy
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
    }

    VectorCreator::VectorCreator(const std::vector<std::shared_ptr<VectorSpace> >& spaces)
      : spaces_(spaces)
    {}


    const std::vector<std::shared_ptr<VectorSpace> >& VectorCreator::subSpaces() const
    {
      return spaces_;
    }

    VectorSpace& VectorCreator::subSpace(unsigned k)
    {
      return *spaces_[k];
    }

    const VectorSpace& VectorCreator::subSpace(unsigned k) const
    {
      return *spaces_[k];
    }


    unsigned VectorCreator::idMap(unsigned k) const
    {
      return idMap_.find(k)->second;
    }

    void VectorCreator::setIdMap(const std::map<unsigned,unsigned>& map)
    {
      idMap_ = map;
    }

    bool VectorCreator::hasId(unsigned k) const
    {
      return idMap_.find(k) != end(idMap_);
    }


    Vector VectorCreator::operator()(const VectorSpace* space) const
    {
      return Vector{*space};
    }


    VectorSpace makeHilbertSpace(const std::vector<std::shared_ptr<VectorSpace> >& spaces)
    {
      return ::Spacy::makeHilbertSpace( VectorCreator{ spaces } , ScalarProduct{} );
    }
  }
}
