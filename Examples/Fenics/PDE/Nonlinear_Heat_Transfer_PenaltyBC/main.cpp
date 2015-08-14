#include <dolfin.h>
#include "NonlinearPoisson.h"

#include "Adapter/fenics.hh"
#include "Algorithm/Newton/newton.hh"
#include "Interface/inducedScalarProduct.hh"

using namespace dolfin;

// Source term (right-hand side)
class Source : public Expression
{
  void eval(Array<double>& values, const Array<double>& x) const
  {
    double dx = x[0] - 0.5;
    double dy = x[1] - 0.5;
    values[0] = 1;//10*exp(-(dx*dx + dy*dy) / 0.02);
  }
};

int main()
{
  // Create mesh and function space
  int n = 256;
  UnitSquareMesh mesh(n,n);
  NonlinearPoisson::FunctionSpace V(mesh);

  // Define variational forms
  NonlinearPoisson::BilinearForm a(V, V);
  NonlinearPoisson::LinearForm L(V);

  Constant c(1e-2), d(1e2);
  Source f;
  Function u(V);
  L.f = f;
  L.c = c;
  L.d = d;
  a.c = c;
  a.d = d;
  
  
  // Compute solution
  using namespace Algorithm;

  auto domain = Fenics::makeFunctionSpace(V);
  auto range = Fenics::makeFunctionSpace(V);
  connectPrimalDual(domain,range);
  
  auto A = Fenics::makeOperator( L , a , {} , domain , range );
  domain.setScalarProduct( inducedScalarProduct( A.linearization(domain.element()) ) );
  
  auto newton = covariantNewton( A );
//  auto newton = contravariantNewton( A );
//  auto newton = localNewton( A );
  newton.setVerbosity( true );
  newton.setRelativeAccuracy(1e-12);
    
  auto result = newton.solve();
  Fenics::copy(result,u);

  
  // Save solution in VTK format
  File file("poisson.pvd");
  file << u;

  // Plot solution
  plot(u);
  interactive();

  return 0;
}
