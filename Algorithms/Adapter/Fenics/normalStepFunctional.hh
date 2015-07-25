//#ifndef ALGORITHM_ADAPTER_FENICS_NORMAL_STEP_FUNCTIONAL_HH
//#define ALGORITHM_ADAPTER_FENICS_NORMAL_STEP_FUNCTIONAL_HH

//#include <memory>

//#include "Interface/abstractFunctionSpaceElement.hh"
//#include "Interface/Functional/abstractC2Functional.hh"
//#include "Interface/Functional/hessian.hh"
//#include "Interface/Operator/abstractC2Operator.hh"
//#include "Util/mixins.hh"

//#include "pc2Functional.hh"
//#include "../../lagrangeFunctional.hh"

//namespace Algorithm
//{
//  namespace Fenics
//  {
//    class NormalStepFunctional
//        : public Interface::AbstractLagrangeFunctional, public Mixin::StateIndex, public Mixin::ControlIndex
//    {
//    public:
//      NormalStepFunctional(std::unique_ptr<Interface::AbstractC2Functional>&& f,
//                           std::unique_ptr<Interface::AbstractC2Operator>&& state,
//                           std::unique_ptr<Interface::AbstractC1Operator>&& control,
//                           std::shared_ptr<Interface::AbstractBanachSpace> domain);

//      double d0(const Interface::AbstractFunctionSpaceElement& x) const final override;

//      double d1(const Interface::AbstractFunctionSpaceElement& x, const Interface::AbstractFunctionSpaceElement& dx) const final override;

//      std::unique_ptr<Interface::AbstractFunctionSpaceElement> d2(const Interface::AbstractFunctionSpaceElement& x, const Interface::AbstractFunctionSpaceElement& dx) const final override;

//      double d2(const Interface::AbstractFunctionSpaceElement& x, const Interface::AbstractFunctionSpaceElement& dx, const Interface::AbstractFunctionSpaceElement& dy) const final override;

//      const Interface::AbstractC2Functional& getCostFunctional() const;

//      const Interface::AbstractC2Operator& getConstraint() const;

//    private:
//      NormalStepFunctional* cloneImpl() const final override;

//      Interface::Hessian makeHessian(const Interface::AbstractFunctionSpaceElement& x) const final override;

//      std::unique_ptr<Interface::AbstractC2Functional> f_;
//      std::unique_ptr<Interface::AbstractC2Operator> state_;
//      std::unique_ptr<Interface::AbstractC1Operator> control_;
//    };

//    ::Algorithm::LagrangeFunctional makeNormalStepFunctional(std::unique_ptr<Interface::AbstractC2Functional>&& f,
//                                                             std::unique_ptr<Interface::AbstractC2Operator>&& state,
//                                                             std::unique_ptr<Interface::AbstractC1Operator>&& control,
//                                                             std::shared_ptr<Interface::AbstractBanachSpace> domain);
//  }
//}

//#endif // ALGORITHM_ADAPTER_FENICS_NORMAL_STEP_FUNCTIONAL_HH
