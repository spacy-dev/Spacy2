#ifndef ALGORITHM_OPERATORS_KASKADE_OPERATOR_HH
#define ALGORITHM_OPERATORS_KASKADE_OPERATOR_HH

#include <memory>

#include "banachSpace.hh"
#include "Interface/Operator/abstractC1Operator.hh"
#include "Util/Mixins/disableAssembly.hh"

#include "vector.hh"

namespace Algorithm
{
  namespace Kaskade
  {
    template <class Functional>
    class C1Operator :
        public Interface::AbstractC1Operator , public Mixin::DisableAssembly
    {
      using VariableSetDescription = typename Functional::AnsatzVars;
      using Spaces = typename VariableSetDescription::Spaces;
      using Variables = typename VariableSetDescription::Variables;
      using VectorImpl = typename VariableSetDescription::template CoefficientVectorRepresentation<>::type;
      using Assembler = ::Kaskade::VariationalFunctionalAssembler< ::Kaskade::LinearizationAt<Functional> >;
      using KaskOperator = ::Kaskade::AssembledGalerkinOperator<Assembler>;

      using Space = typename std::remove_pointer<typename boost::fusion::result_of::value_at_c<Spaces,0>::type>::type;
      using Variable = typename std::remove_pointer<typename boost::fusion::result_of::value_at_c<Variables,0>::type>::type;
      using HilbertSpace = HilbertSpace<Space,Variable>;

    public:
      C1Operator(const Functional& f,
                        std::shared_ptr<Interface::AbstractBanachSpace> domain_, std::shared_ptr<Interface::AbstractBanachSpace> range_)
        : AbstractC1Operator(domain_,range_),
          f_(f), spaces_( castTo<HilbertSpace>(domain()).impl() ),
          assembler_(spaces_)
      {}

      C1Operator(const Functional& f, const FunctionSpace& domain, const FunctionSpace& range)
        : C1Operator(f,domain.sharedImpl(),range.sharedImpl())
      {}

      C1Operator(const C1Operator& g)
        : AbstractC1Operator(g.sharedDomain(),g.sharedRange()),
          f_(g.f_), spaces_(g.spaces_),
          A_(std::make_unique<KaskOperator>(g.A_))
      {}


      C1Operator(const C1Operator& g, bool disableAssembly)
        : AbstractC1Operator(g.sharedDomain(),g.sharedRange()),
          Mixin::DisableAssembly(disableAssembly),
          f_(g.f_), spaces_(g.spaces_),
          A_(std::make_unique<KaskOperator>(g.A_))
      {}

      std::unique_ptr<Interface::AbstractFunctionSpaceElement> operator()(const Interface::AbstractFunctionSpaceElement& x) const final override
      {
        primalDualIgnoreReset(std::bind(&C1Operator::assembleOperator,std::ref(*this), std::placeholders::_1),x);

        return std::make_unique< Vector<VectorImpl> >( assembler.rhs() );
      }

      std::unique_ptr<Interface::AbstractFunctionSpaceElement> d1(const Interface::AbstractFunctionSpaceElement& dx) const final override
      {
        primalDualIgnoreReset(std::bind(&C1Operator::assembleGradient,std::ref(*this), std::placeholders::_1),x);

        auto y = clone(dx);

        A_->apply( dynamic_cast< const Vector<VectorImpl>& >(dx).impl() ,
                   dynamic_cast< Vector<VectorImpl>& >(*y).impl() );

        return y;
      }

    private:
      void assembleOperator(const Interface::AbstractFunctionSpaceElement& x)
      {
        if( assemblyIsDisabled() ) return;
        if( old_X_A_ != nullptr && old_X_A_->equals(x) ) return;

        VariableSetDescription variableSet(spaces);
        VariableSetDescription::Representation u(variableSet);

        boost::fusion::at_c<0>(u.data).coefficients() = castTo< Vector<VectorImpl> >(x).impl();

        assembler_.assemble(::Kaskade::linearization(f_,u_) , Assembler::RHS , nAssemblyThreads );
      }

      void assembleGradient(const Interface::AbstractFunctionSpaceElement& x)
      {
        if( assemblyIsDisabled() ) return;
        if( old_X_dA_ != nullptr && old_X_dA_->equals(x) ) return;

        VariableSetDescription variableSet(spaces);
        VariableSetDescription::Representation u(variableSet);

        boost::fusion::at_c<0>(u.data).coefficients() = castTo< Vector<VectorImpl> >(x).impl();

        assembler_.assemble(::Kaskade::linearization(f_,u_) , Assembler::MATRIX , nAssemblyThreads );
        A_ = std::make_unique< KaskOperator >( assembler_ );
      }

      C1Operator* cloneImpl() const final override
      {
        return new C1Operator(*this);
      }

      std::unique_ptr<Interface::LinearizedOperator> makeLinearization(const Interface::AbstractFunctionSpaceElement& x) const
      {
        primalDualIgnoreReset(std::bind(&C1Operator::assembleOperator,std::ref(*this), std::placeholders::_1),x);
        primalDualIgnoreReset(std::bind(&C1Operator::assembleGradient,std::ref(*this), std::placeholders::_1),x);
        return std::make_unique<Interface::LinearizedOperator>(std::make_unique<C1Operator>(*this,false),x);
      }

//      std::unique_ptr<Interface::AbstractLinearSolver> makeSolver() const
//      {
//        assert (A_ != nullptr);
//        return std::make_unique<LUSolver>( A_ , *F_.function_space(0) , sharedRange() , sharedDomain() );
//      }

      Functional f_;
      Spaces spaces_;
      Assembler assembler_;
      std::unique_ptr< KaskOperator > A_;
      std::unique_ptr< Vector<VectorImpl> > old_X_A_ = nullptr, old_X_dA_ = nullptr;
      unsigned nAssemblyThreads = 1;
    };
  }
}

#endif // ALGORITHM_OPERATORS_KASKADE_OPERATOR_HH
