#ifndef ALGORITHMS_ADAPTER_FENICS_FunctionSpace_HH
#define ALGORITHMS_ADAPTER_FENICS_FunctionSpace_HH

#include <map>
#include <memory>

#include <dolfin.h>

#include "Interface/abstractFunctionSpaceElement.hh"


#include "../../functionSpace.hh"
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
    class FunctionSpace : public Interface::AbstractFunctionSpace , public Mixin::Impl<dolfin::FunctionSpace>
    {
    public:
      explicit FunctionSpace(const dolfin::FunctionSpace& space);

      FunctionSpace(const dolfin::FunctionSpace& space, const std::unordered_map<size_t,size_t>& dofmap);

      size_t dofmap(size_t i) const;

      size_t inverseDofmap(size_t i) const;

    private:
      std::unique_ptr<Interface::AbstractFunctionSpaceElement> elementImpl() const;
      std::unordered_map<size_t,size_t> dofmap_;
      std::vector<size_t> inverseDofmap_;
    };

    template <class FenicsSpace>
    auto makeFunctionSpace(const FenicsSpace& space)
    {
      return createFromSharedImpl< ::Algorithm::FunctionSpace, Fenics::FunctionSpace >(space);
    }

    ::Algorithm::FunctionSpace makeProductSpace(const dolfin::FunctionSpace& space, const std::vector<unsigned>& primalIds, const std::vector<unsigned>& dualIds);
  }
}

#endif // ALGORITHMS_ADAPTER_FENICS_FunctionSpace_HH
