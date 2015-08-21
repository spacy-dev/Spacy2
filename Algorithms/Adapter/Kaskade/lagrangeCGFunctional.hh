//#ifndef ALGORITHM_OPERATORS_KASKADE_LAGRANGE_CG_FUNCTIONAL_HH
//#define ALGORITHM_OPERATORS_KASKADE_LAGRANGE_CG_FUNCTIONAL_HH

//#include <string>

//#include "../../vectorSpace.hh"
//#include "../../functional.hh"
//#include "Interface/abstractFunctional.hh"
//#include "Interface/hessian.hh"
//#include "Util/Mixins/disableAssembly.hh"
//#include "Util/create.hh"

//#include "directSolver.hh"
//#include "vectorSpace.hh"
//#include "vector.hh"
//#include "functional.hh"
//#include "operator.hh"
//#include "linearOperator.hh"
//#include "Algorithm/ConjugateGradients/cgSolver.hh"
//#include "Algorithm/ConjugateGradients/triangularStateConstraintPreconditioner.hh"

//namespace Algorithm
//{
//  namespace Kaskade
//  {
//    template <class FunctionalImpl, int stateId = 0, int controlId = 1, int adjointId = 2>
//    class LagrangeCGFunctional :
//        public Functional<FunctionalImpl> ,
//        public Mixin::AbsoluteAccuracy , public Mixin::RelativeAccuracy , public Mixin::IterativeRefinements ,
//        public Mixin::Eps , public Mixin::Verbosity
//    {
//      using VariableSetDescription = typename FunctionalImpl::AnsatzVars;
//      using VectorImpl = typename VariableSetDescription::template CoefficientVectorRepresentation<>::type;
//      using Spaces = typename VariableSetDescription::Spaces;
//      using Variables = typename VariableSetDescription::Variables;
//      using Assembler = ::Kaskade::VariationalFunctionalAssembler< ::Kaskade::LinearizationAt<FunctionalImpl> >;
//      using Domain = typename Assembler::AnsatzVariableSetDescription::template CoefficientVectorRepresentation<>::type;
//      using Range = typename Assembler::TestVariableSetDescription::template CoefficientVectorRepresentation<>::type;
//      using Matrix = ::Kaskade::MatrixAsTriplet<double>;
//      using KaskadeOperator = ::Kaskade::MatrixRepresentedOperator<Matrix,Domain,Range>;

//    public:
//      LagrangeCGFunctional(const FunctionalImpl& f, ::Algorithm::VectorSpace* domain_, std::string solver="CG",
//                           int rbegin = 0, int rend = FunctionalImpl::AnsatzVars::noOfVariables,
//                           int cbegin = 0, int cend = FunctionalImpl::TestVars::noOfVariables)
//        : Functional<FunctionalImpl>(f,domain_,rbegin,rend,cbegin,cend),
//          solver_(solver)
//      {}

//      LagrangeCGFunctional(const FunctionalImpl& f, ::Algorithm::VectorSpace& domain, std::string solver="CG",
//                           int rbegin = 0, int rend = FunctionalImpl::AnsatzVars::noOfVariables,
//                           int cbegin = 0, int cend = FunctionalImpl::TestVars::noOfVariables)
//        : LagrangeCGFunctional(f,&domain,solver,rbegin,rend,cbegin,cend)
//      {}

//      LagrangeCGFunctional(const LagrangeCGFunctional& g)
//        : Functional<FunctionalImpl>(g),
//          solver_(g.solver_)
//      {}

//      LagrangeCGFunctional(const LagrangeCGFunctional& g, bool disableAssembly)
//        : Functional<FunctionalImpl>(g,disableAssembly),
//          solver_(g.solver_)
//      {}

//      void setSolver(std::string solver)
//      {
//        solver_ = solver;
//      }

//    private:
//      LagrangeCGFunctional* cloneImpl() const final override
//      {
//        return new LagrangeCGFunctional(*this);
//      }

//      void assembleHessian(const Interface::AbstractVector& x) const
//      {
//        Functional<FunctionalImpl>::assembleHessian(x);
//      }

//      std::unique_ptr<Interface::Hessian> makeHessian(const Interface::AbstractVector& x) const override
//      {
//        primalDualIgnoreReset(std::bind(&LagrangeCGFunctional::assembleHessian,std::ref(*this), std::placeholders::_1),x);
//        auto hessian_ = clone(this);
//        hessian_->disableAssembly();
//        return std::make_unique<Interface::Hessian>(std::move(hessian_),x);
//      }

//      std::unique_ptr<Interface::AbstractLinearSolver> makeSolver() const final override
//      {
//        auto matA = this->assembler_->template get<Matrix>(this->onlyLowerTriangle_,adjointId,adjointId+1,stateId,stateId+1);
//        auto matB = this->assembler_->template get<Matrix>(this->onlyLowerTriangle_,adjointId,adjointId+1,controlId,controlId+1);
//        auto matM = this->assembler_->template get<Matrix>(this->onlyLowerTriangle_,controlId,controlId+1,controlId,controlId+1);
//        auto matAt = matA; matAt.transpose();
//        auto matBt = matB; matBt.transpose();


//        using VYSetDescription = Detail::ExtractDescription_t<VariableSetDescription,stateId>;
//        using VUSetDescription = Detail::ExtractDescription_t<VariableSetDescription,controlId>;
//        using VPSetDescription = Detail::ExtractDescription_t<VariableSetDescription,adjointId>;
//        using Domain0 = typename VYSetDescription::template CoefficientVectorRepresentation<>::type;
//        using KaskadeOperator2 = ::Kaskade::MatrixRepresentedOperator<Matrix,Domain0,Domain0>;
//        auto stateSolver = std::make_shared< DirectSolver<VPSetDescription,VYSetDescription> >( KaskadeOperator2(matA), this->spaces_,
//                                                                                                castTo<ProductSpace>(this->domain()).sharedSubSpace(adjointId),
//                                                                                                castTo<ProductSpace>(this->domain()).sharedSubSpace(stateId) );
//        auto controlSolver = std::make_shared< DirectSolver<VUSetDescription,VUSetDescription> >( KaskadeOperator2(matM), this->spaces_,
//                                                                                                  castTo<ProductSpace>(this->domain()).sharedSubSpace(controlId),
//                                                                                                  castTo<ProductSpace>(this->domain()).sharedSubSpace(controlId) );
//        auto adjointSolver = std::make_shared< DirectSolver<VYSetDescription,VPSetDescription> >( KaskadeOperator2(matAt), this->spaces_,
//                                                                                                  castTo<ProductSpace>(this->domain()).sharedSubSpace(stateId),
//                                                                                                  castTo<ProductSpace>(this->domain()).sharedSubSpace(adjointId) );

//        auto B = std::make_unique< Kaskade::LinearOperator<KaskadeOperator2,VUSetDescription,VPSetDescription> >
//            ( KaskadeOperator2(matB) ,
//              castTo<ProductSpace>(this->domain()).sharedSubSpace(controlId) ,
//              castTo<ProductSpace>(this->domain()).sharedSubSpace(adjointId) );
//        auto Bt = std::make_unique< Kaskade::LinearOperator<KaskadeOperator2,VPSetDescription,VUSetDescription> >
//            ( KaskadeOperator2(matBt) ,
//              castTo<ProductSpace>(this->domain()).sharedSubSpace(adjointId) ,
//              castTo<ProductSpace>(this->domain()).sharedSubSpace(controlId) );

//        auto P_ = std::make_unique<TriangularStateConstraintPreconditioner>
//            ( stateSolver ,
//              controlSolver ,
//              adjointSolver ,
//              std::move(B),
//              std::move(Bt),
//              this->sharedDomain() , this->sharedDomain() );

//        P_->setStateIndex(stateId);
//        P_->setControlIndex(controlId);
//        P_->setAdjointIndex(adjointId);
//        auto P = ::Algorithm::Operator(std::move(P_));

//        auto A = ::Algorithm::Operator( std::make_unique< Kaskade::LinearOperator<KaskadeOperator,VariableSetDescription,VariableSetDescription> >( *this->A_ , this->sharedDomain() , this->sharedDomain() ) );

//        auto solver = std::make_unique<CGSolver>(A, P, solver_);
//        solver->setAbsoluteAccuracy(absoluteAccuracy());
//        solver->setRelativeAccuracy(relativeAccuracy());
//        solver->setEps(eps());
//        solver->setVerbosity(verbose());
//        solver->setDetailedVerbosity(verbose_detailed());
//        solver->setIterativeRefinements(iterativeRefinements());

//        return std::move(solver);
//      }

//      std::string solver_ = "CG";
//    };



//    template <int stateId, int controlId, int adjointId, class FunctionalImpl>
//    auto makeLagrangeCGFunctional(const FunctionalImpl& f, ::Algorithm::VectorSpace* domain, std::string solver="CG",
//                                  int rbegin = 0, int rend = FunctionalImpl::AnsatzVars::noOfVariables,
//                                  int cbegin = 0, int cend = FunctionalImpl::TestVars::noOfVariables)
//    {
//      return createFromUniqueImpl< ::Algorithm::Functional , LagrangeCGFunctional<FunctionalImpl,stateId,controlId,adjointId> >( f, domain , solver , rbegin , rend , cbegin , cend );
//    }

//    template <int stateId, int controlId, int adjointId, class FunctionalImpl>
//    auto makeLagrangeCGFunctional(const FunctionalImpl& f, const ::Algorithm::VectorSpace& domain, std::string solver="CG",
//                                  int rbegin = 0, int rend = FunctionalImpl::AnsatzVars::noOfVariables,
//                                  int cbegin = 0, int cend = FunctionalImpl::TestVars::noOfVariables)
//    {
//      return createFromUniqueImpl< ::Algorithm::Functional , LagrangeCGFunctional<FunctionalImpl,stateId,controlId,adjointId> >( f, domain , solver , rbegin, rend , cbegin , cend );
//    }
//  }
//}

//#endif // ALGORITHM_OPERATORS_KASKADE_LAGRANGE_CG_FUNCTIONAL_HH
