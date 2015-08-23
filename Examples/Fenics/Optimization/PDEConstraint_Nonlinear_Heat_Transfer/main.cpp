#include <chrono>

#include <dolfin.h>
#include "NormalStep.h"
#include "LagrangeFunctional.h"

#include "Adapter/fenics.hh"
#include "Algorithm/CompositeStep/affineCovariantCompositeSteps.hh"

using namespace dolfin;
using namespace std::chrono;

// Source term reference solution
class Source : public Expression
{
  void eval(Array<double>& values, const Array<double>& x) const
  {
    double dx = x[0] - 0.5;
    double dy = x[1] - 0.5;
    values[0] = 1;//10*exp(-(dx*dx + dy*dy) / 0.02);
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
  auto n = 256;
  UnitSquareMesh mesh(n,n);
  LagrangeFunctional::FunctionSpace V(mesh);

  // Define variational forms
  LagrangeFunctional::Functional f(V.mesh());
  LagrangeFunctional::LinearForm J(V);
  LagrangeFunctional::BilinearForm H(V,V);
  NormalStep::BilinearForm Norm(V,V);
   std::vector<const DirichletBC*> bcs = {};
  
  Function dummy(V);
  Source y_ref;
  Constant alpha(1e-2);
  Constant c(1e-1);
  Constant d(10);
  f.y0 = y_ref;
  f.alpha = alpha;
  f.c = c;
  f.d = d;
  J.y0 = y_ref;
  J.alpha = alpha;
  J.c = c;
  J.d = d;
  H.alpha = alpha;
  H.c = c;
  H.d = d;
  Norm.alpha = alpha;
  Norm.c = c;
  Norm.d = d;
  
  // Compute solution
  using namespace Algorithm;

  std::vector<unsigned> primalSpaceIds = {0u,1u}, dualSpaceIds = {2u};
  auto productSpace = Fenics::makeHilbertSpace( V , primalSpaceIds , dualSpaceIds );
  
  // functionals
  auto lagrangeFunctional = Fenics::makeFunctional( f , J , H , bcs , productSpace );
  auto normalStepFunctional = Fenics::makeFunctional( f , J , Norm , bcs , productSpace );

  // composite step solve
  AffineCovariantCompositeSteps alg_cs( normalStepFunctional, lagrangeFunctional );
  alg_cs.setRelativeAccuracy(1e-9);
  alg_cs.setEps(1e-12);
  alg_cs.setMaxSteps(500);
  alg_cs.setVerbosity(true);
  alg_cs.setIterativeRefinements(0);

  // solve problem
  auto startTime = high_resolution_clock::now();
  auto sol = alg_cs.solve();
  std::cout << "computation time: " << duration_cast<seconds>(high_resolution_clock::now()-startTime).count() << std::endl;

  // copy back and display solution
  Function u(V);
  Fenics::copy(sol,u);
  
  File filecs("cs.pvd");
  filecs << u[0];

  // Plot solution
  plot(u);
  interactive();

  return 0;
}
