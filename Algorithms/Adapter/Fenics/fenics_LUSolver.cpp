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


  Fenics_LUSolver2::Fenics_LUSolver2(const dolfin::Form& A, const dolfin::Form& b, const std::vector<const dolfin::DirichletBC*>& bcs)
    : A_(A), b_(b), bcs_(bcs)
  {}

  std::unique_ptr<AbstractFunctionSpaceElement> Fenics_LUSolver2::operator()(const AbstractFunctionSpaceElement& x) const
  {
    auto parameters = default_parameters();
    const auto& x_ = fenics_Vector(x);
    auto y = clone(x_);

    using namespace dolfin;
    std::shared_ptr<GenericMatrix> M = y->impl().vector()->factory().create_matrix();
    std::shared_ptr<GenericVector> v = y->impl().vector()->factory().create_vector();

    // Solve linear system
    assemble(*M,A_);
    if(b_.ufc_form())
      assemble(*v,b_);
    else
      M->init_vector(*v,0);

    for(auto& bc : bcs_)
      bc->apply( *M, *v );

    LUSolver solver;
    solver.parameters.update(parameters("lu_solver"));
    solver.parameters["symmetric"] = static_cast<bool>( parameters["symmetric"] );
    solver.solve( *M , *y->impl().vector() , *x_.impl().vector() );


//    dolfin::LinearVariationalSolver solver(problem);
//    solver.parameters.update(dolfin::empty_parameters);
//    solver.solve();



    return std::unique_ptr<AbstractFunctionSpaceElement>( y.release() );
  }
}
