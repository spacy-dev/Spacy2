#include "util.hh"

#include <dolfin.h>

void Algorithm::Fenics::copyCoefficients(const dolfin::Form& F, dolfin::Form& G)
{
  for(std::size_t i=0; i<F.num_coefficients(); ++i) G.set_coefficient( i , F.coefficient(i) );
}
