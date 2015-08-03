#include "hilbertSpace.hh"

#include "scalarProducts.hh"
#include "vector.hh"

#include "Interface/abstractHilbertSpace.hh"
#include "Util/Exceptions/invalidArgumentException.hh"

namespace Algorithm
{
  namespace Fenics
  {
    HilbertSpace::HilbertSpace(const dolfin::FunctionSpace& space)
      : Interface::AbstractHilbertSpace(std::make_shared<l2ScalarProduct>()),
        Mixin::Impl<dolfin::FunctionSpace>(space)
    {}

    HilbertSpace::HilbertSpace(const dolfin::FunctionSpace& space, const std::unordered_map<size_t, size_t>& dofmap)
      : Interface::AbstractHilbertSpace(std::make_shared<l2ScalarProduct>()),
        Mixin::Impl<dolfin::FunctionSpace>(space),
        dofmap_(dofmap),
        inverseDofmap_(dofmap_.size())
    {
      auto mend = end(dofmap_);
      for( auto m = begin(dofmap_) ; m!=mend ; ++m )
        inverseDofmap_[m->first] = m->second;
    }

    size_t HilbertSpace::dofmap(size_t i) const
    {

      auto j = dofmap_.find(i);
      if( j == end(dofmap_) ) throw std::runtime_error("dofmap");
      return j->second;
    }

    size_t HilbertSpace::inverseDofmap(size_t i) const
    {
      if(inverseDofmap_.empty()) throw std::runtime_error("inverseDofmap");
      return inverseDofmap_[i];
    }


    std::unique_ptr<Interface::AbstractFunctionSpaceElement> HilbertSpace::elementImpl() const
    {
      return std::make_unique< Vector >(*this);
    }

    bool isHilbertSpace(const Interface::AbstractBanachSpace& space)
    {
      return dynamic_cast<const Interface::AbstractBanachSpace*>(&space) != nullptr;
    }

    const HilbertSpace& toHilbertSpace(const Interface::AbstractBanachSpace& space)
    {
      if( !Fenics::isHilbertSpace(space) ) throw InvalidArgumentException("toHilbertSpace");
      return dynamic_cast<const HilbertSpace&>(space);
    }

    ::Algorithm::HilbertSpace makeProductSpace(const dolfin::FunctionSpace& space, const std::vector<unsigned>& primalIds, const std::vector<unsigned>& dualIds)
    {
      unsigned maxPrimalId = primalIds.empty() ? 0 : *std::max_element(begin(primalIds),end(primalIds));
      unsigned maxDualId   = dualIds.empty()   ? 0 : *std::max_element(begin(dualIds),end(dualIds));
      std::vector<std::shared_ptr<Interface::AbstractBanachSpace> > spaces( 1 + std::max( maxPrimalId , maxDualId ) );
      for(size_t i : primalIds)
      {
        std::unordered_map<std::size_t,std::size_t> dofmap;
        auto subSpace = space[i]->collapse(dofmap);
        spaces[i] =  std::make_shared<Fenics::HilbertSpace>(*subSpace,dofmap);
      }
      for(size_t i : dualIds)
      {
        std::unordered_map<std::size_t,std::size_t> dofmap;
        auto subSpace = space[i]->collapse(dofmap);
        spaces[i] =  std::make_shared<Fenics::HilbertSpace>(*subSpace,dofmap);
      }

      return createFromSharedImpl< ::Algorithm::HilbertSpace, ProductSpace>( spaces , primalIds , dualIds );
    }
//    BanachSpace makeProductSpace(const dolfin::FunctionSpace& space, const std::vector<unsigned>& primalIds, const std::vector<unsigned>& dualIds)
//    {
//      unsigned maxPrimalId = primalIds.empty() ? 0 : *std::max_element(begin(primalIds),end(primalIds));
//      unsigned maxDualId   = dualIds.empty()   ? 0 : *std::max_element(begin(dualIds),end(dualIds));
//      std::vector<std::shared_ptr<Interface::AbstractBanachSpace> > spaces( 1 + std::max( maxPrimalId , maxDualId ) );
//      std::vector< std::unordered_map<std::size_t,std::size_t> > dofmaps(spaces.size());
//      for(size_t i : primalIds)
//      {
//        auto subSpace = space[i]->collapse(dofmaps[i]);
//        spaces[i] =  std::make_shared<HilbertSpace>(*subSpace,dofmaps[i]);
//      }
//      for(size_t i : dualIds)
//      {
//        auto subSpace = space[i]->collapse(dofmaps[i]);
//        spaces[i] =  std::make_shared<HilbertSpace>(*subSpace,dofmaps[i]);
//      }

//      return create_sharedImpl< ::Algorithm::BanachSpace, ProductSpace>( spaces , primalIds , dualIds );
//    }
  }
}
