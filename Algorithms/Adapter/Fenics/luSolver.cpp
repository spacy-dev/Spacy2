#include "luSolver.hh"

#include "vector.hh"
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
  LUSolver::LUSolver(const dolfin::GenericMatrix& A, const dolfin::FunctionSpace& productSpace,
                             std::shared_ptr<Interface::AbstractBanachSpace> domain , std::shared_ptr<Interface::AbstractBanachSpace> range)
    : Interface::AbstractLinearSolver(domain,range),
      A_(A.copy()), productSpace_(productSpace)
  {
    auto parameters = default_parameters();
    solver.parameters.update(parameters("lu_solver"));
    solver.parameters["symmetric"] = (bool) parameters["symmetric"];
  }

  std::unique_ptr<Interface::AbstractFunctionSpaceElement> LUSolver::operator()(const Interface::AbstractFunctionSpaceElement& x) const
  {

    // Solve linear system
    dolfin::Function tmp(productSpace_);
    auto y_ = std::make_shared<dolfin::Vector>(tmp.vector()->mpi_comm(), tmp.vector()->size()),
         x_ = std::make_shared<dolfin::Vector>(tmp.vector()->mpi_comm(), tmp.vector()->size());
    copy(x,*y_);
    copy(x,*x_);
    solver.solve( *A_ , *y_, * x_ );
    auto y = clone(x);

    copy(*y_, *y);
    return std::unique_ptr<Interface::AbstractFunctionSpaceElement>( y.release() );
  }

  LUSolver* LUSolver::cloneImpl() const
  {
    return new LUSolver(*A_,productSpace_,getSharedDomain(),getSharedRange());
  }

//  std::shared_ptr<Interface::AbstractLinearSolver> createLUSolver(const dolfin::GenericMatrix& A)
//  {
//    return std::make_shared<LUSolver>(A);
//  }

//  template <class... Args>
//  std::shared_ptr<Interface::AbstractLinearSolver> createLUSolver(const Args&...)
//  {
//    throw CallOfUndefinedFunctionException("createLUSolver");
//  }



//  TransposedLUSolver::TransposedLUSolver(const dolfin::GenericMatrix& A)
//    : A_(std::make_unique<dolfin::Matrix>( A ))
//  {
//    auto parameters = default_parameters();
//    solver.parameters.update(parameters("lu_solver"));
//    solver.parameters["symmetric"] = (bool) parameters["symmetric"];
//  }

//  std::unique_ptr<Interface::AbstractFunctionSpaceElement> TransposedLUSolver::operator()(const Interface::AbstractFunctionSpaceElement& x) const
//  {
//    const auto& x_ = toVector(x);
//    auto y = clone(x_);

//    // Solve linear system
//    solver.solve_transpose( *A_ , *y->impl().vector(), *x_.impl().vector() );

//    return std::unique_ptr<Interface::AbstractFunctionSpaceElement>( y.release() );
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
