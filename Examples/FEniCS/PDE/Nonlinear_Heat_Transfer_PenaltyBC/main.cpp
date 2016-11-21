#include <dolfin.h>

#include <Spacy/c1Operator.hh>
#include <Spacy/zeroVectorCreator.hh>
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
  auto mesh = std::make_shared<UnitSquareMesh>(n,n);
  auto V = std::make_shared<NonlinearHeat::FunctionSpace>(mesh);

  // Define variational forms
  NonlinearHeat::BilinearForm a(V, V);
  NonlinearHeat::LinearForm L(V);

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
  std::cout << "domain" << std::endl;
  auto domain = FEniCS::makeHilbertSpace(V);
  
  // create operator
  Spacy::C1Operator A = FEniCS::makeC1Operator( L , a , domain , domain.dualSpace() );
  // set scalar product for affine covariant newton method
  domain.setScalarProduct( InducedScalarProduct( A.linearization(zero(domain)) ) );
  // specify parameters for Newton's method
  auto p = Newton::Parameter{};
  p.setRelativeAccuracy(1e-12);
  // solve A(x)=0
  auto x = covariantNewton(A,p);
//  auto x = contravariantNewton(A,p);
//  auto x = localNewton(A,p);
  
  FEniCS::copy(x,*u);

  
  // Save solution in VTK format
  File file("poisson.pvd");
  file << *u;

  // Plot solution
  plot(u);
  interactive();

  return 0;
}
