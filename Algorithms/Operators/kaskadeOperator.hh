#ifndef ALGORITHM_OPERATORS_KASKADE_OPERATOR_HH
#define ALGORITHM_OPERATORS_KASKADE_OPERATOR_HH

#include "functionSpace.hh"
#include "Interface/Operator/abstractOperator.hh"
#include "Interface/Operator/abstractC1Operator.hh"
#include "FunctionSpaces/KaskadeVectorSpace/vectorSpaceElement.hh"

namespace Algorithm
{
  template <class Functional>
  class KaskadeOperator : public AbstractOperator
  {
    using VariableSet = typename Functional::AnsatzVars::Representation;
    using Vector = typename Functional::AnsatzVars::template CoefficientVectorRepresentation<>::type;
    using Assembler = ::Kaskade::VariationalFunctionalAssembler<::Kaskade::LinearizationAt<Functional> >;
    using KaskOperator = ::Kaskade::AssembledGalerkinOperator<Assembler>;
  public:
      KaskadeOperator(const VariableSet& x, const Functional& f, Assembler& assembler, const AbstractBanachSpace& domain, const AbstractBanachSpace& range)
        : AbstractOperator(domain,range),
          x_(x), assembler_(assembler),
          f_(f), A_(std::make_unique<KaskOperator>(assembler_))
      {}

    KaskadeOperator(const VariableSet& x, const Functional& f, Assembler& assembler, const FunctionSpace& domain, const FunctionSpace& range)
        : KaskadeOperator(x,f,assembler,domain.impl(),range.impl())
    {}

    KaskadeOperator(const KaskadeOperator& g)
      : AbstractOperator(g.getDomain(),g.getRange()),
        x_(g.x_), assembler_(g.assembler_),
        f_(g.f_), A_(std::make_unique<KaskOperator>(assembler_))
    {}

    std::unique_ptr<AbstractFunctionSpaceElement> operator()(const AbstractFunctionSpaceElement& x) const final override
    {
      auto y = clone(x);

      A_->apply( dynamic_cast< const ::Algorithm::Kaskade::VectorSpaceElement<Vector>& >(x).impl() ,
                 dynamic_cast< ::Algorithm::Kaskade::VectorSpaceElement<Vector>& >(*y).impl() );

      return y;
    }

  private:
    KaskadeOperator* cloneImpl() const final override
    {
      return new KaskadeOperator(*this);
    }

    VariableSet x_;
    Assembler& assembler_;
    Functional f_;
    std::unique_ptr< KaskOperator > A_;
  };

  template <class Functional>
  class KaskadeC1Operator : public AbstractC1Operator
  {
    using VariableSet = typename Functional::AnsatzVars::Representation;
    using Vector = typename Functional::AnsatzVars::template CoefficientVectorRepresentation<>::type;
    using Assembler = ::Kaskade::VariationalFunctionalAssembler<::Kaskade::LinearizationAt<Functional> >;
    using KaskOperator = ::Kaskade::AssembledGalerkinOperator<Assembler>;
  public:
      KaskadeC1Operator(const VariableSet& x, const Functional& f, Assembler& assembler, const AbstractBanachSpace& domain, const AbstractBanachSpace& range)
        : AbstractC1Operator(domain,range),
          x_(x), assembler_(assembler),
          f_(f), A_(std::make_unique<KaskOperator>(assembler_))
      {}

    KaskadeC1Operator(const VariableSet& x, const Functional& f, Assembler& assembler, const FunctionSpace& domain, const FunctionSpace& range)
        : KaskadeC1Operator(x,f,assembler,domain.impl(),range.impl())
    {}

    KaskadeC1Operator(const KaskadeC1Operator& g)
      : AbstractC1Operator(g.getDomain(),g.getRange()),
        x_(g.x_), assembler_(g.assembler_),
        f_(g.f_), A_(std::make_unique<KaskOperator>(assembler_))
    {}

    std::unique_ptr<AbstractFunctionSpaceElement> operator()(const AbstractFunctionSpaceElement& x) const final override
    {
      auto y = clone(x);

      A_->apply( dynamic_cast< const ::Algorithm::Kaskade::VectorSpaceElement<Vector>& >(x).impl() ,
                 dynamic_cast< ::Algorithm::Kaskade::VectorSpaceElement<Vector>& >(*y).impl() );

      return y;
    }

    std::unique_ptr<AbstractFunctionSpaceElement> d1(const AbstractFunctionSpaceElement& dx) const final override
    {
      auto y = clone(dx);

      A_->apply( dynamic_cast< const ::Algorithm::Kaskade::VectorSpaceElement<Vector>& >(dx).impl() ,
                 dynamic_cast< ::Algorithm::Kaskade::VectorSpaceElement<Vector>& >(*y).impl() );

      return y;
    }

  private:
    KaskadeC1Operator* cloneImpl() const final override
    {
      return new KaskadeC1Operator(*this);
    }

    VariableSet x_;
    Assembler& assembler_;
    Functional f_;
    std::unique_ptr< KaskOperator > A_;
  };
}

#endif // ALGORITHM_OPERATORS_KASKADE_OPERATOR_HH
