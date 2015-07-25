#include "lagrangeFunctional.hh"

#include "Interface/Functional/hessian.hh"
#include "FunctionSpaces/ProductSpace/productSpaceElement.hh"
#include "Util/Exceptions/invalidArgumentException.hh"
#include "Util/Exceptions/callOfUndefinedFunctionException.hh"
#include "Util/create.hh"
#include "Algorithm/ConjugateGradients/cgSolver.hh"
#include "Algorithm/ConjugateGradients/triangularStateConstraintPreconditioner.hh"
#include "Adapter/Fenics/c2Functional.hh"

#include <utility>

namespace Algorithm
{
  namespace Fenics
  {
//    LagrangeFunctional::LagrangeFunctional(std::unique_ptr<Interface::AbstractC2Functional>&& f,
//                                           std::unique_ptr<Interface::ConstraintOperator>&& constraint,
//                                           std::shared_ptr<Interface::AbstractBanachSpace> domain)
//      : Interface::AbstractLagrangeFunctional(domain),
//        f_(std::move(f)),
//        constraint_(std::move(constraint))
//    {}

//    double LagrangeFunctional::d0(const Interface::AbstractFunctionSpaceElement& x) const
//    {
//      if( !isProductSpaceElement(x)) throw InvalidArgumentException("Fenics::LagrangeFunctional::operator()");

//      const auto& x_ = dynamic_cast<const ProductSpaceElement&>(x);

//      if( !x_.isDualEnabled() )
//      {
//        return (*f_)(x_.primalElement());
//      }

//      return (*f_)(x_.primalElement()) +
//          x_.variable(adjointIndex()) * *constraint()(x_.primalElement());
//    }

//    std::unique_ptr<Interface::AbstractFunctionSpaceElement> LagrangeFunctional::d1(const Interface::AbstractFunctionSpaceElement& x) const
//    {
//      if( !isProductSpaceElement(x)) throw InvalidArgumentException("Fenics::LagrangeFunctional::d1");

//      const auto& x_  = toProductSpaceElement(x);


//      auto result_ = clone(x_);
//      auto& result = toProductSpaceElement(*result_);
//      auto tmpf_ = f_->d1(x_.primalElement());
//      const auto& tmpf = toProductSpaceElement(*tmpf_);
//      result.variable(stateIndex()) = tmpf.variable(stateIndex());
//      result.variable(controlIndex()) = tmpf.variable(controlIndex());
//      result.variable(adjointIndex()) =  *constraint()(x_.primalElement());

//      auto tmp = clone(x_.primalElement());
//      tmp->variable(stateIndex()) = x_.variable(adjointIndex());
//      tmp->variable(controlIndex()) *= 0;
//      result.variable(stateIndex()) += *constraint().d1(x_.primalElement(),*tmp);
//      tmp->variable(controlIndex()) = x_.variable(adjointIndex());
//      tmp->variable(stateIndex()) *= 0;
//      result.variable(controlIndex()) += *constraint().d1(x_.primalElement(),*tmp);

//      return std::move(result_);
//    }

//    std::unique_ptr<Interface::AbstractFunctionSpaceElement> LagrangeFunctional::d2(const Interface::AbstractFunctionSpaceElement& x, const Interface::AbstractFunctionSpaceElement& dx) const
//    {
//      if( !isProductSpaceElement(x)) throw InvalidArgumentException("Fenics::LagrangeFunctional::d2");
//      if( !isProductSpaceElement(dx) ) throw InvalidArgumentException("Fenics::LagrangeFunctional::d2");

//      const auto& x_  = toProductSpaceElement(x);
//      const auto& dx_ = toProductSpaceElement(dx);

//      auto result_ = clone(x_);
//      auto& result = toProductSpaceElement(*result_);
//      auto tmpf_ = f_->d2(x_.primalElement(),dx_.primalElement());
//      const auto& tmpf = toProductSpaceElement(*tmpf_);
//      auto z_ = clone(x_.primalElement());
//      auto& z = toProductSpaceElement(*z_);
//      z.variable(stateIndex()) = dx_.variable(stateIndex());
//      z.variable(controlIndex()) *= 0;
//      result.variable(stateIndex()) = tmpf.variable(stateIndex());
//      result.variable(stateIndex()) += *constraint().d1(x_.primalElement(),z);
//      z.variable(stateIndex()) *= 0;
//      z.variable(controlIndex()) = dx_.variable(controlIndex());
//      result.variable(controlIndex()) = tmpf.variable(controlIndex());
//      result.variable(controlIndex()) += *constraint().d1(x_.primalElement(),z);
//      result.variable(adjointIndex()) = *constraint().d1(x_.primalElement(),dx_.primalElement());
//      return std::move(result_);
//    }

//    double LagrangeFunctional::d2(const Interface::AbstractFunctionSpaceElement& x, const Interface::AbstractFunctionSpaceElement& dx, const Interface::AbstractFunctionSpaceElement& dy) const
//    {
//      if( !isProductSpaceElement(x)) throw InvalidArgumentException("Fenics::LagrangeFunctional::d2");
//      if( !isProductSpaceElement(dx) ) throw InvalidArgumentException("Fenics::LagrangeFunctional::d2");
//      if( !isProductSpaceElement(dy) ) throw InvalidArgumentException("Fenics::LagrangeFunctional::d2");
//      auto reset = [](auto& x, auto& y) { x.reset(); y.reset(); };

//      const auto& x_  = toProductSpaceElement(x);
//      const auto& dx_ = toProductSpaceElement(dx);
//      const auto& dy_ = toProductSpaceElement(dy);

//      if( !dx_.isDualEnabled() && !dy_.isDualEnabled() )
//      {
//        reset(dx_,dy_);
//        return f_->d2(x_.primalElement(),dx_.primalElement(),dy_.primalElement());// + pd2c_->d2(x,dx,dy);
//      }

//      if( !dx_.isDualEnabled() && !dy_.isPrimalEnabled() )
//      {
//        reset(dx_,dy_);
//        return dy_.variable(adjointIndex()) * constraint_->d1(x_.primalElement(),dx_.primalElement());
//      }

//      if( !dx_.isPrimalEnabled() && !dy_.isDualEnabled() )
//      {
//        reset(dx_,dy_);
//        return dx_.variable(adjointIndex()) * constraint_->d1(x_.primalElement(),dy_.primalElement());
//      }

//      return f_->d2(x_.primalElement(),dx_.primalElement(),dy_.primalElement()) +
//          dx_.variable(adjointIndex()) * constraint_->d1(x_.primalElement(),dy_.primalElement()) +
//          dy_.variable(adjointIndex()) * constraint_->d1(x_.primalElement(),dx_.primalElement());
//    }

//    const Interface::AbstractC2Functional& LagrangeFunctional::costFunctional() const
//    {
//      return *f_;
//    }

//    const Interface::ConstraintOperator& LagrangeFunctional::constraint() const
//    {
//      return *constraint_;
//    }

//    LagrangeFunctional* LagrangeFunctional::cloneImpl() const
//    {
//      auto newf = clone(f_);
//      auto newc = clone(constraint());
//      return new LagrangeFunctional(std::move(newf),std::move(newc),sharedDomain());
//    }

//    Interface::Hessian LagrangeFunctional::makeHessian(const Interface::AbstractFunctionSpaceElement& x) const
//    {
//      const auto& x_ = toProductSpaceElement(x);
//      auto A = createFromUniqueImpl< ::Algorithm::Operator, Interface::Hessian>( clone(this) , x );
//      auto P = createFromUniqueImpl< ::Algorithm::Operator, TriangularStateConstraintPreconditioner>( constraint().getLinearization(x_.primalElement()).solver(),
//                                                                                                     costFunctional().getHessian(x_.primalElement()).solver(),
//                                                                                                     constraint().getLinearization(x_.primalElement()).solver(),
//                                                                                                     clone(constraint().control()),
//                                                                                                     clone(constraint().control()),
//                                                                                                     sharedDomain(),
//                                                                                                     sharedDomain());
//      auto solver = std::make_shared<CGSolver>( A , P , 1e-12 , 1e-15 , true);
//      return Interface::Hessian(clone(this),x,solver);
//    }





    LagrangeFunctional::LagrangeFunctional(const AbstractC2Functional& L)
      : Interface::AbstractC2Functional(L.sharedDomain()),
        Mixin::UniqueImpl<Interface::AbstractC2Functional>(clone(L))
    {}


    double LagrangeFunctional::d0(const Interface::AbstractFunctionSpaceElement& x) const
    {
      const auto& x_ = toProductSpaceElement(x);
      if( !x_.isDualEnabled() )
      {
        auto y = clone(x_);
        toProductSpaceElement(*y).variable(adjointIndex()) *= 0;
        return impl()(*y);
      }

      return impl()(x);
    }

    std::unique_ptr<Interface::AbstractFunctionSpaceElement> LagrangeFunctional::d1(const Interface::AbstractFunctionSpaceElement& x) const
    {
      return impl().d1(x);
    }

    std::unique_ptr<Interface::AbstractFunctionSpaceElement> LagrangeFunctional::d2(const Interface::AbstractFunctionSpaceElement& x, const Interface::AbstractFunctionSpaceElement& dx) const
    {
      return impl().d2(x,dx);
    }

    LagrangeFunctional* LagrangeFunctional::cloneImpl() const
    {
      return new LagrangeFunctional(impl());
    }

    std::unique_ptr<Interface::Hessian> LagrangeFunctional::makeHessian(const Interface::AbstractFunctionSpaceElement& x) const
    {
      return impl().hessian(x);
    }

    void LagrangeFunctional::setOrigin(const Interface::AbstractFunctionSpaceElement& x) const
    {
      impl().setOrigin(x);
    }


    //    ::Algorithm::LagrangeFunctional makeLagrangeFunctional(std::unique_ptr<Interface::AbstractC2Functional>&& f,
    //                                                           std::unique_ptr<Interface::AbstractC2Operator>&& state,
    //                                                           std::unique_ptr<Interface::AbstractC1Operator>&& control,
    //                                                           std::unique_ptr<Interface::AbstractC2Functional>&& pd2c)
    //    {
    //      return create_uniqueImpl< ::Algorithm::LagrangeFunctional , ::Algorithm::Fenics::LagrangeFunctional >( std::move(f) ,
    //                                                                                                             std::move(state) ,
    //                                                                                                             std::move( control ) ,
    //                                                                                                             std::move( pd2c ) );
    //    }

  }
}
