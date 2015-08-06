#ifndef ALGORITHMS_ADAPTER_FENICS_HILBERTSPACE_HH
#define ALGORITHMS_ADAPTER_FENICS_HILBERTSPACE_HH

#include <map>
#include <memory>

#include <dolfin.h>

#include "Interface/abstractFunctionSpaceElement.hh"


#include "../../hilbertSpace.hh"
#include "Util/create.hh"

#include "Util/Mixins/impl.hh"
#include "FunctionSpaces/ProductSpace/productSpace.hh"

namespace Algorithm
{
  namespace Interface
  {
    class AbstractFunctionSpaceElement;
  }

  namespace Fenics
  {
    class HilbertSpace : public Interface::AbstractHilbertSpace , public Mixin::Impl<dolfin::FunctionSpace>
    {
    public:
      explicit HilbertSpace(const dolfin::FunctionSpace& space);

      HilbertSpace(const dolfin::FunctionSpace& space, const std::unordered_map<size_t,size_t>& dofmap);

      size_t dofmap(size_t i) const;

      size_t inverseDofmap(size_t i) const;

    private:
      std::unique_ptr<Interface::AbstractFunctionSpaceElement> elementImpl() const;
      std::unordered_map<size_t,size_t> dofmap_;
      std::vector<size_t> inverseDofmap_;
    };

    template <class FenicsSpace>
    auto makeHilbertSpace(const FenicsSpace& space)
    {
      return createFromSharedImpl< ::Algorithm::HilbertSpace, HilbertSpace >(space);
    }

    ::Algorithm::HilbertSpace makeProductSpace(const dolfin::FunctionSpace& space, const std::vector<unsigned>& primalIds, const std::vector<unsigned>& dualIds);

//    inline auto makeProductSpace(const dolfin::FunctionSpace& space,
//                                 const std::vector<unsigned>& primalIds, const std::vector<unsigned>& dualIds,
//                                 const dolfin::FunctionSpace& primalSubSpace, unsigned primalOffset = 0)
//    {
//      unsigned maxPrimalId = primalIds.empty() ? 0 : *std::max_element(begin(primalIds),end(primalIds));
//      unsigned maxDualId   = dualIds.empty()   ? 0 : *std::max_element(begin(dualIds),end(dualIds));
//      std::vector<std::shared_ptr<Interface::AbstractBanachSpace> > spaces( 1 + std::max( maxPrimalId , maxDualId ) );
//      for(size_t i : primalIds)
//      {
//        std::unordered_map<std::size_t,std::size_t> dofmap, primalDofMap;
//        auto subSpace = space[i]->collapse(dofmap);
//        auto psubSpace = primalSubSpace[i-primalOffset]->collapse(primalDofMap);
//        spaces[i] =  std::make_shared<HilbertSpace>(*subSpace,dofmap,primalDofMap);
//      }
//      for(size_t i : dualIds)
//      {
//        std::unordered_map<std::size_t,std::size_t> dofmap;
//        auto subSpace = space[i]->collapse(dofmap);
//        spaces[i] =  std::make_shared<HilbertSpace>(*subSpace,dofmap);
//      }

//      return createFromSharedImpl< ::Algorithm::HilbertSpace, ProductSpace>( spaces , primalIds , dualIds );
//    }
  }
}

#endif // ALGORITHMS_ADAPTER_FENICS_HILBERTSPACE_HH
