#include "vectorSpace.hh"

#include "../../vector.hh"
#include "../../vectorSpace.hh"
#include "vector.hh"
#include "FunctionSpaces/ProductSpace/vectorSpace.hh"

#include "Util/Exceptions/invalidArgumentException.hh"

namespace Algorithm
{
  namespace Fenics
  {
    VectorSpace::VectorSpace(const dolfin::FunctionSpace& space)
      : Mixin::Impl<dolfin::FunctionSpace>(space)
    {}

    VectorSpace::VectorSpace(const dolfin::FunctionSpace& space, const std::unordered_map<size_t, size_t>& dofmap)
      : VectorSpace(space)
    {
      dofmap_ = dofmap;
      inverseDofmap_ = std::vector<size_t>(dofmap_.size());

      auto mend = end(dofmap_);
      for( auto m = begin(dofmap_) ; m!=mend ; ++m )
        inverseDofmap_[m->first] = m->second;
    }

    size_t VectorSpace::dofmap(size_t i) const
    {

      auto j = dofmap_.find(i);
      if( j == end(dofmap_) ) throw std::runtime_error("dofmap");
      return j->second;
    }

    size_t VectorSpace::inverseDofmap(size_t i) const
    {
      if(inverseDofmap_.empty()) throw std::runtime_error("inverseDofmap");
      return inverseDofmap_[i];
    }


    ::Algorithm::Vector VectorSpace::operator()(const ::Algorithm::VectorSpace* space) const
    {
      return Vector{*space};
    }

    ::Algorithm::VectorSpace makeHilbertSpace(const dolfin::FunctionSpace& space, const std::vector<unsigned>& primalIds, const std::vector<unsigned>& dualIds)
    {
      unsigned maxPrimalId = primalIds.empty() ? 0 : *std::max_element(begin(primalIds),end(primalIds));
      unsigned maxDualId   = dualIds.empty()   ? 0 : *std::max_element(begin(dualIds),end(dualIds));
      std::vector<std::shared_ptr< ::Algorithm::VectorSpace > > spaces( 1 + std::max( maxPrimalId , maxDualId ) );
      for(size_t i : primalIds)
      {
        std::unordered_map<std::size_t,std::size_t> dofmap;
        auto subSpace = space[i]->collapse(dofmap);
        spaces[i] =  std::make_shared< ::Algorithm::VectorSpace >( makeHilbertSpace( *subSpace,dofmap) );
      }
      for(size_t i : dualIds)
      {
        std::unordered_map<std::size_t,std::size_t> dofmap;
        auto subSpace = space[i]->collapse(dofmap);
        spaces[i] =  std::make_shared< ::Algorithm::VectorSpace >( makeHilbertSpace( *subSpace,dofmap ) );
      }

      return ProductSpace::makeHilbertSpace( spaces , primalIds , dualIds );
    }
  }
}
