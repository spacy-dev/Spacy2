#ifndef ALGORITHM_ADAPTER_FENICS_L2SCALARPRODUCT
#define ALGORITHM_ADAPTER_FENICS_L2SCALARPRODUCT

#include "Interface/abstractScalarProduct.hh"

namespace dolfin
{
  class GenericMatrix;
}

namespace Algorithm
{
  class Fenics_DefaultScalarProduct : public AbstractScalarProduct
  {
  public:
    double operator()(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y) const final override;
  };

  class Fenics_EnergyScalarProduct : public AbstractScalarProduct
  {
  public:
    explicit Fenics_EnergyScalarProduct(const dolfin::GenericMatrix& A);
    double operator()(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y) const final override;

  private:
    const dolfin::GenericMatrix& A_;
  };
}

#endif // ALGORITHM_ADAPTER_FENICS_L2SCALARPRODUCT

