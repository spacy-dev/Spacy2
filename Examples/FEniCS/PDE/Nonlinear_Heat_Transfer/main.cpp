#include <dolfin.h>

#include <Spacy/zeroVectorCreator.hh>
#include <Spacy/Adapter/fenics.hh>
#include <Spacy/Algorithm/Newton/newton.hh>
#include <Spacy/inducedScalarProduct.hh>
#include <Spacy/c1Operator.hh>

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
  auto mesh = std::make_shared<UnitSquareMesh>(n,n);
  auto V = std::make_shared<NonlinearHeat::FunctionSpace>(mesh);
  std::cout << "degrees of freedom: " << V->dim() << std::endl;

  // Define boundary condition
  auto u0 = std::make_shared<Constant>(0);
  auto boundary = std::make_shared<DirichletBoundary>();
  DirichletBC bc{V, u0, boundary};
  std::vector<const DirichletBC*> bcs { &bc };

   // Define variational forms
  NonlinearHeat::BilinearForm a{V, V};
  NonlinearHeat::LinearForm L{V};
  
  auto c = std::make_shared<Constant>(1e-1);
  auto d = std::make_shared<Constant>(1e2);
  auto f = std::make_shared<Source>();
  auto u = std::make_shared<Function>(V);
  L.f = f;
  L.c = c;
  L.d = d;
  a.c = c;
  a.d = d;
  
  
  // Compute solution
  using namespace Spacy;

  // create spaces
  auto domain = FEniCS::makeHilbertSpace(V);
  
  // create operator
  Spacy::C1Operator A = FEniCS::makeC1Operator( L , a , bcs , domain , domain.dualSpace() );
  // set scalar product for affine covariant newton method
  domain.setScalarProduct( InducedScalarProduct( A.linearization(zero(domain)) ) );

  // specify parameters for Newton's method
  auto p = Spacy::Newton::Parameter{};
  p.setRelativeAccuracy(1e-12);

  // solve A(x) = 0
  auto x = Spacy::covariantNewton(A,p);
//  auto x = Spacy::contravariantNewton(A,p);
//  auto x = Spacy::localNewton(A,p);
  
  FEniCS::copy(x,*u);

  // Save solution in VTK format
  File file("poisson.pvd");
  file << *u;

  // Plot solution
  plot(u);
  interactive();

  return 0;
}
