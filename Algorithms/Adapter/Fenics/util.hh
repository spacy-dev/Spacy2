#ifndef ALGORITHM_ADAPTER_FENICS_UTIL_HH
#define ALGORITHM_ADAPTER_FENICS_UTIL_HH

namespace dolfin
{
  class Form;
}

namespace Algorithm
{
  namespace Fenics
  {
    void copyCoefficients(const dolfin::Form& F, dolfin::Form& G);
  }
}

#endif // ALGORITHM_ADAPTER_FENICS_UTIL_HH

