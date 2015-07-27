#ifndef ALGORITHM_CONJUGATE_GRADIENTS_CG_SOLVER_HH
#define ALGORITHM_CONJUGATE_GRADIENTS_CG_SOLVER_HH

#include <memory>

#include "cg.hh"
#include "operator.hh"

#include "Interface/abstractFunctionSpaceElement.hh"
#include "Interface/abstractLinearSolver.hh"
#include "Util/mixins.hh"
#include "Util/Exceptions/callOfUndefinedFunctionException.hh"

#include <utility>

namespace Algorithm
{
  class Operator;

  template <CGImplementationType impl_t = CGImplementationType::STANDARD>
  class CGSolverImpl
      : public Interface::AbstractLinearSolver, public Mixin::RelativeAccuracy, public Mixin::Eps, public Mixin::Verbosity
  {
  public:
    template <class Op1, class Op2,
              class = std::enable_if_t<std::is_base_of<Operator,std::decay_t<Op1> >::value>,
              class = std::enable_if_t<std::is_base_of<Operator,std::decay_t<Op2> >::value> >
    CGSolverImpl(Op1&& A, Op2&& P,
             double relativeAccuracy = 1e-15, double eps = 1e-15, bool verbose = false)
      : AbstractLinearSolver(A.impl().sharedRange(),A.impl().sharedDomain()),
        Mixin::RelativeAccuracy(relativeAccuracy),
        Mixin::Eps(eps),
        Mixin::Verbosity(verbose),
        cg(std::forward<Op1>(A),std::forward<Op2>(P),verbose,eps)
    {
      cg.terminationCriterion().setRelativeAccuracy(relativeAccuracy);
    }

//    CGSolverImpl(Operator&& A, const Operator& P,
//             double relativeAccuracy = 1e-15, double eps = 1e-15, bool verbose = false)
//      : AbstractLinearSolver(A.impl().getSharedRange(),A.impl().getSharedDomain()),
//        Mixin::RelativeAccuracy(relativeAccuracy),
//        Mixin::Eps(eps),
//        Mixin::Verbosity(verbose),
//        cg(std::move(A),P,verbose,eps)
//    {
//      cg.getTerminationCriterion().setRelativeAccuracy(relativeAccuracy);
//    }

    std::unique_ptr<Interface::AbstractFunctionSpaceElement> operator()(const Interface::AbstractFunctionSpaceElement& y) const final override
    {
      auto z = cg.solve( FunctionSpaceElement( clone(y) ) );
      return clone(z.impl());
    }

    CGImpl<impl_t>& impl()
    {
      return cg;
    }

    bool encounteredNonconvexity() const override
    {
      std::cout << "nonconvexity: " << cg.encounteredNonConvexity() << std::endl;
      return cg.encounteredNonConvexity();
    }

  private:
    CGSolverImpl* cloneImpl() const
    {
      throw CallOfUndefinedFunctionException("CGSolverImpl::cloneImpl()");
      //std::cout << "cloning cg solver" << std::endl;
      //return new CGSolverImpl(*this);
    }

    mutable CGImpl<impl_t> cg;
  };

  using CGSolver   = CGSolverImpl<>;
  using TCGSolver  = CGSolverImpl<CGImplementationType::TRUNCATED>;
  using RCGSolver  = CGSolverImpl<CGImplementationType::REGULARIZED>;
  using TRCGSolver = CGSolverImpl<CGImplementationType::TRUNCATED_REGULARIZED>;
}

#endif // ALGORITHM_CONJUGATE_GRADIENTS_CG_SOLVER_HH
