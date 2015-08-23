#ifndef ALGORITHMS_ADAPTER_FENICS_VECTOR_SPACE_HH
#define ALGORITHMS_ADAPTER_FENICS_VECTOR_SPACE_HH

#include <map>
#include <memory>

#include <dolfin.h>

#include "Util/Mixins/impl.hh"
#include "FunctionSpaces/ProductSpace/productSpace.hh"
#include "scalarProducts.hh"

namespace Algorithm
{
  class VectorSpace;

  namespace Fenics
  {
    class VectorSpace : public Mixin::Impl<dolfin::FunctionSpace>
    {
    public:
      explicit VectorSpace(const dolfin::FunctionSpace& space);

      VectorSpace(const dolfin::FunctionSpace& space, const std::unordered_map<size_t,size_t>& dofmap);

      size_t dofmap(size_t i) const;

      size_t inverseDofmap(size_t i) const;

      ::Algorithm::Vector element(const ::Algorithm::VectorSpace* space) const;

    private:
      std::unordered_map<size_t,size_t> dofmap_;
      std::vector<size_t> inverseDofmap_;
    };

    template <class FenicsSpace>
    auto makeHilbertSpace(const FenicsSpace& space)
    {
      return ::Algorithm::makeHilbertSpace( Fenics::VectorSpace{space} , l2Product{} );
    }

    template <class FenicsSpace>
    auto makeHilbertSpace(const FenicsSpace& space, const std::unordered_map<size_t,size_t>& dofmap)
    {
      return ::Algorithm::makeHilbertSpace( Fenics::VectorSpace{space,dofmap} , l2Product{} );
    }

    ::Algorithm::VectorSpace makeProductSpace(const dolfin::FunctionSpace& space, const std::vector<unsigned>& primalIds, const std::vector<unsigned>& dualIds);
  }
}

#endif // ALGORITHMS_ADAPTER_FENICS_VECTOR_SPACE_HH
