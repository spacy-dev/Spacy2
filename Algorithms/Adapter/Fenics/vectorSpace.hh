#ifndef ALGORITHMS_ADAPTER_FENICS_VECTOR_SPACE_HH
#define ALGORITHMS_ADAPTER_FENICS_VECTOR_SPACE_HH

#include <map>
#include <memory>

#include <dolfin.h>

#include "Interface/abstractVector.hh"


#include "Interface/abstractVectorSpace.hh"
#include "../../vectorSpace.hh"
#include "Util/create.hh"

#include "Util/Mixins/impl.hh"
#include "FunctionSpaces/ProductSpace/productSpace.hh"

namespace Algorithm
{
  namespace Interface
  {
    class AbstractVector;
  }

  namespace Fenics
  {
    class VectorSpace : public Interface::AbstractVectorSpace , public Mixin::Impl<dolfin::FunctionSpace>
    {
    public:
      explicit VectorSpace(const dolfin::FunctionSpace& space);

      VectorSpace(const dolfin::FunctionSpace& space, const std::unordered_map<size_t,size_t>& dofmap);

      size_t dofmap(size_t i) const;

      size_t inverseDofmap(size_t i) const;

    private:
      std::unique_ptr<Interface::AbstractVector> elementImpl() const;
      std::unordered_map<size_t,size_t> dofmap_;
      std::vector<size_t> inverseDofmap_;
    };

    template <class FenicsSpace>
    auto makeFunctionSpace(const FenicsSpace& space)
    {
      return createFromSharedImpl< ::Algorithm::VectorSpace, Fenics::VectorSpace >(space);
    }

    ::Algorithm::VectorSpace makeProductSpace(const dolfin::FunctionSpace& space, const std::vector<unsigned>& primalIds, const std::vector<unsigned>& dualIds);
  }
}

#endif // ALGORITHMS_ADAPTER_FENICS_VECTOR_SPACE_HH
