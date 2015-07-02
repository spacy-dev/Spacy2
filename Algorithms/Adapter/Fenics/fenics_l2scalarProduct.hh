#ifndef FENICS_L2SCALARPRODUCT
#define FENICS_L2SCALARPRODUCT

#include "Interface/abstractScalarProduct.hh"

namespace Algorithm
{
  class Fenics_DefaultScalarProduct : public AbstractScalarProduct
  {
    double operator()(const AbstractFunctionSpaceElement& x, const AbstractFunctionSpaceElement& y) const final override;
  };
}

#endif // FENICS_L2SCALARPRODUCT

