#include <dolfin.h>

#include <Spacy/Adapter/fenics.hh>
#include <Spacy/Algorithm/Newton/newton.hh>
#include <Spacy/inducedScalarProduct.hh>

#include "NonlinearHeat.h"

using namespace dolfin;

// Source term (right-hand side)
class Source : public Expression
{
  void eval(Array<double>& values, const Array<double>& x) const
  {
    double dx = x[0] - 0.5;
    double dy = x[1] - 0.5;
    values[0] = 1;
  }
};

int main()
{
  // Create mesh and function space
  int n = 256;
  UnitSquareMesh mesh(n,n);
  NonlinearHeat::FunctionSpace V(mesh);

  // Define variational forms
  NonlinearHeat::BilinearForm a(V, V);
  NonlinearHeat::LinearForm L(V);

  Constant c(1e-2), d(1e2);
  Source f;
  Function u(V);
  L.f = f;
  L.c = c;
  L.d = d;
  a.c = c;
  a.d = d;
  
  
  // Compute solution
  using namespace Spacy;

  // create spaces
  auto domain = FEniCS::makeHilbertSpace(V);
  auto range = FEniCS::makeHilbertSpace(V);
  connect(domain,range);
  
  // create operator
  auto A = FEniCS::makeC1Operator( L , a , domain , range );
  // set scalar product for affine covariant newton method
  domain.setScalarProduct( InducedScalarProduct( A.linearization(domain.vector()) ) );

  // specify parameters for Newton's method
  auto p = Newton::Parameter{};
  p.setVerbosity(true);
  p.setRelativeAccuracy(1e-12);

  // solve A(x)=0
  auto x = covariantNewton(A,p);
//  auto x = contravariantNewton(A,p);
//  auto x = localNewton(A,p);
  
  FEniCS::copy(x,u);

  
  // Save solution in VTK format
  File file("poisson.pvd");
  file << u;

  // Plot solution
  plot(u);
  interactive();

  return 0;
}
