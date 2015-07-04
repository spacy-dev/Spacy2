#ifndef ALGORITHM_ADAPTER_FENICS_SCALARPRODUCTS
#define ALGORITHM_ADAPTER_FENICS_SCALARPRODUCTS

#include "Interface/abstractScalarProduct.hh"

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
      double operator()(const Interface::AbstractFunctionSpaceElement& x, const Interface::AbstractFunctionSpaceElement& y) const final override;
    };

    class EnergyScalarProduct : public Interface::AbstractScalarProduct
    {
    public:
      explicit EnergyScalarProduct(const dolfin::GenericMatrix& A);
      double operator()(const Interface::AbstractFunctionSpaceElement& x, const Interface::AbstractFunctionSpaceElement& y) const final override;

    private:
      const dolfin::GenericMatrix& A_;
    };
  }
}

#endif // ALGORITHM_ADAPTER_FENICS_SCALARPRODUCTS

