#ifndef ALGORITHM_OPERATORS_KASKADE_OPERATOR_HH
#define ALGORITHM_OPERATORS_KASKADE_OPERATOR_HH

#include <memory>
#include <utility>

#include "banachSpace.hh"
#include "../../c1Operator.hh"
#include "Interface/Operator/linearizedOperator.hh"
#include "Interface/Operator/abstractC1Operator.hh"
#include "Util/Mixins/disableAssembly.hh"
#include "Util/Mixins/impl.hh"
#include "Util/Mixins/primalDualSwitch.hh"
#include "Util/castTo.hh"
#include "Util/create.hh"

#include "directSolver.hh"
#include "vector.hh"

namespace Algorithm
{
  namespace Kaskade
  {



    template <class OperatorImpl>
    class Operator :
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
      using KaskadeOperator = ::Kaskade::MatrixRepresentedOperator<Matrix,Domain,Range>;//::Kaskade::AssembledGalerkinOperator<Assembler>;
      //      using Domain = typename KaskadeOperator::Domain;
//      using Range = typename KaskadeOperator::Range;

      using Space = std::decay_t<std::remove_pointer_t<typename boost::fusion::result_of::value_at_c<Spaces,0>::type> >;
      using Variable = std::decay_t<std::remove_pointer_t<typename boost::fusion::result_of::value_at_c<Variables,0>::type> >;

    public:
      Operator(const OperatorImpl& f,
                        std::shared_ptr<Interface::AbstractBanachSpace> domain_, std::shared_ptr<Interface::AbstractBanachSpace> range_)
        : AbstractC1Operator(domain_,range_),
          f_(f), spaces_( &castTo< HilbertSpace<Space,Variable> >(domain()).impl() ),
          assembler_(spaces_)
      {}

      Operator(const OperatorImpl& f, const BanachSpace& domain, const BanachSpace& range)
        : Operator(f,domain.sharedImpl(),range.sharedImpl())
      {}

      Operator(const Operator& g)
        : AbstractC1Operator(g.sharedDomain(),g.sharedRange()),
          Mixin::DisableAssembly(g.assemblyIsDisabled()),
          f_(g.f_), spaces_(g.spaces_),
          assembler_(spaces_)
      {
        if( g.A_ != nullptr ) A_ = std::make_unique<KaskadeOperator>(*g.A_);
      }


      Operator(const Operator& g, bool disableAssembly)
        : AbstractC1Operator(g.sharedDomain(),g.sharedRange()),
          Mixin::DisableAssembly(disableAssembly),
          f_(g.f_), spaces_(g.spaces_),
          assembler_(spaces_),
          A_( std::make_unique<KaskadeOperator>(*g.A_) )
      {}

      std::unique_ptr<Interface::AbstractFunctionSpaceElement> operator()(const Interface::AbstractFunctionSpaceElement& x) const final override
      {
        //primalDualIgnoreReset(std::bind(&Operator::assembleOperator,std::ref(*this), std::placeholders::_1),x);
        assembleOperator(x);

        return std::make_unique< Vector<VariableSetDescription> >( domain(), VectorImpl(assembler_.rhs()) );
      }

      std::unique_ptr<Interface::AbstractFunctionSpaceElement> d1(const Interface::AbstractFunctionSpaceElement& x, const Interface::AbstractFunctionSpaceElement& dx) const final override
      {
//        primalDualIgnoreReset(std::bind(&Operator::assembleGradient,std::ref(*this), std::placeholders::_1),x);

        assembleGradient(x);
        auto y = clone(dx);

        A_->apply( castTo< Vector<VariableSetDescription> >(dx).impl() ,
                   castTo< Vector<VariableSetDescription> >(*y).impl() );

        return y;
      }

    private:
      void assembleOperator(const Interface::AbstractFunctionSpaceElement& x) const
      {
        if( assemblyIsDisabled() ) return;
        if( old_X_A_ != nullptr && old_X_A_->equals(x) ) return;

        VariableSetDescription variableSet(spaces_);
        typename VariableSetDescription::VariableSet u(variableSet);

        boost::fusion::at_c<0>(u.data) = boost::fusion::at_c<0>(castTo< Vector<VariableSetDescription> >(x).impl().data);

        assembler_.assemble(::Kaskade::linearization(f_,u) , Assembler::RHS , nAssemblyThreads );
      }

      void assembleGradient(const Interface::AbstractFunctionSpaceElement& x) const
      {
        if( assemblyIsDisabled() ) return;
        if( old_X_dA_ != nullptr && old_X_dA_->equals(x) ) return;

        VariableSetDescription variableSet(spaces_);
        typename VariableSetDescription::VariableSet u(variableSet);

        boost::fusion::at_c<0>(u.data).coefficients() = boost::fusion::at_c<0>(castTo< Vector<VariableSetDescription> >(x).impl().data);

        assembler_.assemble(::Kaskade::linearization(f_,u) , Assembler::MATRIX , nAssemblyThreads );
        A_ = std::make_unique< KaskadeOperator >( assembler_.template get<Matrix>(onlyLowerTriangle_) );
      }

      Operator* cloneImpl() const final override
      {
        return new Operator(*this);
      }

      std::unique_ptr<Interface::LinearizedOperator> makeLinearization(const Interface::AbstractFunctionSpaceElement& x) const
      {
        assembleOperator(x);
        assembleGradient(x);
//        primalDualIgnoreReset(std::bind(&Operator::assembleOperator,std::ref(*this), std::placeholders::_1),x);
//        primalDualIgnoreReset(std::bind(&Operator::assembleGradient,std::ref(*this), std::placeholders::_1),x);
        return std::make_unique<Interface::LinearizedOperator>(std::make_unique< Operator<OperatorImpl> >(*this,true),x);
      }

      std::unique_ptr<Interface::AbstractLinearSolver> makeSolver() const
      {
        assert (A_ != nullptr);
        return std::make_unique< LUSolver<VariableSetDescription,Range,Domain> >( *A_ , sharedRange() , sharedDomain() );
      }

      OperatorImpl f_;
      Spaces spaces_;
      mutable Assembler assembler_;
      mutable std::unique_ptr< KaskadeOperator > A_ = nullptr;
      mutable std::unique_ptr< Vector<VariableSetDescription> > old_X_A_ = nullptr, old_X_dA_ = nullptr;
      unsigned nAssemblyThreads = 1;
      bool onlyLowerTriangle_ = false;
    };

    template <class OperatorImpl>
    auto makeOperator(const OperatorImpl& f,
                      std::shared_ptr<Interface::AbstractBanachSpace> domain,
                      std::shared_ptr<Interface::AbstractBanachSpace> range)
    {
      return createFromUniqueImpl< ::Algorithm::C1Operator , Operator<OperatorImpl> >( f, domain , range );
    }
  }
}

#endif // ALGORITHM_OPERATORS_KASKADE_OPERATOR_HH
