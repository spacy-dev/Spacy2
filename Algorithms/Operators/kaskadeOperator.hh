#ifndef ALGORITHM_OPERATORS_KASKADE_OPERATOR_HH
#define ALGORITHM_OPERATORS_KASKADE_OPERATOR_HH

#include "functionSpace.hh"
#include "../Interface/abstractC0Operator.hh"
#include "../FunctionSpaces/VectorSpace/vectorSpaceElement.hh"

namespace Algorithm
{
  template <class Functional>
  class KaskadeOperator : public AbstractC0Operator
  {
    using VariableSet = typename Functional::AnsatzVars::Representation;
    using Vector = typename Functional::AnsatzVars::template CoefficientVectorRepresentation<>::type;
    using Assembler = Kaskade::VariationalFunctionalAssembler<Kaskade::LinearizationAt<Functional> >;
    using Operator = Kaskade::AssembledGalerkinOperator<Assembler>;
  public:
    KaskadeOperator(const VariableSet& x, const Functional& f, Assembler& assembler, const FunctionSpace& domain, const FunctionSpace& range)
      : AbstractC0Operator(domain.impl(),range.impl()),
        x_(x), assembler_(assembler),
        f_(f), A_(std::make_unique<Operator>(assembler_))
    {}

    KaskadeOperator(const KaskadeOperator& g)
      : AbstractC0Operator(g.getDomain(),g.getRange()),
        x_(g.x_), assembler_(g.assembler_),
        f_(g.f_), A_(std::make_unique<Operator>(assembler_))
    {}

    std::unique_ptr<AbstractC0Operator> clone() const final override
    {
      return std::make_unique<KaskadeOperator>(*this);
    }

    void setArgument(const AbstractFunctionSpaceElement &x) final override
    {
      const auto& y = dynamic_cast< const VectorSpaceElement<Vector>& >(x);
      boost::fusion::at_c<0>(x_.data).coefficients() = boost::fusion::at_c<0>(y.impl().data);
      assembler_.assemble(Kaskade::linearization(f_,x_));
      A_ = std::make_unique< Operator >(assembler_);
    }

    std::unique_ptr<AbstractFunctionSpaceElement> operator()(const AbstractFunctionSpaceElement& x) const final override
    {
      auto y = x.clone();

      A_->apply( dynamic_cast< const VectorSpaceElement<Vector>& >(x).impl() ,
                 dynamic_cast< VectorSpaceElement<Vector>& >(*y).impl() );

      return y;
    }

    std::unique_ptr<AbstractFunctionSpaceElement> d0() const final override
    {
      return nullptr;
    }

//    double d1(const AbstractFunctionSpaceElement& dx) const final override
//    {
//      return *gradient * dx;
//    }

//    double d2(const AbstractFunctionSpaceElement& dx, const AbstractFunctionSpaceElement& dy) const final override
//    {
//     const auto& dx_ = dynamic_cast< const VectorSpaceElement<Vector>& >(dx);
//     const auto& dy_ = dynamic_cast< const VectorSpaceElement<Vector>& >(dy);

//     auto b = dx_.impl();
//     A.apply(dx_.impl(),b);
//     return b*dy_.impl();
//    }

  private:
    VariableSet x_;
    Assembler& assembler_;
    Functional f_;
    std::unique_ptr< Operator > A_;

  };
}

#endif // ALGORITHM_OPERATORS_KASKADE_OPERATOR_HH
