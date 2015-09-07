#include "luSolver.hh"

#include "Spacy/vectorSpace.hh"

#include "util.hh"
#include "vectorSpace.hh"

namespace Spacy
{
  namespace
  {
    dolfin::Parameters default_parameters()
    {
      dolfin::Parameters p("linear_variational_solver");

      p.add("linear_solver", "default");

      p.add("print_rhs", false);
      p.add("print_matrix", false);

      p.add(dolfin::LUSolver::default_parameters());

      return p;
    }
  }

  namespace FEniCS
  {
    LUSolver::LUSolver(std::shared_ptr<dolfin::GenericMatrix> A, std::shared_ptr<const dolfin::FunctionSpace> space,
                       const VectorSpace& domain , const VectorSpace& range,
                       bool symmetric, const std::string& solverName)
      : OperatorBase(domain,range),
        solver_(solverName),
        space_(space)
    {
      auto parameters = default_parameters();
      solver_.parameters.update(parameters("lu_solver"));
      solver_.parameters["symmetric"] = symmetric;
      solver_.set_operator(A);
    }

    ::Spacy::Vector LUSolver::operator()(const ::Spacy::Vector& x) const
    {
      // Solve linear system
      auto x_ = dolfin::Function( space_ );
      copy(x,x_);
      auto y_ = x_.vector()->copy();
      solver_.solve( *y_, *x_.vector() );
      auto y = range().vector();

      copy(*y_, y);
      return y;
    }


    TransposedLUSolver::TransposedLUSolver(std::shared_ptr<dolfin::GenericMatrix> A, const dolfin::FunctionSpace& space,
                                           const VectorSpace& domain , const VectorSpace& range,
                                           bool symmetric, const std::string& solverName)
      : OperatorBase(domain,range),
        solver_(solverName),
        space_(space)
    {
      auto parameters = default_parameters();
      solver_.parameters.update(parameters("lu_solver"));
      solver_.parameters["symmetric"] = symmetric;
      solver_.set_operator(A);
    }

    ::Spacy::Vector TransposedLUSolver::operator()(const ::Spacy::Vector& x) const
    {
      // Solve linear system
      dolfin::Function x_(space_);
      copy(x,x_);
      auto y_ = x_.vector()->copy();
      solver_.solve_transpose( *y_, *x_.vector() );
      auto y = range().vector();

      copy(*y_, y);
      return y;
    }
  }
}
