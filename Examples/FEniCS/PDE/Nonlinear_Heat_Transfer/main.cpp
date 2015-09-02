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

// Sub domain for Dirichlet boundary condition
class DirichletBoundary : public SubDomain
{
  bool inside(const Array<double>& x, bool on_boundary) const
  {
    return x[0] < DOLFIN_EPS or x[0] > 1.0 - DOLFIN_EPS or x[1] < DOLFIN_EPS or x[1] > 1. - DOLFIN_EPS;
  }
};

int main()
{
  // Create mesh and function space
  auto n = 256u;
  UnitSquareMesh mesh{n,n};
  NonlinearHeat::FunctionSpace V{mesh};
  std::cout << "degrees of freedom: " << V.dim() << std::endl;

  // Define boundary condition
  Constant u0{0.};
  DirichletBoundary boundary;
  DirichletBC bc{V, u0, boundary};
  std::vector<const DirichletBC*> bcs { &bc };

   // Define variational forms
  NonlinearHeat::BilinearForm a{V, V};
  NonlinearHeat::LinearForm L{V};
  
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
  auto A = FEniCS::makeC1Operator( L , a , bcs , domain , range );
  // set scalar product for affine covariant newton method
  domain.setScalarProduct( InducedScalarProduct( A.linearization(domain.vector()) ) );

  // specify parameters for Newton's method
  auto p = Spacy::Newton::Parameter{};
  p.setVerbosity(true);
  p.setRelativeAccuracy(1e-12);

  // solve A(x) = 0
  auto x = Spacy::covariantNewton(A,p);
//  auto sol = Spacy::contravariantNewton(A,p);
//  auto sol = Spacy::localNewton(A,p);
  
  FEniCS::copy(x,u);

  // Save solution in VTK format
  File file("poisson.pvd");
  file << u;

  // Plot solution
  plot(u);
  interactive();

  return 0;
}
