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
    class l2ScalarProduct : public AbstractScalarProduct
    {
    public:
      double operator()(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y) const final override;
    };

    class EnergyScalarProduct : public AbstractScalarProduct
    {
    public:
      explicit EnergyScalarProduct(const dolfin::GenericMatrix& A);
      double operator()(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y) const final override;

    private:
      const dolfin::GenericMatrix& A_;
    };
  }
}

#endif // ALGORITHM_ADAPTER_FENICS_SCALARPRODUCTS

