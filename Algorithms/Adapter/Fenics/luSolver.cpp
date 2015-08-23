#include "luSolver.hh"

#include "util.hh"
#include "FunctionSpaces/ProductSpace/productSpaceElement.hh"

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

  namespace Fenics
  {
  LUSolver::LUSolver(std::shared_ptr<dolfin::GenericMatrix> A, const dolfin::FunctionSpace& productSpace,
                     ::Algorithm::VectorSpace* domain , ::Algorithm::VectorSpace* range)
    : OperatorBase(domain,range),
      solver_("umfpack"),
      productSpace_(productSpace)
  {
//    std::cout << A->str(true).c_str() << std::endl;
    auto parameters = default_parameters();
    solver_.parameters.update(parameters("lu_solver"));
    solver_.parameters["symmetric"] = (bool) parameters["symmetric"];
    solver_.set_operator(A);
  }

  ::Algorithm::Vector LUSolver::operator()(const ::Algorithm::Vector& x) const
  {
    // Solve linear system
    dolfin::Function tmp(productSpace_);
    auto y_ = std::make_shared<dolfin::Vector>(tmp.vector()->mpi_comm(), tmp.vector()->size()),
         x_ = std::make_shared<dolfin::Vector>(tmp.vector()->mpi_comm(), tmp.vector()->size());
    copy(x,*y_);
    copy(x,*x_);
    solver_.solve( *y_, * x_ );
    auto y = range().element();

    copy(*y_, y);
    return y;
  }



//  TransposedLUSolver::TransposedLUSolver(const dolfin::GenericMatrix& A)
//    : A_(std::make_unique<dolfin::Matrix>( A ))
//  {
//    auto parameters = default_parameters();
//    solver.parameters.update(parameters("lu_solver"));
//    solver.parameters["symmetric"] = (bool) parameters["symmetric"];
//  }

//  std::unique_ptr<Interface::AbstractVector> TransposedLUSolver::operator()(const Interface::AbstractVector& x) const
//  {
//    const auto& x_ = toVector(x);
//    auto y = clone(x_);

//    // Solve linear system
//    solver.solve_transpose( *A_ , *y->impl().vector(), *x_.impl().vector() );

//    return std::unique_ptr<Interface::AbstractVector>( y.release() );
//  }


//  std::shared_ptr<Interface::AbstractLinearSolver> createTransposedLUSolver(const dolfin::GenericMatrix& A)
//  {
//    return std::make_shared<TransposedLUSolver>(A);
//  }

//  template <class... Args>
//  std::shared_ptr<Interface::AbstractLinearSolver> createTransposedLUSolver(const Args&...)
//  {
//    throw CallOfUndefinedFunctionException("createLUSolver");
//  }
  }
}
