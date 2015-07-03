#include "fenics_LUSolver.hh"

#include "fenics_functionSpaceElement.hh"

namespace Algorithm
{
  namespace
  {
    static dolfin::Parameters default_parameters()
    {
      dolfin::Parameters p("linear_variational_solver");

      p.add("linear_solver", "default");
      p.add("preconditioner", "default");
      p.add("symmetric", false);

      p.add("print_rhs", false);
      p.add("print_matrix", false);

      p.add(dolfin::LUSolver::default_parameters());
      p.add(dolfin::KrylovSolver::default_parameters());

      return p;
    }
  }

  Fenics_LUSolver::Fenics_LUSolver(const dolfin::GenericMatrix& A)
    : A_(std::make_unique<dolfin::Matrix>( A ))
  {
    auto parameters = default_parameters();
    solver.parameters.update(parameters("lu_solver"));
    solver.parameters["symmetric"] = (bool) parameters["symmetric"];
  }

  std::unique_ptr<AbstractFunctionSpaceElement> Fenics_LUSolver::operator()(const AbstractFunctionSpaceElement& x) const
  {
    const auto& x_ = fenics_Vector(x);
    auto y = clone(x_);

    // Solve linear system
    solver.solve( *A_ , *y->impl().vector(), *x_.impl().vector() );

    return std::unique_ptr<AbstractFunctionSpaceElement>( y.release() );
  }
}
