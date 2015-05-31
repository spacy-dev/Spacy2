#ifndef ALGORITHM_OPERATORS_KASKADEFUNCTIONAL_HH
#define ALGORITHM_OPERATORS_KASKADEFUNCTIONAL_HH

#include "../Interface/abstractC2Functional.hh"
#include "../FunctionSpaces/VectorSpace/vectorSpaceElement.hh"

namespace Algorithm
{
  template <class Functional>
  class KaskadeFunctional : public AbstractC2Functional
  {
    using Domain = typename Functional::AnsatzVars::template CoefficientVectorRepresentation<>::type;
    using Range = typename Functional::TestVars::template CoefficientVectorRepresentation<>::type;
    using Assembler = Kaskade::VariationalFunctionalAssembler<Kaskade::LinearizationAt<Functional> >;
  public:
    KaskadeFunctional(const AbstractBanachSpace& domain)
      : AbstractC2Functional(domain)
    {}

    KaskadeFunctional(const KaskadeFunctional& g)
      : assembler(g.assembler), f(g.f),
        gradient(g.gradient->clone()),
        A(g.A)
    {}

    std::unique_ptr<AbstractC0Functional> clone() const final override
    {
      return std::make_unique<KaskadeFunctional>(*this);
    }

    void setArgument(const AbstractFunctionSpaceElement &x) final override
    {
      const auto& x_ = dynamic_cast< const VectorSpaceElement<Domain>& >(x);
      assembler.assemble(Kaskade::linearization(f,x_));
      gradient = std::make_unique<VectorSpaceElement<Domain> >( assembler.rhs() , getDomain() );
      A = Kaskade::AssembledGalerkinOperator<Assembler>(assembler);
    }

//    double operator()(const AbstractFunctionSpaceElement& x) const final override;

    double d0() const final override
    {
      return assembler.functional();
    }

    double d1(const AbstractFunctionSpaceElement& dx) const final override
    {
      return *gradient * dx;
    }

    double d2(const AbstractFunctionSpaceElement& dx, const AbstractFunctionSpaceElement& dy) const final override
    {
     const auto& dx_ = dynamic_cast< const VectorSpaceElement<Domain>& >(dx);
     const auto& dy_ = dynamic_cast< const VectorSpaceElement<Domain>& >(dy);

     auto b = dx_.impl();
     A.apply(dx_.impl(),b);
     return b*dy_.impl();
    }

  private:
    Assembler assembler;
    Functional f;
    std::unique_ptr<AbstractFunctionSpaceElement> gradient;
    Kaskade::AssembledGalerkinOperator<Assembler> A;

  };
}

#endif // ALGORITHM_OPERATORS_KASKADEFUNCTIONAL_HH
