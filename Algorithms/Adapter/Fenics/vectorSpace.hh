#ifndef ALGORITHMS_ADAPTER_FENICS_VECTOR_SPACE_HH
#define ALGORITHMS_ADAPTER_FENICS_VECTOR_SPACE_HH

#include <map>
#include <memory>

#include <dolfin.h>

#include "Interface/abstractVector.hh"

#include "Util/Mixins/impl.hh"
#include "FunctionSpaces/ProductSpace/productSpace.hh"
#include "scalarProducts.hh"

namespace Algorithm
{
  class Vector;
  class VectorSpace;

  namespace Interface
  {
    class AbstractVector;
  }

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
    auto makeFunctionSpace(const FenicsSpace& space)
    {
      return ::Algorithm::VectorSpace{ Fenics::VectorSpace{space} , std::make_shared<l2ScalarProduct>() };
//      ::Algorithm::VectorSpace vs(space);
//      space.setScalarProduct( l2ScalarProduct() );
//      return space;
//      return createFromSharedImpl< ::Algorithm::VectorSpace, Fenics::VectorSpace >(space);
    }

    ::Algorithm::VectorSpace makeProductSpace(const dolfin::FunctionSpace& space, const std::vector<unsigned>& primalIds, const std::vector<unsigned>& dualIds);
  }
}

#endif // ALGORITHMS_ADAPTER_FENICS_VECTOR_SPACE_HH
