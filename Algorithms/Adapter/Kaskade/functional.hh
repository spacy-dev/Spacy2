#ifndef ALGORITHM_OPERATORS_KASKADE_FUNCTIONAL_HH
#define ALGORITHM_OPERATORS_KASKADE_FUNCTIONAL_HH

#include "../../functional.hh"
#include "Interface/abstractFunctional.hh"
#include "Interface/hessian.hh"
#include "Util/Mixins/disableAssembly.hh"
#include "Util/create.hh"

#include "directSolver.hh"
#include "vectorSpace.hh"
#include "vector.hh"

namespace Algorithm
{
  namespace Kaskade
  {
    template <class FunctionalImpl>
    class Functional : public Interface::AbstractFunctional , public Mixin::DisableAssembly
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
      Functional(const FunctionalImpl& f, std::shared_ptr<Interface::AbstractVectorSpace> domain_,
                 int rbegin = 0, int rend = FunctionalImpl::AnsatzVars::noOfVariables,
                 int cbegin = 0, int cend = FunctionalImpl::TestVars::noOfVariables)
        : AbstractFunctional(domain_),
          f_(f),
          spaces_( extractSpaces<VariableSetDescription>(domain()) ),
          assembler_(std::make_shared<Assembler>(spaces_)),
          rbegin_(rbegin), rend_(rend), cbegin_(cbegin), cend_(cend)
      {}

      Functional(const FunctionalImpl& f, const ::Algorithm::VectorSpace& domain,
                 int rbegin = 0, int rend = FunctionalImpl::AnsatzVars::noOfVariables,
                 int cbegin = 0, int cend = FunctionalImpl::TestVars::noOfVariables)
        : Functional(f,domain.sharedImpl(),rbegin,rend,cbegin,cend)
      {}

      Functional(const Functional& g)
        : AbstractFunctional(g.sharedDomain()),
          DisableAssembly(g.assemblyIsDisabled()),
          f_(g.f_), spaces_(g.spaces_),
          assembler_(g.assembler_),
          rbegin_(g.rbegin_), rend_(g.rend_), cbegin_(g.cbegin_), cend_(g.cend_)
      {
        if( g.A_ != nullptr ) A_ = std::make_unique<KaskadeOperator>(*g.A_);
      }


      Functional(const Functional& g, bool disableAssembly)
        : AbstractFunctional(g.sharedDomain()),
          DisableAssembly(disableAssembly),
          f_(g.f_), spaces_(g.spaces_),
          assembler_(g.assembler_),
          A_( std::make_unique<KaskadeOperator>(*g.A_) )
      {}

      double d0(const Interface::AbstractVector& x) const override
      {
        primalDualIgnoreReset(std::bind(&Functional::assembleFunctional,std::ref(*this), std::placeholders::_1),x);

        return assembler_->functional();
      }

      std::unique_ptr<Interface::AbstractVector> d1(const Interface::AbstractVector& x) const override
      {
        primalDualIgnoreReset(std::bind(&Functional::assembleGradient,std::ref(*this), std::placeholders::_1),x);

        VectorImpl v( assembler_->rhs() );

        auto y = domain().dualSpacePtr()->element();
        copyFromCoefficientVector<VariableSetDescription>(v,*y);
        return std::move(y);
      }

      std::unique_ptr<Interface::AbstractVector> d2(const Interface::AbstractVector& x, const Interface::AbstractVector& dx) const override
      {
        primalDualIgnoreReset(std::bind(&Functional::assembleHessian,std::ref(*this), std::placeholders::_1),x);

        VectorImpl dx_( VariableSetDescription::template CoefficientVectorRepresentation<>::init(spaces_) );
        copyToCoefficientVector<VariableSetDescription>(dx,dx_);
        VectorImpl y_( VariableSetDescription::template CoefficientVectorRepresentation<>::init(spaces_) );

//        std::cout << "functional dn(dn) = " << dx(dx) << std::endl;

        A_->apply( dx_ , y_ );

        auto y = domain().dualSpacePtr()->element();
        copyFromCoefficientVector<VariableSetDescription>(y_,*y);
//        std::cout << "result = " << (*y)(*y) << std::endl;

        return std::move(y);
      }

    protected:
      void assembleFunctional(const Interface::AbstractVector& x) const
      {
        if( assemblyIsDisabled() ) return;
        if( old_X_f_ != nullptr && old_X_f_->equals(x) ) return;

        VariableSetDescription variableSet(spaces_);
        typename VariableSetDescription::VariableSet u(variableSet);

        copy(x,u);

        assembler_->assemble(::Kaskade::linearization(f_,u) , Assembler::VALUE , nAssemblyThreads );

        old_X_f_ = clone(x);
      }

      void assembleGradient(const Interface::AbstractVector& x) const
      {
        if( assemblyIsDisabled() ) return;
        if( old_X_df_ != nullptr && old_X_df_->equals(x) ) return;

        VariableSetDescription variableSet(spaces_);
        typename VariableSetDescription::VariableSet u(variableSet);

        copy(x,u);

        assembler_->assemble(::Kaskade::linearization(f_,u) , Assembler::RHS , nAssemblyThreads );

        old_X_df_ = clone(x);
      }

      void assembleHessian(const Interface::AbstractVector& x) const
      {
        if( assemblyIsDisabled() ) return;
        if( old_X_ddf_ != nullptr && old_X_ddf_->equals(x) ) return;

        VariableSetDescription variableSet(spaces_);
        typename VariableSetDescription::VariableSet u(variableSet);

        copy(x,u);

        assembler_->assemble(::Kaskade::linearization(f_,u) , Assembler::MATRIX , nAssemblyThreads );
        A_ = std::make_unique< KaskadeOperator >( assembler_->template get<Matrix>(onlyLowerTriangle_,rbegin_,rend_,cbegin_,cend_) );

        old_X_ddf_ = clone(x);
      }

      Functional* cloneImpl() const override
      {
        return new Functional(*this);
      }

      std::unique_ptr<Interface::Hessian> makeHessian(const Interface::AbstractVector& x) const override
      {
        primalDualIgnoreReset(std::bind(&Functional::assembleHessian,std::ref(*this), std::placeholders::_1),x);
        return std::make_unique<Interface::Hessian>(std::make_unique< Functional<FunctionalImpl> >(*this,true),x);
      }


      std::unique_ptr<Interface::AbstractLinearSolver> makeSolver() const override
      {
        assert (A_ != nullptr);
        return std::make_unique< DirectSolver<VariableSetDescription,VariableSetDescription> >( *A_ , spaces_, sharedDomain() , sharedDomain() );
      }


      FunctionalImpl f_;
      Spaces spaces_;
      mutable std::shared_ptr<Assembler> assembler_;
      mutable std::unique_ptr< KaskadeOperator > A_ = nullptr;
      mutable std::unique_ptr< Interface::AbstractVector > old_X_f_ = nullptr, old_X_df_ = nullptr, old_X_ddf_ = nullptr;
      unsigned nAssemblyThreads = 1;
      bool onlyLowerTriangle_ = false;
      int rbegin_=0, rend_=FunctionalImpl::AnsatzVars::noOfVariables;
      int cbegin_=0, cend_=FunctionalImpl::TestVars::noOfVariables;
    };



    template <class FunctionalImpl>
    auto makeFunctional(const FunctionalImpl& f, std::shared_ptr<Interface::AbstractVectorSpace> domain,
                        int rbegin = 0, int rend = FunctionalImpl::AnsatzVars::noOfVariables,
                        int cbegin = 0, int cend = FunctionalImpl::TestVars::noOfVariables)
    {
      return createFromUniqueImpl< ::Algorithm::Functional , Functional<FunctionalImpl> >( f, domain , rbegin , rend , cbegin , cend );
    }

    template <class FunctionalImpl>
    auto makeFunctional(const FunctionalImpl& f, const ::Algorithm::VectorSpace& domain,
                        int rbegin = 0, int rend = FunctionalImpl::AnsatzVars::noOfVariables,
                        int cbegin = 0, int cend = FunctionalImpl::TestVars::noOfVariables)
    {
      return createFromUniqueImpl< ::Algorithm::Functional , Functional<FunctionalImpl> >( f, domain , rbegin, rend , cbegin , cend );
    }
  }
}

#endif // ALGORITHM_OPERATORS_KASKADE_FUNCTIONAL_HH
