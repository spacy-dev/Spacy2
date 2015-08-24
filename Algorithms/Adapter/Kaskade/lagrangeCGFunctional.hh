#ifndef ALGORITHM_OPERATORS_KASKADE_LAGRANGE_CG_FUNCTIONAL_HH
#define ALGORITHM_OPERATORS_KASKADE_LAGRANGE_CG_FUNCTIONAL_HH

#include <string>

#include "../../vector.hh"
#include "../../vectorSpace.hh"
#include "../../functional.hh"
#include "linearSolver.hh"
#include "hessian.hh"
#include "Util/Mixins/disableAssembly.hh"

#include "directSolver.hh"
#include "vectorSpace.hh"
#include "vector.hh"
#include "functional.hh"
#include "operator.hh"
#include "linearOperator.hh"
#include "Algorithm/ConjugateGradients/cgSolver.hh"
#include "Algorithm/ConjugateGradients/triangularStateConstraintPreconditioner.hh"

namespace Algorithm
{
  namespace Kaskade
  {
    template <class FunctionalImpl, int stateId = 0, int controlId = 1, int adjointId = 2>
    class LagrangeCGFunctional :
        public Functional<FunctionalImpl> ,
        public Mixin::AbsoluteAccuracy , public Mixin::RelativeAccuracy , public Mixin::IterativeRefinements ,
        public Mixin::Eps , public Mixin::Verbosity
    {
      using VariableSetDescription = typename FunctionalImpl::AnsatzVars;
      using VectorImpl = typename VariableSetDescription::template CoefficientVectorRepresentation<>::type;
      using Spaces = typename VariableSetDescription::Spaces;
      using Variables = typename VariableSetDescription::Variables;
      using Assembler = ::Kaskade::VariationalFunctionalAssembler< ::Kaskade::LinearizationAt<FunctionalImpl> >;
      using Domain = typename Assembler::AnsatzVariableSetDescription::template CoefficientVectorRepresentation<>::type;
      using Range = typename Assembler::TestVariableSetDescription::template CoefficientVectorRepresentation<>::type;
      using Matrix = ::Kaskade::MatrixAsTriplet<double>;
      using KaskadeOperator = ::Kaskade::MatrixRepresentedOperator<Matrix,Domain,Range>;

    public:
      LagrangeCGFunctional(const FunctionalImpl& f, ::Algorithm::VectorSpace* domain_, std::string solver="CG",
                           int rbegin = 0, int rend = FunctionalImpl::AnsatzVars::noOfVariables,
                           int cbegin = 0, int cend = FunctionalImpl::TestVars::noOfVariables)
        : Functional<FunctionalImpl>(f,domain_,rbegin,rend,cbegin,cend),
          solver_(solver)
      {}

      LagrangeCGFunctional(const FunctionalImpl& f, ::Algorithm::VectorSpace& domain, std::string solver="CG",
                           int rbegin = 0, int rend = FunctionalImpl::AnsatzVars::noOfVariables,
                           int cbegin = 0, int cend = FunctionalImpl::TestVars::noOfVariables)
        : LagrangeCGFunctional(f,&domain,solver,rbegin,rend,cbegin,cend)
      {}

      LagrangeCGFunctional(const LagrangeCGFunctional& g)
        : Functional<FunctionalImpl>(g),
          solver_(g.solver_)
      {}

      LagrangeCGFunctional(const LagrangeCGFunctional& g, bool disableAssembly)
        : Functional<FunctionalImpl>(g,disableAssembly),
          solver_(g.solver_)
      {}

      void setSolver(std::string solver)
      {
        solver_ = solver;
      }

      ::Algorithm::LinearOperator hessian(const ::Algorithm::Vector& x) const
      {
        primalDualIgnoreReset(std::bind(&LagrangeCGFunctional::assembleHessian,std::ref(*this), std::placeholders::_1),x);
        auto copy = *this;
        copy.disableAssembly();
        return Hessian(std::move(copy),x,solver());
      }

      std::shared_ptr<LinearSolver> solver() const
      {
        auto matA = this->assembler_->template get<Matrix>(this->onlyLowerTriangle_,adjointId,adjointId+1,stateId,stateId+1);
        auto matB = this->assembler_->template get<Matrix>(this->onlyLowerTriangle_,adjointId,adjointId+1,controlId,controlId+1);
        auto matM = this->assembler_->template get<Matrix>(this->onlyLowerTriangle_,controlId,controlId+1,controlId,controlId+1);
        auto matAt = matA; matAt.transpose();
        auto matBt = matB; matBt.transpose();


        using VYSetDescription = Detail::ExtractDescription_t<VariableSetDescription,stateId>;
        using VUSetDescription = Detail::ExtractDescription_t<VariableSetDescription,controlId>;
        using VPSetDescription = Detail::ExtractDescription_t<VariableSetDescription,adjointId>;
        using Domain0 = typename VYSetDescription::template CoefficientVectorRepresentation<>::type;
        using KaskadeOperator2 = ::Kaskade::MatrixRepresentedOperator<Matrix,Domain0,Domain0>;
        auto stateSolver = DirectSolver<VPSetDescription,VYSetDescription>( KaskadeOperator2(matA), this->spaces_,
                                                                            cast_ref<ProductSpace::VectorCreator>(this->domain().impl()).subSpace_ptr(adjointId),
                                                                            cast_ref<ProductSpace::VectorCreator>(this->domain().impl()).subSpace_ptr(stateId) );
        auto controlSolver = DirectSolver<VUSetDescription,VUSetDescription>( KaskadeOperator2(matM), this->spaces_,
                                                                              cast_ref<ProductSpace::VectorCreator>(this->domain().impl()).subSpace_ptr(controlId),
                                                                              cast_ref<ProductSpace::VectorCreator>(this->domain().impl()).subSpace_ptr(controlId) );
        auto adjointSolver = DirectSolver<VYSetDescription,VPSetDescription>( KaskadeOperator2(matAt), this->spaces_,
                                                                              cast_ref<ProductSpace::VectorCreator>(this->domain().impl()).subSpace_ptr(stateId),
                                                                              cast_ref<ProductSpace::VectorCreator>(this->domain().impl()).subSpace_ptr(adjointId) );

        auto B = Kaskade::LinearOperator<KaskadeOperator2,VUSetDescription,VPSetDescription>
            ( KaskadeOperator2(matB) ,
              cast_ref<ProductSpace::VectorCreator>(this->domain().impl()).subSpace_ptr(controlId) ,
              cast_ref<ProductSpace::VectorCreator>(this->domain().impl()).subSpace_ptr(adjointId) );
        auto Bt = Kaskade::LinearOperator<KaskadeOperator2,VPSetDescription,VUSetDescription>
            ( KaskadeOperator2(matBt) ,
              cast_ref<ProductSpace::VectorCreator>(this->domain().impl()).subSpace_ptr(adjointId) ,
              cast_ref<ProductSpace::VectorCreator>(this->domain().impl()).subSpace_ptr(controlId) );

        auto P = TriangularStateConstraintPreconditioner
            ( std::move(stateSolver) ,
              std::move(controlSolver) ,
              std::move(adjointSolver) ,
              std::move(B),
              std::move(Bt),
              this->domain_ptr() , this->domain_ptr() );

        P.setStateIndex(stateId);
        P.setControlIndex(controlId);
        P.setAdjointIndex(adjointId);

        auto A = Kaskade::LinearOperator<KaskadeOperator,VariableSetDescription,VariableSetDescription>( *this->A_ , this->domain_ptr() , this->domain_ptr() );

        auto solver = CGSolver(A, P, solver_);
        solver.setAbsoluteAccuracy(absoluteAccuracy());
        solver.setRelativeAccuracy(relativeAccuracy());
        solver.setEps(eps());
        solver.setVerbosity(verbose());
        solver.setDetailedVerbosity(verbose_detailed());
        solver.setIterativeRefinements(iterativeRefinements());

        return std::make_shared<LinearSolver>(std::move(solver));
      }


    private:
      void assembleHessian(const ::Algorithm::Vector& x) const
      {
        Functional<FunctionalImpl>::assembleHessian(x);
      }

      std::string solver_ = "CG";
    };



    template <int stateId, int controlId, int adjointId, class FunctionalImpl>
    auto makeLagrangeCGFunctional(const FunctionalImpl& f, ::Algorithm::VectorSpace* domain, std::string solver="CG",
                                  int rbegin = 0, int rend = FunctionalImpl::AnsatzVars::noOfVariables,
                                  int cbegin = 0, int cend = FunctionalImpl::TestVars::noOfVariables)
    {
      return LagrangeCGFunctional<FunctionalImpl,stateId,controlId,adjointId>( f, domain , solver , rbegin , rend , cbegin , cend );
    }

    template <int stateId, int controlId, int adjointId, class FunctionalImpl>
    auto makeLagrangeCGFunctional(const FunctionalImpl& f, ::Algorithm::VectorSpace& domain, std::string solver="CG",
                                  int rbegin = 0, int rend = FunctionalImpl::AnsatzVars::noOfVariables,
                                  int cbegin = 0, int cend = FunctionalImpl::TestVars::noOfVariables)
    {
      return LagrangeCGFunctional<FunctionalImpl,stateId,controlId,adjointId>( f, domain , solver , rbegin, rend , cbegin , cend );
    }
  }
}

#endif // ALGORITHM_OPERATORS_KASKADE_LAGRANGE_CG_FUNCTIONAL_HH
