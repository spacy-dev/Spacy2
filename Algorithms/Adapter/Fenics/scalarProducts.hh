#ifndef ALGORITHM_ADAPTER_FENICS_SCALARPRODUCTS
#define ALGORITHM_ADAPTER_FENICS_SCALARPRODUCTS

#include <vector>
#include <dolfin.h>

#include "Interface/abstractScalarProduct.hh"
#include "Util/create.hh"

#include "scalarProduct.hh"

namespace dolfin
{
  class GenericMatrix;
}

namespace Algorithm
{
  namespace Fenics
  {
    class l2ScalarProduct : public Interface::AbstractScalarProduct
    {
    public:
      double operator()(const Interface::AbstractVector& x, const Interface::AbstractVector& y) const final override;
    };

    class ScalarProduct : public Interface::AbstractScalarProduct
    {
    public:
      explicit ScalarProduct(std::shared_ptr<dolfin::GenericMatrix> A);
      double operator()(const Interface::AbstractVector& x, const Interface::AbstractVector& y) const final override;

    private:
      std::shared_ptr<dolfin::GenericMatrix> A_;
    };

    template <class FenicsForm>
    auto inducedScalarProduct(const FenicsForm& J, const std::vector<const dolfin::DirichletBC*>& bcs, std::shared_ptr<dolfin::GenericMatrix> A)
    {
      dolfin::assemble(*A,J);
      for( auto& bc : bcs) bc->apply(*A);
      return createFromSharedImpl< ::Algorithm::ScalarProduct , Fenics::ScalarProduct >( A );
    }
  }
}

#endif // ALGORITHM_ADAPTER_FENICS_SCALARPRODUCTS

