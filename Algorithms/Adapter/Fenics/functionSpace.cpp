#include "functionSpace.hh"

#include "scalarProducts.hh"
#include "vector.hh"

#include "Interface/abstractFunctionSpace.hh"
#include "Util/Exceptions/invalidArgumentException.hh"

namespace Algorithm
{
  namespace Fenics
  {
    FunctionSpace::FunctionSpace(const dolfin::FunctionSpace& space)
      : Interface::AbstractFunctionSpace(std::make_shared<l2ScalarProduct>()),
        Mixin::Impl<dolfin::FunctionSpace>(space)
    {}

    FunctionSpace::FunctionSpace(const dolfin::FunctionSpace& space, const std::unordered_map<size_t, size_t>& dofmap)
      : Interface::AbstractFunctionSpace(std::make_shared<l2ScalarProduct>()),
        Mixin::Impl<dolfin::FunctionSpace>(space),
        dofmap_(dofmap),
        inverseDofmap_(dofmap_.size())
    {
      auto mend = end(dofmap_);
      for( auto m = begin(dofmap_) ; m!=mend ; ++m )
        inverseDofmap_[m->first] = m->second;
    }

    size_t FunctionSpace::dofmap(size_t i) const
    {

      auto j = dofmap_.find(i);
      if( j == end(dofmap_) ) throw std::runtime_error("dofmap");
      return j->second;
    }

    size_t FunctionSpace::inverseDofmap(size_t i) const
    {
      if(inverseDofmap_.empty()) throw std::runtime_error("inverseDofmap");
      return inverseDofmap_[i];
    }


    std::unique_ptr<Interface::AbstractFunctionSpaceElement> FunctionSpace::elementImpl() const
    {
      return std::make_unique< Vector >(*this);
    }

    ::Algorithm::FunctionSpace makeProductSpace(const dolfin::FunctionSpace& space, const std::vector<unsigned>& primalIds, const std::vector<unsigned>& dualIds)
    {
      unsigned maxPrimalId = primalIds.empty() ? 0 : *std::max_element(begin(primalIds),end(primalIds));
      unsigned maxDualId   = dualIds.empty()   ? 0 : *std::max_element(begin(dualIds),end(dualIds));
      std::vector<std::shared_ptr<Interface::AbstractFunctionSpace> > spaces( 1 + std::max( maxPrimalId , maxDualId ) );
      for(size_t i : primalIds)
      {
        std::unordered_map<std::size_t,std::size_t> dofmap;
        auto subSpace = space[i]->collapse(dofmap);
        spaces[i] =  std::make_shared<Fenics::FunctionSpace>(*subSpace,dofmap);
      }
      for(size_t i : dualIds)
      {
        std::unordered_map<std::size_t,std::size_t> dofmap;
        auto subSpace = space[i]->collapse(dofmap);
        spaces[i] =  std::make_shared<Fenics::FunctionSpace>(*subSpace,dofmap);
      }

      return createFromSharedImpl< ::Algorithm::FunctionSpace, ProductSpace>( spaces , primalIds , dualIds );
    }
//    BanachSpace makeProductSpace(const dolfin::FunctionSpace& space, const std::vector<unsigned>& primalIds, const std::vector<unsigned>& dualIds)
//    {
//      unsigned maxPrimalId = primalIds.empty() ? 0 : *std::max_element(begin(primalIds),end(primalIds));
//      unsigned maxDualId   = dualIds.empty()   ? 0 : *std::max_element(begin(dualIds),end(dualIds));
//      std::vector<std::shared_ptr<Interface::AbstractFunctionSpace> > spaces( 1 + std::max( maxPrimalId , maxDualId ) );
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
