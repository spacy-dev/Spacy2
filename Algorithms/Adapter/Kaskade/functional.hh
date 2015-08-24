#ifndef ALGORITHM_OPERATORS_KASKADE_FUNCTIONAL_HH
#define ALGORITHM_OPERATORS_KASKADE_FUNCTIONAL_HH

#include <memory>

#include "fem/assemble.hh"
#include "fem/istlinterface.hh"

#include "../../operator.hh"
#include "../../functional.hh"
#include "../../vector.hh"
#include "../../vectorSpace.hh"
#include "hessian.hh"
#include "Util/Mixins/disableAssembly.hh"
#include "Util/Base/functionalBase.hh"

#include "directSolver.hh"
#include "vectorSpace.hh"
#include "vector.hh"

namespace Algorithm
{
  namespace Kaskade
  {
    template <class FunctionalImpl>
    class Functional : public FunctionalBase< Functional<FunctionalImpl> > , public Mixin::DisableAssembly
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
      Functional(const FunctionalImpl& f, ::Algorithm::VectorSpace* domain_,
                 int rbegin = 0, int rend = FunctionalImpl::AnsatzVars::noOfVariables,
                 int cbegin = 0, int cend = FunctionalImpl::TestVars::noOfVariables)
        : FunctionalBase< Functional<FunctionalImpl> >(domain_),
          f_(f),
          spaces_( extractSpaces<VariableSetDescription>(this->domain()) ),
          assembler_(std::make_shared<Assembler>(spaces_)),
          rbegin_(rbegin), rend_(rend), cbegin_(cbegin), cend_(cend)
      {}

      Functional(const FunctionalImpl& f, ::Algorithm::VectorSpace& domain,
                 int rbegin = 0, int rend = FunctionalImpl::AnsatzVars::noOfVariables,
                 int cbegin = 0, int cend = FunctionalImpl::TestVars::noOfVariables)
        : Functional(f,&domain,rbegin,rend,cbegin,cend)
      {}

      Functional(const Functional& g)
        : FunctionalBase< Functional<FunctionalImpl> >(g.domain_ptr()),
          DisableAssembly(g.assemblyIsDisabled()),
          f_(g.f_), spaces_(g.spaces_),
          assembler_(g.assembler_),
          rbegin_(g.rbegin_), rend_(g.rend_), cbegin_(g.cbegin_), cend_(g.cend_)
      {
        if( g.A_ != nullptr ) A_ = std::make_unique<KaskadeOperator>(*g.A_);
      }


      Functional(const Functional& g, bool disableAssembly)
        : FunctionalBase< Functional<FunctionalImpl> >(g.domain_ptr()),
          DisableAssembly(disableAssembly),
          f_(g.f_), spaces_(g.spaces_),
          assembler_(g.assembler_),
          A_( std::make_unique<KaskadeOperator>(*g.A_) )
      {}

      double operator()(const ::Algorithm::Vector& x) const
      {
        primalDualIgnoreReset(std::bind(&Functional::assembleFunctional,std::ref(*this), std::placeholders::_1),x);

        return assembler_->functional();
      }

      LinearOperator hessian(const ::Algorithm::Vector& x) const
      {
        primalDualIgnoreReset(std::bind(&Functional::assembleHessian,std::ref(*this), std::placeholders::_1),x);
        return Hessian(  Functional<FunctionalImpl>(*this,true) , x , solver_ );
      }

      ::Algorithm::Vector d1_(const ::Algorithm::Vector& x) const
      {
        primalDualIgnoreReset(std::bind(&Functional::assembleGradient,std::ref(*this), std::placeholders::_1),x);

        VectorImpl v( assembler_->rhs() );

        auto y = this->domain().dualSpace_ptr()->element();
        copyFromCoefficientVector<VariableSetDescription>(v,y);
        return y;
      }

      ::Algorithm::Vector d2_(const ::Algorithm::Vector& x, const ::Algorithm::Vector& dx) const
      {
        primalDualIgnoreReset(std::bind(&Functional::assembleHessian,std::ref(*this), std::placeholders::_1),x);

        VectorImpl dx_( VariableSetDescription::template CoefficientVectorRepresentation<>::init(spaces_) );
        copyToCoefficientVector<VariableSetDescription>(dx,dx_);
        VectorImpl y_( VariableSetDescription::template CoefficientVectorRepresentation<>::init(spaces_) );

        A_->apply( dx_ , y_ );

        auto y = this->domain().dualSpace_ptr()->element();
        copyFromCoefficientVector<VariableSetDescription>(y_,y);

        return y;
      }

    protected:
      void assembleFunctional(const ::Algorithm::Vector& x) const
      {
        if( assemblyIsDisabled() ) return;
        if( ( (assembler_->valid() & Assembler::VALUE) != 0 ) && (old_X_f_==x) ) return;

        VariableSetDescription variableSet(spaces_);
        typename VariableSetDescription::VariableSet u(variableSet);

        copy(x,u);

        assembler_->assemble(::Kaskade::linearization(f_,u) , Assembler::VALUE , nAssemblyThreads );

        old_X_f_ = x;
      }

      void assembleGradient(const ::Algorithm::Vector& x) const
      {
        if( assemblyIsDisabled() ) return;
        if( ( (assembler_->valid() & Assembler::RHS) != 0 ) && (old_X_df_==x) ) return;

        VariableSetDescription variableSet(spaces_);
        typename VariableSetDescription::VariableSet u(variableSet);

        copy(x,u);

        assembler_->assemble(::Kaskade::linearization(f_,u) , Assembler::RHS , nAssemblyThreads );

        old_X_df_ = x;
      }

      void assembleHessian(const ::Algorithm::Vector& x) const
      {
        if( assemblyIsDisabled() ) return;
        if( ( (assembler_->valid() & Assembler::MATRIX) != 0 ) && (old_X_ddf_==x) ) return;

        VariableSetDescription variableSet(spaces_);
        typename VariableSetDescription::VariableSet u(variableSet);

        copy(x,u);

        assembler_->assemble(::Kaskade::linearization(f_,u) , Assembler::MATRIX , nAssemblyThreads );
        A_ = std::make_unique< KaskadeOperator >( assembler_->template get<Matrix>(onlyLowerTriangle_,rbegin_,rend_,cbegin_,cend_) );

        solver_ = std::make_shared<LinearSolver>( DirectSolver<VariableSetDescription,VariableSetDescription>( *A_ , spaces_, this->domain().dualSpace_ptr() , this->domain_ptr() ) );
        old_X_ddf_ = x;
      }


      FunctionalImpl f_;
      Spaces spaces_;
      mutable std::shared_ptr<Assembler> assembler_;
      mutable std::unique_ptr< KaskadeOperator > A_ = nullptr;
      mutable ::Algorithm::Vector old_X_f_, old_X_df_, old_X_ddf_;
      unsigned nAssemblyThreads = 1;
      bool onlyLowerTriangle_ = false;
      int rbegin_=0, rend_=FunctionalImpl::AnsatzVars::noOfVariables;
      int cbegin_=0, cend_=FunctionalImpl::TestVars::noOfVariables;
      mutable std::shared_ptr<LinearSolver> solver_ = nullptr;
    };



    template <class FunctionalImpl>
    auto makeFunctional(const FunctionalImpl& f, ::Algorithm::VectorSpace* domain,
                        int rbegin = 0, int rend = FunctionalImpl::AnsatzVars::noOfVariables,
                        int cbegin = 0, int cend = FunctionalImpl::TestVars::noOfVariables)
    {
      return Functional<FunctionalImpl>( f, domain , rbegin , rend , cbegin , cend );
    }

    template <class FunctionalImpl>
    auto makeFunctional(const FunctionalImpl& f, ::Algorithm::VectorSpace& domain,
                        int rbegin = 0, int rend = FunctionalImpl::AnsatzVars::noOfVariables,
                        int cbegin = 0, int cend = FunctionalImpl::TestVars::noOfVariables)
    {
      return Functional<FunctionalImpl>( f, domain , rbegin, rend , cbegin , cend );
    }
  }
}

#endif // ALGORITHM_OPERATORS_KASKADE_FUNCTIONAL_HH
