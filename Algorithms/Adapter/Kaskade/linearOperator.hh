#ifndef ALGORITHM_OPERATORS_KASKADE_LINEAR_OPERATOR_HH
#define ALGORITHM_OPERATORS_KASKADE_LINEAR_OPERATOR_HH

#include <memory>
#include <utility>

#include "functionSpace.hh"
#include "../../c1Operator.hh"
#include "Interface/Operator/linearizedOperator.hh"
#include "Interface/Operator/abstractC1Operator.hh"
#include "Util/Mixins/disableAssembly.hh"
#include "Util/create.hh"

#include "directSolver.hh"
#include "vector.hh" // copy

namespace Algorithm
{
  namespace Kaskade
  {
    template <class OperatorImpl>
    class LinearOperator :
        public Interface::AbstractC1Operator , public Mixin::DisableAssembly
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
      LinearOperator(const OperatorImpl& f,
                        std::shared_ptr<Interface::AbstractFunctionSpace> domain_, std::shared_ptr<Interface::AbstractFunctionSpace> range_)
        : AbstractC1Operator(domain_,range_),
          f_(f),
          spaces_( extractSpaces<VariableSetDescription>(domain()) ),
          assembler_(spaces_)
      {}

      LinearOperator(const OperatorImpl& f, const FunctionSpace& domain, const FunctionSpace& range)
        : LinearOperator(f,domain.sharedImpl(),range.sharedImpl())
      {}

      LinearOperator(const LinearOperator& g)
        : AbstractC1Operator(g.sharedDomain(),g.sharedRange()),
          DisableAssembly(g.assemblyIsDisabled()),
          f_(g.f_), spaces_(g.spaces_),
          assembler_(spaces_)
      {
        if( g.A_ != nullptr ) A_ = std::make_unique<KaskadeOperator>(*g.A_);
      }


      LinearOperator(const LinearOperator& g, bool disableAssembly)
        : AbstractC1Operator(g.sharedDomain(),g.sharedRange()),
          DisableAssembly(disableAssembly),
          f_(g.f_), spaces_(g.spaces_),
          assembler_(spaces_),
          A_( std::make_unique<KaskadeOperator>(*g.A_) )
      {}

      std::unique_ptr<Interface::AbstractFunctionSpaceElement> operator()(const Interface::AbstractFunctionSpaceElement& x) const final override
      {
        return d1(x,x);
      }

      std::unique_ptr<Interface::AbstractFunctionSpaceElement> d1(const Interface::AbstractFunctionSpaceElement&, const Interface::AbstractFunctionSpaceElement& dx) const final override
      {
        assembleOperator();

        VectorImpl dx_( VariableSetDescription::template CoefficientVectorRepresentation<>::init(spaces_) );
        copyToCoefficientVector<VariableSetDescription>(dx,dx_);
        VectorImpl y_( VariableSetDescription::template CoefficientVectorRepresentation<>::init(spaces_) );

        A_->apply( dx_ , y_ );

        auto y = range().element();
        copyFromCoefficientVector<VariableSetDescription>(y_,*y);

        return std::move(y);
      }

    private:
      void assembleOperator() const
      {
        if( assemblyIsDisabled() ) return;

        VariableSetDescription variableSet(spaces_);
        typename VariableSetDescription::VariableSet u(variableSet);

        assembler_.assemble(::Kaskade::linearization(f_,u) , Assembler::MATRIX , nAssemblyThreads );

        disableAssembly();
      }

      LinearOperator* cloneImpl() const final override
      {
        return new LinearOperator(*this);
      }

      std::unique_ptr<Interface::LinearizedOperator> makeLinearization(const Interface::AbstractFunctionSpaceElement& x) const
      {
        assembleOperator();
        return std::make_unique<Interface::LinearizedOperator>(std::make_unique< LinearOperator<OperatorImpl> >(*this,true),x);
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
      unsigned nAssemblyThreads = 1;
      bool onlyLowerTriangle_ = false;
    };

    template <class OperatorImpl>
    auto makeLinearOperator(const OperatorImpl& f,
                      std::shared_ptr<Interface::AbstractFunctionSpace> domain,
                      std::shared_ptr<Interface::AbstractFunctionSpace> range)
    {
      return createFromUniqueImpl< ::Algorithm::C1Operator , LinearOperator<OperatorImpl> >( f, domain , range );
    }

    template <class OperatorImpl>
    auto makeLinearOperator(const OperatorImpl& f,
                      const FunctionSpace& domain,
                      const FunctionSpace& range)
    {
      return createFromUniqueImpl< ::Algorithm::C1Operator , LinearOperator<OperatorImpl> >( f, domain , range );
    }
  }
}

#endif // ALGORITHM_OPERATORS_KASKADE_LINEAR_OPERATOR_HH
