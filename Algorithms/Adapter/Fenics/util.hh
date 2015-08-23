#ifndef ALGORITHM_ADAPTER_FENICS_UTIL_HH
#define ALGORITHM_ADAPTER_FENICS_UTIL_HH

#include "../../vector.hh"

namespace dolfin
{
  class Form;
  class GenericVector;
  class Function;
}

namespace Algorithm
{
  namespace Fenics
  {
    void copyCoefficients(const dolfin::Form& F, dolfin::Form& G);

    void copy(const ::Algorithm::Vector& x, dolfin::GenericVector& y);

    void copy(const ::Algorithm::Vector& x, dolfin::Function& y);

    void copy(const dolfin::GenericVector& y, ::Algorithm::Vector& x);
  }
}

#endif // ALGORITHM_ADAPTER_FENICS_UTIL_HH

