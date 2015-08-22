#ifndef ALGORITHM_ADAPTER_FENICS_SCALARPRODUCTS
#define ALGORITHM_ADAPTER_FENICS_SCALARPRODUCTS

#include "../../vector.hh"

namespace Algorithm
{
  namespace Fenics
  {
    class l2Product
    {
    public:
      double operator()(const ::Algorithm::Vector& x, const ::Algorithm::Vector& y) const;
    };
  }
}

#endif // ALGORITHM_ADAPTER_FENICS_SCALARPRODUCTS

