//#include "normalStepFunctional.hh"

//#include "luSolver.hh"

//#include "FunctionSpaces/ProductSpace/productSpaceElement.hh"
//#include "Util/Exceptions/invalidArgumentException.hh"
//#include "Util/Exceptions/callOfUndefinedFunctionException.hh"

//#include "Algorithm/ConjugateGradients/cgSolver.hh"
//#include "Algorithm/ConjugateGradients/triangularStateConstraintPreconditioner.hh"
//#include "Util/create.hh"

//#include <utility>
//#include <iostream>

//namespace Algorithm
//{
//  namespace Fenics
//  {
//    NormalStepFunctional::NormalStepFunctional(std::unique_ptr<Interface::AbstractC2Functional>&& f,
//                                               std::unique_ptr<Interface::AbstractC2Operator>&& state,
//                                               std::unique_ptr<Interface::AbstractC1Operator>&& control,
//                                               std::shared_ptr<Interface::AbstractBanachSpace> domain)
//      : Interface::AbstractLagrangeFunctional(domain),
//        f_(std::move(f)),
//        state_(std::move(state)), control_(std::move(control))
//    {}

//    double NormalStepFunctional::d0(const Interface::AbstractFunctionSpaceElement& x) const
//    {
//      if( !isProductSpaceElement(x)) throw InvalidArgumentException("Fenics::LagrangeFunctional::operator()");
//      return (*f_)( toProductSpaceElement(x).primalElement() );
//    }

//    double NormalStepFunctional::d1(const Interface::AbstractFunctionSpaceElement& x,
//                                    const Interface::AbstractFunctionSpaceElement& dx) const
//    {
//      if( !isProductSpaceElement(x)) throw InvalidArgumentException("Fenics::LagrangeFunctional::d1");
//      if( !isProductSpaceElement(dx)) throw InvalidArgumentException("Fenics::LagrangeFunctional::d1");

//      const auto& x_  = toProductSpaceElement(x);
//      const auto& dx_ = toProductSpaceElement(dx);

//      if( !dx_.isDualEnabled() )
//      {
//        dx_.reset();
//        return f_->d1(x_.primalElement(),dx_.primalElement());
//      }

//      if( !dx_.isPrimalEnabled() )
//      {
//        dx_.reset();
//        return dx_.dualElement() * (*state_)(x_.variable(stateIndex())) +
//            dx_.dualElement() * (*control_)(x_.variable(controlIndex()));
//      }

//      return f_->d1(x_.primalElement(),dx_.primalElement()) +
//          dx_.dualElement() * (*state_)(x_.variable(stateIndex())) +
//          dx_.dualElement() * (*control_)(x_.variable(controlIndex()));
//    }

//    std::unique_ptr<Interface::AbstractFunctionSpaceElement> NormalStepFunctional::d2(const Interface::AbstractFunctionSpaceElement& x, const Interface::AbstractFunctionSpaceElement& dx) const
//    {
//      throw CallOfUndefinedFunctionException("LagrangeFunctional::d2");
//    }

//    double NormalStepFunctional::d2(const Interface::AbstractFunctionSpaceElement& x, const Interface::AbstractFunctionSpaceElement& dx, const Interface::AbstractFunctionSpaceElement& dy) const
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
//        return f_->d2(x_.primalElement(),dx_.primalElement(),dy_.primalElement());
//      }

//      if( !dx_.isDualEnabled() && !dy_.isPrimalEnabled() )
//      {
//        reset(dx_,dy_);
//        return dy_.dualElement() * state_->d1(x_.variable(stateIndex()),dx_.variable(stateIndex())) +
//            dy_.dualElement() * control_->d1(x_.variable(controlIndex()),dx_.variable(controlIndex()));
//      }

//      if( !dx_.isPrimalEnabled() && !dy_.isDualEnabled() )
//      {
//        reset(dx_,dy_);
//        return dx_.dualElement() * state_->d1(x_.variable(stateIndex()),dy_.variable(stateIndex())) +
//            dx_.dualElement() * control_->d1(x_.variable(controlIndex()),dy_.variable(controlIndex()));
//      }

//      return f_->d2(x_.primalElement(),dx_.primalElement(),dy_.primalElement()) +
//          dx_.dualElement() * state_->d1(x_.variable(stateIndex()),dy_.variable(stateIndex())) +
//          dx_.dualElement() * control_->d1(x_.variable(controlIndex()),dy_.variable(controlIndex())) +
//          dy_.dualElement() * state_->d1(x_.variable(stateIndex()),dx_.variable(stateIndex())) +
//          dy_.dualElement() * control_->d1(x_.variable(controlIndex()),dx_.variable(controlIndex()));
//    }

//    const Interface::AbstractC2Functional& NormalStepFunctional::getCostFunctional() const
//    {
//      return *f_;
//    }

//    const Interface::AbstractC2Operator& NormalStepFunctional::getConstraint() const
//    {
//      return *state_;
//    }

//    NormalStepFunctional* NormalStepFunctional::cloneImpl() const
//    {
//      return new NormalStepFunctional(clone(f_),clone(state_),clone(control_),getSharedDomain());
//    }

//    Interface::Hessian NormalStepFunctional::makeHessian(const Interface::AbstractFunctionSpaceElement& x) const
//    {
//      const auto& x_ = toProductSpaceElement(x);
//      std::cout << "state solver" << std::endl;
//      auto stateSolver = state_->getLinearization(x_.variable(stateIndex())).createLinearSolver("Fenics::LUSolver");
//      std::cout << "control solver" << std::endl;
//      auto controlSolver = f_->createLinearSolver("Fenics::LUSolver",x_.primalElement());
//      std::cout << "adjoint solver" << std::endl;
//      auto adjointSolver = state_->getLinearization(x_.variable(stateIndex())).createLinearSolver("Fenics::TransposedLUSolver");

//      std::cout << "preconditioner" << std::endl;
//      auto P = create_uniqueImpl<Operator,TriangularStateConstraintPreconditioner>( stateSolver , controlSolver , adjointSolver ,
//                                                                                    clone(control_->getLinearization(x)) ,
//                                                                                    clone(control_->getLinearization(x)) ,
//                                                                                    getSharedDomain() , getSharedDomain() );

//      auto A = createFromUniqueImpl<Operator,Interface::Hessian>( clone(this) , x );
//      auto solver = std::make_shared< CGSolver >( A , P );
//      return Interface::Hessian(clone(*this),x,solver);
//    }


//    ::Algorithm::LagrangeFunctional makeNormalStepFunctional(std::unique_ptr<Interface::AbstractC2Functional>&& f,
//                                                             std::unique_ptr<Interface::AbstractC2Operator>&& state,
//                                                             std::unique_ptr<Interface::AbstractC1Operator>&& control,
//                                                             std::shared_ptr<Interface::AbstractBanachSpace> domain)
//    {
//      return ::Algorithm::LagrangeFunctional( std::make_unique< ::Algorithm::Fenics::NormalStepFunctional >( std::move(f) ,
//                                                                                                             std::move(state) ,
//                                                                                                             std::move( control ) ,
//                                                                                                             domain ) );
//    }
//  }
//}
