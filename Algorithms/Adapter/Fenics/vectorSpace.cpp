#include "vectorSpace.hh"

#include "../../vector.hh"
#include "../../vectorSpace.hh"
#include "vector.hh"
#include "FunctionSpaces/ProductSpace/productSpaceProduct.hh"

#include "Util/Exceptions/invalidArgumentException.hh"

namespace Algorithm
{
  namespace Fenics
  {
    VectorSpace::VectorSpace(const dolfin::FunctionSpace& space)
      : Mixin::Impl<dolfin::FunctionSpace>(space)
    {}

    VectorSpace::VectorSpace(const dolfin::FunctionSpace& space, const std::unordered_map<size_t, size_t>& dofmap)
      : Mixin::Impl<dolfin::FunctionSpace>(space),
        dofmap_(dofmap),
        inverseDofmap_(dofmap_.size())
    {
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


    ::Algorithm::Vector VectorSpace::element(const ::Algorithm::VectorSpace* space) const
    {
      return ::Algorithm::Vector( std::make_unique< Vector >(*space) );
    }

    ::Algorithm::VectorSpace makeProductSpace(const dolfin::FunctionSpace& space, const std::vector<unsigned>& primalIds, const std::vector<unsigned>& dualIds)
    {
      unsigned maxPrimalId = primalIds.empty() ? 0 : *std::max_element(begin(primalIds),end(primalIds));
      unsigned maxDualId   = dualIds.empty()   ? 0 : *std::max_element(begin(dualIds),end(dualIds));
      std::vector<std::shared_ptr< ::Algorithm::VectorSpace > > spaces( 1 + std::max( maxPrimalId , maxDualId ) );
      for(size_t i : primalIds)
      {
        std::unordered_map<std::size_t,std::size_t> dofmap;
        auto subSpace = space[i]->collapse(dofmap);
        spaces[i] =  std::make_shared< ::Algorithm::VectorSpace >(Fenics::VectorSpace(*subSpace,dofmap), std::make_shared<Fenics::l2ScalarProduct>());
      }
      for(size_t i : dualIds)
      {
        std::unordered_map<std::size_t,std::size_t> dofmap;
        auto subSpace = space[i]->collapse(dofmap);
        spaces[i] =  std::make_shared< ::Algorithm::VectorSpace >(Fenics::VectorSpace(*subSpace,dofmap), std::make_shared<Fenics::l2ScalarProduct>());
      }

      VectorSpaceImpl tmp = ProductSpace( spaces , primalIds , dualIds );
      return ::Algorithm::VectorSpace( tmp , std::make_shared<ProductSpaceProduct>() );
    }
//    BanachSpace makeProductSpace(const dolfin::FunctionSpace& space, const std::vector<unsigned>& primalIds, const std::vector<unsigned>& dualIds)
//    {
//      unsigned maxPrimalId = primalIds.empty() ? 0 : *std::max_element(begin(primalIds),end(primalIds));
//      unsigned maxDualId   = dualIds.empty()   ? 0 : *std::max_element(begin(dualIds),end(dualIds));
//      std::vector<std::shared_ptr<Interface::AbstractVectorSpace> > spaces( 1 + std::max( maxPrimalId , maxDualId ) );
//      std::vector< std::unordered_map<std::size_t,std::size_t> > dofmaps(spaces.size());
//      for(size_t i : primalIds)
//      {
//        auto subSpace = space[i]->collapse(dofmaps[i]);
//        spaces[i] =  std::make_shared<FunctionSpace>(*subSpace,dofmaps[i]);
//      }
//      for(size_t i : dualIds)
//      {
//        auto subSpace = space[i]->collapse(dofmaps[i]);
//        spaces[i] =  std::make_shared<FunctionSpace>(*subSpace,dofmaps[i]);
//      }

//      return create_sharedImpl< ::Algorithm::BanachSpace, ProductSpace>( spaces , primalIds , dualIds );
//    }
  }
}
