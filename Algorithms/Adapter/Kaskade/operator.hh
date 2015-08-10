#ifndef ALGORITHM_OPERATORS_KASKADE_OPERATOR_HH
#define ALGORITHM_OPERATORS_KASKADE_OPERATOR_HH

#include <memory>
#include <utility>

#include "functionSpace.hh"
#include "../../operator.hh"
#include "Interface/Operator/linearizedOperator.hh"
#include "Interface/Operator/abstractOperator.hh"
#include "Util/Mixins/disableAssembly.hh"
#include "Util/castTo.hh"
#include "Util/create.hh"

#include "directSolver.hh"
#include "vector.hh" // copy

namespace Algorithm
{
  namespace Kaskade
  {
    template <class OperatorImpl>
    class Operator :
        public Interface::AbstractOperator , public Mixin::DisableAssembly
    {
      using VariableSetDescription = typename OperatorImpl::AnsatzVars;
      using VectorImpl = typename VariableSetDescription::template CoefficientVectorRepresentation<>::type;
      using Spaces = typename VariableSetDescription::Spaces;
      using Variables = typename VariableSetDescription::Variables;
      using Assembler = ::Kaskade::VariationalFunctionalAssembler< ::Kaskade::LinearizationAt<OperatorImpl> >;
      using Domain = typename Assembler::AnsatzVariableSetDescription::template CoefficientVectorRepresentation<>::type;
      using Range = typename Assembler::TestVariableSetDescription::template CoefficientVectorRepresentation<>::type;
      using Matrix = ::Kaskade::MatrixAsTriplet<double>;
      using KaskadeOperator = ::Kaskade::MatrixRepresentedOperator<Matrix,Domain,Range>;

    public:
      Operator(const OperatorImpl& f,
                        std::shared_ptr<Interface::AbstractFunctionSpace> domain_, std::shared_ptr<Interface::AbstractFunctionSpace> range_)
        : AbstractOperator(domain_,range_),
          f_(f),
          spaces_( extractSpaces<VariableSetDescription>(domain()) ),
          assembler_(spaces_)
      {}

      Operator(const OperatorImpl& f, const ::Algorithm::FunctionSpace& domain, const ::Algorithm::FunctionSpace& range)
        : Operator(f,domain.sharedImpl(),range.sharedImpl())
      {}

      Operator(const Operator& g)
        : AbstractOperator(g.sharedDomain(),g.sharedRange()),
          DisableAssembly(g.assemblyIsDisabled()),
          f_(g.f_), spaces_(g.spaces_),
          assembler_(spaces_)
      {
        if( g.A_ != nullptr ) A_ = std::make_unique<KaskadeOperator>(*g.A_);
      }


      Operator(const Operator& g, bool disableAssembly)
        : AbstractOperator(g.sharedDomain(),g.sharedRange()),
          DisableAssembly(disableAssembly),
          f_(g.f_), spaces_(g.spaces_),
          assembler_(spaces_),
          A_( std::make_unique<KaskadeOperator>(*g.A_) )
      {}

      std::unique_ptr<Interface::AbstractFunctionSpaceElement> operator()(const Interface::AbstractFunctionSpaceElement& x) const final override
      {
        primalDualIgnoreReset(std::bind(&Operator::assembleOperator,std::ref(*this), std::placeholders::_1),x);

        VectorImpl v( assembler_.rhs() );

        auto y = range().element();
        copyFromCoefficientVector<VariableSetDescription>(v,*y);
        return std::move(y);
      }

      std::unique_ptr<Interface::AbstractFunctionSpaceElement> d1(const Interface::AbstractFunctionSpaceElement& x, const Interface::AbstractFunctionSpaceElement& dx) const final override
      {
        primalDualIgnoreReset(std::bind(&Operator::assembleGradient,std::ref(*this), std::placeholders::_1),x);

        VectorImpl dx_( VariableSetDescription::template CoefficientVectorRepresentation<>::init(spaces_) );
        copyToCoefficientVector<VariableSetDescription>(dx,dx_);
        VectorImpl y_( VariableSetDescription::template CoefficientVectorRepresentation<>::init(spaces_) );

        A_->apply( dx_ , y_ );

        auto y = range().element();
        copyFromCoefficientVector<VariableSetDescription>(y_,*y);

        return std::move(y);
      }

    private:
      void assembleOperator(const Interface::AbstractFunctionSpaceElement& x) const
      {
        if( assemblyIsDisabled() ) return;
        if( old_X_A_ != nullptr && old_X_A_->equals(x) ) return;

        VariableSetDescription variableSet(spaces_);
        typename VariableSetDescription::VariableSet u(variableSet);

        copy(x,u);

        assembler_.assemble(::Kaskade::linearization(f_,u) , Assembler::RHS , nAssemblyThreads );

        old_X_A_ = clone(x);
      }

      void assembleGradient(const Interface::AbstractFunctionSpaceElement& x) const
      {
        if( assemblyIsDisabled() ) return;
        if( old_X_dA_ != nullptr && old_X_dA_->equals(x) ) return;

        VariableSetDescription variableSet(spaces_);
        typename VariableSetDescription::VariableSet u(variableSet);

        copy(x,u);

        assembler_.assemble(::Kaskade::linearization(f_,u) , Assembler::MATRIX , nAssemblyThreads );
        A_ = std::make_unique< KaskadeOperator >( assembler_.template get<Matrix>(onlyLowerTriangle_) );

        old_X_dA_ = clone(x);
      }

      Operator* cloneImpl() const final override
      {
        return new Operator(*this);
      }

      std::unique_ptr<Interface::LinearizedOperator> makeLinearization(const Interface::AbstractFunctionSpaceElement& x) const
      {
//        primalDualIgnoreReset(std::bind(&Operator::assembleOperator,std::ref(*this), std::placeholders::_1),x);
        primalDualIgnoreReset(std::bind(&Operator::assembleGradient,std::ref(*this), std::placeholders::_1),x);
        return std::make_unique<Interface::LinearizedOperator>(std::make_unique< Operator<OperatorImpl> >(*this,true),x);
      }

      std::unique_ptr<Interface::AbstractLinearSolver> makeSolver() const
      {
        assert (A_ != nullptr);
        return std::make_unique< DirectSolver<VariableSetDescription,Range,Domain> >( *A_ , spaces_, sharedRange() , sharedDomain() );
      }

      OperatorImpl f_;
      Spaces spaces_;
      mutable Assembler assembler_;
      mutable std::unique_ptr< KaskadeOperator > A_ = nullptr;
      mutable std::unique_ptr< Interface::AbstractFunctionSpaceElement > old_X_A_ = nullptr, old_X_dA_ = nullptr;
      unsigned nAssemblyThreads = 1;
      bool onlyLowerTriangle_ = false;
    };

    template <class OperatorImpl>
    auto makeOperator(const OperatorImpl& f,
                      std::shared_ptr<Interface::AbstractFunctionSpace> domain,
                      std::shared_ptr<Interface::AbstractFunctionSpace> range)
    {
      return createFromUniqueImpl< ::Algorithm::Operator , Operator<OperatorImpl> >( f, domain , range );
    }

    template <class OperatorImpl>
    auto makeOperator(const OperatorImpl& f,
                      const ::Algorithm::FunctionSpace& domain,
                      const ::Algorithm::FunctionSpace& range)
    {
      return createFromUniqueImpl< ::Algorithm::Operator , Operator<OperatorImpl> >( f, domain , range );
    }
  }
}

#endif // ALGORITHM_OPERATORS_KASKADE_OPERATOR_HH
