#ifndef ALGORITHM_OPERATORS_KASKADEFUNCTIONAL_HH
#define ALGORITHM_OPERATORS_KASKADEFUNCTIONAL_HH

#include "c2Functional.hh"
#include "Interface/Functional/abstractC2Functional.hh"
#include "Interface/Functional/hessian.hh"
#include "Util/Mixins/disableAssembly.hh"
#include "Util/create.hh"

#include "directSolver.hh"
#include "hilbertSpace.hh"
#include "vector.hh"

namespace Algorithm
{
  namespace Kaskade
  {
    template <class FunctionalImpl>
    class Functional : public Interface::AbstractC2Functional , public Mixin::DisableAssembly
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
      Functional(const FunctionalImpl& f, std::shared_ptr<Interface::AbstractBanachSpace> domain_)
        : AbstractC2Functional(domain_),
          f_(f),
          spaces_( extractSpaces<VariableSetDescription>(domain()) ),
          assembler_(spaces_)
      {}

      Functional(const FunctionalImpl& f, const BanachSpace& domain)
        : Functional(f,domain.sharedImpl())
      {}

      Functional(const Functional& g)
        : AbstractC2Functional(g.sharedDomain()),
          DisableAssembly(g.assemblyIsDisabled()),
          f_(g.f_), spaces_(g.spaces_),
          assembler_(spaces_)
      {
        if( g.A_ != nullptr ) A_ = std::make_unique<KaskadeOperator>(*g.A_);
      }


      Functional(const Functional& g, bool disableAssembly)
        : AbstractC2Functional(g.sharedDomain()),
          DisableAssembly(disableAssembly),
          f_(g.f_), spaces_(g.spaces_),
          assembler_(spaces_),
          A_( std::make_unique<KaskadeOperator>(*g.A_) )
      {}

      double d0(const Interface::AbstractFunctionSpaceElement& x) const final override
      {
        primalDualIgnoreReset(std::bind(&Functional::assembleFunctional,std::ref(*this), std::placeholders::_1),x);

        return assembler_.functional();
      }

      std::unique_ptr<Interface::AbstractFunctionSpaceElement> d1(const Interface::AbstractFunctionSpaceElement& x) const final override
      {
        primalDualIgnoreReset(std::bind(&Functional::assembleGradient,std::ref(*this), std::placeholders::_1),x);

        VectorImpl v( assembler_.rhs() );

        auto y = domain().dualSpacePtr()->element();
        copyFromCoefficientVector<VariableSetDescription>(v,*y);
        return std::move(y);
      }

      std::unique_ptr<Interface::AbstractFunctionSpaceElement> d2(const Interface::AbstractFunctionSpaceElement& x, const Interface::AbstractFunctionSpaceElement& dx) const final override
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

    private:
      void assembleFunctional(const Interface::AbstractFunctionSpaceElement& x) const
      {
        if( assemblyIsDisabled() ) return;
        if( old_X_f_ != nullptr && old_X_f_->equals(x) ) return;

        VariableSetDescription variableSet(spaces_);
        typename VariableSetDescription::VariableSet u(variableSet);

        copy(x,u);

        assembler_.assemble(::Kaskade::linearization(f_,u) , Assembler::VALUE , nAssemblyThreads );

        old_X_f_ = clone(x);
      }

      void assembleGradient(const Interface::AbstractFunctionSpaceElement& x) const
      {
        if( assemblyIsDisabled() ) return;
        if( old_X_df_ != nullptr && old_X_df_->equals(x) ) return;

        VariableSetDescription variableSet(spaces_);
        typename VariableSetDescription::VariableSet u(variableSet);

        copy(x,u);

        assembler_.assemble(::Kaskade::linearization(f_,u) , Assembler::RHS , nAssemblyThreads );

        old_X_df_ = clone(x);
      }

      void assembleHessian(const Interface::AbstractFunctionSpaceElement& x) const
      {
        if( assemblyIsDisabled() ) return;
        if( old_X_ddf_ != nullptr && old_X_ddf_->equals(x) ) return;

        VariableSetDescription variableSet(spaces_);
        typename VariableSetDescription::VariableSet u(variableSet);

        copy(x,u);

        assembler_.assemble(::Kaskade::linearization(f_,u) , Assembler::MATRIX , nAssemblyThreads );
        A_ = std::make_unique< KaskadeOperator >( assembler_.template get<Matrix>(onlyLowerTriangle_) );

        old_X_ddf_ = clone(x);
      }

      Functional* cloneImpl() const final override
      {
        return new Functional(*this);
      }

      std::unique_ptr<Interface::Hessian> makeHessian(const Interface::AbstractFunctionSpaceElement& x) const final override
      {
        primalDualIgnoreReset(std::bind(&Functional::assembleHessian,std::ref(*this), std::placeholders::_1),x);
        return std::make_unique<Interface::Hessian>(std::make_unique< Functional<FunctionalImpl> >(*this,true),x);
      }


      std::unique_ptr<Interface::AbstractLinearSolver> makeSolver() const final override
      {
        assert (A_ != nullptr);
        return std::make_unique< DirectSolver<VariableSetDescription,Domain,Domain> >( *A_ , spaces_, sharedDomain() , sharedDomain() );
      }

      FunctionalImpl f_;
      Spaces spaces_;
      mutable Assembler assembler_;
      mutable std::unique_ptr< KaskadeOperator > A_ = nullptr;
      mutable std::unique_ptr< Interface::AbstractFunctionSpaceElement > old_X_f_ = nullptr, old_X_df_ = nullptr, old_X_ddf_ = nullptr;
      unsigned nAssemblyThreads = 1;
      bool onlyLowerTriangle_ = false;
    };



    template <class FunctionalImpl>
    auto makeFunctional(const FunctionalImpl& f, std::shared_ptr<Interface::AbstractBanachSpace> domain)
    {
      return createFromUniqueImpl< ::Algorithm::C2Functional , Functional<FunctionalImpl> >( f, domain );
    }

    template <class FunctionalImpl>
    auto makeFunctional(const FunctionalImpl& f, const BanachSpace& domain)
    {
      return createFromUniqueImpl< ::Algorithm::C2Functional , Functional<FunctionalImpl> >( f, domain );
    }
  }
}

#endif // ALGORITHM_OPERATORS_KASKADEFUNCTIONAL_HH
