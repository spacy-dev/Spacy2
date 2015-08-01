#ifndef ALGORITHM_OPERATORS_KASKADEFUNCTIONAL_HH
#define ALGORITHM_OPERATORS_KASKADEFUNCTIONAL_HH

#include "c1Operator.hh"
#include "Interface/Functional/abstractC2Functional.hh"
#include "FunctionSpaces/KaskadeVectorSpace/vectorSpaceElement.hh"
#include "FunctionSpaces/VectorSpace/cgSolver.hh"
#include "Util/callofundefinedfunctionexception.hh"

#include "kaskadeOperator.hh"

namespace Algorithm
{
//  template <class Functional>
//  class KaskadeFunctional : public AbstractC2Functional
//  {
//    using Domain = typename Functional::AnsatzVars::template CoefficientVectorRepresentation<>::type;
//    using Range = typename Functional::TestVars::template CoefficientVectorRepresentation<>::type;
//    using Assembler = Kaskade::VariationalFunctionalAssembler<Kaskade::LinearizationAt<Functional> >;
//  public:
//    KaskadeFunctional(const AbstractBanachSpace& domain)
//      : AbstractC2Functional(domain)
//    {}

//    KaskadeFunctional(const KaskadeFunctional& g)
//      : assembler(g.assembler), f(g.f),
//        gradient(g.gradient->clone()),
//        A(g.A)
//    {}

//    std::unique_ptr<AbstractC0Functional> clone() const final override
//    {
//      return std::make_unique<KaskadeFunctional>(*this);
//    }

//    void setArgument(const AbstractFunctionSpaceElement &x) final override
//    {
//      const auto& x_ = dynamic_cast< const VectorSpaceElement<Domain>& >(x);
//      assembler.assemble(Kaskade::linearization(f,x_));
//      gradient = std::make_unique<VectorSpaceElement<Domain> >( assembler.rhs() , getDomain() );
//      A = Kaskade::AssembledGalerkinOperator<Assembler>(assembler);
//    }

////    double operator()(const AbstractFunctionSpaceElement& x) const final override;

//    double d0() const final override
//    {
//      return assembler.functional();
//    }

//    double d1(const AbstractFunctionSpaceElement& dx) const final override
//    {
//      return *gradient * dx;
//    }

//    double d2(const AbstractFunctionSpaceElement& dx, const AbstractFunctionSpaceElement& dy) const final override
//    {
//     const auto& dx_ = dynamic_cast< const VectorSpaceElement<Domain>& >(dx);
//     const auto& dy_ = dynamic_cast< const VectorSpaceElement<Domain>& >(dy);

//     auto b = dx_.impl();
//     A.apply(dx_.impl(),b);
//     return b*dy_.impl();
//    }

//  private:
//    Assembler assembler;
//    Functional f;
//    std::unique_ptr<AbstractFunctionSpaceElement> gradient;
//    Kaskade::AssembledGalerkinOperator<Assembler> A;
//  };

  template <class Functional>
  class KaskadeFunctional : public AbstractC2Functional
  {
    using VariableSet = typename Functional::AnsatzVars::Representation;
    using Vector = typename Functional::AnsatzVars::template CoefficientVectorRepresentation<>::type;
    using Assembler = ::Kaskade::VariationalFunctionalAssembler<::Kaskade::LinearizationAt<Functional> >;
  public:
    KaskadeFunctional(const VariableSet& x, const Functional& f, Assembler& assembler, const FunctionSpace& domain)
      : AbstractC2Functional(domain.impl()),
        x_(x), assembler_(assembler),
        f_(f),
        dA_(std::make_unique< ::Algorithm::Kaskade::VectorSpaceElement<Vector> >(getDomain().getDualSpace(),Vector(assembler_.rhs())))
    {
      setDerivative( std::make_shared< KaskadeC1Operator<Functional> >(x,f,assembler,getDomain(),getDomain().getDualSpace()) );
      linearization_ = std::make_unique<LinearOperator>( getDerivative()->getLinearization() );
      getDerivative()->getLinearization().setSolver( std::make_shared<CGSolver>( *linearization_ ) );
    }

    KaskadeFunctional(const KaskadeFunctional& g)
      : AbstractC2Functional(g.getDomain()),
        x_(g.x_), assembler_(g.assembler_),
        f_(g.f_),
        dA_(std::make_unique< ::Algorithm::Kaskade::VectorSpaceElement<Vector> >(getDomain().getDualSpace(),Vector(assembler_.rhs())))
    {
      setDerivative( std::make_shared< KaskadeC1Operator<Functional> >( x_,f_,assembler_,g.getDomain(),g.getDomain().getDualSpace()) );
      linearization_ = std::make_unique<LinearOperator>( getDerivative()->getLinearization() );
      getDerivative()->getLinearization().setSolver( std::make_shared<CGSolver>( *linearization_ ) );
    }

    void setArgument(const AbstractFunctionSpaceElement& x)
    {
      x_ = dynamic_cast< const ::Algorithm::Kaskade::VectorSpaceElement<Vector>& >(x).impl();
      assembler_.assemble(::Kaskade::linearization(f_,x_));
      dynamic_cast< ::Algorithm::Kaskade::VectorSpaceElement<Vector>& >(*dA_) = Vector( assembler_.rhs() );
      AbstractC2Functional::setArgument(x);
    }

    double operator()(const AbstractFunctionSpaceElement& x) const final override
    {
      throw CallOfUndefinedFunctionException("KaskadeFunctional::operator()");
//      auto y = x.clone();

//      A_->apply( dynamic_cast< const ::Algorithm::Kaskade::VectorSpaceElement<Vector>& >(x).impl() ,
//                 dynamic_cast< ::Algorithm::Kaskade::VectorSpaceElement<Vector>& >(*y).impl() );

//      return y;
      return 0.;
    }

    double d0() const final override
    {
      return assembler_.functional();
    }

    double d1(const AbstractFunctionSpaceElement &dx) const final override
    {
      return (*dA_) * dx;
    }

    std::unique_ptr<AbstractFunctionSpaceElement> d1() const
    {
      return clone(dA_);
    }

    double d2(const AbstractFunctionSpaceElement &dx, const AbstractFunctionSpaceElement &dy) const final override
    {
      return (*applyOperator(dx)) * dy;
    }

    std::unique_ptr<AbstractFunctionSpaceElement> d2(const AbstractFunctionSpaceElement &dx) const
    {
      return applyOperator(dx);
    }

  private:
    KaskadeFunctional* cloneImpl() const final override
    {
      return new KaskadeFunctional(*this);
    }

    std::unique_ptr<AbstractFunctionSpaceElement> applyOperator(const AbstractFunctionSpaceElement& dx) const
    {
      return getDerivative()->getLinearization()(dx);
    }

    VariableSet x_;
    Assembler& assembler_;
    Functional f_;
    std::unique_ptr<AbstractFunctionSpaceElement> dA_;
    std::unique_ptr<LinearOperator> linearization_;
  };
}

#endif // ALGORITHM_OPERATORS_KASKADEFUNCTIONAL_HH
